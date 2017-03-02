
#include "Utils.h"

// Get current working directory

string Utils::GetCurrentDir()
{
	char buffer[1024];
	string sContent = "";

	DWORD r = GetCurrentDirectory(1024, buffer);

	if (r == 0) return "";
	sContent = buffer;

	return sContent;
}

// Check if a file exists

bool Utils::FileExists(string p_sPath)
{
	DWORD dwAttrib = GetFileAttributes(p_sPath.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES);
}

// Delete a file

bool Utils::DeleteSourceFile(string p_sFile)
{
	return (bool)DeleteFile(p_sFile.c_str());
}

// Function used to read a file

string Utils::ReadSourceFile(string p_sFilename)
{
	string contents = "";

	cout << "Read text file: " << p_sFilename << endl;

	FILE* f = fopen(p_sFilename.c_str(), "rb");

	// Get file size

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);

	char* cdata = new char[size + 1];
	cdata[size] = '\0';

	// Read file

	rewind(f);
	fread(cdata, sizeof(char), size, f);
	fclose(f);

	// Return data

	contents = cdata;
	delete[] cdata;

	return contents;
}

// Get the size of a file

size_t Utils::GetSize(string p_sFilename)
{
	size_t size;
	FILE* f = fopen(p_sFilename.c_str(), "rb");

	// Get file size

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	fclose(f);

	return size;
}

// Read binary file

unsigned char* Utils::ReadBinaryFile(string p_sFilename, size_t *p_rSize)
{
	unsigned char *p = NULL;
	FILE* f = NULL;
	size_t res = 0;

	if (!FileExists(p_sFilename))
	{
		cout << "Binary file does not exists!" << endl;
		return NULL;
	}

	// Get size and allocate space

	*p_rSize = GetSize(p_sFilename);
	if (*p_rSize == 0) return NULL;

	f = fopen(p_sFilename.c_str(), "rb");

	p = new unsigned char[*p_rSize];

	// Read file

	rewind(f);
	res = fread(p, sizeof(unsigned char), *p_rSize, f);
	fclose(f);

	if (res == 0)
	{
		delete[] p;
		return NULL;
	}

	return p;
}

// Write output to file

void Utils::WriteToFile(string p_sOutputFile, string p_sData)
{
	FILE* f = fopen(p_sOutputFile.c_str(), "ab");

	// Write

	fwrite(p_sData.c_str(), p_sData.length(), sizeof(char), f);

	// Close

	fclose(f);
}

// Return hex string for a char

string Utils::CharToHexString(char p_cChar)
{
	stringstream ss;
	ss << hex << (int)p_cChar;
	string mystr = ss.str();

	return mystr;
}

// Convert int to hex

string Utils::IntToHexString(size_t p_iNumber)
{
	std::stringstream stream;
	stream << std::hex << p_iNumber;
	std::string result(stream.str());

	return result;
}

// Function used to check if a character is "string": [a-z][A-Z][0-9][Specail]

bool Utils::IsString(char p_cCharacter)
{
	return (p_cCharacter >= 32 && p_cCharacter <= 126 &&
		p_cCharacter != '"' && p_cCharacter != ')' && p_cCharacter != '(' && p_cCharacter != ',');
}

// Get TEMP folder

string Utils::GetTemp()
{
	char buffer[1024];
	string sContent = "";

	DWORD r = GetTempPath(1024, buffer);

	if (r == 0) return "";
	sContent = buffer;

	return sContent;
}
