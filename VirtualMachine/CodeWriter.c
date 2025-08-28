/**
 * @file CodeWriter.c
 * @brief Assembly code generation module for the Hack Virtual Machine Translator
 * 
 * This file contains functions that generate Hack assembly language code from
 * Virtual Machine commands. It handles all VM command types including arithmetic
 * operations, memory access (push/pop), program flow control, and function calls
 * with proper stack management and calling conventions.
 */

#include "CodeWriter.h"

static int eqCounter = 0;
static int gtCounter = 0;
static int ltCounter = 0;
static int returnCounter = 0;
static char curr[MAX_FILENAME_LENGTH] = "";
static char currFunction[MAX_FILENAME_LENGTH] = "";

/**
 * @brief Sets the current VM file name for static variable naming
 * 
 * @param fileName The name of the current VM file being processed
 */
void setFile(const char * fileName) {
    strcpy(curr, fileName);
}

/**
 * @brief Sets the current function name for label scoping
 * 
 * @param functionName The name of the current function being processed
 */
void setFunction(const char * functionName) {
    strcpy(currFunction, functionName);
}

/**
 * @brief Writes VM initialization code to assembly output
 * 
 * Generates assembly code to initialize the stack pointer to address 256
 * and calls the Sys.init function to start program execution.
 * 
 * @param outputFile File pointer to the assembly output file
 */
void writeInit(FILE * outputFile) {
    fprintf(outputFile, "@256\n");
    fprintf(outputFile, "D=A\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=D\n");
    writeCall(outputFile, "Sys.init", 0);
}

/**
 * @brief Writes label definition to assembly output
 * 
 * Generates assembly code for a label definition, with proper scoping
 * for function-local labels.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param label The label name to define
 */
void writeLabel(FILE * outputFile, const char * label) {
    if (strlen(currFunction) > 0) {
        fprintf(outputFile, "(%s$%s)\n", currFunction, label);
    } else {
        fprintf(outputFile, "(%s)\n", label);
    }
}

/**
 * @brief Writes unconditional jump to assembly output
 * 
 * Generates assembly code for an unconditional jump to a label,
 * with proper scoping for function-local labels.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param label The label to jump to
 */
void writeGoto(FILE * outputFile, const char * label) {
    if (strlen(currFunction) > 0) {
        fprintf(outputFile, "@%s$%s\n", currFunction, label);
    } else {
        fprintf(outputFile, "@%s\n", label);
    }
    fprintf(outputFile, "0;JMP\n");
}

/**
 * @brief Writes conditional jump to assembly output
 * 
 * Generates assembly code for a conditional jump that pops a value
 * from the stack and jumps if the value is non-zero.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param label The label to jump to if condition is true
 */
void writeIf(FILE * outputFile, const char * label) {
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "AM=M-1\n");
    fprintf(outputFile, "D=M\n");
    if (strlen(currFunction) > 0) {
        fprintf(outputFile, "@%s$%s\n", currFunction, label);
    } else {
        fprintf(outputFile, "@%s\n", label);
    }
    fprintf(outputFile, "D;JNE\n");
}

/**
 * @brief Writes function call to assembly output
 * 
 * Generates assembly code for a function call following the Hack calling
 * convention. Saves return address and frame pointers, sets up new frame,
 * and jumps to the called function.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param functionName The name of the function to call
 * @param numArgs The number of arguments passed to the function
 */
void writeCall(FILE * outputFile, const char * functionName, int numArgs) {
    char returnLabel[MAX_RETURN_COUNTER];
    sprintf(returnLabel, "RETURN%d", returnCounter++);
    
    fprintf(outputFile, "@%s\n", returnLabel);
    fprintf(outputFile, "D=A\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    fprintf(outputFile, "@THIS\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    fprintf(outputFile, "@THAT\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@%d\n", numArgs + 5);
    fprintf(outputFile, "D=D-A\n");
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@%s\n", functionName);
    fprintf(outputFile, "0;JMP\n");
    
    fprintf(outputFile, "(%s)\n", returnLabel);
}

/**
 * @brief Writes function return to assembly output
 * 
 * Generates assembly code for a function return following the Hack calling
 * convention. Restores frame pointers, sets return value, and jumps back
 * to the caller.
 * 
 * @param outputFile File pointer to the assembly output file
 */
void writeReturn(FILE * outputFile) {
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@5\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@R14\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "AM=M-1\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "D=M+1\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@1\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@THAT\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@2\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@THIS\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@3\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@4\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "M=D\n");
    
    fprintf(outputFile, "@R14\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "0;JMP\n");
}

/**
 * @brief Writes function definition to assembly output
 * 
 * Generates assembly code for a function definition, including the function
 * label and initialization of local variables to zero.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param functionName The name of the function being defined
 * @param numLocals The number of local variables to initialize
 */
void writeFunction(FILE * outputFile, const char * functionName, int numLocals) {
    setFunction(functionName);
    fprintf(outputFile, "(%s)\n", functionName);
    
    for (int i = 0; i < numLocals; i++) {
        fprintf(outputFile, "@0\n");
        fprintf(outputFile, "D=A\n");
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M\n");
        fprintf(outputFile, "M=D\n");
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "M=M+1\n");
    }
}

/**
 * @brief Writes arithmetic operation to assembly output
 * 
 * Generates assembly code for VM arithmetic commands including add, sub,
 * neg, eq, gt, lt, and, or, not. Uses unique labels for comparison operations.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param command The arithmetic command to translate
 */
void writeArithmetic(FILE * outputFile, const char * command) {
    if (strcmp(command, "add") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "M=M+D\n");
    } else if (strcmp(command, "sub") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "M=M-D\n");
    } else if (strcmp(command, "neg") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-M\n");
    } else if (strcmp(command, "eq") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "D=M-D\n");
        fprintf(outputFile, "@EQ%d\n", eqCounter);
        fprintf(outputFile, "D;JEQ\n");
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=0\n");
        fprintf(outputFile, "@EQDONE%d\n", eqCounter);
        fprintf(outputFile, "0;JMP\n");
        fprintf(outputFile, "(EQ%d)\n", eqCounter);
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-1\n");
        fprintf(outputFile, "(EQDONE%d)\n", eqCounter);
        eqCounter++;
    } else if (strcmp(command, "gt") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "D=M-D\n");
        fprintf(outputFile, "@GT%d\n", gtCounter);
        fprintf(outputFile, "D;JGT\n");
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=0\n");
        fprintf(outputFile, "@GTDONE%d\n", gtCounter);
        fprintf(outputFile, "0;JMP\n");
        fprintf(outputFile, "(GT%d)\n", gtCounter);
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-1\n");
        fprintf(outputFile, "(GTDONE%d)\n", gtCounter);
        gtCounter++;
    } else if (strcmp(command, "lt") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "D=M-D\n");
        fprintf(outputFile, "@LT%d\n", ltCounter);
        fprintf(outputFile, "D;JLT\n");
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=0\n");
        fprintf(outputFile, "@LTDONE%d\n", ltCounter);
        fprintf(outputFile, "0;JMP\n");
        fprintf(outputFile, "(LT%d)\n", ltCounter);
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-1\n");
        fprintf(outputFile, "(LTDONE%d)\n", ltCounter);
        ltCounter++;
    } else if (strcmp(command, "and") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "M=M&D\n");
    } else if (strcmp(command, "or") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "AM=M-1\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "A=A-1\n");
        fprintf(outputFile, "M=M|D\n");
    } else if (strcmp(command, "not") == 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=!M\n");
    } else {
        fprintf(stderr, "Error: Invalid arithmetic command\n");
    }
}

/**
 * @brief Writes push or pop operation to assembly output
 * 
 * Generates assembly code for VM push and pop commands for all memory
 * segments including constant, local, argument, this, that, static,
 * temp, and pointer segments.
 * 
 * @param outputFile File pointer to the assembly output file
 * @param commandType The command type (C_PUSH or C_POP)
 * @param segment The memory segment to access
 * @param index The index within the segment
 */
void writePushPop(FILE * outputFile, int commandType, const char * segment, const char * index) {
    if (commandType == C_PUSH) {
        if (strcmp(segment, "constant") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "local") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@LCL\n");
            fprintf(outputFile, "A=M+D\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "argument") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@ARG\n");
            fprintf(outputFile, "A=M+D\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "this") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@THIS\n");
            fprintf(outputFile, "A=M+D\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "that") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@THAT\n");
            fprintf(outputFile, "A=M+D\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "static") == 0) {
            fprintf(outputFile, "@%s.%s\n", curr, index);
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "temp") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@5\n");
            fprintf(outputFile, "A=A+D\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else if (strcmp(segment, "pointer") == 0) {
            if (strcmp(index, "0") == 0) {
                fprintf(outputFile, "@THIS\n");
            } else {
                fprintf(outputFile, "@THAT\n");
            }
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M+1\n");
        } else {
            fprintf(stderr, "Error: Invalid segment for push\n");
        }
    } else if (commandType == C_POP) {
        if (strcmp(segment, "local") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@LCL\n");
            fprintf(outputFile, "D=M+D\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
        } else if (strcmp(segment, "argument") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@ARG\n");
            fprintf(outputFile, "D=M+D\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
        } else if (strcmp(segment, "this") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@THIS\n");
            fprintf(outputFile, "D=M+D\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
        } else if (strcmp(segment, "that") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@THAT\n");
            fprintf(outputFile, "D=M+D\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n"); 
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
        } else if (strcmp(segment, "static") == 0) {
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@%s.%s\n", curr, index);
            fprintf(outputFile, "M=D\n");
        } else if (strcmp(segment, "temp") == 0) {
            fprintf(outputFile, "@%s\n", index);
            fprintf(outputFile, "D=A\n");
            fprintf(outputFile, "@5\n");
            fprintf(outputFile, "D=A+D\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "M=D\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@R13\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "M=D\n");
        } else if (strcmp(segment, "pointer") == 0) {
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "AM=M-1\n");
            fprintf(outputFile, "D=M\n");
            if (strcmp(index, "0") == 0) {
                fprintf(outputFile, "@THIS\n");
            } else {
                fprintf(outputFile, "@THAT\n");
            }
            fprintf(outputFile, "M=D\n");
        } else {
            fprintf(stderr, "Error: Invalid segment for pop\n");
        }
    } else {
        fprintf(stderr, "Error: Invalid command type\n");
    }
}
