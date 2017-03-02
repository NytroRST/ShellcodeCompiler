
#ifndef CALLFUNCTIONSSTATES_H
#define CALLFUNCTIONSSTATES_H

#include "DeclaredStates.h"
#include "FunctionCalls.h"

// Declared functions states

class CallFunctionsStates
{
public:

	// Call functions states

	static DeclaredStates::State CF_FunctionCall;
	static DeclaredStates::State CF_GetFirstParam;
	static DeclaredStates::State CF_ReadStrInQuotes;
	static DeclaredStates::State CF_GotStrInQuotes;
	static DeclaredStates::State CF_GetNextArg;
	static DeclaredStates::State CF_GetNumber;
	static DeclaredStates::State CF_GotAllArgs;

	static void CreateCallFunctionsStates();
};

#endif 
