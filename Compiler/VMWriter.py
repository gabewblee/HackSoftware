"""
Virtual Machine Writer Module for the Jack Compiler.

This module handles the generation of VM (Virtual Machine) code from
the compilation engine. It provides methods to write all VM commands
including memory access, arithmetic operations, program flow control,
and function calls.

The VMWriter abstracts the VM code generation details and ensures
proper formatting of VM commands according to the VM specification.
"""

from Config import operationMap
from typing import TextIO


class VMWriter:
    """
    Virtual Machine code generator for Jack language compilation.
    
    This class provides a clean interface for writing VM commands
    to an output file. It handles all VM operations including:
    - Memory access (push/pop)
    - Arithmetic and logical operations
    - Program flow control (labels, gotos, if-gotos)
    - Function calls and returns
    
    The writer ensures proper VM command syntax and formatting,
    making the generated code compatible with the VM emulator.
    
    Attributes:
        _outputFile (TextIO): The output file stream for VM code
    """
    
    def __init__(self, outputFile: TextIO) -> None:
        """
        Initialize the VM writer with an output file.
        
        Args:
            outputFile (TextIO): The output file stream where VM code
                                will be written
        """
        self._outputFile = outputFile

    def writePush(self, segment: str, index: int) -> None:
        """
        Write a push command to the VM output.
        
        Generates a VM push command that pushes a value from the
        specified memory segment and index onto the stack.
        
        Args:
            segment (str): The memory segment (local, argument, this,
                          that, constant, static, temp, pointer)
            index (int): The index within the segment
        """
        self._outputFile.write(f"push {segment} {index}\n")

    def writePop(self, segment: str, index: int) -> None:
        """
        Write a pop command to the VM output.
        
        Generates a VM pop command that pops a value from the stack
        and stores it in the specified memory segment and index.
        
        Args:
            segment (str): The memory segment (local, argument, this,
                          that, static, temp, pointer)
            index (int): The index within the segment
        """
        self._outputFile.write(f"pop {segment} {index}\n")
    
    def writeArithmetic(self, command: str) -> None:
        """
        Write an arithmetic/logical command to the VM output.
        
        Generates a VM arithmetic or logical operation command.
        These commands operate on the top elements of the stack.
        
        Args:
            command (str): The VM command (add, sub, neg, eq, gt, lt,
                          and, or, not)
        """
        self._outputFile.write(f"{command}\n")

    def writeLabel(self, label: str) -> None:
        """
        Write a label command to the VM output.
        
        Generates a VM label command that defines a label for
        program flow control (gotos and if-gotos).
        
        Args:
            label (str): The label name (must be unique within the function)
        """
        self._outputFile.write(f"label {label}\n")

    def writeGoto(self, label: str) -> None:
        """
        Write a goto command to the VM output.
        
        Generates a VM goto command that unconditionally transfers
        control to the specified label.
        
        Args:
            label (str): The target label name
        """
        self._outputFile.write(f"goto {label}\n")

    def writeIf(self, label: str) -> None:
        """
        Write an if-goto command to the VM output.
        
        Generates a VM if-goto command that conditionally transfers
        control to the specified label if the top stack value is true.
        
        Args:
            label (str): The target label name
            
        Note:
            The top stack value is popped and control transfers to the
            label only if the value is true (non-zero).
        """
        self._outputFile.write(f"if-goto {label}\n")

    def writeCall(self, name: str, nArgs: int) -> None:
        """
        Write a function call command to the VM output.
        
        Generates a VM call command that invokes a function or method
        with the specified number of arguments.
        
        Args:
            name (str): The fully qualified function/method name
                       (e.g., "Math.multiply", "Square.new")
            nArgs (int): The number of arguments being passed
        """
        self._outputFile.write(f"call {name} {nArgs}\n")

    def writeFunction(self, name: str, nVars: int) -> None:
        """
        Write a function declaration command to the VM output.
        
        Generates a VM function command that declares a function
        and allocates local variables on the stack.
        
        Args:
            name (str): The fully qualified function name
            nVars (int): The number of local variables to allocate
        """
        self._outputFile.write(f"function {name} {nVars}\n")

    def writeReturn(self) -> None:
        """
        Write a return command to the VM output.
        
        Generates a VM return command that returns control from
        the current function to its caller.
        
        Note:
            The return value (if any) should be on top of the stack
            before calling this method.
        """
        self._outputFile.write("return\n")

    def writeOperation(self, operation: str) -> None:
        """
        Write an operation command using the operation map.
        
        Maps Jack language operators to their corresponding VM
        commands and writes them to the output. For simple operations
        like add/sub, writes the command directly. For complex operations
        like multiply/divide, writes a Math library call.
        
        Args:
            operation (str): The Jack language operator (+, -, *, /, &, |, <, >, =)
            
        Note:
            This method uses the operationMap from Config.py to determine
            the appropriate VM command for each operator.
        """
        self.writeArithmetic(operationMap[operation])