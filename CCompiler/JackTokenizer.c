/**
 * @file JackTokenizer.c
 * @brief Jack language tokenizer implementation
 * 
 * This file contains the lexical analyzer for the Jack language.
 * It tokenizes Jack source code by recognizing keywords, symbols,
 * identifiers, integer constants, and string constants.
 * 
 * The tokenizer handles comments, whitespace,
 * and provides a stream-based interface for parsing.
 */

#include "JackTokenizer.h"

/**
 * @brief Initializes a new Jack tokenizer
 * 
 * Allocates memory for a tokenizer and initializes its fields.
 * Sets up the input file and token buffer.
 * 
 * @param inputFile The input file to tokenize
 * @return Pointer to the initialized tokenizer, or NULL on failure
 */
JackTokenizer * initJackTokenizer(FILE * inputFile) {
    if (inputFile == NULL) {
        return NULL;
    }
    
    JackTokenizer * jackTokenizer = malloc(sizeof(JackTokenizer));
    if (jackTokenizer == NULL) {
        return NULL;
    }
    
    jackTokenizer->inputFile = inputFile;
    jackTokenizer->token = malloc(MAX_TOKEN_LENGTH);
    if (jackTokenizer->token == NULL) {
        free(jackTokenizer);
        return NULL;
    }
    
    jackTokenizer->type = INVALID;
    jackTokenizer->hasMore = true;
    return jackTokenizer;
}

/**
 * @brief Skips whitespace and comments in the input file
 * 
 * Handles both single-line and multi-line comments,
 * as well as whitespace characters.
 * 
 * @param file The input file to process
 * @return The next non-whitespace, non-comment character, or EOF
 */
int skipJunk(FILE * file) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) {
            continue;
        }
        
        if (c == '/') {
            int next = fgetc(file);
            if (next == '/') {
                while ((c = fgetc(file)) != EOF && c != '\n') {
                    ;
                }
                continue;
            } else if (next == '*') {
                int prev = -1;
                while ((c = fgetc(file)) != EOF) {
                    if (prev == '*' && c == '/') {
                        break;
                    }
                    prev = c;
                }
                continue;
            } else {
                ungetc(next, file);
                return c;
            }
        }
        
        return c;
    }
    return EOF;
}

/**
 * @brief Checks if there are more tokens to process
 * 
 * @param jackTokenizer Pointer to the tokenizer
 * @return true if there are more tokens, false otherwise
 */
bool hasMoreTokens(JackTokenizer * jackTokenizer) {
    return jackTokenizer->hasMore;
}

/**
 * @brief Advances to the next token in the input
 * 
 * Reads and tokenizes the next token from the input file.
 * Handles string constants, integer constants, identifiers, keywords, and symbols.
 * 
 * @param jackTokenizer Pointer to the tokenizer
 * @return true if a token was successfully read, false otherwise
 */
bool advance(JackTokenizer * jackTokenizer) {
    int c = skipJunk(jackTokenizer->inputFile);
    
    if (c == EOF) {
        jackTokenizer->hasMore = false;
        return false;
    } else if (c == '"') {
        int i = 0;
        while ((c = fgetc(jackTokenizer->inputFile)) != EOF && c != '"' && c != '\n') {
            if (i < MAX_TOKEN_LENGTH - 1) {
                jackTokenizer->token[i++] = c;
            }
        }
        
        if (c != '"') {
            jackTokenizer->hasMore = false;
            return false;
        }
        
        jackTokenizer->token[i] = '\0';
        jackTokenizer->type = STRING_CONSTANT;
        return true;
    } else if (isdigit(c)) {
        int i = 0;
        jackTokenizer->token[i++] = c;
        while ((c = fgetc(jackTokenizer->inputFile)) != EOF && isdigit(c)) {
            if (i < MAX_TOKEN_LENGTH - 1) {
                jackTokenizer->token[i++] = c;
            }
        }
        
        if (c != EOF) {
            ungetc(c, jackTokenizer->inputFile);
        }
        
        jackTokenizer->token[i] = '\0';
        jackTokenizer->type = INTEGER_CONSTANT;
        return true;
    } else if (isalpha(c) || c == '_') {
        int i = 0;
        jackTokenizer->token[i++] = c;
        while ((c = fgetc(jackTokenizer->inputFile)) != EOF && (isalnum(c) || c == '_')) {
            if (i < MAX_TOKEN_LENGTH - 1) {
                jackTokenizer->token[i++] = c;
            }
        }
        
        if (c != EOF) {
            ungetc(c, jackTokenizer->inputFile);
        }
        
        jackTokenizer->token[i] = '\0';
        
        for (int k = 0; k < NUM_KEYWORDS; k++) {
            if (strcmp(jackTokenizer->token, KEYWORDS[k]) == 0) {
                jackTokenizer->type = KEYWORD;
                return true;
            }
        }
        
        jackTokenizer->type = IDENTIFIER;
        return true;
    } else if (strchr(SYMBOLS, c)) {
        *jackTokenizer->token = c;
        jackTokenizer->token[1] = '\0';
        jackTokenizer->type = SYMBOL;
        return true;
    } else {
        jackTokenizer->hasMore = false;
        return false;
    }
}

/**
 * @brief Checks if a token represents a valid integer constant
 * 
 * Validates that the token contains only digits and is within
 * the valid range (0 to 32767).
 * 
 * @param token The token to check
 * @return true if the token is a valid integer, false otherwise
 */
bool isInteger(char * token) {
    if (token == NULL || *token == '\0') {
        return false;
    }
    
    for (char * p = token; *p != '\0'; p++) {
        if (!isdigit(*p)) {
            return false;
        }
    }
    
    int value = atoi(token);
    return value >= 0 && value <= 32767;
}

/**
 * @brief Checks if a token represents a valid string constant
 * 
 * Validates that the token starts and ends with double quotes.
 * 
 * @param token The token to check
 * @return true if the token is a valid string, false otherwise
 */
bool isString(char * token) {
    if (token == NULL || strlen(token) <= 1) {
        return false;
    }
    return *token == '"' && token[strlen(token) - 1] == '"';
}

/**
 * @brief Checks if a token represents a valid identifier
 * 
 * Validates that the token contains only alphanumeric characters
 * and underscores, and is not a keyword.
 * 
 * @param token The token to check
 * @return true if the token is a valid identifier, false otherwise
 */
bool isIdentifier(char * token) {
    if (token == NULL || *token == '\0') {
        return false;
    }
    
    for (char * p = token; *p != '\0'; p++) {
        if (!isalnum(*p) && *p != '_') {
            return false;
        }
    }
    
    return !isKeyword(token);
}

/**
 * @brief Checks if a token represents a keyword
 * 
 * @param token The token to check
 * @return true if the token is a keyword, false otherwise
 */
bool isKeyword(char * token) {
    if (token == NULL) {
        return false;
    }
    
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(token, KEYWORDS[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a token represents a symbol
 * 
 * Validates that the token is a single character and is in the symbols list.
 * 
 * @param token The token to check
 * @return true if the token is a symbol, false otherwise
 */
bool isSymbol(char * token) {
    if (token == NULL || strlen(token) != 1) {
        return false;
    }
    
    return strchr(SYMBOLS, *token) != NULL;
}

/**
 * @brief Gets the current token string
 * 
 * @param jackTokenizer Pointer to the tokenizer
 * @return The current token string, or NULL if tokenizer is NULL
 */
char * getToken(JackTokenizer * jackTokenizer) {
    if (jackTokenizer == NULL) {
        return NULL;
    }
    return jackTokenizer->token;
}

/**
 * @brief Cleans up a tokenizer and frees allocated memory
 * 
 * @param jackTokenizer Pointer to the tokenizer to clean up
 */
void cleanupJackTokenizer(JackTokenizer * jackTokenizer) {
    if (jackTokenizer != NULL) {
        if (jackTokenizer->token != NULL) {
            free(jackTokenizer->token);
        }
        free(jackTokenizer);
    }
}