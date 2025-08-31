from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine
import Config
import os
import sys

if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] == "-h" or sys.argv[1] == "--help":
        print("Usage: python3 JackAnalyzer.py <input_file_or_directory>")
        sys.exit(1)
    
    filePath = sys.argv[1]
    if os.path.isdir(filePath):
        for inputFile in os.listdir(filePath):
            if inputFile.endswith(".jack"):
                outputFile = inputFile.replace(".jack", ".xml")

                fullInputPath = os.path.join(filePath, inputFile)
                fullOutputPath = os.path.join(filePath, outputFile)

                with open(fullInputPath, "r") as inputFile, open(fullOutputPath, "w") as outputFile:
                    inputFileText = inputFile.read()
                    jackTokenizer, compilationEngine = JackTokenizer(inputFileText), CompilationEngine(outputFile)
                    jackTokenizer.advance()
                    while jackTokenizer.hasMoreTokens():
                        tokenType = jackTokenizer.tokenType()
                        outputFile.write("<" + tokenType + ">")
                        outputFile.write(jackTokenizer.currToken)
                        outputFile.write("</" + tokenType + ">")
                        outputFile.write("\n")
                        jackTokenizer.advance()

    elif os.path.isfile(filePath):
        if not filePath.endswith(".jack"):
            print(f"Error: {filePath} is not a valid file")
            sys.exit(1)

        inputFile = filePath
        outputFile = filePath.replace(".jack", ".xml")

        with open(inputFile, "r") as inputFile, open(outputFile, "w") as outputFile:
            inputFileText = inputFile.read()
            jackTokenizer, compilationEngine = JackTokenizer(inputFileText), CompilationEngine(outputFile)
            jackTokenizer.advance()
            while jackTokenizer.hasMoreTokens():
                tokenType = jackTokenizer.tokenType()
                outputFile.write("<" + tokenType + ">")
                outputFile.write(jackTokenizer.currToken)
                outputFile.write("</" + tokenType + ">")
                outputFile.write("\n")
                jackTokenizer.advance()
    else:
        print(f"Error: {filePath} is not a valid file")