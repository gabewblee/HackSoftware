"""
Configuration module for the Jack Compiler.

This module contains all the constants, mappings, and regular expressions used
throughout the Jack compiler implementation. It defines the Jack language syntax,
VM operation mappings, and token recognition patterns.
"""

from typing import Dict, Set

# Jack Language Keywords
KEYWORDS: Set[str] = {
    "class",        # Class declaration
    "constructor",  # Constructor method
    "function",     # Function declaration
    "method",       # Method declaration
    "field",        # Instance field
    "static",       # Static field
    "var",          # Local variable
    "int",          # Integer type
    "char",         # Character type
    "boolean",      # Boolean type
    "void",         # Void return type
    "true",         # Boolean literal
    "false",        # Boolean literal
    "null",         # Null reference
    "this",         # Current object reference
    "let",          # Variable assignment
    "do",           # Method call statement
    "if",           # Conditional statement
    "else",         # Alternative branch
    "while",        # Loop statement
    "return"        # Return statement
}

# Jack Language Symbols
SYMBOLS: Set[str] = {
    '{', '}',      # Block delimiters
    '(', ')',      # Parentheses for expressions and calls
    '[', ']',      # Array indexing
    '.',           # Method/field access
    ',',           # Parameter/argument separator
    ';',           # Statement terminator
    '+', '-',      # Arithmetic operators
    '*', '/',      # Arithmetic operators
    '&', '|',      # Logical operators
    '<', '>',      # Comparison operators
    '=',           # Assignment operator
    '~'            # Logical NOT operator
}

# VM Operation Mapping
# Maps Jack language operators to their corresponding VM commands
# For complex operations like multiply/divide, uses Math library calls
operationMap: Dict[str, str] = {
    '+': 'add',                    # Addition
    '-': 'sub',                    # Subtraction
    '*': 'call Math.multiply 2',   # Multiplication (2 arguments)
    '/': 'call Math.divide 2',     # Division (2 arguments)
    '&': 'and',                    # Logical AND
    '|': 'or',                     # Logical OR
    '<': 'lt',                     # Less than
    '>': 'gt',                     # Greater than
    '=': 'eq',                     # Equality
}

# Regular Expression Patterns
REGEX_COMMENT_LINE: str = r"//.*"
REGEX_COMMENT_BLOCK: str = r"/\*.*?\*/"
REGEX_SYMBOL: str = r'[{}()[\].,;+\-*/&|<>=~]'
REGEX_INTEGER: str = r'\b\d+\b'
REGEX_STRING: str = r'"[^"\n]*"'
REGEX_IDENTIFIER: str = r'\b[a-zA-Z_][a-zA-Z0-9_]*\b'
REGEX_ALL: str = f'({REGEX_SYMBOL})|({REGEX_INTEGER})|({REGEX_STRING})|({REGEX_IDENTIFIER})'