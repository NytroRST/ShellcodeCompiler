
#include "FunctionOffsetAddress.h"

// Global data

vector<FunctionOffsetAddress::FunctionOffset> FunctionOffsetAddress::FunctionOffsets;
size_t FunctionOffsetAddress::CurrentFunctionOffset = 1;

// Check if a function offset already exists

bool FunctionOffsetAddress::FunctionOffsetExists(string p_sFunctionName)
{
	for (size_t i = 0; i < FunctionOffsets.size(); i++)
		if (FunctionOffsets[i].Name.compare(p_sFunctionName) == 0) return true;

	return false;
}

// Add a function offset

void FunctionOffsetAddress::AddFunctionOffset(string p_sFunctionName)
{
	FunctionOffset func;

	// Check if DLL already exists

	if (FunctionOffsetExists(p_sFunctionName)) return;

	func.Name = p_sFunctionName;
	func.Offset = CurrentFunctionOffset;
	FunctionOffsets.push_back(func);

	CurrentFunctionOffset++;
}

// Get function offset

size_t FunctionOffsetAddress::GetFunctionOffset(string p_sFunctionName)
{
	for (size_t i = 0; i < FunctionOffsets.size(); i++)
		if (FunctionOffsets[i].Name.compare(p_sFunctionName) == 0) return FunctionOffsets[i].Offset;

	cout << "Error: Cannot find function offset for " << p_sFunctionName << endl;

	return 0;
}
