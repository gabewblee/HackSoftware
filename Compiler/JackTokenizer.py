"""
Jack Language Tokenizer Module.

This module provides lexical analysis for the Jack programming language.
It tokenizes Jack source code by removing comments and breaking the input
into meaningful tokens (keywords, symbols, integers, strings, identifiers).

The tokenizer follows the Jack language specification and provides methods
to advance through tokens and determine their types.
"""

import Config
import re
from typing import Optional


class JackTokenizer:
    """
    Tokenizer for the Jack programming language.
    
    This class handles the lexical analysis of Jack source code by:
    1. Removing comments (both single-line and multi-line)
    2. Breaking the input into tokens using regular expressions
    3. Providing methods to navigate through tokens and identify their types
    
    Attributes:
        _file (str): The original source code content
        _tokens (List[str]): List of extracted tokens
        _index (int): Current position in the token list
        currToken (Optional[str]): Current token being processed
    """
    
    def __init__(self, file: str) -> None:
        """
        Initialize the tokenizer with Jack source code.
        
        Args:
            file (str): The Jack source code as a string
            
        Note:
            The constructor automatically strips comments and tokenizes
            the input file upon initialization.
        """
        self._file = file
        self._stripComments()
        self._findTokens()
        self._index = -1
        self.currToken = None

    def _stripComments(self) -> None:
        """
        Remove all comments from the source code.
        
        Removes both single-line comments (// ...) and multi-line comments
        (/* ... */) from the source code before tokenization.
        """
        self._file = re.sub(Config.REGEX_COMMENT_LINE, "", self._file)
        self._file = re.sub(Config.REGEX_COMMENT_BLOCK, "", self._file, flags=re.S)

    def _findTokens(self) -> None:
        """
        Extract tokens from the cleaned source code.
        
        Uses regular expressions to identify and extract all tokens
        (keywords, symbols, integers, strings, identifiers) from the
        source code. The tokens are stored in the _tokens list.
        """
        self._tokens = re.findall(Config.REGEX_ALL, self._file)
        self._tokens = [token for match in self._tokens for token in match if token]

    def isKeyword(self) -> bool:
        """
        Check if the current token is a Jack keyword.
        
        Returns:
            bool: True if current token is a reserved keyword, False otherwise
        """
        return self.currToken in Config.KEYWORDS
    
    def isSymbol(self) -> bool:
        """
        Check if the current token is a Jack symbol.
        
        Returns:
            bool: True if current token is a special symbol, False otherwise
        """
        return self.currToken in Config.SYMBOLS
    
    def isInteger(self) -> bool:
        """
        Check if the current token is a valid integer constant.
        
        Jack integers must be in the range 0-32767.
        
        Returns:
            bool: True if current token is a valid integer, False otherwise
        """
        return self.currToken.isdigit() and 0 <= int(self.currToken) <= 32767
    
    def isString(self) -> bool:
        """
        Check if the current token is a string literal.
        
        String literals must be enclosed in double quotes and
        cannot contain newlines.
        
        Returns:
            bool: True if current token is a string literal, False otherwise
        """
        return len(self.currToken) >= 2 and self.currToken.startswith('"') and self.currToken.endswith('"')
    
    def isIdentifier(self) -> bool:
        """
        Check if the current token is a valid identifier.
        
        Identifiers must start with a letter or underscore and
        can contain letters, digits, and underscores.
        
        Args:
            currToken (str): The token to check
            
        Returns:
            bool: True if current token is a valid identifier, False otherwise
        """
        if not self.currToken or self.currToken[0].isdigit():
            return False

        return all(c.isalnum() or c == "_" for c in self.currToken)
    
    def hasMoreTokens(self) -> bool:
        """
        Check if there are more tokens to process.
        
        Returns:
            bool: True if there are more tokens available, False otherwise
        """
        return self._index < len(self._tokens) - 1
    
    def advance(self) -> Optional[str]:
        """
        Move to the next token and return it.
        
        Advances the internal index and updates the current token.
        If no more tokens are available, sets currToken to None.
        
        Returns:
            Optional[str]: The next token, or None if no more tokens
        """
        if self._index < len(self._tokens) - 1:
            self._index += 1
            self.currToken = self._tokens[self._index]
        else:
            self._index = len(self._tokens)
            self.currToken = None
        return self.currToken

    def tokenType(self) -> str:
        """
        Determine the type of the current token.
        
        Returns the token type as a string that matches the Jack
        language specification.
        
        Returns:
            str: Token type: "keyword", "symbol", "integerConstant",
                 "stringConstant", "identifier", or "invalid"
        """
        if self.isKeyword():
            return "keyword"
        elif self.isSymbol():
            return "symbol"
        elif self.isInteger():
            return "integerConstant"
        elif self.isString():
            return "stringConstant"
        elif self.isIdentifier():
            return "identifier"
        else:
            return "invalid"
    
    def peek(self) -> Optional[str]:
        """
        Look at the next token without advancing.
        
        Returns the next token in the sequence without changing
        the current position. Useful for lookahead parsing.
        
        Returns:
            Optional[str]: The next token, or None if no more tokens
        """
        if self._index + 1 < len(self._tokens):
            return self._tokens[self._index + 1]
        return None



                