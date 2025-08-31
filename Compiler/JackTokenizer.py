import Config
import re

class JackTokenizer:
    def __init__(self, file: str):
        self._file = file
        self._stripComments()
        self._findTokens()
        self._index = -1
        self.currToken = None

    def _stripComments(self) -> None:
        self._file = re.sub(Config.REGEX_COMMENT_LINE, "", self._file)
        self._file = re.sub(Config.REGEX_COMMENT_BLOCK, "", self._file, flags=re.S)

    def _findTokens(self) -> list:
        self._tokens = re.findall(Config.REGEX_ALL, self._file)
        self._tokens = [token for match in self._tokens for token in match if token]

    def isKeyword(self) -> bool:
        return self.currToken in Config.KEYWORDS
    
    def isSymbol(self) -> bool:
        return self.currToken in Config.SYMBOLS
    
    def isInteger(self) -> bool:
        return self.currToken.isdigit() and 0 <= int(self.currToken) <= 32767
    
    def isString(self) -> bool:
        return len(self.currToken) >= 2 and self.currToken.startswith('"') and self.currToken.endswith('"')
    
    def isIdentifier(self) -> bool:
        if not self.currToken or self.currToken[0].isdigit():
            return False

        return all(c.isalnum() or c == "_" for c in self.currToken)
    
    def hasMoreTokens(self) -> bool:
        return self._index < len(self._tokens) - 1
    
    def advance(self) -> None:
        if self._index < len(self._tokens) - 1:
            self._index += 1
            self.currToken = self._tokens[self._index]
        else:
            self._index = len(self._tokens)
            self.currToken = None

    def tokenType(self) -> str:
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
    

                