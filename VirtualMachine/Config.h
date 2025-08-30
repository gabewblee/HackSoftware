/**
 * @file Config.h
 * @brief Configuration and constants header for the Hack Virtual Machine Translator
 * 
 * This header file defines all constants, configuration values, and command
 * type constants used throughout the VM Translator. It includes maximum
 * length definitions and command type constants for all VM operations.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Maximum Lengths
#define MAX_LINE_LENGTH         256
#define MAX_PATH_LENGTH         256
#define MAX_FILENAME_LENGTH     256
#define MAX_ARG_LENGTH          256
#define MAX_RETURN_COUNTER      256

// Command Types
#define C_ARITHMETIC    0
#define C_PUSH          1
#define C_POP           2
#define C_LABEL         3
#define C_GOTO          4
#define C_IF            5
#define C_FUNCTION      6
#define C_RETURN        7
#define C_CALL          8
#define C_UNKNOWN       9

#endif