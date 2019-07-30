
#include "Utils.h"

// Check if a file exists

bool Utils::FileExists(string p_sPath)
{
	if (FILE * file = fopen(p_sPath.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

// Delete a file

bool Utils::DeleteSourceFile(string p_sFile)
{
	return (bool)remove(p_sFile.c_str());
}

// Function used to read a file

string Utils::ReadSourceFile(string p_sFilename)
{
	string contents = "";

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

// Write a binary file

size_t Utils::WriteBinaryFile(string p_sOutputFile, unsigned char *p_pcData, size_t p_nSize)
{
	size_t nResult = 0;
	FILE* f = fopen(p_sOutputFile.c_str(), "wb");

	// Write

	nResult = fwrite(p_pcData, p_nSize, sizeof(char), f);

	// Close

	fclose(f);
	return nResult;
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

// Function to convert a string to lower

string Utils::ToLower(string p_sString)
{
	string result = "";

	for (size_t i = 0; i < p_sString.length(); i++)
	{
		if (p_sString[i] >= 65 && p_sString[i] <= 90) result += (char)(p_sString[i] + 32);
		else result += p_sString[i];
	}

	return result;
}
