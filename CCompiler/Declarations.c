/**
 * @file Declarations.c
 * @brief Declaration parsing module for the Jack Compiler
 * 
 * This file contains the declaration parsing implementation for the Jack Compiler.
 * It implements parsing and VM code generation for Jack class variable declarations,
 * parameter lists, variable declarations, subroutine bodies, and subroutine declarations.
 * 
 * The module handles class-level variable declarations (static and field variables),
 * subroutine parameter parsing, local variable declarations, subroutine body parsing,
 * and subroutine declaration parsing (methods, functions, constructors).
 * 
 * Each declaration type manages symbol table entries and generates appropriate
 * VM code for function declarations and variable management.
 */

#include "Declarations.h"
#include "Utils.h"
#include "Statements.h"
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

/**
 * @brief Parses a class variable declaration and adds variables to the symbol table
 * 
 * Handles static and field variable declarations. Supports multiple variables
 * of the same type separated by commas.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseClassVarDec(CompilationEngine * compilationEngine) {
    char * kind = getToken(compilationEngine->jackTokenizer);
    int varKind = (strcmp(kind, "static") == 0) ? STATIC : FIELD;
    advance(compilationEngine->jackTokenizer);
    
    char * type = getToken(compilationEngine->jackTokenizer);
    advance(compilationEngine->jackTokenizer);
    
    char * name = getToken(compilationEngine->jackTokenizer);
    advance(compilationEngine->jackTokenizer);
    
    define(compilationEngine->classSymbolTable, name, type, varKind);
    
    while (checkSymbol(compilationEngine, ",")) {
        requireSymbol(compilationEngine, ",");
        name = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        define(compilationEngine->classSymbolTable, name, type, varKind);
    }
    
    requireSymbol(compilationEngine, ";");
}

/**
 * @brief Parses a parameter list and adds parameters to the symbol table
 * 
 * Handles subroutine parameter declarations. All parameters are added to the
 * subroutine symbol table as arguments (ARG kind).
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseParameterList(CompilationEngine * compilationEngine) {
    if (isType(compilationEngine)) {
        char * type = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        
        char * name = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        
        define(compilationEngine->subroutineSymbolTable, name, type, ARG);
        
        while (checkSymbol(compilationEngine, ",")) {
            requireSymbol(compilationEngine, ",");
            type = getToken(compilationEngine->jackTokenizer);
            advance(compilationEngine->jackTokenizer);
            name = getToken(compilationEngine->jackTokenizer);
            advance(compilationEngine->jackTokenizer);
            define(compilationEngine->subroutineSymbolTable, name, type, ARG);
        }
    }
}

/**
 * @brief Parses a variable declaration and adds variables to the symbol table
 * 
 * Handles local variable declarations within subroutines. Supports multiple
 * variables of the same type separated by commas.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseVarDec(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "var");
    
    char * typeToken = getToken(compilationEngine->jackTokenizer);
    char * type = strdup(typeToken);
    advance(compilationEngine->jackTokenizer);
    
    char * nameToken = getToken(compilationEngine->jackTokenizer);
    char * name = strdup(nameToken);
    advance(compilationEngine->jackTokenizer);
    
    define(compilationEngine->subroutineSymbolTable, name, type, VAR);
    
    while (checkSymbol(compilationEngine, ",")) {
        requireSymbol(compilationEngine, ",");
        name = getToken(compilationEngine->jackTokenizer);
        advance(compilationEngine->jackTokenizer);
        define(compilationEngine->subroutineSymbolTable, name, type, VAR);
    }
    
    requireSymbol(compilationEngine, ";");
}

/**
 * @brief Parses a subroutine body and generates the function declaration
 * 
 * Handles variable declarations, generates the VM function declaration,
 * and parses the subroutine statements.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseSubroutineBody(CompilationEngine * compilationEngine) {
    requireSymbol(compilationEngine, "{");
    
    while (isVarDec(compilationEngine)) {
        parseVarDec(compilationEngine);
    }
    
    char fullFunctionName[256];
    snprintf(fullFunctionName, sizeof(fullFunctionName), "%s.%s", 
             compilationEngine->className, compilationEngine->currentFunctionName);
    writeVMFunction(compilationEngine, fullFunctionName, 
                   varCount(compilationEngine->subroutineSymbolTable, VAR));
    
    parseStatements(compilationEngine);
    requireSymbol(compilationEngine, "}");
}

/**
 * @brief Parses a subroutine declaration and sets up the compilation context
 * 
 * Handles constructor, function, and method declarations. Resets the subroutine
 * symbol table, captures the function name, and parses parameters and body.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseSubroutine(CompilationEngine * compilationEngine) {
    cleanupSymbolTable(compilationEngine->subroutineSymbolTable);
    compilationEngine->subroutineSymbolTable = initSymbolTable();
    
    getToken(compilationEngine->jackTokenizer);
    advance(compilationEngine->jackTokenizer);
    
    getToken(compilationEngine->jackTokenizer);
    advance(compilationEngine->jackTokenizer);
    
    char * functionName = getToken(compilationEngine->jackTokenizer);
    compilationEngine->currentFunctionName = strdup(functionName);
    setFunctionName(compilationEngine->vmWriter, functionName);
    advance(compilationEngine->jackTokenizer);
    
    requireSymbol(compilationEngine, "(");
    parseParameterList(compilationEngine);
    requireSymbol(compilationEngine, ")");
    
    parseSubroutineBody(compilationEngine);
}
