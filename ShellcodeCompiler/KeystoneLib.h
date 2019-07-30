#ifndef KEYSTONELIB_H
#define KEYSTONELIB_H

#include <string>
#include <iostream>
#include <cstring>

#include "Platform.h"

// Keystone: http://www.keystone-engine.org/

#include "../Keystone/include/keystone/keystone.h"

#if defined _M_IX86
#pragma comment(lib, "../Keystone/keystone_win32.lib")
#elif defined _M_X64
#pragma comment(lib, "../Keystone/keystone_win64.lib")
#endif

using namespace std;

// Class to compile ASM

class KeystoneLib
{
public:
	static unsigned char* Assemble(size_t *p_pSize, string p_sASM);
};

#endif
