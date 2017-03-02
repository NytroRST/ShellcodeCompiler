
#ifndef FUNCTIONOFFSETADDRESS_H
#define FUNCTIONOFFSETADDRESS_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Class to work with function addresses on stack

class FunctionOffsetAddress
{
public:
	// Struct to save function address offset on the stack

	struct FunctionOffset
	{
		string Name;
		size_t Offset;
	};

	static vector<FunctionOffset> FunctionOffsets;
	static size_t CurrentFunctionOffset;

	// Check if a function offset already exists

	static bool FunctionOffsetExists(string p_sFunctionName);

	// Add a function offset

	static void AddFunctionOffset(string p_sFunctionName);

	// Get function offset

	static size_t GetFunctionOffset(string p_sFunctionName);
};

#endif
