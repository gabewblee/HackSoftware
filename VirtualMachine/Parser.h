/**
 * @file Parser.h
 * @brief Virtual Machine language parsing header for the Hack VM Translator
 * 
 * This header file declares functions for parsing Hack Virtual Machine language
 * commands, including command type identification, argument extraction, and
 * whitespace removal. The parser handles all VM command types.
 */

#ifndef PARSE_H
#define PARSE_H

#include "Config.h"

/**
 * @brief Determines the type of VM command
 * 
 * @param line The VM command line to analyze
 * @return Command type constant (C_ARITHMETIC, C_PUSH, C_POP, etc.)
 */
int getCommandType(const char * line);

/**
 * @brief Removes whitespace and comments from VM command line
 * 
 * @param line The VM command line to clean
 * @return Pointer to cleaned line, or NULL if line is empty after cleaning
 */
char * removeWhitespace(char * line);

/**
 * @brief Extracts the first argument from a VM command
 * 
 * @param line The VM command line
 * @param commandType The type of command being parsed
 * @param buffer Buffer to store the extracted argument
 * @param bufferSize Size of the buffer
 * @return Pointer to the extracted argument, or NULL if extraction fails
 */
char * getArg1(const char * line, int commandType, char * buffer, size_t bufferSize);

/**
 * @brief Extracts the second argument from a VM command
 * 
 * @param line The VM command line
 * @param buffer Buffer to store the extracted argument
 * @param bufferSize Size of the buffer
 * @return Pointer to the extracted argument, or NULL if extraction fails
 */
char * getArg2(const char * line, char * buffer, size_t bufferSize);

#endif