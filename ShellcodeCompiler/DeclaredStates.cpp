
#include "DeclaredStates.h"

// Process data for FUNCTION STRING PARAMETER

void DeclaredStates::ProcessStateData_FunctionCallStringArg(string p_sString)
{
	FunctionCalls::AddFunctionCallStringParameter(p_sString);
}

// Process data for FUNCTION INT PARAMETER

void DeclaredStates::ProcessStateData_FunctionCallIntArg(string p_sString)
{
	FunctionCalls::AddFunctionCallIntParameter(p_sString);
}

// Process data for FUNCTION NAME

void DeclaredStates::ProcessStateData_FunctionName(string p_sString)
{
	DeclaredFunctions::AddFunctionName(p_sString);
}

// Process data for FUNCTION DLL NAME

void DeclaredStates::ProcessStateData_FunctionDLLName(string p_sString)
{
	DeclaredFunctions::AddFunctionDLLName(p_sString);
}
