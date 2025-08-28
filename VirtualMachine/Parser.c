/**
 * @file Parser.c
 * @brief Virtual Machine language parsing module for the Hack VM Translator
 * 
 * This file contains functions for parsing Hack Virtual Machine language commands,
 * including command type identification, argument extraction, and whitespace
 * removal. The parser handles all VM command types including arithmetic,
 * memory access, program flow, and function calls.
 */

#include "Parser.h"

/**
 * @brief Determines the type of VM command
 * 
 * Analyzes a line of VM code to determine the command type. Recognizes
 * arithmetic commands (add, sub, neg, eq, gt, lt, and, or, not), memory
 * access commands (push, pop), program flow commands (label, goto, if-goto),
 * and function commands (function, call, return).
 * 
 * @param line The VM command line to analyze
 * @return Command type constant (C_ARITHMETIC, C_PUSH, C_POP, etc.)
 */
int getCommandType(const char * line) {
    char * lineCopy = strdup(line);
    if (lineCopy == NULL) {
        return C_UNKNOWN;
    }
    
    char * command = strtok(lineCopy, " \t\n\r"); 
    int result;
    if (command == NULL) {
        result = C_UNKNOWN;
    } else if (strcmp(command, "add") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "sub") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "neg") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "eq") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "gt") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "lt") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "and") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "or") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "not") == 0) {
        result = C_ARITHMETIC;
    } else if (strcmp(command, "push") == 0) {
        result = C_PUSH;
    } else if (strcmp(command, "pop") == 0) {
        result = C_POP;
    } else if (strcmp(command, "label") == 0) {
        result = C_LABEL;
    } else if (strcmp(command, "goto") == 0) {
        result = C_GOTO;
    } else if (strcmp(command, "if-goto") == 0) {
        result = C_IF;
    } else if (strcmp(command, "function") == 0) {
        result = C_FUNCTION;
    } else if (strcmp(command, "return") == 0) {
        result = C_RETURN;
    } else if (strcmp(command, "call") == 0) {
        result = C_CALL;
    } else {
        result = C_UNKNOWN;
    }
    
    free(lineCopy);
    return result;
}

/**
 * @brief Removes whitespace and comments from VM command line
 * 
 * Strips leading/trailing whitespace and removes inline comments
 * (everything from // to end of line) from a VM command.
 * 
 * @param line The VM command line to clean
 * @return Pointer to cleaned line, or NULL if line is empty after cleaning
 */
char * removeWhitespace(char * line) {
    char * comment = strstr(line, "//");
    if (comment) {
        *comment = '\0';
    }

    while (*line && isspace(*line)) {
        line++;
    }

    char * end = line + strlen(line) - 1;
    while (end > line && isspace(*end)) {
        *end = '\0';
        end--;
    }

    if (*line == '\0') {
        return NULL;
    }

    return line;
}

/**
 * @brief Extracts the first argument from a VM command
 * 
 * For arithmetic commands, returns the command itself as the first argument.
 * For other commands, returns the first argument following the command.
 * 
 * @param line The VM command line
 * @param commandType The type of command being parsed
 * @param buffer Buffer to store the extracted argument
 * @param bufferSize Size of the buffer
 * @return Pointer to the extracted argument, or NULL if extraction fails
 */
char * getArg1(const char * line, int commandType, char * buffer, size_t bufferSize) {
    char * lineCopy = strdup(line);
    if (lineCopy == NULL) {
        return NULL;
    }
    
    char * command = strtok(lineCopy, " \t\n\r");
    if (command == NULL) {
        free(lineCopy);
        return NULL;
    }
    
    const char * source;
    if (commandType == C_ARITHMETIC) {
        source = command;
    } else {
        char * arg1 = strtok(NULL, " \t\n\r");
        if (arg1 == NULL) {
            free(lineCopy);
            return NULL;
        }
        source = arg1;
    }
    
    if (strlen(source) >= bufferSize) {
        free(lineCopy);
        return NULL;
    }
    
    strcpy(buffer, source);
    free(lineCopy);
    return buffer;
}

/**
 * @brief Extracts the second argument from a VM command
 * 
 * Returns the second argument following the command and first argument.
 * Used for commands that require two arguments (push, pop, function, call).
 * 
 * @param line The VM command line
 * @param buffer Buffer to store the extracted argument
 * @param bufferSize Size of the buffer
 * @return Pointer to the extracted argument, or NULL if extraction fails
 */
char * getArg2(const char * line, char * buffer, size_t bufferSize) {
    char * lineCopy = strdup(line);
    if (lineCopy == NULL) {
        return NULL;
    }
    
    char * command = strtok(lineCopy, " \t\n\r");
    if (command == NULL) {
        free(lineCopy);
        return NULL;
    }
    
    char * arg1 = strtok(NULL, " \t\n\r");
    if (arg1 == NULL) {
        free(lineCopy);
        return NULL;
    }
    
    char * arg2 = strtok(NULL, " \t\n\r");
    if (arg2 == NULL) {
        free(lineCopy);
        return NULL;
    }
    
    if (strlen(arg2) >= bufferSize) {
        free(lineCopy);
        return NULL;
    }
    
    strcpy(buffer, arg2);
    free(lineCopy);
    return buffer;
}