/**
 * @file CodeWriter.h
 * @brief Assembly code generation header for the Hack Virtual Machine Translator
 * 
 * This header file declares functions for generating Hack assembly language
 * code from Virtual Machine commands. It includes functions for handling
 * all VM command types including arithmetic, memory access, program flow,
 * and function calls.
 */

#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "Config.h"

/**
 * @brief Sets the current VM file name for static variable naming
 * 
 * @param fileName The name of the current VM file being processed
 */
void setFile(const char * fileName);

/**
 * @brief Sets the current function name for label scoping
 * 
 * @param functionName The name of the current function being processed
 */
void setFunction(const char * functionName);

/**
 * @brief Writes VM initialization code to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 */
void writeInit(FILE * outputFile);

/**
 * @brief Writes label definition to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param label The label name to define
 */
void writeLabel(FILE * outputFile, const char * label);

/**
 * @brief Writes unconditional jump to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param label The label to jump to
 */
void writeGoto(FILE * outputFile, const char * label);

/**
 * @brief Writes conditional jump to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param label The label to jump to if condition is true
 */
void writeIf(FILE * outputFile, const char * label);

/**
 * @brief Writes function call to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param functionName The name of the function to call
 * @param numArgs The number of arguments passed to the function
 */
void writeCall(FILE * outputFile, const char * functionName, int numArgs);

/**
 * @brief Writes function return to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 */
void writeReturn(FILE * outputFile);

/**
 * @brief Writes function definition to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param functionName The name of the function being defined
 * @param numLocals The number of local variables to initialize
 */
void writeFunction(FILE * outputFile, const char * functionName, int numLocals);

/**
 * @brief Writes arithmetic operation to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param command The arithmetic command to translate
 */
void writeArithmetic(FILE * outputFile, const char * command);

/**
 * @brief Writes push or pop operation to assembly output
 * 
 * @param outputFile File pointer to the assembly output file
 * @param commandType The command type (C_PUSH or C_POP)
 * @param segment The memory segment to access
 * @param index The index within the segment
 */
void writePushPop(FILE * outputFile, int commandType, const char * segment, const char * index);

#endif