
#ifndef STRINGOFFSETADDRESS_H
#define STRINGOFFSETADDRESS_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Class to work with strings offsets on the stack

class StringOffsetAddress
{
public:

	// Struct to save strings (for function calls) on the stack

	struct StringOffset
	{
		string Name;
		size_t Offset;
	};

	static vector<StringOffset> StringOffsets;
	static size_t CurrentStringOffset;

	// Check if a string offset already exists

	static bool StringOffsetExists(string p_sString);

	// Add a string offset

	static void AddStringOffset(string p_sString);

	// Get string offset

	static size_t GetStringOffset(string p_sString);
};

#endif
