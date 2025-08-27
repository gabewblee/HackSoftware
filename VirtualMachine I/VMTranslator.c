#include "Config.h"
#include "CodeWriter.h"
#include "Parser.h"
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

FILE * createOutputFile(const char * basePath, const char * suffix) {
    size_t baseLen = strlen(basePath);
    size_t suffixLen = strlen(suffix);
    size_t outputFileNameLen = baseLen + suffixLen;
    
    char * outputFileName = malloc(outputFileNameLen + 1);
    if (outputFileName == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    
    strcpy(outputFileName, basePath);
    strcat(outputFileName, suffix);
    
    FILE * outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Failed to create output file %s\n", outputFileName);
        free(outputFileName);
        return NULL;
    }
    
    free(outputFileName);
    return outputFile;
}

int processVMFile(const char * inputPath, FILE * outputFile) {
    FILE * inputFile = fopen(inputPath, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Failed to open file %s\n", inputPath);
        return 1;
    }

    char currLine[MAX_LINE_LENGTH];
    char arg1Buffer[MAX_ARG_LENGTH];
    char arg2Buffer[MAX_ARG_LENGTH];
    
    while (fgets(currLine, sizeof(currLine), inputFile)) {
        char * trimmed = removeWhitespace(currLine);
        if (trimmed == NULL) {
            continue;
        }

        int commandType = getCommandType(trimmed);
        if (commandType == C_UNKNOWN) {
            fprintf(stderr, "Error: Unknown command type\n");
            fclose(inputFile);
            return 1;
        }

        if (commandType == C_RETURN) {
            printf("Command: %d (RETURN)\n", commandType);
            continue;
        }
        
        char * arg1 = getArg1(trimmed, commandType, arg1Buffer, sizeof(arg1Buffer));
        if (arg1 == NULL) {
            fprintf(stderr, "Error: Failed to get first argument\n");
            fclose(inputFile);
            return 1;
        }
        
        if (commandType == C_PUSH || commandType == C_POP || commandType == C_FUNCTION || commandType == C_CALL) {
            char * arg2 = getArg2(trimmed, arg2Buffer, sizeof(arg2Buffer));
            if (arg2 == NULL) {
                fprintf(stderr, "Error: Failed to get second argument\n");
                fclose(inputFile);
                return 1;
            }
            
            if (commandType == C_PUSH || commandType == C_POP) {
                writePushPop(outputFile, commandType, arg1, arg2);
            }
        } else {
            if (commandType == C_ARITHMETIC) {
                writeArithmetic(outputFile, arg1);
            }
        }
    }
    
    fclose(inputFile);
    return 0;
}

int main(int argc, char * argv[]) {
    if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: VMTranslator [FILE]\n");
        return 1;
    }    

    char * fileName = argv[1];

    struct stat pathStat;
    if (stat(fileName, &pathStat) == 0) {
        if (S_ISDIR(pathStat.st_mode)) {
            FILE * outputFile = createOutputFile(fileName, ".asm");
            if (outputFile == NULL) {
                return 1;
            }
            
            DIR * dir = opendir(fileName);
            if (dir == NULL) {
                fprintf(stderr, "Error: Failed to open directory\n");
                fclose(outputFile);
                return 1;
            }

            struct dirent * entry;
            while ((entry = readdir(dir)) != NULL) {
                char * extension = strrchr(entry->d_name, '.');
                if (extension == NULL || strcmp(extension, ".vm") != 0) {
                    fprintf(stderr, "Error: Invalid file type\n");
                    closedir(dir);
                    fclose(outputFile);
                    return 1;
                }

                char fullPath[MAX_PATH_LENGTH];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", fileName, entry->d_name);
                
                if (processVMFile(fullPath, outputFile) != 0) {
                    closedir(dir);
                    fclose(outputFile);
                    return 1;
                }
            }
            
            closedir(dir);
            fclose(outputFile);
            return 0;
        } else if (S_ISREG(pathStat.st_mode)) {
            char * extension = strrchr(fileName, '.');
            if (extension == NULL || strcmp(extension, ".vm") != 0) {
                fprintf(stderr, "Error: Invalid file type\n");
                return 1;
            }
            
            char basePath[MAX_PATH_LENGTH];
            size_t fileNameLen = strlen(fileName);
            strncpy(basePath, fileName, fileNameLen - 3);
            basePath[fileNameLen - 3] = '\0';
            
            FILE * outputFile = createOutputFile(basePath, ".asm");
            if (outputFile == NULL) {
                return 1;
            }

            int result = processVMFile(fileName, outputFile);
            
            fclose(outputFile);
            return result;
        } else {
            fprintf(stderr, "Error: Invalid file type\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error: File not found\n");
        return 1;
    }
}