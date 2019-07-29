
#ifndef LINUXSYSCALLS_H
#define LINUXSYSCALLS_H

#include "Utils.h"
#include "Platform.h"

#include <string>
#include <vector>

#define LINUX_SYSCALL_UNKNOWN -1

using namespace std;

// Class used for ASM Header

class LinuxSyscalls
{
public:

	// Syscall definition

	struct Syscall
	{
		string Name = "";
		int x86 = -1;
		int x64 = -1;
	};

	static vector<Syscall> s_vSystemCallsTable;

	// Get syscall numbers

	static int GetSyscallNr(string p_sSyscallName);
	static bool SyscallExists(string p_sSyscallName);
	static int GetSyscallNr_x86(string p_sSyscallName);
	static int GetSyscallNr_x64(string p_sSyscallName);
};

#endif