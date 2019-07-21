
#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
#include "Utils.h"

using namespace std;

// Platform types

enum PLATFORM_TYPE
{
	PLATFORM_TYPE_WINDOWS_X86,
	PLATFORM_TYPE_WINDOWS_X64,
	PLATFORM_TYPE_LINUX_X86,
	PLATFORM_TYPE_LINUX_X64
};

// Class used for Platform

class Platform
{
	static PLATFORM_TYPE s_ePlatformType;
public:
	static PLATFORM_TYPE GetPlatform();
	static string GetPlatformString();
	static void SetPlatform(PLATFORM_TYPE p_ePlatformType);
	static void SetPlatformString(string p_sPlatformType);
};

#endif
