
#include "CallFunctionsStates.h"

// Declare call functions states

DeclaredStates::State CallFunctionsStates::CF_FunctionCall;
DeclaredStates::State CallFunctionsStates::CF_GetFirstParam;
DeclaredStates::State CallFunctionsStates::CF_ReadStrInQuotes;
DeclaredStates::State CallFunctionsStates::CF_GotStrInQuotes;
DeclaredStates::State CallFunctionsStates::CF_GetNextArg;
DeclaredStates::State CallFunctionsStates::CF_GetNumber;
DeclaredStates::State CallFunctionsStates::CF_GotAllArgs;

// Function used to create the declare functions states

void CallFunctionsStates::CreateCallFunctionsStates()
{
	DeclaredStates::Transform T;

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
	CF_ReadStrInQuotes.ProcessStateData = DeclaredStates::ProcessStateData_FunctionCallStringArg;

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
	CF_GetNumber.ProcessStateData = DeclaredStates::ProcessStateData_FunctionCallIntArg;

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
