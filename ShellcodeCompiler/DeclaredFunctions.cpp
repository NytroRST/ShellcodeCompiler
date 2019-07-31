
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
	if (Platform::GetPlatform() == PLATFORM_TYPE_WINDOWS_X64) return GenerateLoadLibraryCall_x64(p_sDLLName);
	else return GenerateLoadLibraryCall_x86(p_sDLLName);
}

// Generate a call to LoadLibrary("string") for x86

string DeclaredFunctions::GenerateLoadLibraryCall_x86(string p_sDLLName)
{
	string sContent = "";
	size_t Len = p_sDLLName.length();

	// Check if the DLL was already loades

	if (DLLBaseAddress::DLLBaseExists(p_sDLLName)) return "";

	sContent = "; LoadLibrary(" + p_sDLLName + ")\r\n\r\n";

	if (Len % 4 == 0)
	{
		sContent += "xor eax, eax                           ; EAX = 0                       \r\n";
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
	sContent += "call [esp + ";
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

// Generate a call to LoadLibrary("string") for x64

string DeclaredFunctions::GenerateLoadLibraryCall_x64(string p_sDLLName)
{
	string sContent = "";
	size_t Len = p_sDLLName.length();

	// Check if the DLL was already loades

	if (DLLBaseAddress::DLLBaseExists(p_sDLLName)) return "";

	sContent = "; LoadLibrary(" + p_sDLLName + ")\r\n\r\n";

	if (Len % 8 == 0)
	{
		sContent += "xor rax, rax                           ; EAX = 0                       \r\n";
		sContent += "push rax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 8 == 1)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov al, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 2)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov ax, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 3)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov eax, 0x23";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 3]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
		sContent += "sub dword [rsp + 7], 0x23\r\n";
	}
	else if (Len % 8 == 4)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov eax, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 3]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 4]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 5)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 3]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 4]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 5]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 6)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 3]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 4]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 5]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 6]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 7)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sDLLName[Len - 1]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 2]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 3]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 4]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 5]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 6]);
		sContent += Utils::CharToHexString(p_sDLLName[Len - 7]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else cout << "Imaginary number?" << endl;

	// Put the string as hex data pushes on the stack

	size_t Times = Len / 8;
	for (size_t i = Times; i > 0; i--)
	{
		sContent += "mov rax, 0x";
		for (size_t j = 8; j > 0; j--)
		{
			sContent += Utils::CharToHexString(p_sDLLName[i * 8 - 8 + j - 1]);
		}
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}

	// LoadLibrary function call

	sContent += "mov rcx, rsp                           ; RCX = DLL name                \r\n";

	if((Times + NrBasesToStack + 1) % 2)
		sContent += "sub rsp, 0x28                          ; Stack space                   \r\n";
	else 
		sContent += "sub rsp, 0x30                          ; Stack space                   \r\n";

	sContent += "call rsi                               ; LoadLibrary                   \r\n";
	sContent += "add rsp, ";
	
	if ((Times + NrBasesToStack + 1) % 2) sContent += to_string( ((Times + 1) * 8) + 0x28);
	else sContent += to_string(((Times + 1) * 8) + 0x30);
	
	sContent += "\r\n";
	sContent += "push rax                               ; DLL base on the stack         \r\n\r\n";

	// Add DLL to base list and increment bases on stack number

	DLLBaseAddress::AddDLLBase(p_sDLLName);
	NrBasesToStack++;

	return sContent;
}

// Generate a call to GetProcAddress(DLLBase, "FunctionName")

string DeclaredFunctions::GenerateGetProcAddressCall(string p_sDLLName, string p_sFunctionName)
{
	if (Platform::GetPlatform() == PLATFORM_TYPE_WINDOWS_X64) return GenerateGetProcAddressCall_x64(p_sDLLName, p_sFunctionName);
	else return GenerateGetProcAddressCall_x86(p_sDLLName, p_sFunctionName);
}

// Generate a call to GetProcAddress(DLLBase, "FunctionName") for x86

string DeclaredFunctions::GenerateGetProcAddressCall_x86(string p_sDLLName, string p_sFunctionName)
{
	string sContent = "";
	size_t Len = p_sFunctionName.length();

	// Check if the function address was already found

	if (FunctionOffsetAddress::FunctionOffsetExists(p_sFunctionName)) return "";

	sContent = "; GetProcAddress(" + p_sDLLName + ", " + p_sFunctionName + ")\r\n\r\n";

	if (Len % 4 == 0)
	{
		sContent += "xor eax, eax                           ; EAX = 0                       \r\n";
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

	// DLL base address

	sContent += "push esp                               ; String on the stack            \r\n";
	sContent += "push dword [esp + ";
	sContent += to_string((NrFunctionsToStack * 4) + ((NrBasesToStack + 3 - DLLBaseAddress::GetDLLBase(p_sDLLName)) * 4) + ((Times + 2) * 4));
	sContent += "] \r\n";

	// Call GetProcAddress

	sContent += "call [esp + ";
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

// Generate a call to GetProcAddress(DLLBase, "FunctionName") for x64

string DeclaredFunctions::GenerateGetProcAddressCall_x64(string p_sDLLName, string p_sFunctionName)
{
	string sContent = "";
	size_t Len = p_sFunctionName.length();

	// Check if the function address was already found

	if (FunctionOffsetAddress::FunctionOffsetExists(p_sFunctionName)) return "";

	sContent = "; GetProcAddress(" + p_sDLLName + ", " + p_sFunctionName + ")\r\n\r\n";

	if (Len % 8 == 0)
	{
		sContent += "xor rax, rax                           ; EAX = 0                       \r\n";
		sContent += "push rax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 8 == 1)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov al, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 2)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov ax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 3)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov eax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 3]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 4)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov eax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 3]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 4]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 5)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 3]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 4]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 5]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 6)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 3]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 4]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 5]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 6]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 7)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 1]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 2]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 3]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 4]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 5]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 6]);
		sContent += Utils::CharToHexString(p_sFunctionName[Len - 7]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else cout << "Imaginary number?" << endl;

	// Put the string as hex data pushes on the stack

	size_t Times = Len / 8;
	for (size_t i = Times; i > 0; i--)
	{
		sContent += "mov rax, 0x";
		for (size_t j = 8; j > 0; j--)
		{
			sContent += Utils::CharToHexString(p_sFunctionName[i * 8 - 8 + j - 1]);
		}
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}

	// DLL base address

	sContent += "mov rdx, rsp                           ; RDX = Function name            \r\n";

	if(p_sDLLName.compare("kernel32.dll") == 0) sContent += "mov rcx, rbx                           ; RCX = kernel32.dll            \r\n";
	else
	{
		sContent += "mov rcx, [rsp + ";
		sContent += to_string((NrFunctionsToStack * 8) + ((NrBasesToStack - DLLBaseAddress::GetDLLBase(p_sDLLName)) * 8) + ((Times + 1) * 8));
		sContent += "] \r\n";
	}
	
	// Call GetProcAddress

	if ((Times + NrFunctionsToStack + NrBasesToStack + 1) % 2)
		sContent += "sub rsp, 0x28                          ; Stack space                   \r\n";
	else
		sContent += "sub rsp, 0x30                          ; Stack space                   \r\n";

	sContent += "call rdi                               ; GetProcAddress                \r\n";

	sContent += "add rsp, ";

	if ((Times + NrFunctionsToStack  + NrBasesToStack + 1) % 2) sContent += to_string(((Times + 1) * 8) + 0x28);
	else sContent += to_string(((Times + 1) * 8) + 0x30);

	sContent += "\r\n";
	sContent += "push rax                               ; Function on the stack         \r\n\r\n";

	// Add function address to the list and increment nr of functions on stack number

	FunctionOffsetAddress::AddFunctionOffset(p_sFunctionName);
	NrFunctionsToStack++;

	// Do it!

	return sContent;
}
