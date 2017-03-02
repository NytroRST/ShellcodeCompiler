
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

// Put a string (function parameter) on the stack

string FunctionCalls::GeneratePutStringToStack(string p_sString)
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

	// Add dtring offset 

	sContent += "push esp\r\n\r\n";
	StringOffsetAddress::CurrentStringOffset = StringOffsetAddress::CurrentStringOffset + Times + 2;
	StringOffsetAddress::AddStringOffset(p_sString);

	return sContent;
}

// Generate a function call

string FunctionCalls::GenerateFunctionCall(FunctionCalls::FunctionCall p_oFunctionCall)
{
	string sContent = "";
	size_t NrParam = p_oFunctionCall.Parameters.size();
	size_t CurrentParamNr = 1;

	// First, put all string parameters on the stack

	for (size_t i = 0; i < p_oFunctionCall.Parameters.size(); i++)
		if (p_oFunctionCall.Parameters[i].Type == FunctionCalls::PARAMETER_TYPE_STRING) sContent += GeneratePutStringToStack(p_oFunctionCall.Parameters[i].StringValue);

	// Parse all arguments (from the end)

	for (size_t i = NrParam - 1; ; i--)
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

	sContent += "call dword [ESP + ";
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
