
#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <iostream>

#include "DeclaredFunctions.h"
#include "FunctionCalls.h"

using namespace std;

// Class used for debugging purposes: Dump data, test shellcode

class DebugUtils
{
public:
	// Dump all data - debug purposes

	static void DumpAllData();

	// Test the generated shellcode

	static void TestShellcode(string p_sFilename);
};

#endif
