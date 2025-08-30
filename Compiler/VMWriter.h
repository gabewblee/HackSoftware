/**
 * @file VMWriter.h
 * @brief Header file for the VM Writer module
 * 
 * This header file declares the public interface for the VM Writer,
 * which generates Virtual Machine code commands for the Jack Compiler.
 */

#ifndef VMWRITER_H
#define VMWRITER_H

#include "Config.h"

/**
 * @brief Initializes a new VM writer
 * 
 * @param outputFile The output file to write VM commands to
 * @return Pointer to the initialized VM writer, or NULL on failure
 */
VMWriter * initVMWriter(FILE * outputFile);

/**
 * @brief Cleans up a VM writer and frees allocated memory
 * 
 * @param vmWriter Pointer to the VM writer to clean up
 */
void cleanupVMWriter(VMWriter * vmWriter);

/**
 * @brief Sets the current class name in the VM writer
 * 
 * @param vmWriter Pointer to the VM writer
 * @param className The class name to set
 */
void setClassName(VMWriter * vmWriter, const char * className);

/**
 * @brief Sets the current function name in the VM writer
 * 
 * @param vmWriter Pointer to the VM writer
 * @param functionName The function name to set
 */
void setFunctionName(VMWriter * vmWriter, const char * functionName);

/**
 * @brief Writes a push command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param segment The memory segment
 * @param index The index within the segment
 */
void writePush(VMWriter * vmWriter, const char * segment, int index);

/**
 * @brief Writes a pop command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param segment The memory segment
 * @param index The index within the segment
 */
void writePop(VMWriter * vmWriter, const char * segment, int index);

/**
 * @brief Writes an arithmetic command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param command The arithmetic command
 */
void writeArithmetic(VMWriter * vmWriter, const char * command);

/**
 * @brief Writes a label command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param label The label name
 */
void writeLabel(VMWriter * vmWriter, const char * label);

/**
 * @brief Writes a goto command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param label The label to jump to
 */
void writeGoto(VMWriter * vmWriter, const char * label);

/**
 * @brief Writes an if-goto command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param label The label to jump to if condition is true
 */
void writeIf(VMWriter * vmWriter, const char * label);

/**
 * @brief Writes a call command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param name The function name to call
 * @param nArgs The number of arguments
 */
void writeCall(VMWriter * vmWriter, const char * name, int nArgs);

/**
 * @brief Writes a function declaration to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param name The function name
 * @param nLocals The number of local variables
 */
void writeFunction(VMWriter * vmWriter, const char * name, int nLocals);

/**
 * @brief Writes a return command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 */
void writeReturn(VMWriter * vmWriter);

/**
 * @brief Generates a unique label for control flow
 * 
 * @param vmWriter Pointer to the VM writer
 * @param prefix The label prefix
 * @return A unique label string
 */
char * generateLabel(VMWriter * vmWriter, const char * prefix);

#endif
