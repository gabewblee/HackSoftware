#ifndef PARSE_H
#define PARSE_H

#include "Config.h"

int getCommandType(const char * line);
char * removeWhitespace(char * line);
char * getArg1(const char * line, int commandType, char * buffer, size_t bufferSize);
char * getArg2(const char * line, char * buffer, size_t bufferSize);

#endif