KEYWORDS = {
    "class",
    "constructor",
    "function",
    "method",
    "field",
    "static",
    "var",
    "int",
    "char",
    "boolean",
    "void",
    "true",
    "false",
    "null",
    "this",
    "let",
    "do",
    "if",
    "else",
    "while",
    "return"
}

SYMBOLS = {
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    '.',
    ',',
    ';',
    '+',
    '-',
    '*',
    '/',
    '&',
    '|',
    '<',
    '>',
    '=',
    '~'
}

# Regex
REGEX_COMMENT_LINE      = r"//.*"
REGEX_COMMENT_BLOCK     = r"/\*.*?\*/"
REGEX_SYMBOL            = r'[{}()[\].,;+\-*/&|<>=~]'
REGEX_INTEGER           = r'\b\d+\b'
REGEX_STRING            = r'"[^"\n]*"'
REGEX_IDENTIFIER        = r'\b[a-zA-Z_][a-zA-Z0-9_]*\b'
REGEX_ALL               = f'({REGEX_SYMBOL})|({REGEX_INTEGER})|({REGEX_STRING})|({REGEX_IDENTIFIER})'