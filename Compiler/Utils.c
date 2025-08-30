/**
 * @file Utils.c
 * @brief Utility functions for the Jack Compiler
 * 
 * This file contains utility functions for the Jack Compiler, providing
 * VM code generation helpers and parsing utility functions that are used
 * throughout the compilation process.
 * 
 * The module contains VM code generation functions (push, pop, arithmetic,
 * control flow), token requirement and validation functions, predicate
 * checking functions for parsing decisions, and type checking and validation utilities.
 */

#include "Utils.h"
#include "JackTokenizer.h"

/**
 * @brief Writes a comment to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param comment The comment string to write
 */
void writeVMComment(CompilationEngine * compilationEngine, const char * comment) {
    fprintf(compilationEngine->outputFile, "// %s\n", comment);
}

/**
 * @brief Writes a push command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param segment The memory segment (constant, local, argument, etc.)
 * @param index The index within the segment
 */
void writeVMPush(CompilationEngine * compilationEngine, const char * segment, int index) {
    fprintf(compilationEngine->outputFile, "push %s %d\n", segment, index);
}

/**
 * @brief Writes a pop command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param segment The memory segment (local, argument, static, etc.)
 * @param index The index within the segment
 */
void writeVMPop(CompilationEngine * compilationEngine, const char * segment, int index) {
    fprintf(compilationEngine->outputFile, "pop %s %d\n", segment, index);
}

/**
 * @brief Writes an arithmetic command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param command The arithmetic command (add, sub, neg, etc.)
 */
void writeVMArithmetic(CompilationEngine * compilationEngine, const char * command) {
    fprintf(compilationEngine->outputFile, "%s\n", command);
}

/**
 * @brief Writes a label command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param label The label name
 */
void writeVMLabel(CompilationEngine * compilationEngine, const char * label) {
    fprintf(compilationEngine->outputFile, "label %s\n", label);
}

/**
 * @brief Writes a goto command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param label The label to jump to
 */
void writeVMGoto(CompilationEngine * compilationEngine, const char * label) {
    fprintf(compilationEngine->outputFile, "goto %s\n", label);
}

/**
 * @brief Writes an if-goto command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param label The label to jump to if condition is true
 */
void writeVMIf(CompilationEngine * compilationEngine, const char * label) {
    fprintf(compilationEngine->outputFile, "if-goto %s\n", label);
}

/**
 * @brief Writes a call command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param name The function name to call
 * @param nArgs The number of arguments
 */
void writeVMCall(CompilationEngine * compilationEngine, const char * name, int nArgs) {
    fprintf(compilationEngine->outputFile, "call %s %d\n", name, nArgs);
}

/**
 * @brief Writes a function declaration to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param name The function name
 * @param nLocals The number of local variables
 */
void writeVMFunction(CompilationEngine * compilationEngine, const char * name, int nLocals) {
    fprintf(compilationEngine->outputFile, "function %s %d\n", name, nLocals);
}

/**
 * @brief Writes a return command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void writeVMReturn(CompilationEngine * compilationEngine) {
    fprintf(compilationEngine->outputFile, "return\n");
}

/**
 * @brief Requires a specific token type and optionally a specific value
 * 
 * Validates that the current token matches the expected type and value.
 * If validation fails, the program exits with an error message.
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param expectedType The expected token type
 * @param expectedValue The expected token value (can be NULL)
 */
void require(CompilationEngine * compilationEngine, int expectedType, const char * expectedValue) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        if (expectedValue) {
            fprintf(stderr, "Error: Unexpected end of file, expected %s\n", expectedValue);
        } else {
            fprintf(stderr, "Error: Unexpected end of file\n");
        }
        exit(1);
    }
    
    char * token = getToken(compilationEngine->jackTokenizer);

    if (compilationEngine->jackTokenizer->type != expectedType) {
        if (expectedValue) {
            fprintf(stderr, "Error: Expected %s, got token type %d\n", expectedValue, compilationEngine->jackTokenizer->type);
        } else {
            fprintf(stderr, "Error: Expected token type %d, got token type %d\n", expectedType, compilationEngine->jackTokenizer->type);
        }
        exit(1);
    }
    
    if (expectedValue && strcmp(token, expectedValue) != 0) {
        fprintf(stderr, "Error: Expected %s, got %s\n", expectedValue, token);
        exit(1);
    }
    
    advance(compilationEngine->jackTokenizer);
}

/**
 * @brief Requires a specific symbol token
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param symbol The expected symbol
 */
void requireSymbol(CompilationEngine * compilationEngine, const char * symbol) {
    require(compilationEngine, SYMBOL, symbol);
}

/**
 * @brief Requires a specific keyword token
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param keyword The expected keyword
 */
void requireKeyword(CompilationEngine * compilationEngine, const char * keyword) {
    require(compilationEngine, KEYWORD, keyword);
}

/**
 * @brief Requires an identifier token
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void requireIdentifier(CompilationEngine * compilationEngine) {
    require(compilationEngine, IDENTIFIER, NULL);
}

/**
 * @brief Checks if the current token is a specific keyword
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param keyword The keyword to check for
 * @return true if the current token matches the keyword, false otherwise
 */
bool checkKeyword(CompilationEngine * compilationEngine, const char * keyword) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        return false;
    }

    char * token = getToken(compilationEngine->jackTokenizer);
    return compilationEngine->jackTokenizer->type == KEYWORD && strcmp(token, keyword) == 0;
}

/**
 * @brief Checks if the current token is a specific symbol
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param symbol The symbol to check for
 * @return true if the current token matches the symbol, false otherwise
 */
bool checkSymbol(CompilationEngine * compilationEngine, const char * symbol) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        return false;
    }

    char * token = getToken(compilationEngine->jackTokenizer);
    return compilationEngine->jackTokenizer->type == SYMBOL && strcmp(token, symbol) == 0;
}

/**
 * @brief Checks if the current token is an identifier
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is an identifier, false otherwise
 */
bool checkIdentifier(CompilationEngine * compilationEngine) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        return false;
    }
    return compilationEngine->jackTokenizer->type == IDENTIFIER;
}

/**
 * @brief Checks if the current token is a valid type
 * 
 * Valid types include the keywords "int", "char", "boolean", "void" and identifiers.
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is a valid type, false otherwise
 */
bool isType(CompilationEngine * compilationEngine) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        return false;
    }

    char * token = getToken(compilationEngine->jackTokenizer);
    
    if (compilationEngine->jackTokenizer->type == KEYWORD) {
        return strcmp(token, "int") == 0 || strcmp(token, "char") == 0 || strcmp(token, "boolean") == 0 || strcmp(token, "void") == 0;
    }
    return compilationEngine->jackTokenizer->type == IDENTIFIER;
}

/**
 * @brief Checks if the current token starts a class variable declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "static" or "field", false otherwise
 */
bool isClassVarDec(CompilationEngine * compilationEngine) {
    return checkKeyword(compilationEngine, "static") || checkKeyword(compilationEngine, "field");
}

/**
 * @brief Checks if the current token starts a subroutine declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "constructor", "function", or "method", false otherwise
 */
bool isSubroutine(CompilationEngine * compilationEngine) {
    return checkKeyword(compilationEngine, "constructor") || checkKeyword(compilationEngine, "function") || checkKeyword(compilationEngine, "method");
}

/**
 * @brief Checks if the current token starts a variable declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "var", false otherwise
 */
bool isVarDec(CompilationEngine * compilationEngine) {
    return checkKeyword(compilationEngine, "var");
}

/**
 * @brief Checks if the current token starts a statement
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "let", "if", "while", "do", or "return", false otherwise
 */
bool isStatement(CompilationEngine * compilationEngine) {
    return checkKeyword(compilationEngine, "let") || checkKeyword(compilationEngine, "if") || checkKeyword(compilationEngine, "while") || 
           checkKeyword(compilationEngine, "do") || checkKeyword(compilationEngine, "return");
}

/**
 * @brief Checks if the current token starts a term
 * 
 * Terms include constants, identifiers, parenthesized expressions,
 * unary operations, and keywords like "true", "false", "null", "this".
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token starts a term, false otherwise
 */
bool isTerm(CompilationEngine * compilationEngine) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        return false;
    }

    char * token = getToken(compilationEngine->jackTokenizer);
    
    return compilationEngine->jackTokenizer->type == INTEGER_CONSTANT ||
           compilationEngine->jackTokenizer->type == STRING_CONSTANT ||
           checkIdentifier(compilationEngine) ||
           checkSymbol(compilationEngine, "(") ||
           checkSymbol(compilationEngine, "-") ||
           checkSymbol(compilationEngine, "~") ||
           (compilationEngine->jackTokenizer->type == KEYWORD && 
            (strcmp(token, "true") == 0 || 
             strcmp(token, "false") == 0 || 
             strcmp(token, "null") == 0 || 
             strcmp(token, "this") == 0));
}

/**
 * @brief Checks if the current token is an operator
 * 
 * Operators include +, -, *, /, &, |, <, >, =.
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is an operator, false otherwise
 */
bool isOp(CompilationEngine * compilationEngine) {
    if (!hasMoreTokens(compilationEngine->jackTokenizer)) {
        return false;
    }

    char * token = getToken(compilationEngine->jackTokenizer);

    if (compilationEngine->jackTokenizer->type != SYMBOL) {
        return false;
    }
    
    return strchr("+-*/&|<>=", *token) != NULL;
}
