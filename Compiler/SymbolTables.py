"""
Symbol Tables Module for the Jack Compiler.

This module implements symbol table management for the Jack programming language.
It maintains separate symbol tables for class-level and method-level scopes,
handling different variable kinds (field, static, local, argument) and
providing lookup functionality for type, kind, and index information.

The symbol tables are essential for semantic analysis and code generation,
ensuring proper variable scoping and memory allocation in the VM.
"""

from typing import Optional, Dict, Any


class SymbolTables:
    """
    Symbol table manager for Jack language compilation.
    
    This class maintains two symbol tables:
    1. Class-level symbols (fields and static variables)
    2. Method-level symbols (local variables and parameters)
    
    Each symbol entry contains type, kind, and index information used
    for VM code generation and memory allocation.
    
    Attributes:
        _classST (Dict[str, Dict[str, Any]]): Class-level symbol table
        _methodST (Dict[str, Dict[str, Any]]): Method-level symbol table
        _fieldCount (int): Number of instance fields
        _staticCount (int): Number of static variables
        _localCount (int): Number of local variables
        _argCount (int): Number of parameters
    """
    
    def __init__(self) -> None:
        """
        Initialize empty symbol tables and counters.
        
        Creates empty dictionaries for class and method symbol tables,
        and initializes counters for each variable kind to zero.
        """
        self._methodST = {}
        self._classST = {}
        self._fieldCount = 0
        self._staticCount = 0
        self._localCount = 0
        self._argCount = 0

    def reset(self) -> None:
        """
        Reset method-level symbol table and counters.
        
        Clears the method symbol table and resets local variable
        and argument counters. Used when starting a new method.
        """
        self._methodST = {}
        self._localCount = 0
        self._argCount = 0

    def startSubroutine(self) -> None:
        """
        Start a new subroutine scope.
        
        Clears the method-level symbol table and resets counters
        for local variables and arguments. This method should be
        called at the beginning of each method/function compilation.
        """
        self._methodST = {}
        self._localCount = 0
        self._argCount = 0

    def define(self, name: str, type: str, kind: str) -> None:
        """
        Define a new symbol in the appropriate symbol table.
        
        Adds a symbol entry with the specified name, type, and kind.
        The symbol is placed in either the class or method table
        depending on its kind, and assigned the next available index.
        
        Args:
            name (str): The symbol name (identifier)
            type (str): The data type (int, char, boolean, or class name)
            kind (str): The variable kind: "field", "static", "local", or "argument"
            
        Raises:
            ValueError: If an invalid kind is specified
            
        Note:
            Field and static variables go to class table, while
            local variables and arguments go to method table.
        """
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
        """
        Get the count of variables of a specific kind.
        
        Returns the total number of variables defined for the
        specified kind across all scopes.
        
        Args:
            kind (str): The variable kind: "field", "static", "local", or "argument"
            
        Returns:
            int: The count of variables of the specified kind
            
        Raises:
            ValueError: If an invalid kind is specified
        """
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
        """
        Get the VM segment kind for a symbol.
        
        Returns the VM memory segment that should be used for
        the specified symbol. Field variables map to "this" segment,
        while other kinds map directly to their VM segments.
        
        Args:
            name (str): The symbol name to look up
            
        Returns:
            Optional[str]: The VM segment kind, or None if symbol not found
            
        Note:
            Field variables return "this" (for instance variables),
            while static, local, and argument return their direct names.
        """
        if name in self._methodST:
            kind = self._methodST[name]["kind"]
        elif name in self._classST:
            kind = self._classST[name]["kind"]
        else:
            return None
        
        if kind == "field":
            return "this"
        else:
            return kind

    def typeOf(self, name: str) -> Optional[str]:
        """
        Get the data type of a symbol.
        
        Looks up the symbol in both method and class tables
        and returns its declared type.
        
        Args:
            name (str): The symbol name to look up
            
        Returns:
            Optional[str]: The symbol's data type, or None if not found
        """
        if name in self._methodST:
            return self._methodST[name]["type"]
        elif name in self._classST:
            return self._classST[name]["type"]
        else:
            return None

    def indexOf(self, name: str) -> Optional[int]:
        """
        Get the index of a symbol within its scope.
        
        Returns the index of the symbol within its kind category.
        This index is used for VM memory allocation and access.
        
        Args:
            name (str): The symbol name to look up
            
        Returns:
            Optional[int]: The symbol's index, or None if not found
        """
        if name in self._methodST:
            return self._methodST[name]["index"]
        elif name in self._classST:
            return self._classST[name]["index"]
        else:
            return None
