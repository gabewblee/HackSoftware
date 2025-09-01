"""
Jack Language Compiler - Main Module.

This is the main entry point for the Jack language compiler.
"""

from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine
import os
import sys

def main():
    """
    Main entry point for the Jack compiler.
    
    Compiles Jack source files (.jack) to Virtual Machine bytecode (.vm).
    Can process either a single file or all .jack files in a directory.
    
    Command line usage:
        python3 JackCompiler.py <inputFile or directory>
        
    Args:
        Command line argument 1: Path to .jack file or directory containing .jack files
        
    Examples:
        python3 JackCompiler.py Main.jack          # Compile single file
        python3 JackCompiler.py Square/            # Compile all .jack files in directory
    """
    if len(sys.argv) != 2:
        print("Usage: python3 JackCompiler.py <input_file_or_directory>")
        sys.exit(1)
    
    file_path = sys.argv[1]
    
    if os.path.isdir(file_path):
        print(f"Compiling all .jack files in directory: {file_path}")
        for filename in os.listdir(file_path):
            if filename.endswith(".jack"):
                input_file_path = os.path.join(file_path, filename)
                output_filename = filename.replace(".jack", ".vm")
                output_file_path = os.path.join(file_path, output_filename)
                
                print(f"Compiling {filename} -> {output_filename}")
                
                with open(input_file_path, "r") as input_file, open(output_file_path, "w") as output_file:
                    input_file_text = input_file.read()
                    jack_tokenizer = JackTokenizer(input_file_text)
                    compilation_engine = CompilationEngine(output_file, jack_tokenizer)
                    jack_tokenizer.advance()
                    compilation_engine.compileClass()
        
        print("Directory compilation completed successfully")
        
    elif os.path.isfile(file_path):
        if not file_path.endswith(".jack"):
            print(f"Error: '{file_path}' is not a valid .jack file")
            sys.exit(1)
        
        print(f"Compiling single file: {file_path}")
        
        output_file_path = file_path.replace(".jack", ".vm")
        
        with open(file_path, "r") as input_file, open(output_file_path, "w") as output_file:
            input_file_text = input_file.read()
            jack_tokenizer = JackTokenizer(input_file_text)
            compilation_engine = CompilationEngine(output_file, jack_tokenizer)
            jack_tokenizer.advance()
            compilation_engine.compileClass()
        
        print(f"Compilation completed: {output_file_path}")
        
    else:
        print(f"Error: '{file_path}' is not a valid file or directory")
        sys.exit(1)


if __name__ == "__main__":
    main()