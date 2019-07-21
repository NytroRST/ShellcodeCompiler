
#ifndef DECLAREDFUNCTIONS_H
#define DECLAREDFUNCTIONS_H

#include <string>
#include <vector>
#include "Utils.h"
#include "DLLBaseAddress.h"
#include "FunctionOffsetAddress.h"
#include "Platform.h"

using namespace std;

class DeclaredFunctions
{
public:

	// Vector with declared functions

	struct DeclaredFunction
	{
		string Name;
		string DLL;
	};

	static size_t AllDeclaredFunctionsNr;
	static vector<DeclaredFunction> AllDeclaredFunctions;

	// Used to generate ASM

	static size_t NrBasesToStack;
	static size_t NrFunctionsToStack;

	static string GenerateLoadLibraryCall(string p_sDLLName);
	static string GenerateLoadLibraryCall_x86(string p_sDLLName);
	static string GenerateLoadLibraryCall_x64(string p_sDLLName);
	static string GenerateGetProcAddressCall(string p_sDLLName, string p_sFunctionName);
	static string GenerateGetProcAddressCall_x86(string p_sDLLName, string p_sFunctionName);
	static string GenerateGetProcAddressCall_x64(string p_sDLLName, string p_sFunctionName);

	// Constructor

	DeclaredFunctions()
	{
		AllDeclaredFunctionsNr = 0;
	}

	// Add function name

	static void AddFunctionName(string p_sFunctionName);

	// Add function DLL name

	static void AddFunctionDLLName(string p_sDLLName);
};

#endif // !DECLAREDFUNCTIONS_H
