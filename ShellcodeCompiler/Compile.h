
#ifndef COMPILE_H
#define COMPILE_H

#include <string>

#include "DeclaredStates.h"
#include "DeclaredFunctionsStates.h"
#include "CallFunctionsStates.h"
#include "DeclaredFunctions.h"
#include "FunctionCalls.h"
#include "ASMHeader.h"
#include "LinuxSyscalls.h"
#include "Platform.h"

using namespace std;

// Class used for shellcode compilation

class Compile
{
public:

	// Function used to parse data

	static bool ParseFile(string p_sFileData);

	// Compile all parsed data into ASM string

	static string CompileAllData();
};

#endif
