/**
 * @file CompilationEngine.h
 * @brief Header file for the Compilation Engine module
 * 
 * This header file declares the public interface for the Compilation Engine,
 * which orchestrates the entire compilation process from Jack source code
 * to Virtual Machine code.
 */

#ifndef COMPILATIONENGINE_H
#define COMPILATIONENGINE_H

#include "Config.h"

/**
 * @brief Initializes a new compilation engine
 * 
 * @param inputFile The input Jack source file
 * @param outputFile The output VM file
 * @return Pointer to the initialized compilation engine, or NULL on failure
 */
CompilationEngine * initCompilationEngine(FILE * inputFile, FILE * outputFile);

/**
 * @brief Cleans up a compilation engine and frees all allocated memory
 * 
 * @param compilationEngine Pointer to the compilation engine to clean up
 */
void cleanupCompilationEngine(CompilationEngine * compilationEngine);

/**
 * @brief Compiles a Jack class and generates VM code
 * 
 * @param compilationEngine Pointer to the compilation engine
 */
void compileClass(CompilationEngine * compilationEngine);

#endif