
#include "Platform.h"

// Default is Windows x86

PLATFORM_TYPE Platform::s_ePlatformType = PLATFORM_TYPE_WINDOWS_X86;

// Return the platofrm type

PLATFORM_TYPE Platform::GetPlatform()
{
	return s_ePlatformType;
}

// Return the platofrm type as a string

string Platform::GetPlatformString()
{
	switch (s_ePlatformType)
	{
	case PLATFORM_TYPE_WINDOWS_X86:
		return "Windows x86 (32 bits)";
	case PLATFORM_TYPE_WINDOWS_X64:
		return "Windows x64 (64 bits)";
	case PLATFORM_TYPE_LINUX_X86:
		return "Linux x86 (32 bits)";
	case PLATFORM_TYPE_LINUX_X64:
		return "Linux x64 (64 bits)";
	default:
		return "Default: Windows x86 (32 bits)";
	}
}

// Set the platform type

void Platform::SetPlatform(PLATFORM_TYPE p_ePlatformType)
{
	s_ePlatformType = p_ePlatformType;
}

// Set the platform type as a string

void Platform::SetPlatformString(string p_sPlatformType)
{
	if (Utils::ToLower(p_sPlatformType).find("win") != string::npos)
	{
		if (Utils::ToLower(p_sPlatformType).find("64") != string::npos) s_ePlatformType = PLATFORM_TYPE_WINDOWS_X64;
		else s_ePlatformType = PLATFORM_TYPE_WINDOWS_X86;
	}
	else if (Utils::ToLower(p_sPlatformType).find("lin") != string::npos)
	{
		if (Utils::ToLower(p_sPlatformType).find("64") != string::npos) s_ePlatformType = PLATFORM_TYPE_LINUX_X64;
		else s_ePlatformType = PLATFORM_TYPE_LINUX_X86;
	}
}
