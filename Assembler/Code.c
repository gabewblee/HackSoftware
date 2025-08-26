#include "Code.h"

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