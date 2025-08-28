/**
 * @file Code.h
 * @brief Binary code generation header for the Hack Assembler
 * 
 * This header file declares functions for converting Hack assembly language
 * components into their corresponding binary representations according to
 * the Hack machine language specification.
 */

#ifndef CODE_H
#define CODE_H

#include "Config.h"

/**
 * @brief Converts a decimal address to 16-bit binary representation
 * 
 * @param address String representation of decimal address
 * @return Pointer to static buffer containing 16-bit binary string
 */
const char * convertAddress(const char * address);

/**
 * @brief Converts destination mnemonic to 3-bit binary code
 * 
 * @param dest Destination mnemonic string (can be NULL)
 * @return Pointer to 3-bit binary string constant
 */
const char * convertDest(const char * dest);

/**
 * @brief Converts computation mnemonic to 7-bit binary code
 * 
 * @param comp Computation mnemonic string (can be NULL)
 * @return Pointer to 7-bit binary string constant
 */
const char * convertComp(const char * comp);

/**
 * @brief Converts jump mnemonic to 3-bit binary code
 * 
 * @param jump Jump mnemonic string (can be NULL)
 * @return Pointer to 3-bit binary string constant
 */
const char * convertJump(const char * jump);

#endif