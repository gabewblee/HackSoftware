from JackTokenizer import JackTokenizer

class CompilationEngine:
    def __init__(self, outputFile, jackTokenizer: JackTokenizer):
        self._outputFile = outputFile
        self._jackTokenizer = jackTokenizer

    def process(self, expected: str):
        if self._jackTokenizer.currToken == expected:
            tokenType = self._jackTokenizer.tokenType()
            self._outputFile.write(f"<{tokenType}> {self._jackTokenizer.currToken} </{tokenType}>\n")
        else:
            raise SyntaxError(f"Syntax Error: Expected '{expected}' but got '{self._jackTokenizer.currToken}'")
        self._jackTokenizer.advance()

    def compileClass(self):
        self._outputFile.write("<class>\n")
        self.process("class")
        self.process(self._jackTokenizer.currToken)
        self.process("{")
        while self.isClassVarDec():
            self.compileClassVarDec()

        while self.isSubroutineDec():
            self.compileSubroutineDec()
        self.process("}")
        self._outputFile.write("</class>\n")

    def compileClassVarDec(self):
        self._outputFile.write("<classVarDec>\n")
        self.process(self._jackTokenizer.currToken)
        if self.isInteger() or self.isChar() or self.isBoolean() or self.isIdentifier():
            self.process(self._jackTokenizer.currToken)
        else:
            raise SyntaxError(f"Syntax Error: Expected 'int', 'char', 'boolean', or 'identifier' but got '{self._jackTokenizer.currToken}'")

        if self.isIdentifier():
            self.process(self._jackTokenizer.currToken)
        else:
            raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

        while self._jackTokenizer.currToken == ",":
            self.process(",")
            if self.isIdentifier():
                self.process(self._jackTokenizer.currToken)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

        self.process(";")
        self._outputFile.write("</classVarDec>\n")

    def compileDo(self):
        self._outputFile.write("<doStatement>\n")
        self.process("do")
        self.compileSubroutineCall()
        self.process(";")
        self._outputFile.write("</doStatement>\n")

    def compileExpression(self):
        self._outputFile.write("<expression>\n")
        self.compileTerm()
        while self.isBinaryOperation():
            self.process(self._jackTokenizer.currToken)
            self.compileTerm()
        self._outputFile.write("</expression>\n")

    def compileExpressionList(self):
        self._outputFile.write("<expressionList>\n")
        if self._jackTokenizer.currToken != ")":
            self.compileExpression()
            while self._jackTokenizer.currToken == ",":
                self.process(",")
                self.compileExpression()
        self._outputFile.write("</expressionList>\n")

    def compileIf(self):
        self._outputFile.write("<ifStatement>\n")
        self.process("if")
        self.process("(")
        self.compileExpression()
        self.process(")")
        self.process("{")
        self.compileStatements()
        self.process("}")
        if self._jackTokenizer.currToken == "else":
            self.process("else")
            self.process("{")
            self.compileStatements()
            self.process("}")
        self._outputFile.write("</ifStatement>\n")

    def compileLet(self):
        self._outputFile.write("<letStatement>\n")
        self.process("let")
        if self.isIdentifier():
            self.process(self._jackTokenizer.currToken)
        else:
            raise SyntaxError(f"Syntax Error: Expected 'varName' but got '{self._jackTokenizer.currToken}'")

        if self._jackTokenizer.currToken == "[":
            self.process("[")
            self.compileExpression()
            self.process("]")

        self.process("=")
        self.compileExpression()
        self.process(";")
        self._outputFile.write("</letStatement>\n")

    def compileParameterList(self):
        self._outputFile.write("<parameterList>\n")
        if self._jackTokenizer.currToken != ")":
            if self.isInteger() or self.isChar() or self.isBoolean() or self.isIdentifier():
                self.process(self._jackTokenizer.currToken)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'int', 'char', 'boolean', or 'identifier' but got '{self._jackTokenizer.currToken}'")

            if self.isIdentifier():
                self.process(self._jackTokenizer.currToken)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

            while self._jackTokenizer.currToken == ",":
                self.process(",")
                if self.isInteger() or self.isChar() or self.isBoolean() or self.isIdentifier():
                    self.process(self._jackTokenizer.currToken)
                else:
                    raise SyntaxError(f"Syntax Error: Expected 'int', 'char', 'boolean', or 'identifier' but got '{self._jackTokenizer.currToken}'")
                    
                if self.isIdentifier():
                    self.process(self._jackTokenizer.currToken)
                else:
                    raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

        self._outputFile.write("</parameterList>\n")

    def compileReturn(self):
        self._outputFile.write("<returnStatement>\n")
        self.process("return")
        if self._jackTokenizer.currToken != ";":
            self.compileExpression()
        self.process(";")
        self._outputFile.write("</returnStatement>\n")

    def compileStatements(self):
        self._outputFile.write("<statements>\n")

        while self.isStatement():
            if self._jackTokenizer.currToken == "let":
                self.compileLet()
            elif self._jackTokenizer.currToken == "if":
                self.compileIf()
            elif self._jackTokenizer.currToken == "while":
                self.compileWhile()
            elif self._jackTokenizer.currToken == "do":
                self.compileDo()
            else:
                self.compileReturn()

        self._outputFile.write("</statements>\n")

    def compileSubroutineBody(self):
        self._outputFile.write("<subroutineBody>\n")
        self.process("{")
        while self._jackTokenizer.currToken == "var":
            self.compileVarDec()
        self.compileStatements()
        self.process("}")
        self._outputFile.write("</subroutineBody>\n")

    def compileSubroutineCall(self, name: str = None):
        if name is None:
            if self.isIdentifier():
                firstName = self._jackTokenizer.currToken
                self.process(firstName)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")
        else:
            firstName = name
            self._outputFile.write(f"<identifier> {firstName} </identifier>\n")

        if self._jackTokenizer.currToken == ".":
            self.process(".")
            if self.isIdentifier():
                self.process(self._jackTokenizer.currToken)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'subroutineName' but got '{self._jackTokenizer.currToken}'")

        self.process("(")
        self.compileExpressionList()
        self.process(")")

    def compileSubroutineDec(self):
        self._outputFile.write("<subroutineDec>\n")
        self.process(self._jackTokenizer.currToken)
        if self.isVoid() or self.isInteger() or self.isChar() or self.isBoolean() or self.isIdentifier():
            self.process(self._jackTokenizer.currToken)
        else:
            raise SyntaxError(f"Syntax Error: Expected 'void', 'int', 'char', 'boolean', or 'identifier' but got '{self._jackTokenizer.currToken}'")

        if self.isIdentifier():
            self.process(self._jackTokenizer.currToken)
        else:
            raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

        self.process("(")
        self.compileParameterList()
        self.process(")")
        self.compileSubroutineBody()
        self._outputFile.write("</subroutineDec>\n")

    def compileTerm(self):
        self._outputFile.write("<term>\n")

        tokenType = self._jackTokenizer.tokenType()
        token = self._jackTokenizer.currToken

        if tokenType == "integerConstant":
            self.process(token)
        elif tokenType == "stringConstant":
            self.process(token)
        elif self.isKeywordConstant():
            self.process(token)
        elif self.isIdentifier():
            firstName = token
            self.process(firstName)
            if self._jackTokenizer.currToken == "[":
                self.process("[")
                self.compileExpression()
                self.process("]")
            elif self._jackTokenizer.currToken in ("(", "."):
                self.compileSubroutineCall(firstName)
        elif token == "(":
            self.process("(")
            self.compileExpression()
            self.process(")")
        elif self.isUnaryOperation():
            self.process(token)
            self.compileTerm()
        else:
            raise SyntaxError(f"Syntax Error: Expected 'valid term' but got '{token}'")

        self._outputFile.write("</term>\n")

    def compileVarDec(self):
        self._outputFile.write("<varDec>\n")
        if self._jackTokenizer.currToken == "var":
            self.process("var")
            if self.isInteger() or self.isChar() or self.isBoolean() or self.isIdentifier():
                self.process(self._jackTokenizer.currToken)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'int', 'char', 'boolean', or 'identifier' but got '{self._jackTokenizer.currToken}'")

            if self.isIdentifier():
                self.process(self._jackTokenizer.currToken)
            else:
                raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

            while self._jackTokenizer.currToken == ",":
                self.process(",")
                if self.isIdentifier():
                    self.process(self._jackTokenizer.currToken)
                else:
                    raise SyntaxError(f"Syntax Error: Expected 'identifier' but got '{self._jackTokenizer.currToken}'")

            self.process(";")
        self._outputFile.write("</varDec>\n")

    def compileWhile(self):
        self._outputFile.write("<whileStatement>\n")
        self.process("while")
        self.process("(")
        self.compileExpression()
        self.process(")")
        self.process("{")
        self.compileStatements()
        self.process("}")
        self._outputFile.write("</whileStatement>\n")

    def isBinaryOperation(self) -> bool:
        return self._jackTokenizer.currToken in ("+", "-", "*", "/", "&", "|", "<", ">", "=")

    def isBoolean(self) -> bool:
        return self._jackTokenizer.currToken == "boolean"

    def isChar(self) -> bool:
        return self._jackTokenizer.currToken == "char"

    def isClassVarDec(self) -> bool:
        return self._jackTokenizer.currToken in ("static", "field")

    def isIdentifier(self) -> bool:
        return self._jackTokenizer.tokenType() == "identifier"

    def isInteger(self) -> bool:
        return self._jackTokenizer.currToken == "int"

    def isKeywordConstant(self) -> bool:
        return self._jackTokenizer.currToken in ("true", "false", "null", "this")

    def isStatement(self) -> bool:
        return self._jackTokenizer.currToken in ("let", "if", "while", "do", "return")

    def isString(self) -> bool:
        return self._jackTokenizer.tokenType() == "stringConstant"

    def isSubroutineDec(self) -> bool:
        return self._jackTokenizer.currToken in ("constructor", "function", "method")

    def isUnaryOperation(self) -> bool:
        return self._jackTokenizer.currToken in ("-", "~")

    def isVoid(self) -> bool:
        return self._jackTokenizer.currToken == "void"
