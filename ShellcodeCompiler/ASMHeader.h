
#ifndef ASMHEADER_H
#define ASMHEADER_H

#include <string>
#include "Platform.h"

using namespace std;

// Class used for ASM Header

class ASMHeader
{
public:
	static string GetASMHeader();
	static string GetASMHeader_x86();
	static string GetASMHeader_x64();
};

#endif
