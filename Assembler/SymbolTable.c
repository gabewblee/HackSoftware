#include "SymbolTable.h"

void initSymbolTable(SymbolTable * symbolTable) {
    symbolTable->symbols[22] = (Symbol){"KBD", 24576};
    symbolTable->symbols[21] = (Symbol){"SCREEN", 16384};
    symbolTable->symbols[20] = (Symbol){"R15", 15};
    symbolTable->symbols[19] = (Symbol){"R14", 14};
    symbolTable->symbols[18] = (Symbol){"R13", 13};
    symbolTable->symbols[17] = (Symbol){"R12", 12};
    symbolTable->symbols[16] = (Symbol){"R11", 11};
    symbolTable->symbols[15] = (Symbol){"R10", 10};
    symbolTable->symbols[14] = (Symbol){"R9", 9};
    symbolTable->symbols[13] = (Symbol){"R8", 8};
    symbolTable->symbols[12] = (Symbol){"R7", 7};
    symbolTable->symbols[11] = (Symbol){"R6", 6};
    symbolTable->symbols[10] = (Symbol){"R5", 5};
    symbolTable->symbols[9]  = (Symbol){"R4", 4};
    symbolTable->symbols[8]  = (Symbol){"R3", 3};
    symbolTable->symbols[7]  = (Symbol){"R2", 2};
    symbolTable->symbols[6]  = (Symbol){"R1", 1};
    symbolTable->symbols[5]  = (Symbol){"R0", 0};
    symbolTable->symbols[4]  = (Symbol){"THAT", 4};
    symbolTable->symbols[3]  = (Symbol){"THIS", 3};
    symbolTable->symbols[2]  = (Symbol){"ARG", 2};
    symbolTable->symbols[1]  = (Symbol){"LCL", 1};
    symbolTable->symbols[0]  = (Symbol){"SP", 0};
    symbolTable->size        = 23;
    symbolTable->romAddress  = 0;
    symbolTable->ramAddress  = 16;
}

void addEntry(SymbolTable * symbolTable, const char * symbol, uint16_t address) {
    if (symbolTable->size < SCREEN) {
        snprintf(symbolTable->symbols[symbolTable->size].name, MAX_NAME_LENGTH, "%s", symbol);
        symbolTable->symbols[symbolTable->size].name[MAX_NAME_LENGTH - 1] = '\0';
        symbolTable->symbols[symbolTable->size].address = address;
        symbolTable->size++;
    } else {
        fprintf(stderr, "Error: Symbol table overflow!\n");
        exit(1);
    }
}

bool contains(SymbolTable * symbolTable, const char * symbol) {
    for (uint16_t i = 0; i < symbolTable->size; i++) {
        if (strcmp(symbolTable->symbols[i].name, symbol) == 0) {
            return true;
        }
    }
    return false;
}

uint16_t getAddress(SymbolTable * symbolTable, const char * symbol) {
    for (uint16_t i = 0; i < symbolTable->size; i++) {
        if (strcmp(symbolTable->symbols[i].name, symbol) == 0) {
            return symbolTable->symbols[i].address;
        }
    }
    return 0xFFFF;
}