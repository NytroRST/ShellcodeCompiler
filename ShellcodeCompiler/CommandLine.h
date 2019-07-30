
#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <string>
#include <iostream>

#if defined(_WIN32)
#include <Windows.h>
#else 
#include <unistd.h>
#endif

#include "Utils.h"
#include "Compile.h"
#include "DebugUtils.h"
#include "KeystoneLib.h"
#include "Platform.h"

using namespace std;

// Class to work with command line parameters

class CommandLine
{
public:

	// Functions

	static void PrintHelp(string p_sFile);
	static void ParseCommandLine(int argc, char *argv[]);

	// Data

	static bool g_bHelp;
	static bool g_bVerbose;
	static bool g_bTest;
	static bool bError;

	static bool g_bPlatform;
	static string g_sPlatform;
	static PLATFORM_TYPE g_ePlatform;

	static bool   g_bReadFile;
	static string g_sReadFile;
	static bool   g_bOutputFile;
	static string g_sOutputFile;
	static bool   g_bASMFile;
	static string g_sASMFile;
};

#endif // !COMMANDLINE_H
