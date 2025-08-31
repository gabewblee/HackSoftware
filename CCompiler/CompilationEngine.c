/**
 * @file CompilationEngine.c
 * @brief Main driver module for the Jack Compiler
 * 
 * This file contains the main driver logic for the Jack Compiler.
 * It coordinates the compilation process by initializing all components
 * and managing the parsing of Jack classes into VM code.
 * 
 * The module handles initialization and cleanup of all compiler components,
 * top-level class parsing and structure, coordination between tokenizer,
 * symbol tables, and VM writer, and memory management for the compilation process.
 */

#include "CompilationEngine.h"
#include "Utils.h"
#include "Statements.h"
#include "Expressions.h"
#include "Declarations.h"
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

/**
 * @brief Initializes a new compilation engine
 * 
 * Allocates and initializes all components of the compilation engine:
 * tokenizer, symbol tables, and VM writer. Advances to the first token.
 * 
 * @param inputFile The input Jack source file
 * @param outputFile The output VM file
 * @return Pointer to the initialized compilation engine, or NULL on failure
 */
CompilationEngine * initCompilationEngine(FILE * inputFile, FILE * outputFile) {
    CompilationEngine * compilationEngine = malloc(sizeof(CompilationEngine));
    if (compilationEngine == NULL) {
        return NULL;
    }
    
    compilationEngine->outputFile = outputFile;
    compilationEngine->jackTokenizer = initJackTokenizer(inputFile);
    if (compilationEngine->jackTokenizer == NULL) {
        free(compilationEngine);
        return NULL;
    }
    
    compilationEngine->classSymbolTable = initSymbolTable();
    if (compilationEngine->classSymbolTable == NULL) {
        cleanupJackTokenizer(compilationEngine->jackTokenizer);
        free(compilationEngine);
        return NULL;
    }
    
    compilationEngine->subroutineSymbolTable = initSymbolTable();
    if (compilationEngine->subroutineSymbolTable == NULL) {
        cleanupSymbolTable(compilationEngine->classSymbolTable);
        cleanupJackTokenizer(compilationEngine->jackTokenizer);
        free(compilationEngine);
        return NULL;
    }
    
    compilationEngine->vmWriter = initVMWriter(outputFile);
    if (compilationEngine->vmWriter == NULL) {
        cleanupSymbolTable(compilationEngine->subroutineSymbolTable);
        cleanupSymbolTable(compilationEngine->classSymbolTable);
        cleanupJackTokenizer(compilationEngine->jackTokenizer);
        free(compilationEngine);
        return NULL;
    }
    
    compilationEngine->className = NULL;
    compilationEngine->currentFunctionName = NULL;
    compilationEngine->labelCounter = 0;
    
    if (!advance(compilationEngine->jackTokenizer)) {
        cleanupVMWriter(compilationEngine->vmWriter);
        cleanupSymbolTable(compilationEngine->subroutineSymbolTable);
        cleanupSymbolTable(compilationEngine->classSymbolTable);
        cleanupJackTokenizer(compilationEngine->jackTokenizer);
        free(compilationEngine);
        return NULL;
    }
    
    return compilationEngine;
}

/**
 * @brief Compiles a Jack class and generates VM code
 * 
 * Parses the class structure including class name, variable declarations,
 * and subroutine declarations. Sets up the class context for compilation.
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void compileClass(CompilationEngine * compilationEngine) {
    requireKeyword(compilationEngine, "class");
    char * className = getToken(compilationEngine->jackTokenizer);
    compilationEngine->className = strdup(className);
    setClassName(compilationEngine->vmWriter, className);
    requireIdentifier(compilationEngine);
    
    requireSymbol(compilationEngine, "{");
    
    while (isClassVarDec(compilationEngine)) {
        parseClassVarDec(compilationEngine);
    }
    
    while (isSubroutine(compilationEngine)) {
        parseSubroutine(compilationEngine);
    }
    
    requireSymbol(compilationEngine, "}");
}

/**
 * @brief Cleans up a compilation engine and frees all allocated memory
 * 
 * Cleans up all components of the compilation engine including tokenizer,
 * symbol tables, VM writer, and the engine itself.
 * 
 * @param compilationEngine Pointer to the compilation engine to clean up
 */
void cleanupCompilationEngine(CompilationEngine * compilationEngine) {
    if (compilationEngine != NULL) {
        if (compilationEngine->jackTokenizer != NULL) {
            cleanupJackTokenizer(compilationEngine->jackTokenizer);
        }
        if (compilationEngine->classSymbolTable != NULL) {
            cleanupSymbolTable(compilationEngine->classSymbolTable);
        }
        if (compilationEngine->subroutineSymbolTable != NULL) {
            cleanupSymbolTable(compilationEngine->subroutineSymbolTable);
        }
        if (compilationEngine->vmWriter != NULL) {
            cleanupVMWriter(compilationEngine->vmWriter);
        }
        free(compilationEngine->className);
        free(compilationEngine->currentFunctionName);
        free(compilationEngine);
    }
}
