#include "CodeWriter.h"

static int eqCounter = 0;
static int gtCounter = 0;
static int ltCounter = 0;

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
        fprintf(outputFile, "@EQEND%d\n", eqCounter);
        fprintf(outputFile, "0;JMP\n");
        fprintf(outputFile, "(EQ%d)\n", eqCounter);
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-1\n");
        fprintf(outputFile, "(EQEND%d)\n", eqCounter);
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
        fprintf(outputFile, "@GTEND%d\n", gtCounter);
        fprintf(outputFile, "0;JMP\n");
        fprintf(outputFile, "(GT%d)\n", gtCounter);
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-1\n");
        fprintf(outputFile, "(GTEND%d)\n", gtCounter);
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
        fprintf(outputFile, "@LTEND%d\n", ltCounter);
        fprintf(outputFile, "0;JMP\n");
        fprintf(outputFile, "(LT%d)\n", ltCounter);
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "A=M-1\n");
        fprintf(outputFile, "M=-1\n");
        fprintf(outputFile, "(LTEND%d)\n", ltCounter);
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
            fprintf(outputFile, "@Static.%s\n", index);
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
            fprintf(outputFile, "@Static.%s\n", index);
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
