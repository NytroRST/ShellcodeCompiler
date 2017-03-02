
#include "DeclaredFunctions.h"

// Global data

size_t DeclaredFunctions::AllDeclaredFunctionsNr;
vector<DeclaredFunctions::DeclaredFunction> DeclaredFunctions::AllDeclaredFunctions;
size_t DeclaredFunctions::NrBasesToStack = 0;
size_t DeclaredFunctions::NrFunctionsToStack = 0;

// Add function name

void DeclaredFunctions::AddFunctionName(string p_sFunctionName)
{
	DeclaredFunction Function;
	Function.Name = p_sFunctionName;
	AllDeclaredFunctions.push_back(Function);
}

// Add function DLL name

void DeclaredFunctions::AddFunctionDLLName(string p_sDLLName)
{
	AllDeclaredFunctions[AllDeclaredFunctionsNr].DLL = p_sDLLName;
	AllDeclaredFunctionsNr++;
}

// Generate a call to LoadLibrary("string")

string DeclaredFunctions::GenerateLoadLibraryCall(string p_sDLLName)
{
	string sContent = "";
	size_t Len = p_sDLLName.length();

	// Check if the DLL was already loades

	if (DLLBaseAddress::DLLBaseExists(p_sDLLName)) return "";

	if (Len % 4 == 0)
	{
		sContent = "xor eax, eax                            ; EAX = 0                       \r\n";
		sContent += "push eax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 4 == 1)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov al, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 2)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov ax, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 3)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov eax, 0x23";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 3]);
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
			sContent += Utils::CharToHexString(p_sDLLName[i * 4 - 4 + j - 1]);
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

	DLLBaseAddress::AddDLLBase(p_sDLLName);
	NrBasesToStack++;

	return sContent;
}

// Generate a call to GetProcAddress(DLLBase, "FunctionName")

string DeclaredFunctions::GenerateGetProcAddressCall(string p_sDLLName, string p_sFunctionName)
{
	string sContent = "";
	size_t Len = p_sFunctionName.length();

	// Check if the function address was already found

	if (FunctionOffsetAddress::FunctionOffsetExists(p_sFunctionName)) return "";

	if (Len % 4 == 0)
	{
		sContent = "xor eax, eax                            ; EAX = 0                       \r\n";
		sContent += "push eax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 4 == 1)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov al, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 2)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov ax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 3)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov eax, 0x23";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 3]);
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
			sContent += Utils::CharToHexString(p_sFunctionName[i * 4 - 4 + j - 1]);
		}
		sContent += "\r\n";
	}

	// LoadLibrary function call

	sContent += "push esp                               ; String on the stack            \r\n";
	sContent += "push dword [esp + ";
	sContent += to_string((NrFunctionsToStack * 4) + ((NrBasesToStack + 3 - DLLBaseAddress::GetDLLBase(p_sDLLName)) * 4) + ((Times + 2) * 4));
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

	FunctionOffsetAddress::AddFunctionOffset(p_sFunctionName);
	NrFunctionsToStack++;

	// Do it!

	return sContent;
}

