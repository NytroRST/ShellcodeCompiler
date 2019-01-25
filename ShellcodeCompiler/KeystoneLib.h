#ifndef KEYSTONELIB_H
#define KEYSTONELIB_H

#include <string>
#include <iostream>

// Keystone: http://www.keystone-engine.org/

#include "../Keystone/include/keystone/keystone.h"
#pragma comment(lib, "../Keystone/keystone.lib")

using namespace std;

// Class to compile ASM

class KeystoneLib
{
public:
	static unsigned char* Assemble(size_t *p_pSize, string p_sASM);
};

#endif
