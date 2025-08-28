/**
 * @file SymbolTable.h
 * @brief Symbol table management header for the Hack Assembler
 * 
 * This header file declares functions for managing a symbol table that maps
 * assembly language symbols to their corresponding memory addresses. The
 * symbol table is implemented as a linked list and includes predefined
 * symbols for the Hack computer architecture.
 */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Config.h"

/**
 * @brief Initializes a symbol table with predefined symbols
 * 
 * @param symbolTable Pointer to the symbol table structure to initialize
 */
void initSymbolTable(SymbolTable * symbolTable);

/**
 * @brief Adds a new symbol-entry pair to the symbol table
 * 
 * @param symbolTable Pointer to the symbol table
 * @param symbol The symbol name to add
 * @param address The memory address associated with the symbol
 */
void addEntry(SymbolTable * symbolTable, const char * symbol, uint16_t address);

/**
 * @brief Checks if a symbol exists in the symbol table
 * 
 * @param symbolTable Pointer to the symbol table
 * @param symbol The symbol name to search for
 * @return true if the symbol exists, false otherwise
 */
bool contains(SymbolTable * symbolTable, const char * symbol);

/**
 * @brief Retrieves the address associated with a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param symbol The symbol name to look up
 * @return The memory address associated with the symbol, or 0xFFFF if not found
 */
uint16_t getAddress(SymbolTable * symbolTable, const char * symbol);

/**
 * @brief Frees all memory allocated by the symbol table
 * 
 * @param symbolTable Pointer to the symbol table to clean up
 */
void cleanupSymbolTable(SymbolTable * symbolTable);

#endif