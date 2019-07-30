
#include "DebugUtils.h"

#if defined(_WIN32)
	#include "SEHUtils.h"
#else 
	#include <cstdlib>
	#include <cstring>
	#include <sys/mman.h>
#endif

// Dump all data - debug purposes

void DebugUtils::DumpAllData()
{
	cout << endl;
	for (size_t i = 0; i < DeclaredFunctions::AllDeclaredFunctions.size(); i++)
	{
		cout << "Declared:" << DeclaredFunctions::AllDeclaredFunctions[i].Name << " @ " << DeclaredFunctions::AllDeclaredFunctions[i].DLL << endl;
	}
	cout << endl;

	for (size_t i = 0; i < FunctionCalls::AllFunctionCalls.size(); i++)
	{
		cout << "Call function: " << FunctionCalls::AllFunctionCalls[i].Name << endl;
		for (size_t j = 0; j < FunctionCalls::AllFunctionCalls[i].Parameters.size(); j++)
		{
			cout << ((FunctionCalls::AllFunctionCalls[i].Parameters[j].Type == FunctionCalls::PARAMETER_TYPE_STRING) ? "String" : "Int") << " parameter: ";
			if (FunctionCalls::AllFunctionCalls[i].Parameters[j].Type == FunctionCalls::PARAMETER_TYPE_STRING) cout << FunctionCalls::AllFunctionCalls[i].Parameters[j].StringValue << endl;
			else cout << FunctionCalls::AllFunctionCalls[i].Parameters[j].IntValue << endl;
		}
	}
	cout << endl;
}

// Test the generated shellcode

void DebugUtils::TestShellcode(string p_sFilename)
{
	unsigned char *p = NULL;
	size_t size = 0;

	p = Utils::ReadBinaryFile(p_sFilename, &size);

	// Check if successful read

	if (size == 0 || p == NULL)
	{
		cout << "Error: Cannot read shellcode file!" << endl;
		return;
	}

#if defined(_WIN32)

	// Get space for shellcode

	void *sc = VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (sc == NULL)
	{
		cout << "Error: Cannot allocate space for shellcode!" << endl;
		return;
	}

	// Copy shellcode and execute it
	try
	{
		_set_se_translator(CxxTranslateSehException);
		memcpy(sc, p, size);
		(*(int(*)()) sc)();
	}
	catch (const seh_exception& e)
	{
		cout << "Error when executing shellcode: "
			 << e.what() << endl;
	}

#else 

	// Test shellcode on Linux

	unsigned char *sc = (unsigned char*)valloc(size);

	if (sc == NULL)
	{
		cout << "Error: Cannot allocate space for shellcode!" << endl;
		return;
	}

	memcpy(sc, p, size);
	mprotect(sc, size, PROT_READ | PROT_EXEC);
	(*(int(*)())sc)();

#endif
}

