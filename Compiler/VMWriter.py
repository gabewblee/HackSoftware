from Config import operationMap

class VMWriter:
    def __init__(self, outputFile):
        self._outputFile = outputFile

    def writePush(self, segment: str, index: int):
        self._outputFile.write(f"push {segment} {index}\n")

    def writePop(self, segment: str, index: int):
        self._outputFile.write(f"pop {segment} {index}\n")
    
    def writeArithmetic(self, command: str):
        self._outputFile.write(f"{command}\n")

    def writeLabel(self, label: str):
        self._outputFile.write(f"label {label}\n")

    def writeGoto(self, label: str):
        self._outputFile.write(f"goto {label}\n")

    def writeIf(self, label: str):
        self._outputFile.write(f"if-goto {label}\n")

    def writeCall(self, name: str, nArgs: int):
        self._outputFile.write(f"call {name} {nArgs}\n")

    def writeFunction(self, name: str, nVars: int):
        self._outputFile.write(f"function {name} {nVars}\n")

    def writeReturn(self):
        self._outputFile.write("return\n")

    def writeOperation(self, operation: str):
        self.writeArithmetic(operationMap[operation])