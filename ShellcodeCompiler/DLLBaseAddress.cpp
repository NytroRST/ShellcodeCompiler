
#include "DLLBaseAddress.h"

// Global data

vector<DLLBaseAddress::DLLBase> DLLBaseAddress::DLLOffsets;
size_t DLLBaseAddress::CurrentDLLOffset = 4;

// Create kernel32 DLL base index

void DLLBaseAddress::InitDLLBaseAddress()
{
	DLLBase kernel;
	kernel.Name = "kernel32.dll";
	kernel.Offset = 1;
	DLLOffsets.push_back(kernel);
}

// Check if a DLL base index already exists

bool DLLBaseAddress::DLLBaseExists(string p_sDLLName)
{
	for (size_t i = 0; i < DLLOffsets.size(); i++)
		if (DLLOffsets[i].Name.compare(p_sDLLName) == 0) return true;

	return false;
}

// Add a DLL base index

void DLLBaseAddress::AddDLLBase(string p_sDLLName)
{
	DLLBase dll;

	// Check if DLL already exists

	if (DLLBaseExists(p_sDLLName)) return;

	dll.Name = p_sDLLName;
	dll.Offset = CurrentDLLOffset;
	DLLOffsets.push_back(dll);

	CurrentDLLOffset++;
}

// Get DLL base index

size_t DLLBaseAddress::GetDLLBase(string p_sDLLName)
{
	for (size_t i = 0; i < DLLOffsets.size(); i++)
		if (DLLOffsets[i].Name.compare(p_sDLLName) == 0) 
		{
			// For Windows x64, we do not have other things on the stack: kernel32 base, GetProcAddress and Loadlibrary so we substract 3 

			if(Platform::GetPlatform() == PLATFORM_TYPE_WINDOWS_X64) return DLLOffsets[i].Offset - 3;
			else return DLLOffsets[i].Offset;
		}

	cout << "Error: Cannot find DLL base index for " << p_sDLLName << endl;

	return 0;
}
