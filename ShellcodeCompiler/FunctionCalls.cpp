
#include "FunctionCalls.h"

// Static members

const int FunctionCalls::PARAMETER_TYPE_INT = 0;
const int FunctionCalls::PARAMETER_TYPE_STRING = 1;

size_t FunctionCalls::AllFunctionCallsNr;
vector<FunctionCalls::FunctionCall> FunctionCalls::AllFunctionCalls;
bool FunctionCalls::FirstFunctionCall = true;

// Add function call name

void FunctionCalls::AddFunctionCallName(string p_sFunctionName)
{
	FunctionCall Function;
	Function.Name = p_sFunctionName;
	AllFunctionCalls.push_back(Function);
	if (FirstFunctionCall) FirstFunctionCall = false;
	else AllFunctionCallsNr++;
}

// Add function call int parameter

void FunctionCalls::AddFunctionCallIntParameter(string p_sIntParameter)
{
	Parameter Param;
	Param.Type = PARAMETER_TYPE_INT;
	Param.IntValue = (size_t)stoll(p_sIntParameter);
	AllFunctionCalls[AllFunctionCallsNr].Parameters.push_back(Param);
}

// Add function call string parameter

void FunctionCalls::AddFunctionCallStringParameter(string p_sStringParameter)
{
	Parameter Param;
	Param.Type = PARAMETER_TYPE_STRING;
	Param.StringValue = p_sStringParameter;
	AllFunctionCalls[AllFunctionCallsNr].Parameters.push_back(Param);
}

// Check if a function exists

bool FunctionCalls::FunctionExists(string p_sFunctionName)
{
	for (size_t i = 0; i < DeclaredFunctions::AllDeclaredFunctions.size(); i++)
		if (DeclaredFunctions::AllDeclaredFunctions[i].Name.compare(p_sFunctionName) == 0) return true;
	return false;
}

// Put a string (function parameter) on the stack for x86

string FunctionCalls::GeneratePutStringToStack_x86(string p_sString)
{
	string sContent = "";
	size_t Len = p_sString.length();

	// Check if the string was already on the stack

	if (StringOffsetAddress::StringOffsetExists(p_sString)) return "";

	if (Len % 4 == 0)
	{
		sContent = "xor eax, eax                            ; EAX = 0                       \r\n";
		sContent += "push eax                                ; NULL on the stack             \r\n";
	}
	else if (Len % 4 == 1)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov al, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 2)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov ax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += "\r\n";
		sContent += "push eax\r\n";
	}
	else if (Len % 4 == 3)
	{
		sContent = "xor eax, eax\r\n";
		sContent += "mov eax, 0x23";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += Utils::CharToHexString(p_sString[Len - 3]);
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
			sContent += Utils::CharToHexString(p_sString[i * 4 - 4 + j - 1]);
		}
		sContent += "\r\n";
	}

	// Add string offset 

	sContent += "push esp\r\n\r\n";
	StringOffsetAddress::CurrentStringOffset = StringOffsetAddress::CurrentStringOffset + Times + 2;
	StringOffsetAddress::AddStringOffset(p_sString);

	return sContent;
}

// Put a string (function parameter) on the stack for x64

string FunctionCalls::GeneratePutStringToStack_x64(string p_sString)
{
	string sContent = "";
	size_t Len = p_sString.length();

	// Check if the string was already on the stack

	if (StringOffsetAddress::StringOffsetExists(p_sString)) return "";

	if (Len % 8 == 0)
	{
		sContent = "xor rax, rax                            ; EAX = 0                       \r\n";
		sContent += "push rax                               ; NULL on the stack             \r\n";
	}
	else if (Len % 8 == 1)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov al, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 2)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov ax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 3)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov eax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += Utils::CharToHexString(p_sString[Len - 3]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 4)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov eax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += Utils::CharToHexString(p_sString[Len - 3]);
		sContent += Utils::CharToHexString(p_sString[Len - 4]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 5)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += Utils::CharToHexString(p_sString[Len - 3]);
		sContent += Utils::CharToHexString(p_sString[Len - 4]);
		sContent += Utils::CharToHexString(p_sString[Len - 5]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 6)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += Utils::CharToHexString(p_sString[Len - 3]);
		sContent += Utils::CharToHexString(p_sString[Len - 4]);
		sContent += Utils::CharToHexString(p_sString[Len - 5]);
		sContent += Utils::CharToHexString(p_sString[Len - 6]);
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}
	else if (Len % 8 == 7)
	{
		sContent = "xor rax, rax\r\n";
		sContent += "mov rax, 0x";
		sContent += Utils::CharToHexString(p_sString[Len - 1]);
		sContent += Utils::CharToHexString(p_sString[Len - 2]);
		sContent += Utils::CharToHexString(p_sString[Len - 3]);
		sContent += Utils::CharToHexString(p_sString[Len - 4]);
		sContent += Utils::CharToHexString(p_sString[Len - 5]);
		sContent += Utils::CharToHexString(p_sString[Len - 6]);
		sContent += Utils::CharToHexString(p_sString[Len - 7]);
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
			sContent += Utils::CharToHexString(p_sString[i * 8 - 8 + j - 1]);
		}
		sContent += "\r\n";
		sContent += "push rax\r\n";
	}

	// Add string offset 

	sContent += "push rsp\r\n\r\n";
	StringOffsetAddress::CurrentStringOffset = StringOffsetAddress::CurrentStringOffset + Times + 2;
	StringOffsetAddress::AddStringOffset(p_sString);

	return sContent;
}

// Generate a function call

string FunctionCalls::GenerateFunctionCall(FunctionCalls::FunctionCall p_oFunctionCall)
{
	if (Platform::GetPlatform() == PLATFORM_TYPE_LINUX_X86) return GenerateLinuxSyscall_x86(p_oFunctionCall);
	else if (Platform::GetPlatform() == PLATFORM_TYPE_LINUX_X64) return GenerateLinuxSyscall_x64(p_oFunctionCall);
	else if (Platform::GetPlatform() == PLATFORM_TYPE_WINDOWS_X64) return GenerateFunctionCall_x64(p_oFunctionCall);
	else return GenerateFunctionCall_x86(p_oFunctionCall);
}

// Generate a function call for x86

string FunctionCalls::GenerateFunctionCall_x86(FunctionCalls::FunctionCall p_oFunctionCall)
{
	string sContent = "";
	size_t NrParam = p_oFunctionCall.Parameters.size();
	size_t CurrentParamNr = 1;

	// First, put all string parameters on the stack

	for (size_t i = 0; i < p_oFunctionCall.Parameters.size(); i++)
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING) sContent += GeneratePutStringToStack_x86(p_oFunctionCall.Parameters[i].StringValue);

	// Parse all arguments (from the end)

	for (size_t i = NrParam - 1; NrParam; i--)
	{
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING)
		{
			sContent += "push dword [ESP + ";
			sContent += to_string(((CurrentParamNr - 1) * 4) + ((StringOffsetAddress::CurrentStringOffset - StringOffsetAddress::GetStringOffset(p_oFunctionCall.Parameters[i].StringValue)) * 4));
			sContent += "]\r\n";
		}
		else if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_INT)
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
				sContent += Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue);
				sContent += "\r\n";
			}
		}
		else cout << "Error: Undefined parameter type!" << endl;

		CurrentParamNr++;
		if (i == 0) break;
	}

	// Call function

	sContent += "call [ESP + ";
	sContent += to_string(((NrParam - 1) * 4) + (StringOffsetAddress::CurrentStringOffset * 4) + ((FunctionOffsetAddress::CurrentFunctionOffset - FunctionOffsetAddress::GetFunctionOffset(p_oFunctionCall.Name)) * 4));
	sContent += "]\r\n";

	// Clean string parameters

	if (StringOffsetAddress::CurrentStringOffset > 0)
	{
		sContent += "add ESP, ";
		sContent += to_string(StringOffsetAddress::CurrentStringOffset * 4);
		sContent += "\r\n";
	}

	// Clean global strings data

	StringOffsetAddress::CurrentStringOffset = 0;
	StringOffsetAddress::StringOffsets.clear();

	return sContent;
}

// Generate a function call for x64

string FunctionCalls::GenerateFunctionCall_x64(FunctionCalls::FunctionCall p_oFunctionCall)
{
	string sContent = "";
	size_t NrParam = p_oFunctionCall.Parameters.size();
	size_t CurrentParamNr = 1;
	size_t ParamsInRegisters = (NrParam >= 4) ? 4 : NrParam;
	size_t AdjustStack = 0;

	// First, put all string parameters on the stack

	for (size_t i = 0; i < p_oFunctionCall.Parameters.size(); i++)
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING) sContent += GeneratePutStringToStack_x64(p_oFunctionCall.Parameters[i].StringValue);

	// Align stack after strings are placed on the stack

	AdjustStack = (StringOffsetAddress::CurrentStringOffset + (NrParam - ParamsInRegisters) + DeclaredFunctions::NrFunctionsToStack + DeclaredFunctions::NrBasesToStack ) % 2;
	if (AdjustStack) sContent += "push r12 \r\n";

	// Parse all arguments (from the end)

	for (size_t i = NrParam - 1; NrParam; i--)
	{
		// We treat separatelty first 4 registers
		
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING)
		{
			size_t CurrentParamNotInRegisters = (CurrentParamNr > 4) ? (CurrentParamNr - 4) : ( (NrParam <= 4) ? 0 : (NrParam - 4));
			string str_offset = to_string(((CurrentParamNotInRegisters) * 8) + ((StringOffsetAddress::CurrentStringOffset + AdjustStack - StringOffsetAddress::GetStringOffset(p_oFunctionCall.Parameters[i].StringValue)) * 8));

			if(i == 0) sContent += "mov rcx, [RSP + " + str_offset + "] \r\n";
			else if(i == 1) sContent += "mov rdx, [RSP + " + str_offset + "] \r\n";
			else if(i == 2) sContent += "mov r8,  [RSP + " + str_offset + "] \r\n";
			else if(i == 3) sContent += "mov r9,  [RSP + " + str_offset + "] \r\n";
			else sContent += "push [RSP + " + str_offset + "]\r\n";
		}
		else if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_INT)
		{
			// If int parameter is 0, avoid NULL

			if (p_oFunctionCall.Parameters[i].IntValue == 0)
			{
				if (i == 0) sContent += "xor rcx, rcx \r\n";
				else if (i == 1) sContent += "xor rdx, rdx \r\n";
				else if (i == 2) sContent += "xor r8, r8 \r\n";
				else if (i == 3) sContent += "xor r9, r9 \r\n";
				else sContent += "xor rax, rax \r\npush rax \r\n";
			}
			else
			{
				if (i == 0) sContent += "mov rcx, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 1) sContent += "mov rdx, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 2) sContent += "mov r8, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 3) sContent += "mov r9, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else sContent += "push 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
			}
		}
		else cout << "Error: Undefined parameter type!" << endl;

		CurrentParamNr++;
		if (i == 0) break;
	}

	// Allocate stack and correct function call (and cleanup)

	sContent += "sub rsp, 0x20                          ; Stack space                   \r\n";

	// Call function

	size_t FunctOffset = ((NrParam - ParamsInRegisters - 1) * 8);                                                                         // Nr of parameters pushed on the stack
	FunctOffset += (StringOffsetAddress::CurrentStringOffset * 8);                                                                        // Strings pushed on the stack
	FunctOffset += ((FunctionOffsetAddress::CurrentFunctionOffset - FunctionOffsetAddress::GetFunctionOffset(p_oFunctionCall.Name)) * 8); // Function table offset

	if (AdjustStack) FunctOffset += 0x8;
	FunctOffset += 0x20;

	sContent += "call [RSP + ";
	sContent += to_string(FunctOffset);
	sContent += "]\r\n";

	// Cleanup stack

	if (AdjustStack) sContent += "add rsp, 0x28\r\n";
	else sContent += "add rsp, 0x20\r\n";

	// Clean string parameters

	if (StringOffsetAddress::CurrentStringOffset > 0)
	{
		sContent += "add RSP, ";
		sContent += to_string((NrParam - ParamsInRegisters) * 8 + StringOffsetAddress::CurrentStringOffset * 8);
		sContent += "\r\n";
	}

	// Clean global strings data

	StringOffsetAddress::CurrentStringOffset = 0;
	StringOffsetAddress::StringOffsets.clear();

	return sContent;
}

// Generate a Linux syscall for x86

string FunctionCalls::GenerateLinuxSyscall_x86(FunctionCalls::FunctionCall p_oFunctionCall)
{
	string sContent = "";
	size_t NrParam = p_oFunctionCall.Parameters.size();
	size_t ParamsInRegisters = (NrParam >= 6) ? 6 : NrParam;
	size_t CurrentParamNr = 1;

	// First, put all string parameters on the stack

	for (size_t i = 0; i < p_oFunctionCall.Parameters.size(); i++)
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING) sContent += GeneratePutStringToStack_x86(p_oFunctionCall.Parameters[i].StringValue);

	// Parse all arguments (from the end)

	for (size_t i = NrParam - 1; NrParam; i--)
	{
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING)
		{
			size_t CurrentParamNotInRegisters = (CurrentParamNr > 6) ? (CurrentParamNr - 6) : ((NrParam <= 6) ? 0 : (NrParam - 6));
			string str_offset = to_string(((CurrentParamNotInRegisters) * 4) + ((StringOffsetAddress::CurrentStringOffset - StringOffsetAddress::GetStringOffset(p_oFunctionCall.Parameters[i].StringValue)) * 4));

			if (i == 0) sContent += "mov ebx, [ESP + " + str_offset + "] \r\n";
			else if (i == 1) sContent += "mov ecx, [ESP + " + str_offset + "] \r\n";
			else if (i == 2) sContent += "mov edx,  [ESP + " + str_offset + "] \r\n";
			else if (i == 3) sContent += "mov esi,  [ESP + " + str_offset + "] \r\n";
			else if (i == 4) sContent += "mov edi,  [ESP + " + str_offset + "] \r\n";
			else if (i == 5) sContent += "mov ebp,  [ESP + " + str_offset + "] \r\n";
			else sContent += "push [ESP + " + str_offset + "]\r\n";
		}
		else if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_INT)
		{
			// If int parameter is 0, avoid NULL

			if (p_oFunctionCall.Parameters[i].IntValue == 0)
			{
				if (i == 0) sContent += "xor ebx, ebx \r\n";
				else if (i == 1) sContent += "xor ecx, ecx \r\n";
				else if (i == 2) sContent += "xor edx, edx \r\n";
				else if (i == 3) sContent += "xor esi, esi \r\n";
				else if (i == 4) sContent += "xor edi, edi \r\n";
				else if (i == 5) sContent += "xor ebp, ebp \r\n";
				else sContent += "xor eax, eax \r\npush eax \r\n";
			}
			else
			{
				if (i == 0) sContent += "mov ebx, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 1) sContent += "mov ecx, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 2) sContent += "mov edx, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 3) sContent += "mov esi, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 4) sContent += "mov edi, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 5) sContent += "mov ebp, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else sContent += "push 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
			}
		}
		else cout << "Error: Undefined parameter type!" << endl;

		CurrentParamNr++;
		if (i == 0) break;
	}

	// Syscall

	int syscallnr = LinuxSyscalls::GetSyscallNr(p_oFunctionCall.Name);
	if (syscallnr == LINUX_SYSCALL_UNKNOWN) return "; " + p_oFunctionCall.Name + " syscall not found! \r\n\r\n";

	sContent += "mov eax, 0x" + Utils::IntToHexString(syscallnr) + "\r\n";
	sContent += "int 0x80 ; Syscall\r\n";

	// Clean string parameters and parameters

	if ( (StringOffsetAddress::CurrentStringOffset > 0) || (NrParam > 6) )
	{
		sContent += "add ESP, ";
		sContent += to_string( (StringOffsetAddress::CurrentStringOffset + (NrParam > 6 ? (NrParam - 6) : 0) ) * 4);
		sContent += "\r\n";
	}

	sContent += "\r\n";

	// Clean global strings data

	StringOffsetAddress::CurrentStringOffset = 0;
	StringOffsetAddress::StringOffsets.clear();

	return sContent;
}

// Generate a Linux syscall for x64

string FunctionCalls::GenerateLinuxSyscall_x64(FunctionCalls::FunctionCall p_oFunctionCall)
{
	string sContent = "";
	size_t NrParam = p_oFunctionCall.Parameters.size();
	size_t ParamsInRegisters = (NrParam >= 6) ? 6 : NrParam;
	size_t CurrentParamNr = 1;

	// First, put all string parameters on the stack

	for (size_t i = 0; i < p_oFunctionCall.Parameters.size(); i++)
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING) sContent += GeneratePutStringToStack_x64(p_oFunctionCall.Parameters[i].StringValue);

	// Parse all arguments (from the end)

	for (size_t i = NrParam - 1; NrParam; i--)
	{
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING)
		{
			size_t CurrentParamNotInRegisters = (CurrentParamNr > 6) ? (CurrentParamNr - 6) : ((NrParam <= 6) ? 0 : (NrParam - 6));
			string str_offset = to_string(((CurrentParamNotInRegisters) * 8) + ((StringOffsetAddress::CurrentStringOffset - StringOffsetAddress::GetStringOffset(p_oFunctionCall.Parameters[i].StringValue)) * 8));

			if (i == 0) sContent += "mov rdi, [RSP + " + str_offset + "] \r\n";
			else if (i == 1) sContent += "mov rsi, [RSP + " + str_offset + "] \r\n";
			else if (i == 2) sContent += "mov rdx,  [RSP + " + str_offset + "] \r\n";
			else if (i == 3) sContent += "mov r10,  [RSP + " + str_offset + "] \r\n";
			else if (i == 4) sContent += "mov r8,  [RSP + " + str_offset + "] \r\n";
			else if (i == 5) sContent += "mov r9,  [RSP + " + str_offset + "] \r\n";
			else sContent += "push [RSP + " + str_offset + "]\r\n";
		}
		else if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_INT)
		{
			// If int parameter is 0, avoid NULL

			if (p_oFunctionCall.Parameters[i].IntValue == 0)
			{
				if (i == 0) sContent += "xor rdi, rdi \r\n";
				else if (i == 1) sContent += "xor rsi, rsi \r\n";
				else if (i == 2) sContent += "xor rdx, rdx \r\n";
				else if (i == 3) sContent += "xor r10, r10 \r\n";
				else if (i == 4) sContent += "xor r8, r8 \r\n";
				else if (i == 5) sContent += "xor r9, r9 \r\n";
				else sContent += "xor rax, rax \r\npush rax \r\n";
			}
			else
			{
				if (i == 0) sContent += "mov rdi, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 1) sContent += "mov rsi, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 2) sContent += "mov rdx, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 3) sContent += "mov r10, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 4) sContent += "mov r8, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else if (i == 5) sContent += "mov r9, 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
				else sContent += "push 0x" + Utils::IntToHexString(p_oFunctionCall.Parameters[i].IntValue) + "\r\n";
			}
		}
		else cout << "Error: Undefined parameter type!" << endl;

		CurrentParamNr++;
		if (i == 0) break;
	}

	// Syscall

	int syscallnr = LinuxSyscalls::GetSyscallNr(p_oFunctionCall.Name);
	if (syscallnr == LINUX_SYSCALL_UNKNOWN) return "; " + p_oFunctionCall.Name + " syscall not found! \r\n\r\n";

	sContent += "mov rax, 0x" + Utils::IntToHexString(syscallnr) + "\r\n";
	sContent += "syscall ; Syscall\r\n";

	// Clean string parameters and parameters

	if ((StringOffsetAddress::CurrentStringOffset > 0) || (NrParam > 6))
	{
		sContent += "add RSP, ";
		sContent += to_string((StringOffsetAddress::CurrentStringOffset + (NrParam > 6 ? (NrParam - 6) : 0)) * 8);
		sContent += "\r\n";
	}

	sContent += "\r\n";

	// Clean global strings data

	StringOffsetAddress::CurrentStringOffset = 0;
	StringOffsetAddress::StringOffsets.clear();

	return sContent;
}
