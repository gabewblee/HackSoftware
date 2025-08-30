/**
 * @file Config.h
 * @brief Configuration and constants header for the Jack Compiler
 * 
 * This header file defines all constants, data structures, and function declarations
 * used throughout the Jack Compiler. It includes token types and lexicon constants,
 * JackTokenizer structure for lexical analysis, SymbolTable structures for variable
 * management, VMWriter structures for code generation, and CompilationEngine
 * structure for orchestration.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <ctype.h>
#include <dirent.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Maximum Lengths
#define MAX_LINE_LENGTH     256
#define MAX_TOKEN_LENGTH    256

// Lexicon Lengths
#define NUM_KEYWORDS     21
#define NUM_SYMBOLS      19

// Keywords
static const char * KEYWORDS[] = { 
    "class", 
    "constructor", 
    "function", 
    "method", 
    "field", 
    "static",
    "var", 
    "int", 
    "char", 
    "boolean", 
    "void", 
    "true", 
    "false", 
    "null",
    "this", 
    "let", 
    "do", 
    "if", 
    "else", 
    "while", 
    "return"
};

// Symbols
static const char SYMBOLS[] = {
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    '.',
    ',',
    ';',
    '+',
    '-',
    '*',
    '/',
    '&',
    '|',
    '<',
    '>',
    '=',
    '~'
};

// Token Types
#define INVALID            -1
#define KEYWORD             0
#define SYMBOL              1
#define INTEGER_CONSTANT    2
#define STRING_CONSTANT     3
#define IDENTIFIER          4

// Jack Tokenizer
typedef struct {
    FILE * inputFile;
    char * token;
    int type;
    bool hasMore;
} JackTokenizer;

// Variable kinds
#define STATIC    0
#define FIELD     1
#define ARG       2
#define VAR       3

// Symbol table entry
typedef struct {
    char * name;
    char * type;
    int kind;
    int index;
} SymbolEntry;

// Symbol table
typedef struct {
    SymbolEntry * entries;
    int count;
    int capacity;
    int staticCount;
    int fieldCount;
    int argCount;
    int varCount;
} SymbolTable;

// VM Writer
typedef struct {
    FILE * outputFile;
    char * className;
    char * functionName;
    int labelCounter;
} VMWriter;

// Compilation Engine
typedef struct {
    FILE * outputFile;
    JackTokenizer * jackTokenizer;
    SymbolTable * classSymbolTable;
    SymbolTable * subroutineSymbolTable;
    VMWriter * vmWriter;
    char * className;
    char * currentFunctionName;
    int labelCounter;
} CompilationEngine;

#endif