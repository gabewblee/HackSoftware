/**
 * @file Expressions.c
 * @brief Expression parsing module for the Jack Compiler
 * 
 * This file contains the expression parsing implementation for the Jack Compiler.
 * It implements parsing and VM code generation for Jack expressions, terms,
 * and subroutine calls.
 * 
 * The module handles arithmetic expressions, boolean expressions, comparison
 * operations, unary operations, array access, variable references, constants,
 * and subroutine calls. Each expression type generates appropriate VM code
 * for stack manipulation and arithmetic/logical operations.
 */

#include "Expressions.h"
#include "Utils.h"
#include "JackTokenizer.h"
#include "SymbolTable.h"

/**
 * @brief Parses a subroutine call and generates corresponding VM code
 * 
 * Handles both method calls (with dot notation) and function calls.
 * Parses the expression list for arguments.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseSubroutineCall(CompilationEngine * compilationEngine) {
    if (checkIdentifier(compilationEngine)) {
        requireIdentifier(compilationEngine);
    }
    
    if (checkSymbol(compilationEngine, ".")) {
        requireSymbol(compilationEngine, ".");
        requireIdentifier(compilationEngine);
    }
    
    requireSymbol(compilationEngine, "(");
    parseExpressionList(compilationEngine);
    requireSymbol(compilationEngine, ")");
}

/**
 * @brief Parses a term and generates corresponding VM code
 * 
 * Handles constants, variables, array access, subroutine calls, unary operations,
 * and parenthesized expressions. Generates appropriate VM code for each term type.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseTerm(CompilationEngine * compilationEngine) {
    char * currentToken = getToken(compilationEngine->jackTokenizer);
    
    if (compilationEngine->jackTokenizer->type == INTEGER_CONSTANT) {
        writeVMPush(compilationEngine, "constant", atoi(currentToken));
        advance(compilationEngine->jackTokenizer);
    } else if (compilationEngine->jackTokenizer->type == STRING_CONSTANT) {
        writeVMPush(compilationEngine, "constant", 0);
        advance(compilationEngine->jackTokenizer);
    } else if (compilationEngine->jackTokenizer->type == KEYWORD) {
        if (strcmp(currentToken, "true") == 0) {
            writeVMPush(compilationEngine, "constant", 0);
            writeVMArithmetic(compilationEngine, "not");
        } else if (strcmp(currentToken, "false") == 0 || strcmp(currentToken, "null") == 0) {
            writeVMPush(compilationEngine, "constant", 0);
        } else if (strcmp(currentToken, "this") == 0) {
            writeVMPush(compilationEngine, "pointer", 0);
        }
        advance(compilationEngine->jackTokenizer);
    } else if (checkSymbol(compilationEngine, "(")) {
        requireSymbol(compilationEngine, "(");
        parseExpression(compilationEngine);
        requireSymbol(compilationEngine, ")");
    } else if (checkSymbol(compilationEngine, "-") || checkSymbol(compilationEngine, "~")) {
        char * op = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        parseTerm(compilationEngine);
        
        if (strcmp(op, "-") == 0) {
            writeVMArithmetic(compilationEngine, "neg");
        } else if (strcmp(op, "~") == 0) {
            writeVMArithmetic(compilationEngine, "not");
        }
    } else if (checkIdentifier(compilationEngine)) {
        char * varName = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        
        if (checkSymbol(compilationEngine, "[")) {
            requireSymbol(compilationEngine, "[");
            parseExpression(compilationEngine);
            requireSymbol(compilationEngine, "]");
            
            int kind = kindOf(compilationEngine->subroutineSymbolTable, varName);
            if (kind == -1) {
                kind = kindOf(compilationEngine->classSymbolTable, varName);
            }
            if (kind != -1) {
                const char * segment = kindToSegment(kind);
                int index = indexOf(compilationEngine->subroutineSymbolTable, varName);
                if (index == -1) {
                    index = indexOf(compilationEngine->classSymbolTable, varName);
                }
                writeVMPush(compilationEngine, segment, index);
            }
            
            writeVMArithmetic(compilationEngine, "add");
            writeVMPop(compilationEngine, "pointer", 1);
            writeVMPush(compilationEngine, "that", 0);
        } else if (checkSymbol(compilationEngine, "(") || checkSymbol(compilationEngine, ".")) {
            parseSubroutineCall(compilationEngine);
        } else {
            int kind = kindOf(compilationEngine->subroutineSymbolTable, varName);
            if (kind == -1) {
                kind = kindOf(compilationEngine->classSymbolTable, varName);
            }
            if (kind != -1) {
                const char * segment = kindToSegment(kind);
                int index = indexOf(compilationEngine->subroutineSymbolTable, varName);
                if (index == -1) {
                    index = indexOf(compilationEngine->classSymbolTable, varName);
                }
                writeVMPush(compilationEngine, segment, index);
            }
        }
    }
}

/**
 * @brief Parses an expression and generates corresponding VM code
 * 
 * Handles arithmetic and logical expressions with operator precedence.
 * Generates appropriate VM commands for each operator type.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseExpression(CompilationEngine * compilationEngine) {
    parseTerm(compilationEngine);
    
    while (isOp(compilationEngine)) {
        char * op = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        parseTerm(compilationEngine);
        
        if (strcmp(op, "+") == 0) {
            writeVMArithmetic(compilationEngine, "add");
        } else if (strcmp(op, "-") == 0) {
            writeVMArithmetic(compilationEngine, "sub");
        } else if (strcmp(op, "*") == 0) {
            writeVMCall(compilationEngine, "Math.multiply", 2);
        } else if (strcmp(op, "/") == 0) {
            writeVMCall(compilationEngine, "Math.divide", 2);
        } else if (strcmp(op, "&") == 0) {
            writeVMArithmetic(compilationEngine, "and");
        } else if (strcmp(op, "|") == 0) {
            writeVMArithmetic(compilationEngine, "or");
        } else if (strcmp(op, "<") == 0) {
            writeVMArithmetic(compilationEngine, "lt");
        } else if (strcmp(op, ">") == 0) {
            writeVMArithmetic(compilationEngine, "gt");
        } else if (strcmp(op, "=") == 0) {
            writeVMArithmetic(compilationEngine, "eq");
        }
    }
}

/**
 * @brief Parses an expression list and counts arguments
 * 
 * Handles comma-separated expressions for subroutine calls.
 * Counts the number of arguments and stores it for later use.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseExpressionList(CompilationEngine * compilationEngine) {
    int argCount = 0;
    
    if (isTerm(compilationEngine)) {
        parseExpression(compilationEngine);
        argCount++;
        
        while (checkSymbol(compilationEngine, ",")) {
            requireSymbol(compilationEngine, ",");
            parseExpression(compilationEngine);
            argCount++;
        }
    }
    
    compilationEngine->labelCounter = argCount;
}
