/**
 * @file VMWriter.c
 * @brief Virtual Machine code writer for the Jack Compiler
 * 
 * This file contains the Virtual Machine code writer implementation for the Jack Compiler.
 * It generates VM commands that can be executed by the Virtual Machine emulator.
 * 
 * The VM writer supports all VM command types: memory access (push/pop),
 * arithmetic and logical operations, control flow (label/goto/if-goto),
 * function calls and returns, and function declarations.
 * 
 * The writer maintains class and function name context for generating
 * properly qualified function names and unique labels for control flow.
 */

#include "Config.h"
#include "VMWriter.h"

/**
 * @brief Initializes a new VM writer
 * 
 * Allocates memory for a VM writer and initializes its fields.
 * 
 * @param outputFile The output file to write VM commands to
 * @return Pointer to the initialized VM writer, or NULL on failure
 */
VMWriter * initVMWriter(FILE * outputFile) {
    VMWriter * vmWriter = malloc(sizeof(VMWriter));
    if (vmWriter == NULL) {
        return NULL;
    }
    
    vmWriter->outputFile = outputFile;
    vmWriter->className = NULL;
    vmWriter->functionName = NULL;
    vmWriter->labelCounter = 0;
    
    return vmWriter;
}

/**
 * @brief Cleans up a VM writer and frees allocated memory
 * 
 * @param vmWriter Pointer to the VM writer to clean up
 */
void cleanupVMWriter(VMWriter * vmWriter) {
    if (vmWriter != NULL) {
        free(vmWriter->className);
        free(vmWriter->functionName);
        free(vmWriter);
    }
}

/**
 * @brief Sets the current class name in the VM writer
 * 
 * @param vmWriter Pointer to the VM writer
 * @param className The class name to set
 */
void setClassName(VMWriter * vmWriter, const char * className) {
    free(vmWriter->className);
    vmWriter->className = strdup(className);
}

/**
 * @brief Sets the current function name in the VM writer
 * 
 * @param vmWriter Pointer to the VM writer
 * @param functionName The function name to set
 */
void setFunctionName(VMWriter * vmWriter, const char * functionName) {
    free(vmWriter->functionName);
    vmWriter->functionName = strdup(functionName);
}

/**
 * @brief Writes a push command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param segment The memory segment
 * @param index The index within the segment
 */
void writePush(VMWriter * vmWriter, const char * segment, int index) {
    fprintf(vmWriter->outputFile, "push %s %d\n", segment, index);
}

/**
 * @brief Writes a pop command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param segment The memory segment
 * @param index The index within the segment
 */
void writePop(VMWriter * vmWriter, const char * segment, int index) {
    fprintf(vmWriter->outputFile, "pop %s %d\n", segment, index);
}

/**
 * @brief Writes an arithmetic command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param command The arithmetic command
 */
void writeArithmetic(VMWriter * vmWriter, const char * command) {
    fprintf(vmWriter->outputFile, "%s\n", command);
}

/**
 * @brief Writes a label command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param label The label name
 */
void writeLabel(VMWriter * vmWriter, const char * label) {
    fprintf(vmWriter->outputFile, "label %s\n", label);
}

/**
 * @brief Writes a goto command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param label The label to jump to
 */
void writeGoto(VMWriter * vmWriter, const char * label) {
    fprintf(vmWriter->outputFile, "goto %s\n", label);
}

/**
 * @brief Writes an if-goto command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param label The label to jump to if condition is true
 */
void writeIf(VMWriter * vmWriter, const char * label) {
    fprintf(vmWriter->outputFile, "if-goto %s\n", label);
}

/**
 * @brief Writes a call command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param name The function name to call
 * @param nArgs The number of arguments
 */
void writeCall(VMWriter * vmWriter, const char * name, int nArgs) {
    fprintf(vmWriter->outputFile, "call %s %d\n", name, nArgs);
}

/**
 * @brief Writes a function declaration to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 * @param name The function name
 * @param nLocals The number of local variables
 */
void writeFunction(VMWriter * vmWriter, const char * name, int nLocals) {
    fprintf(vmWriter->outputFile, "function %s %d\n", name, nLocals);
}

/**
 * @brief Writes a return command to the VM output file
 * 
 * @param vmWriter Pointer to the VM writer
 */
void writeReturn(VMWriter * vmWriter) {
    fprintf(vmWriter->outputFile, "return\n");
}

/**
 * @brief Generates a unique label for control flow
 * 
 * Creates a label by combining the prefix with an incrementing counter.
 * The caller is responsible for freeing the returned string.
 * 
 * @param vmWriter Pointer to the VM writer
 * @param prefix The label prefix
 * @return A unique label string
 */
char * generateLabel(VMWriter * vmWriter, const char * prefix) {
    char * label = malloc(100);
    snprintf(label, 100, "%s_%d", prefix, vmWriter->labelCounter++);
    return label;
}
