
#ifndef DECLAREDFUNCTIONSSTATES_H
#define DECLAREDFUNCTIONSSTATES_H

#include "DeclaredStates.h"

// Declared functions states

class DeclaredFunctionsStates
{
public:

	// States 

	static DeclaredStates::State DF_function;
	static DeclaredStates::State DF_FunctionName;
	static DeclaredStates::State DF_FunctionDLLStr;
	static DeclaredStates::State DF_ReadStringInQuotes;
	static DeclaredStates::State DF_GotStrInQuotes;
	static DeclaredStates::State DF_FinishStrInQuotes;

	static void CreateDeclareFunctionsStates();
};

#endif