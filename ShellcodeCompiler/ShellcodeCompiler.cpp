// Shellcode Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandLine.h"
#include "DLLBaseAddress.h"
#include "DeclaredFunctionsStates.h"
#include "CallFunctionsStates.h"

using namespace std;

// Main

int main(int argc, char *argv[])
{
	// If there is no argument, print help

	if (argc == 1)
	{
		CommandLine::PrintHelp(argv[0]);
		return 0;
	}
	
	DLLBaseAddress::InitDLLBaseAddress();
	DeclaredFunctionsStates::CreateDeclareFunctionsStates();
	CallFunctionsStates::CreateCallFunctionsStates();

	// Parse command line arguments

	CommandLine::ParseCommandLine(argc, argv);

	return 0;
}

