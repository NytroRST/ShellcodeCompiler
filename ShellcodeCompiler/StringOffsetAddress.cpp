
#include "StringOffsetAddress.h"

// Global data

vector<StringOffsetAddress::StringOffset> StringOffsetAddress::StringOffsets;
size_t StringOffsetAddress::CurrentStringOffset = 0;

// Check if a string offset already exists

bool StringOffsetAddress::StringOffsetExists(string p_sString)
{
	for (size_t i = 0; i < StringOffsets.size(); i++)
		if (StringOffsets[i].Name.compare(p_sString) == 0) return true;

	return false;
}

// Add a string offset

void StringOffsetAddress::AddStringOffset(string p_sString)
{
	StringOffset str;

	// Check if string already exists

	if (StringOffsetExists(p_sString)) return;

	str.Name = p_sString;
	str.Offset = CurrentStringOffset;
	StringOffsets.push_back(str);
}

// Get string offset

size_t StringOffsetAddress::GetStringOffset(string p_sString)
{
	for (size_t i = 0; i < StringOffsets.size(); i++)
		if (StringOffsets[i].Name.compare(p_sString) == 0) return StringOffsets[i].Offset;

	cout << "Error: Cannot find string offset for " << p_sString << endl;

	return 0;
}

