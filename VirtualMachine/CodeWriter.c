#include "CodeWriter.h"

static int eqCounter = 0;
static int gtCounter = 0;
static int ltCounter = 0;
static int returnCounter = 0;
static char curr[MAX_FILENAME_LENGTH] = "";
static char currFunction[MAX_FILENAME_LENGTH] = "";

void setFile(const char * fileName) {
    strcpy(curr, fileName);
}

void setFunction(const char * functionName) {
    strcpy(currFunction, functionName);
}

void writeInit(FILE * outputFile) {
    fprintf(outputFile, "@256\n");
    fprintf(outputFile, "D=A\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=D\n");
    writeCall(outputFile, "Sys.init", 0);
}

void writeLabel(FILE * outputFile, const char * label) {
    if (strlen(currFunction) > 0) {
        fprintf(outputFile, "(%s$%s)\n", currFunction, label);
    } else {
        fprintf(outputFile, "(%s)\n", label);
    }
}

void writeGoto(FILE * outputFile, const char * label) {
    if (strlen(currFunction) > 0) {
        fprintf(outputFile, "@%s$%s\n", currFunction, label);
    } else {
        fprintf(outputFile, "@%s\n", label);
    }
    fprintf(outputFile, "0;JMP\n");
}

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

void writeCall(FILE * outputFile, const char * functionName, int numArgs) {
    char returnLabel[MAX_RETURN_COUNTER];
    sprintf(returnLabel, "RETURN%d", returnCounter++);
    
    // push return-address
    fprintf(outputFile, "@%s\n", returnLabel);
    fprintf(outputFile, "D=A\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    // push LCL
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    // push ARG
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    // push THIS
    fprintf(outputFile, "@THIS\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    // push THAT
    fprintf(outputFile, "@THAT\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=M+1\n");
    
    // ARG = SP - n - 5
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@%d\n", numArgs + 5);
    fprintf(outputFile, "D=D-A\n");
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "M=D\n");
    
    // LCL = SP
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "M=D\n");
    
    // goto f
    fprintf(outputFile, "@%s\n", functionName);
    fprintf(outputFile, "0;JMP\n");
    
    // (return-address)
    fprintf(outputFile, "(%s)\n", returnLabel);
}

void writeReturn(FILE * outputFile) {
    // FRAME = LCL
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "M=D\n");
    
    // RET = *(FRAME - 5)
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@5\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@R14\n");
    fprintf(outputFile, "M=D\n");
    
    // *ARG = pop()
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "AM=M-1\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "M=D\n");
    
    // SP = ARG + 1
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "D=M+1\n");
    fprintf(outputFile, "@SP\n");
    fprintf(outputFile, "M=D\n");
    
    // THAT = *(FRAME - 1)
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@1\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@THAT\n");
    fprintf(outputFile, "M=D\n");
    
    // THIS = *(FRAME - 2)
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@2\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@THIS\n");
    fprintf(outputFile, "M=D\n");
    
    // ARG = *(FRAME - 3)
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@3\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@ARG\n");
    fprintf(outputFile, "M=D\n");
    
    // LCL = *(FRAME - 4)
    fprintf(outputFile, "@R13\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@4\n");
    fprintf(outputFile, "A=D-A\n");
    fprintf(outputFile, "D=M\n");
    fprintf(outputFile, "@LCL\n");
    fprintf(outputFile, "M=D\n");
    
    // goto RET
    fprintf(outputFile, "@R14\n");
    fprintf(outputFile, "A=M\n");
    fprintf(outputFile, "0;JMP\n");
}

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
