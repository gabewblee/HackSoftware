#include "Config.h"
#include "CodeWriter.h"
#include "Parser.h"

int main(int argc, char * argv[]) {
    if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: VMTranslator [FILE]\n");
        return 1;
    }    

    char * fileName = argv[1];

    struct stat pathStat;
    if (stat(fileName, &pathStat) == 0) {
        if (S_ISDIR(pathStat.st_mode)) {
            char * dirName = strrchr(fileName, '/');
            if (dirName == NULL) {
                dirName = fileName;
            } else {
                dirName++;
            }
            
            char outputFileName[strlen(fileName) + strlen(dirName) + 5];
            snprintf(outputFileName, sizeof(outputFileName), "%s/%s.asm", fileName, dirName);
            
            FILE * outputFile = fopen(outputFileName, "w");
            if (outputFile == NULL) {
                fprintf(stderr, "Error: Failed to open output file %s\n", outputFileName);
                return 1;
            }
            
            writeInit(outputFile);
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
                    continue;
                }

                char fullPath[MAX_PATH_LENGTH];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", fileName, entry->d_name);
                
                FILE * inputFile = fopen(fullPath, "r");
                if (inputFile == NULL) {
                    fprintf(stderr, "Error: Failed to input file\n");
                    closedir(dir);
                    fclose(outputFile);
                    return 1;
                }
                
                setFile(entry->d_name);

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
                        closedir(dir);
                        fclose(outputFile);
                        return 1;
                    }

                    if (commandType == C_RETURN) {
                        writeReturn(outputFile);
                        continue;
                    }
                    
                    char * arg1 = getArg1(trimmed, commandType, arg1Buffer, sizeof(arg1Buffer));
                    if (arg1 == NULL) {
                        fprintf(stderr, "Error: Failed to get first argument\n");
                        fclose(inputFile);
                        closedir(dir);
                        fclose(outputFile);
                        return 1;
                    }
                    
                    if (commandType == C_PUSH || commandType == C_POP || commandType == C_FUNCTION || commandType == C_CALL) {
                        char * arg2 = getArg2(trimmed, arg2Buffer, sizeof(arg2Buffer));
                        if (arg2 == NULL) {
                            fprintf(stderr, "Error: Failed to get second argument\n");
                            fclose(inputFile);
                            closedir(dir);
                            fclose(outputFile);
                            return 1;
                        }
                        
                        if (commandType == C_PUSH || commandType == C_POP) {
                            writePushPop(outputFile, commandType, arg1, arg2);
                        } else if (commandType == C_FUNCTION) {
                            writeFunction(outputFile, arg1, atoi(arg2));
                        } else if (commandType == C_CALL) {
                            writeCall(outputFile, arg1, atoi(arg2));
                        } else {
                            fprintf(stderr, "Error: Unknown command type\n");
                            fclose(inputFile);
                            closedir(dir);
                            fclose(outputFile);
                            return 1;
                        }
                    } else {
                        if (commandType == C_ARITHMETIC) {
                            writeArithmetic(outputFile, arg1);
                        } else if (commandType == C_LABEL) {
                            writeLabel(outputFile, arg1);
                        } else if (commandType == C_GOTO) {
                            writeGoto(outputFile, arg1);
                        } else if (commandType == C_IF) {
                            writeIf(outputFile, arg1);
                        } else {
                            fprintf(stderr, "Error: Unknown command type\n");
                            fclose(inputFile);
                            closedir(dir);
                            fclose(outputFile);
                            return 1;
                        }
                    }
                }
                
                fclose(inputFile);
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
            
            FILE * inputFile = fopen(fileName, "r");
            if (inputFile == NULL) {
                fprintf(stderr, "Error: Failed to input file\n");
                return 1;
            }
            
            fileName[strlen(fileName) - 3] = '\0';
            setFile(fileName);

            char outputFileName[strlen(fileName) + 5];
            snprintf(outputFileName, sizeof(outputFileName), "%s.asm", fileName);
            
            FILE * outputFile = fopen(outputFileName, "w");
            if (outputFile == NULL) {
                fprintf(stderr, "Error: Failed to open output file %s\n", outputFileName);
                fclose(inputFile);
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
                    fclose(outputFile);
                    return 1;
                }

                if (commandType == C_RETURN) {
                    writeReturn(outputFile);
                    continue;
                }
                
                char * arg1 = getArg1(trimmed, commandType, arg1Buffer, sizeof(arg1Buffer));
                if (arg1 == NULL) {
                    fprintf(stderr, "Error: Failed to get first argument\n");
                    fclose(inputFile);
                    fclose(outputFile);
                    return 1;
                }
                
                if (commandType == C_PUSH || commandType == C_POP || commandType == C_FUNCTION || commandType == C_CALL) {
                    char * arg2 = getArg2(trimmed, arg2Buffer, sizeof(arg2Buffer));
                    if (arg2 == NULL) {
                        fprintf(stderr, "Error: Failed to get second argument\n");
                        fclose(inputFile);
                        fclose(outputFile);
                        return 1;
                    }
                    
                    if (commandType == C_PUSH || commandType == C_POP) {
                        writePushPop(outputFile, commandType, arg1, arg2);
                    } else if (commandType == C_FUNCTION) {
                        writeFunction(outputFile, arg1, atoi(arg2));
                    } else if (commandType == C_CALL) {
                        writeCall(outputFile, arg1, atoi(arg2));
                    } else {
                        fprintf(stderr, "Error: Unknown command type\n");
                        fclose(inputFile);
                        fclose(outputFile);
                        return 1;
                    }
                } else {
                    if (commandType == C_ARITHMETIC) {
                        writeArithmetic(outputFile, arg1);
                    } else if (commandType == C_LABEL) {
                        writeLabel(outputFile, arg1);
                    } else if (commandType == C_GOTO) {
                        writeGoto(outputFile, arg1);
                    } else if (commandType == C_IF) {
                        writeIf(outputFile, arg1);
                    } else {
                        fprintf(stderr, "Error: Unknown command type\n");
                        fclose(inputFile);
                        fclose(outputFile);
                        return 1;
                    }
                }
            }
            
            fclose(inputFile);
            fclose(outputFile);
            return 0;
        } else {
            fprintf(stderr, "Error: Invalid file type\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error: File not found\n");
        return 1;
    }
}