/**
 * @file Utils.h
 * @brief Header file for the Utils module
 * 
 * This header file declares utility functions for the Jack Compiler,
 * including VM code generation helpers and parsing utility functions.
 */

#ifndef UTILS_H
#define UTILS_H

#include "Config.h"

/**
 * @brief Writes a comment to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param comment The comment string to write
 */
void writeVMComment(CompilationEngine * compilationEngine, const char * comment);

/**
 * @brief Writes a push command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param segment The memory segment (constant, local, argument, etc.)
 * @param index The index within the segment
 */
void writeVMPush(CompilationEngine * compilationEngine, const char * segment, int index);

/**
 * @brief Writes a pop command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param segment The memory segment (local, argument, static, etc.)
 * @param index The index within the segment
 */
void writeVMPop(CompilationEngine * compilationEngine, const char * segment, int index);

/**
 * @brief Writes an arithmetic command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param command The arithmetic command (add, sub, neg, etc.)
 */
void writeVMArithmetic(CompilationEngine * compilationEngine, const char * command);

/**
 * @brief Writes a label command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param label The label name
 */
void writeVMLabel(CompilationEngine * compilationEngine, const char * label);

/**
 * @brief Writes a goto command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param label The label to jump to
 */
void writeVMGoto(CompilationEngine * compilationEngine, const char * label);

/**
 * @brief Writes an if-goto command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param label The label to jump to if condition is true
 */
void writeVMIf(CompilationEngine * compilationEngine, const char * label);

/**
 * @brief Writes a call command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param name The function name to call
 * @param nArgs The number of arguments
 */
void writeVMCall(CompilationEngine * compilationEngine, const char * name, int nArgs);

/**
 * @brief Writes a function declaration to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param name The function name
 * @param nLocals The number of local variables
 */
void writeVMFunction(CompilationEngine * compilationEngine, const char * name, int nLocals);

/**
 * @brief Writes a return command to the VM output file
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void writeVMReturn(CompilationEngine * compilationEngine);

/**
 * @brief Requires a specific token type and optionally a specific value
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param expectedType The expected token type
 * @param expectedValue The expected token value (can be NULL)
 */
void require(CompilationEngine * compilationEngine, int expectedType, const char * expectedValue);

/**
 * @brief Requires a specific symbol token
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param symbol The expected symbol
 */
void requireSymbol(CompilationEngine * compilationEngine, const char * symbol);

/**
 * @brief Requires a specific keyword token
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param keyword The expected keyword
 */
void requireKeyword(CompilationEngine * compilationEngine, const char * keyword);

/**
 * @brief Requires an identifier token
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void requireIdentifier(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token is a specific keyword
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param keyword The keyword to check for
 * @return true if the current token matches the keyword, false otherwise
 */
bool checkKeyword(CompilationEngine * compilationEngine, const char * keyword);

/**
 * @brief Checks if the current token is a specific symbol
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @param symbol The symbol to check for
 * @return true if the current token matches the symbol, false otherwise
 */
bool checkSymbol(CompilationEngine * compilationEngine, const char * symbol);

/**
 * @brief Checks if the current token is an identifier
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is an identifier, false otherwise
 */
bool checkIdentifier(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token is a valid type
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is a valid type, false otherwise
 */
bool isType(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token starts a class variable declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "static" or "field", false otherwise
 */
bool isClassVarDec(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token starts a subroutine declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "constructor", "function", or "method", false otherwise
 */
bool isSubroutine(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token starts a variable declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "var", false otherwise
 */
bool isVarDec(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token starts a statement
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is "let", "if", "while", "do", or "return", false otherwise
 */
bool isStatement(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token starts a term
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token starts a term, false otherwise
 */
bool isTerm(CompilationEngine * compilationEngine);

/**
 * @brief Checks if the current token is an operator
 * 
 * @param compilationEngine Pointer to the compilation engine
 * @return true if the current token is an operator, false otherwise
 */
bool isOp(CompilationEngine * compilationEngine);

#endif
