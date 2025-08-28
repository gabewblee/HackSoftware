/**
 * @file SymbolTable.c
 * @brief Symbol table management module for the Hack Assembler
 * 
 * This file contains functions for managing a symbol table that maps assembly
 * language symbols to their corresponding memory addresses. The symbol table
 * is implemented as a linked list and includes predefined symbols for the
 * Hack computer architecture.
 */

#include "SymbolTable.h"

/**
 * @brief Initializes a symbol table with predefined symbols
 * 
 * Creates a new symbol table and populates it with the standard predefined
 * symbols for the Hack computer architecture, including virtual registers
 * (R0-R15), special purpose registers (SP, LCL, ARG, THIS, THAT), and
 * memory-mapped I/O addresses (SCREEN, KBD).
 * 
 * @param symbolTable Pointer to the symbol table structure to initialize
 */
void initSymbolTable(SymbolTable * symbolTable) {
    symbolTable->head = NULL;
    symbolTable->tail = NULL;
    symbolTable->size = 0;
    symbolTable->romAddress = 0;
    symbolTable->ramAddress = 16;
    addEntry(symbolTable, "SP", 0);
    addEntry(symbolTable, "LCL", 1);
    addEntry(symbolTable, "ARG", 2);
    addEntry(symbolTable, "THIS", 3);
    addEntry(symbolTable, "THAT", 4);
    addEntry(symbolTable, "R0", 0);
    addEntry(symbolTable, "R1", 1);
    addEntry(symbolTable, "R2", 2);
    addEntry(symbolTable, "R3", 3);
    addEntry(symbolTable, "R4", 4);
    addEntry(symbolTable, "R5", 5);
    addEntry(symbolTable, "R6", 6);
    addEntry(symbolTable, "R7", 7);
    addEntry(symbolTable, "R8", 8);
    addEntry(symbolTable, "R9", 9);
    addEntry(symbolTable, "R10", 10);
    addEntry(symbolTable, "R11", 11);
    addEntry(symbolTable, "R12", 12);
    addEntry(symbolTable, "R13", 13);
    addEntry(symbolTable, "R14", 14);
    addEntry(symbolTable, "R15", 15);
    addEntry(symbolTable, "SCREEN", 16384);
    addEntry(symbolTable, "KBD", 24576);
}

/**
 * @brief Adds a new symbol-entry pair to the symbol table
 * 
 * Creates a new symbol entry and adds it to the symbol table if the symbol
 * does not already exist. The symbol is stored as a dynamically allocated
 * string and linked to the specified address.
 * 
 * @param symbolTable Pointer to the symbol table
 * @param symbol The symbol name to add
 * @param address The memory address associated with the symbol
 * @note Function exits with error if memory allocation fails
 */
void addEntry(SymbolTable * symbolTable, const char * symbol, uint16_t address) {
    if (!contains(symbolTable, symbol)) {
        Symbol * newSymbol = malloc(sizeof(Symbol));
        if (newSymbol == NULL) {
            fprintf(stderr, "Error: Memory allocation failed!\n");
            exit(1);
        }
        
        size_t nameLen = strlen(symbol) + 1;
        newSymbol->name = malloc(nameLen);
        if (newSymbol->name == NULL) {
            fprintf(stderr, "Error: Memory allocation failed!\n");
            free(newSymbol);
            exit(1);
        }
        
        strcpy(newSymbol->name, symbol);
        newSymbol->address = address;
        newSymbol->next = NULL;
        
        if (symbolTable->head == NULL) {
            symbolTable->head = newSymbol;
            symbolTable->tail = newSymbol;
        } else {
            symbolTable->tail->next = newSymbol;
            symbolTable->tail = newSymbol;
        }
        symbolTable->size++;
    }
}

/**
 * @brief Checks if a symbol exists in the symbol table
 * 
 * Searches the symbol table for the specified symbol name and returns
 * whether it exists in the table.
 * 
 * @param symbolTable Pointer to the symbol table
 * @param symbol The symbol name to search for
 * @return true if the symbol exists, false otherwise
 */
bool contains(SymbolTable * symbolTable, const char * symbol) {
    Symbol * curr = symbolTable->head;
    while (curr != NULL) {
        if (strcmp(curr->name, symbol) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

/**
 * @brief Retrieves the address associated with a symbol
 * 
 * Searches the symbol table for the specified symbol and returns its
 * associated memory address.
 * 
 * @param symbolTable Pointer to the symbol table
 * @param symbol The symbol name to look up
 * @return The memory address associated with the symbol, or 0xFFFF if not found
 */
uint16_t getAddress(SymbolTable * symbolTable, const char * symbol) {
    Symbol * curr = symbolTable->head;
    while (curr != NULL) {
        if (strcmp(curr->name, symbol) == 0) {
            return curr->address;
        }
        curr = curr->next;
    }
    return 0xFFFF;
}

/**
 * @brief Frees all memory allocated by the symbol table
 * 
 * Deallocates all dynamically allocated memory used by the symbol table,
 * including all symbol names and symbol structures. Resets the table
 * to an empty state.
 * 
 * @param symbolTable Pointer to the symbol table to clean up
 */
void cleanupSymbolTable(SymbolTable * symbolTable) {
    Symbol * curr = symbolTable->head;
    while (curr != NULL) {
        Symbol * next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    symbolTable->head = NULL;
    symbolTable->tail = NULL;
    symbolTable->size = 0;
}