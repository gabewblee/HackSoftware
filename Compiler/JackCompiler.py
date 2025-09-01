"""
Jack Language Compiler - Main Module.

This is the main entry point for the Jack language compiler. It provides
a command-line interface for compiling Jack source files (.jack) into
Virtual Machine bytecode (.vm) files.

The compiler can process either individual Jack files or entire directories
containing multiple Jack files. It orchestrates the compilation process
by coordinating the tokenizer, compilation engine, and VM writer.

Usage:
    python3 JackCompiler.py <input_file_or_directory>
    
Examples:
    python3 JackCompiler.py Main.jack          # Compile single file
    python3 JackCompiler.py Square/            # Compile all .jack files in directory
"""

from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine
import Config
import os
import sys


def compile_jack_file(input_path: str, output_path: str) -> None:
    """
    Compile a single Jack source file to VM bytecode.
    
    This function handles the complete compilation pipeline for a single
    Jack file: reading the source, tokenizing, parsing, and generating
    VM code.
    
    Args:
        input_path (str): Path to the input .jack file
        output_path (str): Path where the output .vm file should be written
        
    Raises:
        FileNotFoundError: If the input file doesn't exist
        IOError: If there are issues reading/writing files
        SyntaxError: If the Jack source contains syntax errors
    """
    try:
        with open(input_path, "r") as input_file, open(output_path, "w") as output_file:
            # Read the entire source file
            input_file_text = input_file.read()
            
            # Create tokenizer and parse the source
            jack_tokenizer = JackTokenizer(input_file_text)
            
            # Create compilation engine with tokenizer and output file
            compilation_engine = CompilationEngine(output_file, jack_tokenizer)
            
            # Start compilation by advancing to first token and compiling the class
            jack_tokenizer.advance()
            compilation_engine.compileClass()
            
    except FileNotFoundError:
        print(f"Error: Input file '{input_path}' not found")
        sys.exit(1)
    except IOError as e:
        print(f"Error: IO error processing '{input_path}': {e}")
        sys.exit(1)
    except SyntaxError as e:
        print(f"Error: Syntax error in '{input_path}': {e}")
        sys.exit(1)


def compile_directory(directory_path: str) -> None:
    """
    Compile all Jack files in a directory.
    
    Iterates through all .jack files in the specified directory and
    compiles each one to a corresponding .vm file in the same directory.
    
    Args:
        directory_path (str): Path to directory containing .jack files
        
    Note:
        Output .vm files are created in the same directory as the input
        .jack files, with the same base name but .vm extension.
    """
    try:
        for filename in os.listdir(directory_path):
            if filename.endswith(".jack"):
                # Construct input and output file paths
                input_file_path = os.path.join(directory_path, filename)
                output_filename = filename.replace(".jack", ".vm")
                output_file_path = os.path.join(directory_path, output_filename)
                
                print(f"Compiling {filename} -> {output_filename}")
                
                # Compile the individual file
                compile_jack_file(input_file_path, output_file_path)
                
        print(f"Directory compilation completed successfully")
        
    except OSError as e:
        print(f"Error: Failed to process directory '{directory_path}': {e}")
        sys.exit(1)


def main() -> None:
    """
    Main entry point for the Jack compiler.
    
    Parses command line arguments and initiates compilation of either
    a single Jack file or all Jack files in a directory.
    
    Command line usage:
        python3 JackCompiler.py <input_file_or_directory>
        
    The compiler automatically detects whether the input is a file or
    directory and processes accordingly.
    """
    # Check command line arguments
    if len(sys.argv) != 2 or sys.argv[1] in ["-h", "--help"]:
        print("Jack Language Compiler")
        print("======================")
        print("")
        print("Usage: python3 JackCompiler.py <input_file_or_directory>")
        print("")
        print("Arguments:")
        print("  input_file_or_directory  Path to .jack file or directory containing .jack files")
        print("")
        print("Examples:")
        print("  python3 JackCompiler.py Main.jack          # Compile single file")
        print("  python3 JackCompiler.py Square/            # Compile all .jack files in directory")
        print("")
        print("Options:")
        print("  -h, --help              Show this help message")
        print("")
        print("The compiler generates .vm files in the same location as the input .jack files.")
        sys.exit(1)
    
    file_path = sys.argv[1]
    
    # Determine if input is a directory or file
    if os.path.isdir(file_path):
        print(f"Compiling all .jack files in directory: {file_path}")
        compile_directory(file_path)
        
    elif os.path.isfile(file_path):
        # Validate file extension
        if not file_path.endswith(".jack"):
            print(f"Error: '{file_path}' is not a valid .jack file")
            print("Please provide a file with .jack extension or a directory containing .jack files")
            sys.exit(1)
        
        print(f"Compiling single file: {file_path}")
        
        # Generate output filename by replacing .jack with .vm
        output_file_path = file_path.replace(".jack", ".vm")
        
        # Compile the single file
        compile_jack_file(file_path, output_file_path)
        
        print(f"Compilation completed: {output_file_path}")
        
    else:
        print(f"Error: '{file_path}' is not a valid file or directory")
        print("Please check the path and try again")
        sys.exit(1)


if __name__ == "__main__":
    main()