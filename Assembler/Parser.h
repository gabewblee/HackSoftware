/**
 * @file Parser.h
 * @brief Assembly language parsing header for the Hack Assembler
 * 
 * This header file declares functions for parsing Hack assembly language
 * instructions, including command type identification, symbol extraction,
 * and component parsing for A-commands and C-commands.
 */

#ifndef PARSER_H
#define PARSER_H

#include "Config.h"

/**
 * @brief Determines the type of assembly command
 * 
 * @param line The assembly instruction line to analyze
 * @return Command type constant (A_COMMAND, C_COMMAND, or L_COMMAND)
 */
int getCommandType(const char * line);

/**
 * @brief Checks if a string represents a valid decimal number
 * 
 * @param line The string to check for numeric content
 * @return true if the string is a valid number, false otherwise
 */
bool isNumber(const char * line);

/**
 * @brief Removes whitespace and comments from assembly line
 * 
 * @param line The assembly line to clean
 * @return Pointer to cleaned line, or NULL if line is empty after cleaning
 */
char * removeWhitespace(char * line);

/**
 * @brief Extracts symbol from A-command or L-command
 * 
 * @param line The assembly instruction line
 * @return Dynamically allocated string containing the symbol
 */
char * getSymbol(const char * line);

/**
 * @brief Extracts destination field from C-command
 * 
 * @param line The C-command assembly line
 * @return Dynamically allocated string containing destination, or NULL
 */
char * getDest(const char * line);

/**
 * @brief Extracts computation field from C-command
 * 
 * @param line The C-command assembly line
 * @return Dynamically allocated string containing computation
 */
char * getComp(const char * line);

/**
 * @brief Extracts jump field from C-command
 * 
 * @param line The C-command assembly line
 * @return Dynamically allocated string containing jump condition, or NULL
 */
char * getJump(const char * line);

/**
 * @brief Frees memory allocated by parser functions
 * 
 * @param symbol Symbol string to free (can be NULL)
 * @param dest Destination string to free (can be NULL)
 * @param comp Computation string to free (can be NULL)
 * @param jump Jump string to free (can be NULL)
 */
void freeParserStrings(char * symbol, char * dest, char * comp, char * jump);

#endif