#ifndef PARSER_H
#define PARSER_H

#include "Config.h"

int getCommandType(const char * line);
bool isNumber(const char * line);
char * removeWhitespace(char * line);
char * getSymbol(const char * line);
char * getDest(const char * line);
char * getComp(const char * line);
char * getJump(const char * line);
void freeParserStrings(char * symbol, char * dest, char * comp, char * jump);

#endif