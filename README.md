# Neo 5

This repository contains an emulator for a 13-bit CPU with file management capabilities and basic arithmetic operations. The CPU emulator simulates a custom CPU architecture, including memory management, instruction execution, and system commands. 

## Features
- **13-bit CPU Emulator**: Simulates a CPU with a custom instruction set.
- **File System Commands**: Create, view, delete, and list files within a simulated environment.
- **Math Operations**: Support for basic arithmetic commands such as addition, subtraction, multiplication, division, and modulo.
- **Register and RAM Inspection**: View the contents of registers and RAM.
- **Assembly Program Loading**: Load and execute programs written in a custom assembly language.

## Instructions

### Running the Emulator

1. **Compilation**  
   To compile the code, run the following in your terminal:
   ```bash
   gcc -o cpu_emulator cpu_emulator.c

2. Starting the Emulator
Run the compiled program:

./cpu_emulator


3. Available Commands
The emulator provides an interactive shell for you to interact with. Below are the supported commands:

## Command Reference

**run <filename>**
Loads an assembly program file and runs it.
Example:

CPU$ run program.asm

create <filename>
Creates a new file and allows the user to input content. The content is saved until CTRL+D is pressed to stop input.
Example:

**CPU$ create test.txt**

view <filename>
Views the contents of an existing file.
Example:

CPU$ view test.txt

delete <filename>
Deletes a file.
Example:

**CPU$ delete test.txt**

list
Lists all files in the current directory.
Example:

CPU$ list

**cd <directory>**
Changes the current working directory.
Example:

CPU$ cd /path/to/directory

**math <expression>**
Performs a mathematical operation with the syntax:

CPU$ math 5 + 3
CPU$ math 10 - 2
CPU$ math 6 * 7
CPU$ math 15 / 3
CPU$ math 10 % 3

**print regs**
Prints the contents of all 8 registers.
Example:

**CPU$ print regs**

**print ram**
Prints the first 16 bytes of RAM.
Example:

CPU$ print ram

*exit**
Exits the emulator.
Example:

CPU$ exit


## CPU Instruction Set

The CPU has a custom 13-bit instruction set with the following opcodes:

0x1 (LOAD): Load an immediate value into a register.

0x2 (ADD): Add the values of two registers and store the result in the first register.

0x3 (STORE): Store the value of a register into RAM at a specified address.

0x4 (JUMP): Jump to a memory address.

0xF (HALT): Stop the execution of the program.


Memory and Registers

RAM: The CPU has 8192 bytes of RAM, addressable with 13 bits.

Registers: There are 8 general-purpose registers, each 13 bits wide.

Program Counter (PC): The address of the current instruction to execute.

Instruction Register (IR): Holds the current instruction to be executed.


Math Command Syntax

The math command allows performing basic arithmetic operations:

Addition: num1 + num2

Subtraction: num1 - num2

Multiplication: num1 * num2

Division: num1 / num2 (checks for division by zero)

Modulo: num1 % num2 (checks for division by zero)


Example Program

The CPU can execute assembly programs that are loaded into memory. A simple program might look like this:

// Load 5 into register 0
1 0 0 5
// Load 3 into register 1
1 1 0 3
// Add register 0 and register 1, store the result in register 0
2 0 1 0
// Halt the CPU
F 0 0 0

This program loads two values into registers, adds them together, and stores the result back into register 0. Then, it halts the CPU.

## Conclusion

This emulator allows users to simulate the operation of a custom 13-bit CPU, execute assembly programs, and interact with files and perform basic arithmetic. It's a great way to understand low-level CPU architecture, memory management, and the inner workings of simple virtual machines.
