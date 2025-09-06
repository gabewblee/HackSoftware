# HackSoftware

## Description

A complete implementation of the software components from the **The Elements of Computing Systems: Building a Modern Computer from First Principles** book, featuring a Jack Compiler, VM Translator, Assembler, and OS that runs on the Hack computer architecture.

## Features

This project implements the complete software toolchain described in "The Elements of Computing Systems" by Noam Nisan and Shimon Schocken (MIT Press). It includes:

1. **Jack Compiler**: Translates high-level Jack programs into VM code
2. **VM Translator**: Converts VM code into Hack assembly language  
3. **Hack Assembler**: Assembles Hack assembly into machine code
4. **JackOS**: A lightweight operating system written in Jack

### Prerequisites

1. **C Compiler**
2. **Python3** 
3. **Make** utility

### Building

From the HackSoftware directory, to build all components:
```bash
make all
```
To build only the Assembler:
```bash
make assembler
```
To build only the VM Translator:
```base
make vm
```

### Compilation

To compile a single Jack file XXX.jack, run the following:
```bash
make <path/XXX.jack>
```
To compile a directory containing Jack files, run the following:
```bash
make directory DIRECTORY=<path>
```
Where **<>** should be removed and **path** should be replaced with the path to the file.

### Running

To run the supplied VM Emulator:
```bash
./Tools/VMEmulator.sh
```
To run the supplied CPU Emulator:
```bash
./Tools/CPUEmulator.sh
```
To run the supplied Hack Hardware Simulator:
```bash
./Tools/HardwareSimulator.sh
```

## Sample Programs

The software suite supplied with the book includes several programs for execution:

1. **Square**: Interactive square drawing game
2. **Pong**: Classic Pong game implementation
3. **Average**: Calculates average of user input
4. **ArrayTest**: Demonstrates array operations
5. **ComplexArrays**: Advanced array manipulation
6. **ConvertToBin**: Binary conversion utility
7. **Seven**: Simple number display program

The JackOS code should be compiled and placed on the same folder as the compiled sample programs, which get jointly run on the emulators. More information can be found here: https://www.nand2tetris.org

## Credits

1. Noam Nisan and Shimon Schocken, "The Elements of Computing Systems: Building a Modern Computer from First Principles", MIT Press
