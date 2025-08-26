#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Config.h"

void initSymbolTable(SymbolTable * symbolTable);
void addEntry(SymbolTable * symbolTable, const char * symbol, uint16_t address);
bool contains(SymbolTable * symbolTable, const char * symbol);
uint16_t getAddress(SymbolTable * symbolTable, const char * symbol);
void cleanupSymbolTable(SymbolTable * symbolTable);

#endif