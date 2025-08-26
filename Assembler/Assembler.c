#include "Config.h"
#include "Code.h"
#include "Parser.h"
#include "SymbolTable.h"

int main(int argc, char * argv[]) {
    if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: Assembler [FILE]\n");
        return 1;
    }    

    size_t inputLen = strlen(argv[1]);
    char * fileName = malloc(inputLen + 1);
    if (fileName == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    
    strcpy(fileName, argv[1]);

    char * extension = strrchr(fileName, '.');
    if (extension == NULL || strcmp(extension, ".asm") != 0) {
        fprintf(stderr, "Error: Input file must have .asm extension\n");
        free(fileName);
        return 1;
    }

    size_t newLen = (extension - fileName) + 6;
    char * newFileName = realloc(fileName, newLen);
    if (newFileName == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(fileName);
        return 1;
    }
    fileName = newFileName;
    strcpy(extension, ".hack");

    FILE * inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("fopen failed");
        return 1;
    }

    FILE * outputFile = fopen(fileName, "w");
    if (!outputFile) {
        perror("fopen output failed");
        fclose(inputFile);
        return 1;
    }

    SymbolTable symbolTable;
    initSymbolTable(&symbolTable);
    
    // First Pass: Build Symbol Table
    char currLine[MAX_LINE_LENGTH];
    while(fgets(currLine, sizeof(currLine), inputFile)) {
        char * trimmed = removeWhitespace(currLine);
        if (trimmed == NULL) {
            continue;
        }

        int commandType = getCommandType(trimmed);
        if (commandType == L_COMMAND) {
            char * symbol = getSymbol(trimmed);
            addEntry(&symbolTable, symbol, symbolTable.romAddress);
        } else if (commandType == A_COMMAND || commandType == C_COMMAND) {
            symbolTable.romAddress++;
        } else {
            fprintf(stderr, "Error: Unknown command type\n");
            fclose(inputFile);
            fclose(outputFile);
            cleanupSymbolTable(&symbolTable);
            return 1;
        }
    }

    // Second Pass: Generate Code
    rewind(inputFile);
    const char * toWrite;

    while (fgets(currLine, sizeof(currLine), inputFile)) {
        char * trimmed = removeWhitespace(currLine);
        if (trimmed == NULL) {
            continue;
        }

        int commandType = getCommandType(trimmed);

        if (commandType == A_COMMAND) {
            char * symbol = getSymbol(trimmed);

            if (isNumber(symbol)) {
                toWrite = convertAddress(symbol);
                free(symbol);
            } else {
                if (!contains(&symbolTable, symbol)) {
                    addEntry(&symbolTable, symbol, symbolTable.ramAddress);
                    symbolTable.ramAddress++;
                }
                uint16_t address = getAddress(&symbolTable, symbol);
                char buffer[5];
                snprintf(buffer, sizeof(buffer), "%u", address);
                toWrite = convertAddress(buffer);
                free(symbol);
            }
        } 
        else if (commandType == C_COMMAND) {
            char * dest = getDest(trimmed);
            char * comp = getComp(trimmed);
            char * jump = getJump(trimmed); 
            const char * destBits = convertDest(dest);
            const char * compBits = convertComp(comp);
            const char * jumpBits = convertJump(jump);
            static char binary[17];
            snprintf(binary, sizeof(binary), "111%s%s%s", compBits, destBits, jumpBits);
            toWrite = binary;
            freeParserStrings(NULL, dest, comp, jump);
        } 
        else if (commandType == L_COMMAND) {
            continue;
        } 
        else {
            fprintf(stderr, "Error: Unknown command type\n");
            fclose(inputFile);
            fclose(outputFile);
            cleanupSymbolTable(&symbolTable);
            return 1;
        }

        fprintf(outputFile, "%s\n", toWrite);
    }

    fclose(inputFile);
    fclose(outputFile);
    cleanupSymbolTable(&symbolTable);
    free(fileName);
    return 0;
}