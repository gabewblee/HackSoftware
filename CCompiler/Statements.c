/**
 * @file Statements.c
 * @brief Statement parsing module for the Jack Compiler
 * 
 * This file contains the statement parsing implementation for the Jack Compiler.
 * It implements parsing and VM code generation for all Jack statement types.
 * 
 * The module handles let statements (variable assignment, including array access),
 * if-else statements with conditional branching, while loops with conditional
 * iteration, do statements (subroutine calls), return statements (with optional
 * expressions), and compound statements (statement sequences).
 * 
 * Each statement type generates appropriate VM code for control flow,
 * variable manipulation, and subroutine calls.
 */

#include "Statements.h"
#include "Utils.h"
#include "Expressions.h"
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

/**
 * @brief Parses a let statement and generates corresponding VM code
 * 
 * Handles variable assignment including array access. For array assignments,
 * generates code to compute the array address and store the value.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseLet(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "let");
    
    char * varNameToken = getToken(compilationEngine->jackTokenizer);
    char * varName = strdup(varNameToken);
    advance(compilationEngine->jackTokenizer);
    
    if (checkSymbol(compilationEngine, "[")) {
        requireSymbol(compilationEngine, "[");
        parseExpression(compilationEngine);
        requireSymbol(compilationEngine, "]");
        
        writeVMArithmetic(compilationEngine, "add");
    }
    
    requireSymbol(compilationEngine, "=");
    parseExpression(compilationEngine);
    requireSymbol(compilationEngine, ";");
    
    int kind = kindOf(compilationEngine->subroutineSymbolTable, varName);
    int index = -1;
    if (kind != -1) {
        index = indexOf(compilationEngine->subroutineSymbolTable, varName);
    } else {
        kind = kindOf(compilationEngine->classSymbolTable, varName);
        if (kind != -1) {
            index = indexOf(compilationEngine->classSymbolTable, varName);
        }
    }
    
    if (kind != -1 && index != -1) {
        const char * segment = kindToSegment(kind);
        writeVMPop(compilationEngine, segment, index);
    }
}

/**
 * @brief Parses an if statement and generates corresponding VM code
 * 
 * Handles if-else statements with conditional branching. Generates labels
 * for control flow and handles both if and else branches.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseIf(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "if");
    
    char * label1 = generateLabel(compilationEngine->vmWriter, "IF_TRUE");
    char * label2 = generateLabel(compilationEngine->vmWriter, "IF_FALSE");
    char * label3 = generateLabel(compilationEngine->vmWriter, "IF_END");
    
    requireSymbol(compilationEngine, "(");
    parseExpression(compilationEngine);
    requireSymbol(compilationEngine, ")");
    
    writeVMArithmetic(compilationEngine, "not");
    writeVMIf(compilationEngine, label1);
    
    requireSymbol(compilationEngine, "{");
    parseStatements(compilationEngine);
    requireSymbol(compilationEngine, "}");
    
    writeVMGoto(compilationEngine, label3);
    writeVMLabel(compilationEngine, label1);
    
    if (checkKeyword(compilationEngine, "else")) {
        advance(compilationEngine->jackTokenizer);
        requireSymbol(compilationEngine, "{");
        parseStatements(compilationEngine);
        requireSymbol(compilationEngine, "}");
    }
    
    writeVMLabel(compilationEngine, label3);
    
    free(label1);
    free(label2);
    free(label3);
}

/**
 * @brief Parses a while statement and generates corresponding VM code
 * 
 * Handles while loops with conditional iteration. Generates labels for
 * the loop condition check and loop end.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseWhile(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "while");
    
    char * label1 = generateLabel(compilationEngine->vmWriter, "WHILE_EXP");
    char * label2 = generateLabel(compilationEngine->vmWriter, "WHILE_END");
    
    writeVMLabel(compilationEngine, label1);
    
    requireSymbol(compilationEngine, "(");
    parseExpression(compilationEngine);
    requireSymbol(compilationEngine, ")");
    
    writeVMArithmetic(compilationEngine, "not");
    writeVMIf(compilationEngine, label2);
    
    requireSymbol(compilationEngine, "{");
    parseStatements(compilationEngine);
    requireSymbol(compilationEngine, "}");
    
    writeVMGoto(compilationEngine, label1);
    writeVMLabel(compilationEngine, label2);
    
    free(label1);
    free(label2);
}

/**
 * @brief Parses a do statement and generates corresponding VM code
 * 
 * Handles subroutine calls in do statements. Generates a call command
 * and discards the return value by popping it to temp 0.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseDo(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "do");
    parseSubroutineCall(compilationEngine);
    requireSymbol(compilationEngine, ";");
    
    writeVMPop(compilationEngine, "temp", 0);
}

/**
 * @brief Parses a return statement and generates corresponding VM code
 * 
 * Handles return statements with optional expressions. For void returns,
 * pushes constant 0 as the return value.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseReturn(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "return");
    
    if (!checkSymbol(compilationEngine, ";")) {
        parseExpression(compilationEngine);
    } else {
        writeVMPush(compilationEngine, "constant", 0);
    }
    
    requireSymbol(compilationEngine, ";");
    writeVMReturn(compilationEngine);
}

/**
 * @brief Parses a sequence of statements and generates corresponding VM code
 * 
 * Continuously parses statements until no more statement tokens are found.
 * Dispatches to the appropriate statement parser based on the statement type.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseStatements(CompilationEngine * compilationEngine) {
    while (isStatement(compilationEngine)) {
        if (checkKeyword(compilationEngine, "let")) {
            parseLet(compilationEngine);
        } else if (checkKeyword(compilationEngine, "if")) {
            parseIf(compilationEngine);
        } else if (checkKeyword(compilationEngine, "while")) {
            parseWhile(compilationEngine);
        } else if (checkKeyword(compilationEngine, "do")) {
            parseDo(compilationEngine);
        } else if (checkKeyword(compilationEngine, "return")) {
            parseReturn(compilationEngine);
        } else {
            ;
        }
    }
}
