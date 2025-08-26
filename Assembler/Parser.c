#include "Parser.h"

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

bool isNumber(const char * line) {
    if (*line == '\0') {
        return false;
    }
    char *end;
    strtol(line, &end, 10);
    return *end == '\0';
}

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


char * getSymbol(const char * line) {
    if (getCommandType(line) == A_COMMAND) {
        const char * sign = strchr(line, '@');
        if (sign == NULL) {
            fprintf(stderr, "Error: No '@' found in A_COMMAND\n");
            exit(1);
        }

        const char * symbolStart = sign + 1;
        size_t length = strlen(symbolStart);
        
        // Remove newline if present
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