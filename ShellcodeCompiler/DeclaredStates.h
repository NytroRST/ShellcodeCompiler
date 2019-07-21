
#ifndef DECLAREDSTATES_H
#define DECLAREDSTATES_H

#include <string>
#include <vector>

#include "DeclaredFunctions.h"
#include "FunctionCalls.h"

using namespace std;

// Special characters

#define CHR_READ_ALPHANUMSTR '\x01'
#define CHR_READ_NUMBER	     '\x02'

// Structures used by the states

class DeclaredStates
{
public:

	// Transform struct

	struct State;

	struct Transform
	{
		char   Character;
		State* NextState;
	};

	// State struct

	struct State
	{
		string Data = "";
		bool Process = false;
		vector<Transform> Transforms;
		string Name = "";
		void(*ProcessStateData)(string);
	};

	// Process data for FUNCTION STRING PARAMETER

	static void ProcessStateData_FunctionCallStringArg(string p_sString);

	// Process data for FUNCTION INT PARAMETER

	static void ProcessStateData_FunctionCallIntArg(string p_sString);

	// Process data for FUNCTION NAME

	static void ProcessStateData_FunctionName(string p_sString);

	// Process data for FUNCTION DLL NAME

	static void ProcessStateData_FunctionDLLName(string p_sString);
};

#endif