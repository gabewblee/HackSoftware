/**
 * @file Parser.c
 * @brief Assembly language parsing module for the Hack Assembler
 * 
 * This file contains functions for parsing Hack assembly language instructions,
 * including command type identification, symbol extraction, and component
 * parsing for A-commands and C-commands.
 */

#include "Parser.h"

/**
 * @brief Determines the type of assembly command
 * 
 * Analyzes a line of assembly code to determine whether it is an A-command
 * (address instruction), C-command (compute instruction), or L-command
 * (label definition).
 * 
 * @param line The assembly instruction line to analyze
 * @return Command type constant (A_COMMAND, C_COMMAND, or L_COMMAND)
 * @note Function exits with error if command type cannot be determined
 */
int getCommandType(const char * line) {
    if (strchr(line, '@') != NULL) { 
        return A_COMMAND;
    } else if (strchr(line, '=') != NULL || strchr(line, ';') != NULL) {
        return C_COMMAND;
    } else if (strchr(line, '(') != NULL && strchr(line, ')') != NULL) {
        return L_COMMAND;
    } else {
        fprintf(stderr, "Error: Invalid command type\n");
        exit(1);
    }
}

/**
 * @brief Checks if a string represents a valid decimal number
 * 
 * Validates whether a string contains only decimal digits and represents
 * a valid integer value.
 * 
 * @param line The string to check for numeric content
 * @return true if the string is a valid number, false otherwise
 */
bool isNumber(const char * line) {
    if (*line == '\0') {
        return false;
    }
    char *end;
    strtol(line, &end, 10);
    return *end == '\0';
}

/**
 * @brief Removes whitespace and comments from assembly line
 * 
 * Strips leading/trailing whitespace and removes inline comments
 * (everything from // to end of line) from an assembly instruction.
 * 
 * @param line The assembly line to clean
 * @return Pointer to cleaned line, or NULL if line is empty after cleaning
 */
char * removeWhitespace(char * line) {
    char * comment = strstr(line, "//");
    if (comment) {
        *comment = '\0';
    }

    char * src = line;
    char * dst = line;

    while (*src) {
        if (!isspace(*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';

    if (*line == '\0') {
        return NULL;
    }
    return line;
}

/**
 * @brief Extracts symbol from A-command or L-command
 * 
 * For A-commands, extracts the symbol following the @ symbol.
 * For L-commands, extracts the label name between parentheses.
 * 
 * @param line The assembly instruction line
 * @return Dynamically allocated string containing the symbol
 * @note Caller is responsible for freeing the returned string
 * @note Function exits with error if symbol cannot be extracted
 */
char * getSymbol(const char * line) {
    if (getCommandType(line) == A_COMMAND) {
        const char * sign = strchr(line, '@');
        if (sign == NULL) {
            fprintf(stderr, "Error: No '@' found in A_COMMAND\n");
            exit(1);
        }

        const char * symbolStart = sign + 1;
        size_t length = strlen(symbolStart);
        
        if (length > 0 && symbolStart[length - 1] == '\n') {
            length--;
        }
        
        char * symbol = malloc(length + 1);
        if (symbol == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        
        strncpy(symbol, symbolStart, length);
        symbol[length] = '\0';
        return symbol;
    } else if (getCommandType(line) == L_COMMAND) {
        const char * openParenthesis = strchr(line, '(');
        const char * closeParenthesis = strchr(line, ')');
        if (openParenthesis != NULL && closeParenthesis != NULL && closeParenthesis > openParenthesis) {
            size_t length = (size_t) (closeParenthesis - openParenthesis - 1);
            
            char * symbol = malloc(length + 1);
            if (symbol == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                exit(1);
            }
            
            strncpy(symbol, openParenthesis + 1, length);
            symbol[length] = '\0';
            return symbol;
        } else {
            fprintf(stderr, "Error: Invalid L_COMMAND format\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Error: getSymbol called on non A_COMMAND and non L_COMMAND\n");
        exit(1);
    }
}

/**
 * @brief Extracts destination field from C-command
 * 
 * Parses the destination part of a C-command (everything before the = sign).
 * Returns NULL if no destination is specified.
 * 
 * @param line The C-command assembly line
 * @return Dynamically allocated string containing destination, or NULL
 * @note Caller is responsible for freeing the returned string
 */
char * getDest(const char * line) {
    char * equalSign = strchr(line, '=');
    if (equalSign == NULL) {
        return NULL;
    }

    size_t len = (size_t)(equalSign - line);
    
    char * dest = malloc(len + 1);
    if (dest == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    strncpy(dest, line, len);
    dest[len] = '\0';
    return dest;
}

/**
 * @brief Extracts computation field from C-command
 * 
 * Parses the computation part of a C-command, which is the portion between
 * the = and ; signs, or the entire command if no = or ; is present.
 * 
 * @param line The C-command assembly line
 * @return Dynamically allocated string containing computation
 * @note Caller is responsible for freeing the returned string
 */
char * getComp(const char * line) {
    const char * equalSign = strchr(line, '=');
    const char * semicolon = strchr(line, ';');

    size_t len;
    char * comp;

    if (equalSign != NULL && semicolon != NULL) {
        len = (size_t)(semicolon - equalSign - 1);
        comp = malloc(len + 1);
        if (comp == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strncpy(comp, equalSign + 1, len);
        comp[len] = '\0';
    } else if (equalSign != NULL) {
        len = strlen(equalSign + 1);
        comp = malloc(len + 1);
        if (comp == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strcpy(comp, equalSign + 1);
    } else if (semicolon != NULL) {
        len = (size_t)(semicolon - line);
        comp = malloc(len + 1);
        if (comp == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strncpy(comp, line, len);
        comp[len] = '\0';
    } else {
        len = strlen(line);
        comp = malloc(len + 1);
        if (comp == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strcpy(comp, line);
    }

    return comp;
}

/**
 * @brief Extracts jump field from C-command
 * 
 * Parses the jump part of a C-command (everything after the ; sign).
 * Returns NULL if no jump condition is specified.
 * 
 * @param line The C-command assembly line
 * @return Dynamically allocated string containing jump condition, or NULL
 * @note Caller is responsible for freeing the returned string
 */
char * getJump(const char * line) {
    const char * semicolon = strchr(line, ';');

    if (semicolon == NULL) {
        return NULL;
    }

    const char * jumpStart = semicolon + 1;
    size_t length = strlen(jumpStart);
    
    if (length > 0 && jumpStart[length - 1] == '\n') {
        length--;
    }
    
    char * jump = malloc(length + 1);
    if (jump == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    strncpy(jump, jumpStart, length);
    jump[length] = '\0';
    
    return jump;
}

/**
 * @brief Frees memory allocated by parser functions
 * 
 * Convenience function to free multiple dynamically allocated strings
 * returned by parser functions. NULL pointers are safely ignored.
 * 
 * @param symbol Symbol string to free (can be NULL)
 * @param dest Destination string to free (can be NULL)
 * @param comp Computation string to free (can be NULL)
 * @param jump Jump string to free (can be NULL)
 */
void freeParserStrings(char * symbol, char * dest, char * comp, char * jump) {
    if (symbol != NULL) {
        free(symbol);
    }

    if (dest != NULL) {
        free(dest);
    }

    if (comp != NULL) {
        free(comp);
    }

    if (jump != NULL) {
        free(jump);
    }
}