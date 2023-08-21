# ntoskrnl-eat-function-rva
Pass in function name exported by NTOSKRNL and this will return the Relative Virtual Address (RVA) of the function if found.

## Description

This project is a simple C++ program that will read `ntoskrnl.exe` on disk, and scan its Export Address Table (EAT) for the memory location of a given function.

## Usage: 

`ntoskrnl-eat.exe <FunctionName>`

Example:
![image](https://github.com/0xv1n/ntoskrnl-eat-function-rva/assets/11021725/317c18ae-2862-4312-a54a-e13dca85afda)
