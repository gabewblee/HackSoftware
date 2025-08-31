/**
 * @file JackCompiler.c
 * @brief Main entry point for the Jack Compiler
 * 
 * This file contains the main entry point for the Jack Compiler (Project 11 from Nand2Tetris),
 * which translates Jack source code (.jack files) into Virtual Machine code (.vm files).
 * The compiler implements a recursive descent parser that translates Jack syntax
 * into VM commands for arithmetic, memory access, control flow, and function calls.
 * 
 * The compiler generates VM code that can be executed by the Virtual Machine
 * emulator, implementing the complete Jack language specification including
 * classes, methods, functions, constructors, variables, expressions, and statements.
 */

#include "Config.h"
#include "CompilationEngine.h"

/**
 * @brief Main entry point for the Jack Compiler
 * 
 * Processes command line arguments and orchestrates the compilation process.
 * Supports both single file compilation and directory processing. For directories,
 * all .jack files are compiled to corresponding .vm files.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on successful compilation, 1 on error
 */
int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.jack or directory>\n", argv[0]);
        return 1;
    }
    
    char * fileName = argv[1];
    struct stat pathStat;
    
    if (stat(fileName, &pathStat) == 0) {
        if (S_ISDIR(pathStat.st_mode)) {
            DIR * dir = opendir(fileName);
            if (dir == NULL) {
                fprintf(stderr, "Error: Failed to open directory\n");
                return 1;
            }

            struct dirent * entry;
            while ((entry = readdir(dir)) != NULL) {
                char * extension = strrchr(entry->d_name, '.');
                if (extension != NULL && strcmp(extension, ".jack") == 0) {
                    char fullPath[strlen(fileName) + 1 + strlen(entry->d_name) + 1];
                    snprintf(fullPath, sizeof(fullPath), "%s/%s", fileName, entry->d_name);

                    FILE * inputFile = fopen(fullPath, "r");
                    if (inputFile == NULL) {
                        fprintf(stderr, "Error: Failed to open input file %s\n", fullPath);
                        closedir(dir);
                        return 1;
                    }

                    char outputFileName[strlen(fullPath)];
                    snprintf(outputFileName, sizeof(outputFileName), "%.*s.vm", (int)(strlen(fullPath) - 5), fullPath);

                    FILE * outputFile = fopen(outputFileName, "w");
                    if (outputFile == NULL) {
                        fprintf(stderr, "Error: Failed to open output file %s\n", outputFileName);
                        fclose(inputFile);
                        closedir(dir);
                        return 1;
                    }
                    
                    CompilationEngine * engine = initCompilationEngine(inputFile, outputFile);
                    if (engine == NULL) {
                        fprintf(stderr, "Error: Failed to initialize compilation engine for %s\n", fullPath);
                        fclose(inputFile);
                        fclose(outputFile);
                        closedir(dir);
                        return 1;
                    }
                    
                    compileClass(engine);
                    cleanupCompilationEngine(engine);
                    fclose(inputFile);
                    fclose(outputFile);
                }
            }
            
            closedir(dir);
            return 0;
        } else if (S_ISREG(pathStat.st_mode)) {
            char * extension = strrchr(fileName, '.');
            if (extension == NULL || strcmp(extension, ".jack") != 0) {
                fprintf(stderr, "Error: Invalid file type\n");
                return 1;
            }

            FILE * inputFile = fopen(fileName, "r");
            if (inputFile == NULL) {
                fprintf(stderr, "Error: Failed to open input file\n");
                return 1;
            }

            char outputFileName[strlen(fileName)];
            snprintf(outputFileName, sizeof(outputFileName), "%.*s.vm", (int)(strlen(fileName) - 5), fileName);

            FILE * outputFile = fopen(outputFileName, "w");
            if (outputFile == NULL) {
                fprintf(stderr, "Error: Failed to open output file\n");
                fclose(inputFile);
                return 1;
            }

            CompilationEngine * compilationEngine = initCompilationEngine(inputFile, outputFile);
            if (compilationEngine == NULL) {
                fprintf(stderr, "Error: Failed to initialize compilation engine\n");
                fclose(inputFile);
                fclose(outputFile);
                return 1;
            }
            
            compileClass(compilationEngine);
            cleanupCompilationEngine(compilationEngine);
            fclose(inputFile);
            fclose(outputFile);
            return 0;
        } else {
            fprintf(stderr, "Error: Invalid file type\n");
        }
    } else {
        fprintf(stderr, "Error: File not found\n");
    }

    return 1;
}
