
#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// A class containing functions for access to files and conversions

class Utils
{
public:

	// All utilities

	static string GetCurrentDir();
	static bool   FileExists(string p_sPath);
	static bool   DeleteSourceFile(string p_sFile);
	static string ReadSourceFile(string p_sFilename);
	static size_t GetSize(string p_sFilename);
	static unsigned char* ReadBinaryFile(string p_sFilename, size_t *p_rSize);
	static void   WriteToFile(string p_sOutputFile, string p_sData);
	static string CharToHexString(char p_cChar);
	static string IntToHexString(size_t p_iNumber);
	static bool   IsString(char p_cCharacter);
	static string GetTemp();
};

#endif // !UTILS_H
