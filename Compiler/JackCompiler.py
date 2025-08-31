from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine
import Config
import os
import sys

if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] == "-h" or sys.argv[1] == "--help":
        print("Usage: python3 JackCompiler.py <input_file_or_directory>")
        sys.exit(1)
    
    filePath = sys.argv[1]
    if os.path.isdir(filePath):
        for inputFile in os.listdir(filePath):
            if inputFile.endswith(".jack"):
                outputFile = inputFile.replace(".jack", ".vm")

                fullInputPath = os.path.join(filePath, inputFile)
                fullOutputPath = os.path.join(filePath, outputFile)

                with open(fullInputPath, "r") as inputFile, open(fullOutputPath, "w") as outputFile:
                    inputFileText = inputFile.read()
                    jackTokenizer = JackTokenizer(inputFileText)
                    compilationEngine = CompilationEngine(outputFile, jackTokenizer)
                    jackTokenizer.advance()
                    compilationEngine.compileClass()

    elif os.path.isfile(filePath):
        if not filePath.endswith(".jack"):
            print(f"Error: {filePath} is not a valid file")
            sys.exit(1)

        inputFile = filePath
        outputFile = filePath.replace(".jack", ".vm")

        with open(inputFile, "r") as inputFile, open(outputFile, "w") as outputFile:
            inputFileText = inputFile.read()
            jackTokenizer = JackTokenizer(inputFileText)
            compilationEngine = CompilationEngine(outputFile, jackTokenizer)
            jackTokenizer.advance()
            compilationEngine.compileClass()
    else:
        print(f"Error: {filePath} is not a valid file")