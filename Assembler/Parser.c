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

char * removeWhitespace(char *line) {
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
    static char symbol[MAX_LINE_LENGTH];
    if (getCommandType(line) == A_COMMAND) {
        const char * sign = strchr(line, '@');
        if (sign == NULL) {
            fprintf(stderr, "Error: No '@' found in A_COMMAND\n");
            exit(1);
        }

        strncpy(symbol, sign + 1, sizeof(symbol));
        symbol[sizeof(symbol) - 1] = '\0';
        char * newLine = strchr(symbol, '\n');
        if (newLine != NULL) {
            *newLine = '\0';
        }
        return symbol;
    } else if (getCommandType(line) == L_COMMAND) {
        const char * openParenthesis = strchr(line, '(');
        const char * closeParenthesis = strchr(line, ')');
        if (openParenthesis != NULL && closeParenthesis != NULL && closeParenthesis > openParenthesis) {
            size_t length = (size_t) (closeParenthesis - openParenthesis - 1);
            if (length >= sizeof(symbol)) {
                fprintf(stderr, "Error: Invalid symbol length\n");
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

    static char dest[MAX_LINE_LENGTH];

    size_t len = (size_t)(equalSign - line);
    if (len >= sizeof(dest)) {
        len = sizeof(dest) - 1;
    }

    strncpy(dest, line, len);
    dest[len] = '\0';
    return dest;
}

char * getComp(const char * line) {
    const char *equalSign = strchr(line, '=');
    const char *semicolon = strchr(line, ';');

    static char comp[MAX_LINE_LENGTH];
    size_t len;

    if (equalSign != NULL && semicolon != NULL) {
        len = (size_t)(semicolon - equalSign - 1);
        if (len >= sizeof(comp)) {
            len = sizeof(comp) - 1;
        }

        strncpy(comp, equalSign + 1, len);
        comp[len] = '\0';
    } else if (equalSign != NULL) {
        strncpy(comp, equalSign + 1, sizeof(comp) - 1);
        comp[sizeof(comp) - 1] = '\0';
    } else if (semicolon != NULL) {
        len = (size_t)(semicolon - line);
        if (len >= sizeof(comp)) {
            len = sizeof(comp) - 1;
        }

        strncpy(comp, line, len);
        comp[len] = '\0';
    } else {
        strncpy(comp, line, sizeof(comp) - 1);
        comp[sizeof(comp) - 1] = '\0';
    }

    return comp;
}

char * getJump(const char * line) {
    const char * semicolon = strchr(line, ';');

    if (semicolon == NULL) {
        return NULL;
    }

    static char jump[MAX_LINE_LENGTH];
    strncpy(jump, semicolon + 1, sizeof(jump) - 1);
    jump[sizeof(jump) - 1] = '\0';
    printf("Jump part: '%s'\n", jump);
    return jump;
}