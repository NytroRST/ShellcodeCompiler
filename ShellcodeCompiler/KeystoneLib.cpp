
#include "KeystoneLib.h"

// Function used to assemble code with keystone. Code taken from keystone sample and adapted

unsigned char* KeystoneLib::Assemble(size_t *p_pSize, string p_sASM)
{
	ks_engine *ks;
	ks_err err = KS_ERR_ARCH;
	size_t count;
	unsigned char *encode;
	size_t size;
	unsigned char *output = NULL;

	// Open keystone engine

	if(Platform::GetPlatform() == PLATFORM_TYPE_WINDOWS_X64 || Platform::GetPlatform() == PLATFORM_TYPE_LINUX_X64) err = ks_open(KS_ARCH_X86, KS_MODE_64, &ks);
	else err = ks_open(KS_ARCH_X86, KS_MODE_32, &ks);

	if (err != KS_ERR_OK) {
		cout << "ERROR: Failed on ks_open()" << endl;
		return NULL;
	}

	// Assemble the code

	ks_option(ks, KS_OPT_SYNTAX, KS_OPT_SYNTAX_NASM);

	if (ks_asm(ks, p_sASM.c_str(), 0, &encode, &size, &count)) {
		cout << "ERROR: Failed on ks_asm() with count = " << count << ", error code = " <<  ks_errno(ks) << endl;
		return NULL;
	}

	// If everything was OK, set the size and output and return

	output = new unsigned char[size];

	if (!output)
	{
		cout << "ERROR: Cannot allocate memmory for output" << endl;
		return NULL;
	}

	*p_pSize = size;
	memcpy(output, encode, size);

	// Assembly information 

	cout << "Assembled: " << size << " bytes, " << count << " statements" << endl;

	// Cleanup allocated data

	ks_free(encode);
	ks_close(ks);

	return output;
}
