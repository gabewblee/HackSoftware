#include "SymbolTable.h"

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