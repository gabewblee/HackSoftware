"""
Compilation Engine Module for the Jack Compiler.

This module implements the core compilation logic for the Jack programming language.
It parses Jack source code tokens and generates corresponding Virtual Machine
bytecode. The compilation engine follows a recursive descent parsing approach
and handles all Jack language constructs including classes, methods, statements,
and expressions.

The engine coordinates between the tokenizer, symbol tables, and VM writer to
produce semantically correct VM code that can be executed by the VM emulator.
"""

from JackTokenizer import JackTokenizer
from SymbolTables import SymbolTables
from VMWriter import VMWriter
from typing import Optional


class CompilationEngine:
    """
    Main compilation engine for Jack language to VM bytecode conversion.
    
    This class implements a recursive descent parser that processes Jack
    source code tokens and generates corresponding VM commands. It handles
    the complete compilation pipeline from token analysis to VM code generation.
    
    The engine maintains symbol tables for variable scoping, generates
    unique labels for control flow, and coordinates with the VM writer
    to produce properly formatted VM code.
    
    Attributes:
        _jackTokenizer (JackTokenizer): Tokenizer for source code analysis
        _symbolTables (SymbolTables): Symbol table manager for scoping
        _vmWriter (VMWriter): VM code generator
        _labelCounter (int): Counter for generating unique labels
        className (str): Name of the class being compiled
    """
    
    def __init__(self, outputFile, jackTokenizer: JackTokenizer) -> None:
        """
        Initialize the compilation engine.
        
        Args:
            outputFile: The output file stream for VM code
            jackTokenizer (JackTokenizer): Tokenizer for source code analysis
        """
        self._jackTokenizer = jackTokenizer
        self._symbolTables = SymbolTables()
        self._vmWriter = VMWriter(outputFile)
        self._labelCounter = 0

    def newLabel(self) -> int:
        """
        Generate a unique label number for control flow.
        
        Returns:
            int: A unique label number for this compilation session
        """
        labelNumber = self._labelCounter
        self._labelCounter += 1
        return labelNumber

    def compileClass(self) -> None:
        """
        Compile a complete Jack class declaration.
        
        Parses and compiles a Jack class, including:
        - Class name and opening brace
        - Class variable declarations (fields and statics)
        - Subroutine declarations (constructors, functions, methods)
        - Closing brace
        
        The method processes the class structure and delegates to
        specialized compilation methods for different components.
        
        Raises:
            SyntaxError: If the class structure is invalid
        """
        if self._jackTokenizer.currToken == "class":
            self._jackTokenizer.advance()

            className = self._jackTokenizer.currToken
            self.className = className
            self._jackTokenizer.advance()

            if self._jackTokenizer.currToken == "{":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected '{{' after class name, got '{self._jackTokenizer.currToken}'")

            while self.isClassVarDec(self._jackTokenizer.currToken):
                self.compileClassVarDec()

            while self.isSubroutineDec(self._jackTokenizer.currToken):
                self.compileSubroutineDec()

            if self._jackTokenizer.currToken == "}":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected '}}' at end of class, got '{self._jackTokenizer.currToken}'")
        else:
            raise SyntaxError(f"Expected 'class', got '{self._jackTokenizer.currToken}'")

    def compileClassVarDec(self) -> None:
        """
        Compile a class variable declaration.
        
        Parses and processes class-level variable declarations including
        fields (instance variables) and static variables. Handles multiple
        variables of the same type separated by commas.
        
        The method updates the symbol table with variable information
        and advances the tokenizer appropriately.
        
        Raises:
            SyntaxError: If the variable declaration syntax is invalid
        """
        if self.isClassVarDec(self._jackTokenizer.currToken):
            kind = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

            typeName = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

            varName = self._jackTokenizer.currToken
            self._symbolTables.define(varName, typeName, kind)
            self._jackTokenizer.advance()

            while self._jackTokenizer.currToken == ",":
                self._jackTokenizer.advance()
                varName = self._jackTokenizer.currToken
                self._symbolTables.define(varName, typeName, kind)
                self._jackTokenizer.advance()

            if self._jackTokenizer.currToken == ";":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected ';' after class var declaration, got '{self._jackTokenizer.currToken}'")
        else:
            raise SyntaxError(f"Expected class var declaration, got '{self._jackTokenizer.currToken}'")

    def compileDo(self) -> None:
        """
        Compile a do-statement.
        
        Parses and compiles a do-statement, which is used for method calls
        that don't return values. The method call is compiled and the
        return value (if any) is discarded by popping to temp segment.
        
        Raises:
            SyntaxError: If the do-statement syntax is invalid
        """
        if self._jackTokenizer.currToken == "do":
            self._jackTokenizer.advance() 
            
            name = self._jackTokenizer.currToken
            if not self.isIdentifier(name):
                raise SyntaxError(f"Expected identifier after 'do', got '{name}'")
            self._jackTokenizer.advance()
            
            self.compileSubroutineCall(name)

            if self._jackTokenizer.currToken == ";":
                self._jackTokenizer.advance() 
                self._vmWriter.writePop("temp", 0) 
            else:
                raise SyntaxError(f"Expected ';' after do-statement, got '{self._jackTokenizer.currToken}'")
        else:
            raise SyntaxError(f"Expected 'do', got '{self._jackTokenizer.currToken}'")

    def compileExpression(self) -> None:
        """
        Compile an expression.
        
        Parses and compiles a Jack expression, which consists of terms
        connected by binary operators. The method handles operator
        precedence by processing terms and operators in sequence.
        
        For each binary operation, it compiles the left term, then the
        right term, and finally generates the appropriate VM command
        for the operator.
        """
        self.compileTerm()
        while self.isBinaryOperation(self._jackTokenizer.currToken):
            operation = self._jackTokenizer.currToken
            self._jackTokenizer.advance()
            self.compileTerm()
            self._vmWriter.writeOperation(operation)

    def compileExpressionList(self) -> int:
        """
        Compile a list of expressions.
        
        Parses and compiles a comma-separated list of expressions,
        typically used for function/method arguments. Returns the
        count of expressions compiled.
        
        Returns:
            int: The number of expressions in the list
        """
        nArgs = 0
        if self._jackTokenizer.currToken != ")":
            self.compileExpression()
            nArgs += 1
            while self._jackTokenizer.currToken == ",":
                self._jackTokenizer.advance()
                self.compileExpression()
                nArgs += 1
        return nArgs


    def compileIf(self):
        if self._jackTokenizer.currToken == "if":
            self._jackTokenizer.advance()  
            
            if self._jackTokenizer.currToken == "(":
                self._jackTokenizer.advance()
                self.compileExpression()
                if self._jackTokenizer.currToken == ")":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected ')', got '{self._jackTokenizer.currToken}'")
            else:
                raise SyntaxError(f"Expected '(', got '{self._jackTokenizer.currToken}'")

            labelElse = f"IF.ELSE{self.newLabel()}"
            labelEnd  = f"IF.END{self.newLabel()}"
            
            self._vmWriter.writeArithmetic("not")
            self._vmWriter.writeIf(labelElse)

            if self._jackTokenizer.currToken == "{":
                self._jackTokenizer.advance()
                self.compileStatements()
                if self._jackTokenizer.currToken == "}":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected '}}', got '{self._jackTokenizer.currToken}'")
            else:
                raise SyntaxError(f"Expected '{{', got '{self._jackTokenizer.currToken}'")

            if self._jackTokenizer.currToken == "else":
                self._vmWriter.writeGoto(labelEnd)     
                self._vmWriter.writeLabel(labelElse)   
                self._jackTokenizer.advance()          
                if self._jackTokenizer.currToken == "{":
                    self._jackTokenizer.advance()
                    self.compileStatements()
                    if self._jackTokenizer.currToken == "}":
                        self._jackTokenizer.advance()
                    else:
                        raise SyntaxError(f"Expected '}}' after else, got '{self._jackTokenizer.currToken}'")
                else:
                    raise SyntaxError(f"Expected '{{' after else, got '{self._jackTokenizer.currToken}'")
                self._vmWriter.writeLabel(labelEnd)
            else:
                self._vmWriter.writeLabel(labelElse)
        else:
            raise SyntaxError(f"Expected 'if', got '{self._jackTokenizer.currToken}'")

    def compileLet(self):
        if self._jackTokenizer.currToken == "let":
            self._jackTokenizer.advance()
            varName = self._jackTokenizer.currToken
            if self.isIdentifier(varName):
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected 'identifier' but got '{varName}'")
            
            isArray = False
            if self._jackTokenizer.currToken == "[":
                isArray = True
                segment = self._symbolTables.kindOf(varName)
                index = self._symbolTables.indexOf(varName)
                self._vmWriter.writePush(segment, index)
                self._jackTokenizer.advance()
                self.compileExpression()
                if self._jackTokenizer.currToken == "]":
                    self._jackTokenizer.advance()
                    self._vmWriter.writeArithmetic("add")
                else:
                    raise SyntaxError(f"Expected ']' but got '{self._jackTokenizer.currToken}'")
            
            if self._jackTokenizer.currToken == "=":
                self._jackTokenizer.advance()
                self.compileExpression()
                if self._jackTokenizer.currToken == ";":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected ';' but got '{self._jackTokenizer.currToken}'")
            else:
                raise SyntaxError(f"Expected '=' but got '{self._jackTokenizer.currToken}'")

            if isArray:
                self._vmWriter.writePop("temp", 0)
                self._vmWriter.writePop("pointer", 1)
                self._vmWriter.writePush("temp", 0)
                self._vmWriter.writePop("that", 0)
            else:
                segment = self._symbolTables.kindOf(varName)
                index = self._symbolTables.indexOf(varName)
                self._vmWriter.writePop(segment, index)
        else:
            raise SyntaxError(f"Expected 'let' but got '{self._jackTokenizer.currToken}'")


    def compileParameterList(self):
        if self._jackTokenizer.currToken != ")":
            typeName = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

            varName = self._jackTokenizer.currToken
            self._symbolTables.define(varName, typeName, "argument")
            self._jackTokenizer.advance()

            while self._jackTokenizer.currToken == ",":
                self._jackTokenizer.advance()
                typeName = self._jackTokenizer.currToken
                self._jackTokenizer.advance()
                varName = self._jackTokenizer.currToken
                self._symbolTables.define(varName, typeName, "argument")
                self._jackTokenizer.advance()

    def compileReturn(self):
        if self._jackTokenizer.currToken == "return":
            self._jackTokenizer.advance()
        else:
            raise SyntaxError(f"Expected 'return' but got '{self._jackTokenizer.currToken}'")

        if self._jackTokenizer.currToken == ";":
            self._vmWriter.writePush("constant", 0)
        else:
            self.compileExpression()

        if self._jackTokenizer.currToken == ";":
            self._jackTokenizer.advance()
            self._vmWriter.writeReturn()
        else:
            raise SyntaxError(f"Expected ';', got '{self._jackTokenizer.currToken}'")

    def compileStatements(self):
        while self.isStatement(self._jackTokenizer.currToken):
            if self._jackTokenizer.currToken == "let":
                self.compileLet()
            elif self._jackTokenizer.currToken == "if":
                self.compileIf()
            elif self._jackTokenizer.currToken == "while":
                self.compileWhile()
            elif self._jackTokenizer.currToken == "do":
                self.compileDo()
            elif self._jackTokenizer.currToken == "return":
                self.compileReturn()
            else:
                pass

    def compileSubroutineBody(self, subroutineName, subroutineKind):
        if self._jackTokenizer.currToken == "{":
            self._jackTokenizer.advance()

            while self._jackTokenizer.currToken == "var":
                self.compileVarDec()

            nLocals = self._symbolTables.varCount("local")
            fullName = f"{self.className}.{subroutineName}"
            self._vmWriter.writeFunction(fullName, nLocals)

            
            if subroutineKind == "constructor":
                nFields = self._symbolTables.varCount("field")
                self._vmWriter.writePush("constant", nFields)
                self._vmWriter.writeCall("Memory.alloc", 1)
                self._vmWriter.writePop("pointer", 0)
            elif subroutineKind == "method":
                self._vmWriter.writePush("argument", 0)
                self._vmWriter.writePop("pointer", 0)

            self.compileStatements()

            if self._jackTokenizer.currToken == "}":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected '}}' at end of subroutine body, got '{self._jackTokenizer.currToken}'")
        else:
            raise SyntaxError(f"Expected '{{' at start of subroutine body, got '{self._jackTokenizer.currToken}'")

    def compileSubroutineCall(self, name: str = None):
        
        if name is None:
            name = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

        nArgs = 0
        fullName = ""

        if self._jackTokenizer.currToken == ".":
            self._jackTokenizer.advance()
            subName = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

            if self._symbolTables.kindOf(name) is not None:
                
                segment = self._symbolTables.kindOf(name)
                index = self._symbolTables.indexOf(name)
                typeName = self._symbolTables.typeOf(name)
                self._vmWriter.writePush(segment, index)
                nArgs += 1
                fullName = f"{typeName}.{subName}"
            else:
                
                fullName = f"{name}.{subName}"
        else:
            
            self._vmWriter.writePush("pointer", 0)
            nArgs += 1
            fullName = f"{self.className}.{name}"

        if self._jackTokenizer.currToken == "(":
            self._jackTokenizer.advance()
            nArgs += self.compileExpressionList()
            if self._jackTokenizer.currToken == ")":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected ')', got '{self._jackTokenizer.currToken}'")
        else:
            raise SyntaxError(f"Expected '(', got '{self._jackTokenizer.currToken}'")

        self._vmWriter.writeCall(fullName, nArgs)

    def compileSubroutineDec(self):
        if self.isSubroutineDec(self._jackTokenizer.currToken):
            self._symbolTables.startSubroutine()  

            subroutineKind = self._jackTokenizer.currToken  
            self._jackTokenizer.advance()

            returnType = self._jackTokenizer.currToken      
            self._jackTokenizer.advance()

            subroutineName = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

            # Set method flag before compiling parameters
            if subroutineKind == "method":
                self._symbolTables.setMethod(True)
            else:
                self._symbolTables.setMethod(False)

            if self._jackTokenizer.currToken == "(":
                self._jackTokenizer.advance()
                self.compileParameterList()
                if self._jackTokenizer.currToken == ")":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected ')', got '{self._jackTokenizer.currToken}'")
            else:
                raise SyntaxError(f"Expected '(', got '{self._jackTokenizer.currToken}'")

            self.compileSubroutineBody(subroutineName, subroutineKind)
        else:
            raise SyntaxError(f"Expected subroutine declaration, got '{self._jackTokenizer.currToken}'")

    def compileTerm(self):
        tokenType = self._jackTokenizer.tokenType()
        tokenVal  = self._jackTokenizer.currToken

        if tokenType == "integerConstant":
            self._vmWriter.writePush("constant", int(tokenVal))
            self._jackTokenizer.advance()
        elif tokenType == "stringConstant":
            stringVal = tokenVal
            self._vmWriter.writePush("constant", len(stringVal))
            self._vmWriter.writeCall("String.new", 1)
            for c in stringVal:
                self._vmWriter.writePush("constant", ord(c))
                self._vmWriter.writeCall("String.appendChar", 2)
            self._jackTokenizer.advance()
        elif self.isKeywordConstant(tokenVal):
            if tokenVal == "true":
                self._vmWriter.writePush("constant", 0)
                self._vmWriter.writeArithmetic("not")
            elif tokenVal in ("false", "null"):
                self._vmWriter.writePush("constant", 0)
            elif tokenVal == "this":
                self._vmWriter.writePush("pointer", 0)
            else:
                raise SyntaxError(f"Unexpected keyword constant '{tokenVal}'")
            self._jackTokenizer.advance()
        elif tokenType == "identifier":
            name = tokenVal
            nextTok = self._jackTokenizer.peek()

            if nextTok == "[":
                segment = self._symbolTables.kindOf(name)
                index = self._symbolTables.indexOf(name)
                self._vmWriter.writePush(segment, index)
                self._jackTokenizer.advance()  
                self._jackTokenizer.advance()  
                self.compileExpression()
                if self._jackTokenizer.currToken == "]":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected ']', got '{self._jackTokenizer.currToken}'")
                self._vmWriter.writeArithmetic("add")
                self._vmWriter.writePop("pointer", 1)
                self._vmWriter.writePush("that", 0)
            elif nextTok in ("(", "."):
                self._jackTokenizer.advance()
                self.compileSubroutineCall(name)
            else:
                segment = self._symbolTables.kindOf(name)
                index = self._symbolTables.indexOf(name)
                self._vmWriter.writePush(segment, index)
                self._jackTokenizer.advance()
        elif tokenVal == "(":
            self._jackTokenizer.advance()
            self.compileExpression()
            if self._jackTokenizer.currToken == ")":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected ')', got '{self._jackTokenizer.currToken}'")
        elif self.isUnaryOperation(tokenVal):
            op = tokenVal
            self._jackTokenizer.advance()
            self.compileTerm()
            if op == "-":
                self._vmWriter.writeArithmetic("neg")
            elif op == "~":
                self._vmWriter.writeArithmetic("not")
        else:
            raise SyntaxError(f"Unexpected term '{tokenVal}'")

    def compileVarDec(self):
        if self._jackTokenizer.currToken == "var":
            self._jackTokenizer.advance()  
            typeName = self._jackTokenizer.currToken
            self._jackTokenizer.advance()

            varName = self._jackTokenizer.currToken
            self._symbolTables.define(varName, typeName, "local")
            self._jackTokenizer.advance()

            while self._jackTokenizer.currToken == ",":
                self._jackTokenizer.advance()
                varName = self._jackTokenizer.currToken
                self._symbolTables.define(varName, typeName, "local")
                self._jackTokenizer.advance()

            if self._jackTokenizer.currToken == ";":
                self._jackTokenizer.advance()
            else:
                raise SyntaxError(f"Expected ';' after var declaration, got '{self._jackTokenizer.currToken}'")
        else:
            raise SyntaxError(f"Expected 'var', got '{self._jackTokenizer.currToken}'")

    def compileWhile(self):
        if self._jackTokenizer.currToken == "while":
            labelStart = f"WHILE.EXP{self.newLabel()}"
            labelEnd   = f"WHILE.END{self.newLabel()}"

            self._vmWriter.writeLabel(labelStart)
            self._jackTokenizer.advance()

            if self._jackTokenizer.currToken == "(":
                self._jackTokenizer.advance()
                self.compileExpression()
                if self._jackTokenizer.currToken == ")":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected ')', got '{self._jackTokenizer.currToken}'")
            else:
                raise SyntaxError(f"Expected '(', got '{self._jackTokenizer.currToken}'")

            self._vmWriter.writeArithmetic("not")
            self._vmWriter.writeIf(labelEnd)

            if self._jackTokenizer.currToken == "{":
                self._jackTokenizer.advance()
                self.compileStatements()
                if self._jackTokenizer.currToken == "}":
                    self._jackTokenizer.advance()
                else:
                    raise SyntaxError(f"Expected '}}' after while body, got '{self._jackTokenizer.currToken}'")
            else:
                raise SyntaxError(f"Expected '{{' after while condition, got '{self._jackTokenizer.currToken}'")

            self._vmWriter.writeGoto(labelStart)
            self._vmWriter.writeLabel(labelEnd)
        else:
            raise SyntaxError(f"Expected 'while', got '{self._jackTokenizer.currToken}'")


    def isBinaryOperation(self, token: Optional[str]) -> bool:
        return token in ("+", "-", "*", "/", "&", "|", "<", ">", "=")

    def isBoolean(self, token: Optional[str]) -> bool:
        return token == "boolean"

    def isChar(self, token: Optional[str]) -> bool:
        return token == "char"

    def isClassVarDec(self, token: Optional[str]) -> bool:
        return token in ("static", "field")

    def isIdentifier(self, token: Optional[str]) -> bool:
        return self._jackTokenizer.tokenType() == "identifier"

    def isInteger(self, token: Optional[str]) -> bool:
        return token == "int"

    def isKeywordConstant(self, token: Optional[str]) -> bool:
        return token in ("true", "false", "null", "this")

    def isStatement(self, token: Optional[str]) -> bool:
        return token in ("let", "if", "while", "do", "return")

    def isString(self, token: Optional[str]) -> bool:
        return token == "stringConstant"

    def isSubroutineDec(self, token: Optional[str]) -> bool:
        return token in ("constructor", "function", "method")

    def isUnaryOperation(self, token: Optional[str]) -> bool:
        return token in ("-", "~")

    def isVoid(self, token: Optional[str]) -> bool:
        return token == "void"
