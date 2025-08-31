/**
 * @file Statements.h
 * @brief Header file for the Statements module
 * 
 * This header file declares statement parsing functions for the Jack Compiler,
 * including parsing for let, if, while, do, return, and compound statements.
 */

#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "Config.h"

/**
 * @brief Parses a let statement and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseLet(CompilationEngine * compilationEngine);

/**
 * @brief Parses an if statement and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseIf(CompilationEngine * compilationEngine);

/**
 * @brief Parses a while statement and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseWhile(CompilationEngine * compilationEngine);

/**
 * @brief Parses a do statement and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseDo(CompilationEngine * compilationEngine);

/**
 * @brief Parses a return statement and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseReturn(CompilationEngine * compilationEngine);

/**
 * @brief Parses a sequence of statements and generates corresponding VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseStatements(CompilationEngine * compilationEngine);

#endif
