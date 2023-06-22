Simulator of SIC-XE Assembler which converts assembly code into machine understandable object code.

Assembler : Data Structures
•	CSECTIONS : Vector of control sections structs(csect in the code) .
•	REGISTER : Contains information of the registers like its numeric equivalent , bool representing whether such register exists or not .
•	LITTAB : Contains information of literals like its value , address , block number , a bool representing whether the literal exists in literal table or not .
•	SYMTAB : Contains information of labels like , name , address , block number , bool representing whether the label exists in the symbol table or not , a bool     representing whether label is relative or not .
•	OPTAB : Contains information of opcode like name , format , a bool representing whether the opcode is valid or not .

Steps to COMPILE and EXECUTE the ASSEMBLER :
Step1 : Open the folder in which you have stored all the files of the assembler using Visual Studio Code text editor.
Step2 : Run the file pass2.cpp in VSCode.
•	Test input is given in input.txt file inside the folder. It is given as Figure 2.16 in System Software by Leland L. Beck
•	The intermediate file is formed in intermediate.txt in the folder 
•	The object program is formed in object_program.txt in the folder
