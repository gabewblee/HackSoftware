#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "Config.h"

void setFile(const char * fileName);
void setFunction(const char * functionName);
void writeInit(FILE * outputFile);
void writeLabel(FILE * outputFile, const char * label);
void writeGoto(FILE * outputFile, const char * label);
void writeIf(FILE * outputFile, const char * label);
void writeCall(FILE * outputFile, const char * functionName, int numArgs);
void writeReturn(FILE * outputFile);
void writeFunction(FILE * outputFile, const char * functionName, int numLocals);
void writeArithmetic(FILE * outputFile, const char * command);
void writePushPop(FILE * outputFile, int commandType, const char * segment, const char * index);

#endif