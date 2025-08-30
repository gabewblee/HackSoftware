/**
 * @file Expressions.h
 * @brief Header file for the Expressions module
 * 
 * This header file declares expression parsing functions for the Jack Compiler,
 * including parsing for expressions, terms, expression lists, and subroutine calls.
 */

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "Config.h"

/**
 * @brief Parses an expression and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseExpression(CompilationEngine * compilationEngine);

/**
 * @brief Parses a term and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseTerm(CompilationEngine * compilationEngine);

/**
 * @brief Parses an expression list and counts arguments
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseExpressionList(CompilationEngine * compilationEngine);

/**
 * @brief Parses a subroutine call and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseSubroutineCall(CompilationEngine * compilationEngine);

#endif
