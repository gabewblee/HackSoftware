/**
 * @file JackTokenizer.h
 * @brief Header file for the Jack Tokenizer module
 * 
 * This header file declares the public interface for the Jack Tokenizer,
 * which provides lexical analysis for the Jack language.
 */

#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include "Config.h"

/**
 * @brief Initializes a new Jack tokenizer
 * 
 * @param inputFile The input file to tokenize
 * @return Pointer to the initialized tokenizer, or NULL on failure
 */
JackTokenizer * initJackTokenizer(FILE * inputFile);

/**
 * @brief Skips whitespace and comments in the input file
 * 
 * @param inputFile The input file to process
 * @return The next non-whitespace, non-comment character, or EOF
 */
int skipJunk(FILE * inputFile);

/**
 * @brief Checks if there are more tokens to process
 * 
 * @param jackTokenizer Pointer to the tokenizer
 * @return true if there are more tokens, false otherwise
 */
bool hasMoreTokens(JackTokenizer * jackTokenizer);

/**
 * @brief Advances to the next token in the input
 * 
 * @param jackTokenizer Pointer to the tokenizer
 * @return true if a token was successfully read, false otherwise
 */
bool advance(JackTokenizer * jackTokenizer);

/**
 * @brief Checks if a token represents a valid integer constant
 * 
 * @param token The token to check
 * @return true if the token is a valid integer, false otherwise
 */
bool isInteger(char * token);

/**
 * @brief Checks if a token represents a valid string constant
 * 
 * @param token The token to check
 * @return true if the token is a valid string, false otherwise
 */
bool isString(char * token);

/**
 * @brief Checks if a token represents a valid identifier
 * 
 * @param token The token to check
 * @return true if the token is a valid identifier, false otherwise
 */
bool isIdentifier(char * token);

/**
 * @brief Checks if a token represents a keyword
 * 
 * @param token The token to check
 * @return true if the token is a keyword, false otherwise
 */
bool isKeyword(char * token);

/**
 * @brief Checks if a token represents a symbol
 * 
 * @param token The token to check
 * @return true if the token is a symbol, false otherwise
 */
bool isSymbol(char * token);

/**
 * @brief Gets the current token string
 * 
 * @param jackTokenizer Pointer to the tokenizer
 * @return The current token string, or NULL if tokenizer is NULL
 */
char * getToken(JackTokenizer * jackTokenizer);

/**
 * @brief Cleans up a tokenizer and frees allocated memory
 * 
 * @param jackTokenizer Pointer to the tokenizer to clean up
 */
void cleanupJackTokenizer(JackTokenizer * jackTokenizer);

#endif
