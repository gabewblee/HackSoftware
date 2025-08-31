/**
 * @file SymbolTable.h
 * @brief Header file for the Symbol Table module
 * 
 * This header file declares the public interface for the Symbol Table,
 * which manages variable and parameter information during compilation.
 */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Config.h"

/**
 * @brief Initializes a new symbol table
 * 
 * @return Pointer to the initialized symbol table, or NULL on failure
 */
SymbolTable * initSymbolTable(void);

/**
 * @brief Cleans up a symbol table and frees all allocated memory
 * 
 * @param symbolTable Pointer to the symbol table to clean up
 */
void cleanupSymbolTable(SymbolTable * symbolTable);

/**
 * @brief Defines a new symbol in the symbol table
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name
 * @param type The symbol type
 * @param kind The symbol kind (STATIC, FIELD, ARG, VAR)
 */
void define(SymbolTable * symbolTable, const char * name, const char * type, int kind);

/**
 * @brief Returns the number of variables of a given kind
 * 
 * @param symbolTable Pointer to the symbol table
 * @param kind The kind to count (STATIC, FIELD, ARG, VAR)
 * @return The number of variables of the specified kind
 */
int varCount(SymbolTable * symbolTable, int kind);

/**
 * @brief Returns the kind of a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name to look up
 * @return The kind of the symbol, or -1 if not found
 */
int kindOf(SymbolTable * symbolTable, const char * name);

/**
 * @brief Returns the type of a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name to look up
 * @return The type of the symbol, or NULL if not found
 */
char * typeOf(SymbolTable * symbolTable, const char * name);

/**
 * @brief Returns the index of a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name to look up
 * @return The index of the symbol, or -1 if not found
 */
int indexOf(SymbolTable * symbolTable, const char * name);

/**
 * @brief Converts a symbol kind to its corresponding VM segment name
 * 
 * @param kind The symbol kind (STATIC, FIELD, ARG, VAR)
 * @return The corresponding VM segment name
 */
const char * kindToSegment(int kind);

#endif
