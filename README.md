# SIC-XE Assembler Simulator

The SIC-XE Assembler Simulator is a program that converts assembly code into machine-readable object code. It provides a tool for understanding and processing assembly language programs.

## Assembler: Data Structures

- `CSECTIONS`: A vector of control section structs (`csect` in the code).
- `REGISTER`: Contains information about registers, such as its numeric equivalent and a boolean representing whether such a register exists or not.
- `LITTAB`: Contains information about literals, including its value, address, block number, and a boolean representing whether the literal exists in the literal table or not.
- `SYMTAB`: Contains information about labels, including the label's name, address, block number, a boolean representing whether the label exists in the symbol table or not, and a boolean representing whether the label is relative or not.
- `OPTAB`: Contains information about opcodes, including the opcode's name, format, and a boolean representing whether the opcode is valid or not.

## Steps to Compile and Execute the Assembler

Step 1: Open the folder in which you have stored all the files of the assembler using the Visual Studio Code text editor.
Step 2: Run the `pass2.cpp` file in Visual Studio Code.
   - The test input is provided in the `input.txt` file inside the folder (Figure 2.16 in System Software by Leland L. Beck).
   - The intermediate file is generated as `intermediate.txt` in the folder.
   - The object program is generated as `object_program.txt` in the folder.
