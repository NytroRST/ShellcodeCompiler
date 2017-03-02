
#include "DeclaredFunctionsStates.h"

// Declare functions states

DeclaredStates::State DeclaredFunctionsStates::DF_function;
DeclaredStates::State DeclaredFunctionsStates::DF_FunctionName;
DeclaredStates::State DeclaredFunctionsStates::DF_FunctionDLLStr;
DeclaredStates::State DeclaredFunctionsStates::DF_ReadStringInQuotes;
DeclaredStates::State DeclaredFunctionsStates::DF_GotStrInQuotes;
DeclaredStates::State DeclaredFunctionsStates::DF_FinishStrInQuotes;

void DeclaredFunctionsStates::CreateDeclareFunctionsStates()
{
	DeclaredStates::Transform T;

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
	DF_FunctionName.ProcessStateData = DeclaredStates::ProcessStateData_FunctionName;

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
	DF_ReadStringInQuotes.ProcessStateData = DeclaredStates::ProcessStateData_FunctionDLLName;

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
