/**
 * @file SymbolTable.c
 * @brief Symbol table implementation for the Jack Compiler
 * 
 * This file contains the symbol table implementation for the Jack Compiler.
 * It manages variable and parameter information during compilation, including
 * their names, types, kinds (static, field, argument, local), and indices.
 * 
 * The symbol table supports two scopes: class-level (for static and field
 * variables) and subroutine-level (for arguments and local variables).
 * It provides functions for defining symbols, looking up symbol information,
 * and counting variables of each kind.
 */

#include "Config.h"
#include "SymbolTable.h"

/**
 * @brief Initializes a new symbol table
 * 
 * Allocates memory for a symbol table with initial capacity of 100 entries.
 * Initializes all counters to zero.
 * 
 * @return Pointer to the initialized symbol table, or NULL on failure
 */
SymbolTable * initSymbolTable(void) {
    SymbolTable * symbolTable = malloc(sizeof(SymbolTable));
    if (symbolTable == NULL) {
        return NULL;
    }
    
    symbolTable->capacity = 100;
    symbolTable->entries = malloc(symbolTable->capacity * sizeof(SymbolEntry));
    if (symbolTable->entries == NULL) {
        free(symbolTable);
        return NULL;
    }
    
    symbolTable->count = 0;
    symbolTable->staticCount = 0;
    symbolTable->fieldCount = 0;
    symbolTable->argCount = 0;
    symbolTable->varCount = 0;
    
    return symbolTable;
}

/**
 * @brief Cleans up a symbol table and frees all allocated memory
 * 
 * Frees all symbol entry names and types, then frees the entries array
 * and the symbol table structure itself.
 * 
 * @param symbolTable Pointer to the symbol table to clean up
 */
void cleanupSymbolTable(SymbolTable * symbolTable) {
    if (symbolTable != NULL) {
        for (int i = 0; i < symbolTable->count; i++) {
            free(symbolTable->entries[i].name);
            free(symbolTable->entries[i].type);
        }
        free(symbolTable->entries);
        free(symbolTable);
    }
}

/**
 * @brief Defines a new symbol in the symbol table
 * 
 * Adds a new symbol entry with the given name, type, and kind. Automatically
 * assigns an index based on the kind and resizes the table if needed.
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name
 * @param type The symbol type
 * @param kind The symbol kind (STATIC, FIELD, ARG, VAR)
 */
void define(SymbolTable * symbolTable, const char * name, const char * type, int kind) {
    if (symbolTable->count >= symbolTable->capacity) {
        symbolTable->capacity *= 2;
        symbolTable->entries = realloc(symbolTable->entries, symbolTable->capacity * sizeof(SymbolEntry));
    }
    
    SymbolEntry * entry = &symbolTable->entries[symbolTable->count];
    entry->name = strdup(name);
    entry->type = strdup(type);
    entry->kind = kind;
    
    switch (kind) {
        case STATIC:
            entry->index = symbolTable->staticCount++;
            break;
        case FIELD:
            entry->index = symbolTable->fieldCount++;
            break;
        case ARG:
            entry->index = symbolTable->argCount++;
            break;
        case VAR:
            entry->index = symbolTable->varCount++;
            break;
    }
    
    symbolTable->count++;
}

/**
 * @brief Returns the number of variables of a given kind
 * 
 * @param symbolTable Pointer to the symbol table
 * @param kind The kind to count (STATIC, FIELD, ARG, VAR)
 * @return The number of variables of the specified kind
 */
int varCount(SymbolTable * symbolTable, int kind) {
    switch (kind) {
        case STATIC: return symbolTable->staticCount;
        case FIELD: return symbolTable->fieldCount;
        case ARG: return symbolTable->argCount;
        case VAR: return symbolTable->varCount;
        default: return 0;
    }
}

/**
 * @brief Returns the kind of a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name to look up
 * @return The kind of the symbol, or -1 if not found
 */
int kindOf(SymbolTable * symbolTable, const char * name) {
    for (int i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->entries[i].name, name) == 0) {
            return symbolTable->entries[i].kind;
        }
    }
    return -1;
}

/**
 * @brief Returns the type of a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name to look up
 * @return The type of the symbol, or NULL if not found
 */
char * typeOf(SymbolTable * symbolTable, const char * name) {
    for (int i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->entries[i].name, name) == 0) {
            return symbolTable->entries[i].type;
        }
    }
    return NULL;
}

/**
 * @brief Returns the index of a symbol
 * 
 * @param symbolTable Pointer to the symbol table
 * @param name The symbol name to look up
 * @return The index of the symbol, or -1 if not found
 */
int indexOf(SymbolTable * symbolTable, const char * name) {
    for (int i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->entries[i].name, name) == 0) {
            return symbolTable->entries[i].index;
        }
    }
    return -1;
}

/**
 * @brief Converts a symbol kind to its corresponding VM segment name
 * 
 * @param kind The symbol kind (STATIC, FIELD, ARG, VAR)
 * @return The corresponding VM segment name
 */
const char * kindToSegment(int kind) {
    switch (kind) {
        case STATIC: return "static";
        case FIELD: return "this";
        case ARG: return "argument";
        case VAR: return "local";
        default: return "unknown";
    }
}
