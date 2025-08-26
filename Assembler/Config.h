#ifndef CONFIG_H
#define CONFIG_H

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum Lengths
#define MAX_PATH_LENGTH   1024
#define MAX_LINE_LENGTH   1024
#define MAX_NAME_LENGTH   1024
// #define MAX_NUM_SYMBOLS   16384
// #define MAX_NUM_SYMBOLS   300

// Command Types
#define A_COMMAND   -1
#define C_COMMAND    0
#define L_COMMAND    1

// Jumps
#define JUMP_NULL   "000"
#define JGT         "001"
#define JEQ         "010"
#define JGE         "011"
#define JLT         "100"
#define JNE         "101"
#define JLE         "110"
#define JMP         "111"

// Destinations
#define DEST_NULL   "000"
#define M           "001"
#define D           "010"
#define MD          "011"
#define A           "100"
#define AM          "101"
#define AD          "110"
#define AMD         "111"

// Computations: a=0
#define COMP_NULL   "0000000"
#define ZERO        "0101010"
#define ONE         "0111111"
#define NEG_ONE     "0111010"
#define D_REG       "0001100"
#define A_REG       "0110000"
#define NOT_D       "0001101"
#define NOT_A       "0110001"
#define NEG_D       "0001111"
#define NEG_A       "0110011"
#define D_PLUS_1    "0011111"
#define A_PLUS_1    "0110111"
#define D_MINUS_1   "0001110"
#define A_MINUS_1   "0110010"
#define D_PLUS_A    "0000010"
#define D_MINUS_A   "0010011"
#define A_MINUS_D   "0000111"
#define D_AND_A     "0000000"
#define D_OR_A      "0010101"

// Computations: a=1
#define M_REG       "1110000"
#define NOT_M       "1110001"
#define NEG_M       "1110011"
#define M_PLUS_1    "1110111"
#define M_MINUS_1   "1110010"
#define D_PLUS_M    "1000010"
#define D_MINUS_M   "1010011"
#define M_MINUS_D   "1000111"
#define D_AND_M     "1000000"
#define D_OR_M      "1010101"

// Predefined Symbols
#define SP       0
#define LCL      1
#define ARG      2
#define THIS     3
#define THAT     4
#define R0       0
#define R1       1
#define R2       2
#define R3       3
#define R4       4
#define R5       5
#define R6       6
#define R7       7
#define R8       8
#define R9       9
#define R10      10
#define R11      11
#define R12      12
#define R13      13
#define R14      14
#define R15      15
#define SCREEN   16384
#define KBD      24576

// Symbol Definition
typedef struct Symbol{
    char name[MAX_NAME_LENGTH];
    uint16_t address;
} Symbol;

// Symbol Table Definition
typedef struct SymbolTable {
    Symbol symbols[SCREEN];
    uint16_t size;
    uint16_t romAddress;
    uint16_t ramAddress;
} SymbolTable;

#endif