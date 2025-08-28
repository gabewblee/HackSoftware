/**
 * @file Code.c
 * @brief Binary code generation module for the Hack Assembler
 * 
 * This file contains functions that convert Hack assembly language components
 * (addresses, destinations, computations, and jumps) into their corresponding
 * binary representations according to the Hack machine language specification.
 */

#include "Code.h"

/**
 * @brief Converts a decimal address to 16-bit binary representation
 * 
 * Takes a decimal address string and converts it to a 16-bit binary string
 * with the format "0xxxxxxxxxxxxxxx" where x represents the binary digits.
 * The address must be in the range [0, 32767] (15-bit address space).
 * 
 * @param address String representation of decimal address
 * @return Pointer to static buffer containing 16-bit binary string
 * @note Function exits with error if address is out of valid range
 */
const char * convertAddress(const char * address) {
    static char binary[17];
    int addr = atoi(address);

    if (addr < 0 || addr > 32767) {
        fprintf(stderr, "Error: Address out of range (%d)\n", addr);
        exit(1);
    }

    binary[0] = '0';
    for (int i = 14; i >= 0; i--) {
        binary[15 - i] = ((addr >> i) & 1) ? '1' : '0';
    }
    
    binary[16] = '\0';
    return binary;
}

/**
 * @brief Converts destination mnemonic to 3-bit binary code
 * 
 * Maps destination mnemonics (M, D, MD, A, AM, AD, AMD) to their
 * corresponding 3-bit binary representations as defined in the Hack
 * machine language specification.
 * 
 * @param dest Destination mnemonic string (can be NULL)
 * @return Pointer to 3-bit binary string constant
 */
const char * convertDest(const char * dest) {
    if (dest == NULL) {
        return DEST_NULL;
    } else if (strcmp(dest, "M") == 0) {
        return M;
    } else if (strcmp(dest, "D") == 0) {
        return D;
    } else if (strcmp(dest, "MD") == 0) {
        return MD;
    } else if (strcmp(dest, "A") == 0) {
        return A;
    } else if (strcmp(dest, "AM") == 0) {
        return AM;
    } else if (strcmp(dest, "AD") == 0) {
        return AD;
    } else if (strcmp(dest, "AMD") == 0) {
        return AMD;
    } else {
        return DEST_NULL;
    }
}

/**
 * @brief Converts computation mnemonic to 7-bit binary code
 * 
 * Maps computation mnemonics to their corresponding 7-bit binary
 * representations. The computation field includes the 'a' bit that
 * determines whether to use the A register (a=0) or M register (a=1).
 * 
 * @param comp Computation mnemonic string (can be NULL)
 * @return Pointer to 7-bit binary string constant
 */
const char * convertComp(const char * comp) {
    if (comp == NULL) {
        return COMP_NULL;
    } else if (strcmp(comp, "0") == 0) {
        return ZERO;
    } else if (strcmp(comp, "1") == 0) {
        return ONE;
    } else if (strcmp(comp, "-1") == 0) {
        return NEG_ONE;
    } else if (strcmp(comp, "D") == 0) {
        return D_REG;
    } else if (strcmp(comp, "A") == 0) {
        return A_REG;
    } else if (strcmp(comp, "!D") == 0) {
        return NOT_D;
    } else if (strcmp(comp, "!A") == 0) {
        return NOT_A;
    } else if (strcmp(comp, "-D") == 0) {
        return NEG_D;
    } else if (strcmp(comp, "-A") == 0) {
        return NEG_A;
    } else if (strcmp(comp, "D+1") == 0) {
        return D_PLUS_1;
    } else if (strcmp(comp, "A+1") == 0) {
        return A_PLUS_1;
    } else if (strcmp(comp, "D-1") == 0) {
        return D_MINUS_1;
    } else if (strcmp(comp, "A-1") == 0) {
        return A_MINUS_1;
    } else if (strcmp(comp, "D+A") == 0) {
        return D_PLUS_A;
    } else if (strcmp(comp, "D-A") == 0) {
        return D_MINUS_A;
    } else if (strcmp(comp, "A-D") == 0) {
        return A_MINUS_D;
    } else if (strcmp(comp, "D&A") == 0) {
        return D_AND_A;
    } else if (strcmp(comp, "D|A") == 0) {
        return D_OR_A;
    } else if (strcmp(comp, "M") == 0) {
        return M_REG;
    } else if (strcmp(comp, "!M") == 0) {
        return NOT_M;
    } else if (strcmp(comp, "-M") == 0) {
        return NEG_M;
    } else if (strcmp(comp, "M+1") == 0) {
        return M_PLUS_1;
    } else if (strcmp(comp, "M-1") == 0) {
        return M_MINUS_1;
    } else if (strcmp(comp, "D+M") == 0) {
        return D_PLUS_M;
    } else if (strcmp(comp, "D-M") == 0) {
        return D_MINUS_M;
    } else if (strcmp(comp, "M-D") == 0) {
        return M_MINUS_D;
    } else if (strcmp(comp, "D&M") == 0) {
        return D_AND_M;
    } else if (strcmp(comp, "D|M") == 0) {
        return D_OR_M;
    } else {
        return COMP_NULL;
    }
}

/**
 * @brief Converts jump mnemonic to 3-bit binary code
 * 
 * Maps jump mnemonics (JGT, JEQ, JGE, JLT, JNE, JLE, JMP) to their
 * corresponding 3-bit binary representations as defined in the Hack
 * machine language specification.
 * 
 * @param jump Jump mnemonic string (can be NULL)
 * @return Pointer to 3-bit binary string constant
 */
const char * convertJump(const char * jump) {
    if (jump == NULL) {
        return JUMP_NULL;
    } else if (strcmp(jump, "JGT") == 0) {
        return JGT;
    } else if (strcmp(jump, "JEQ") == 0) {
        return JEQ;
    } else if (strcmp(jump, "JGE") == 0) {
        return JGE;
    } else if (strcmp(jump, "JLT") == 0) {
        return JLT;
    } else if (strcmp(jump, "JNE") == 0) {
        return JNE;
    } else if (strcmp(jump, "JLE") == 0) {
        return JLE;
    } else if (strcmp(jump, "JMP") == 0) {
        return JMP;
    } else {
        return JUMP_NULL;
    }
}