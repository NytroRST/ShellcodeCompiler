
#ifndef DLLBASE_H
#define DLLBASE_H

#include <vector>
#include <string>
#include <iostream>
#include "Platform.h"

using namespace std;

// Class to work with DLL base address on the stack

class DLLBaseAddress
{
public:
	// Struct to save DLL base offset address on the stack

	struct DLLBase
	{
		string Name;
		size_t Offset;
	};

	static vector<DLLBase> DLLOffsets;
	static size_t CurrentDLLOffset;

	// Create kernel32 DLL base index

	static void InitDLLBaseAddress();

	// Check if a DLL base index already exists

	static bool DLLBaseExists(string p_sDLLName);

	// Add a DLL base index

	static void AddDLLBase(string p_sDLLName);

	// Get DLL base index

	static size_t GetDLLBase(string p_sDLLName);
};

#endif
