#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "Config.h"

void writeArithmetic(FILE * outputFile, const char * command);
void writePushPop(FILE * outputFile, int commandType, const char * segment, const char * index);

#endif