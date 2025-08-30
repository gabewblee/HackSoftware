/**
 * @file Declarations.h
 * @brief Header file for the Declarations module
 * 
 * This header file declares declaration parsing functions for the Jack Compiler,
 * including parsing for class variables, parameters, variables, subroutine bodies,
 * and subroutine declarations.
 */

#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "Config.h"

/**
 * @brief Parses a class variable declaration and adds variables to the symbol table
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseClassVarDec(CompilationEngine * compilationEngine);

/**
 * @brief Parses a parameter list and adds parameters to the symbol table
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseParameterList(CompilationEngine * compilationEngine);

/**
 * @brief Parses a variable declaration and adds variables to the symbol table
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseVarDec(CompilationEngine * compilationEngine);

/**
 * @brief Parses a subroutine body and generates the function declaration
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseSubroutineBody(CompilationEngine * compilationEngine);

/**
 * @brief Parses a subroutine declaration and sets up the compilation context
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void parseSubroutine(CompilationEngine * compilationEngine);

#endif
