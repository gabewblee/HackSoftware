from typing import Optional

class SymbolTables:
    def __init__(self):
        self._methodST = {}
        self._classST = {}
        self._fieldCount = 0
        self._staticCount = 0
        self._localCount = 0
        self._argCount = 0

    def reset(self):
        self._methodST = {}
        self._localCount = 0
        self._argCount = 0

    def startSubroutine(self):
        self._methodST = {}
        self._localCount = 0
        self._argCount = 0

    def define(self, name: str, type: str, kind: str):
        if kind == "field":
            index = self._fieldCount
            self._fieldCount += 1
            self._classST[name] = {
                "type": type,
                "kind": kind,
                "index": index,
            }
        elif kind == "static":
            index = self._staticCount
            self._staticCount += 1
            self._classST[name] = {
                "type": type,
                "kind": kind,
                "index": index,
            }
        elif kind == "local":
            index = self._localCount
            self._localCount += 1
            self._methodST[name] = {
                "type": type,
                "kind": kind,
                "index": index,
            }
        elif kind == "argument":
            index = self._argCount
            self._argCount += 1
            self._methodST[name] = {
                "type": type,
                "kind": kind,
                "index": index,
            }
        else:
            raise ValueError(f"Error: Invalid kind {kind}")

    def varCount(self, kind: str) -> int:
        if kind == "field":
            return self._fieldCount
        elif kind == "static":
            return self._staticCount
        elif kind == "local":
            return self._localCount
        elif kind == "argument":
            return self._argCount
        else:
            raise ValueError(f"Error: Invalid kind {kind}")
    
    def kindOf(self, name: str) -> Optional[str]:
        if name in self._methodST:
            kind = self._methodST[name]["kind"]
        elif name in self._classST:
            kind = self._classST[name]["kind"]
        else:
            return None
        
        if kind == "field":
            return "this"
        elif kind == "static":
            return "static"
        elif kind == "local":
            return "local"
        elif kind == "argument":
            return "argument"
        else:
            return kind

    def typeOf(self, name: str) -> Optional[str]:
        if name in self._methodST:
            return self._methodST[name]["type"]
        elif name in self._classST:
            return self._classST[name]["type"]
        else:
            return None

    def indexOf(self, name: str) -> Optional[int]:
        if name in self._methodST:
            return self._methodST[name]["index"]
        elif name in self._classST:
            return self._classST[name]["index"]
        else:
            return None
