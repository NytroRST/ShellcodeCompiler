
#include "Compile.h"

// Function used to parse data

bool Compile::ParseFile(string p_sFileData)
{
	bool result = true;
	string sReadString = "";
	DeclaredStates::State CurrentState;

	for (size_t i = 0; i < p_sFileData.length(); i++)
	{
		// Check for a string

		if (isalnum(p_sFileData[i]))
		{
			sReadString += p_sFileData[i];
		}

		// If it is not an alphanumeric character

		else
		{
			// We have a "function" definition

			if (sReadString.compare("function") == 0)
			{
				CurrentState = DeclaredFunctionsStates::DF_function;

				// PROCESS "function" - Declare a function

				while (true)
				{
					// Parse states transforms

					for (size_t s = 0; s < CurrentState.Transforms.size(); s++)
					{
						if (CurrentState.Transforms[s].Character == p_sFileData[i])
						{
							//cout << "We found a transform (" << i << "): " << p_sFileData[i] << " [" << (int)p_sFileData[i] << "] - Current state: " << CurrentState.Name << endl;

							// Process state

							if (CurrentState.Process)
							{
								// Do stuff

								CurrentState.ProcessStateData(CurrentState.Data);
								CurrentState.Data = "";
							}

							// Go to next state

							CurrentState = *(CurrentState.Transforms[s].NextState);
							//cout << "Moved to state: " << CurrentState.Name << endl;
							break;
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_ALPHANUMSTR && Utils::IsString(p_sFileData[i]))
						{
							CurrentState.Data = CurrentState.Data + p_sFileData[i];

							// Do not change current state
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_ALPHANUMSTR && Utils::IsString(p_sFileData[i]))
						{
							CurrentState.Data = CurrentState.Data + p_sFileData[i];

							// Do not change current state
						}
						//else cout << "Stuff: " << p_sFileData[i] << " on " << CurrentState.Name << endl;
					}

					i++;
					if (p_sFileData[i] == ';' || i == p_sFileData.length()) break;
				}
			}
			else if (FunctionCalls::FunctionExists(sReadString) || LinuxSyscalls::SyscallExists(sReadString))
			{
				FunctionCalls::AddFunctionCallName(sReadString);

				CurrentState = CallFunctionsStates::CF_FunctionCall;

				// PROCESS "function" - Declare a function

				while (true)
				{
					// Parse states transforms

					for (size_t s = 0; s < CurrentState.Transforms.size(); s++)
					{
						if (CurrentState.Transforms[s].Character == p_sFileData[i])
						{
							//cout << "We found a transform (" << i << "): " << p_sFileData[i] << " [" << (int)p_sFileData[i] << "] - Current state: " << CurrentState.Name << endl;

							// Process state

							if (CurrentState.Process)
							{
								// Do stuff

								CurrentState.ProcessStateData(CurrentState.Data);
								CurrentState.Data.clear();
							}

							// Go to next state

							CurrentState = *(CurrentState.Transforms[s].NextState);
							//cout << "Moved to state: " << CurrentState.Name << endl;
							break;
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_NUMBER && isdigit(p_sFileData[i]))
						{
							if (CurrentState.Name.compare((CurrentState.Transforms[s].NextState)->Name) != 0) (CurrentState.Transforms[s].NextState)->Data = "";

							(CurrentState.Transforms[s].NextState)->Data += p_sFileData[i];
							CurrentState = *(CurrentState.Transforms[s].NextState);
						}
						else if (CurrentState.Transforms[s].Character == CHR_READ_ALPHANUMSTR && Utils::IsString(p_sFileData[i]))
						{
							CurrentState.Data = CurrentState.Data + p_sFileData[i];

							// Do not change current state
						}
						//else cout << "Stuff: " << p_sFileData[i] << " on " << CurrentState.Name << endl;
					}

					i++;
					if (p_sFileData[i] == ';' || i == p_sFileData.length()) break;
				}
			}

			// Reset string

			sReadString = "";
		}
	}

	return result;
}

// Compile all parsed data into ASM file

string Compile::CompileAllData()
{
	string sOutput = "";
	
	// Compile for Windows

	if (Platform::GetPlatform() == PLATFORM_TYPE_LINUX_X86 || Platform::GetPlatform() == PLATFORM_TYPE_LINUX_X64)
	{
		// Generate system calls for all system calls

		for (size_t i = 0; i < FunctionCalls::AllFunctionCalls.size(); i++)
		{
			sOutput += FunctionCalls::GenerateFunctionCall(FunctionCalls::AllFunctionCalls[i]);
		}
	}
	else
	{
		sOutput += ASMHeader::GetASMHeader();

		// Generate LoadLibrary for all DLLs (from declared functions)

		for (size_t i = 0; i < DeclaredFunctions::AllDeclaredFunctions.size(); i++)
		{
			sOutput += DeclaredFunctions::GenerateLoadLibraryCall(DeclaredFunctions::AllDeclaredFunctions[i].DLL);
		}

		// Generate GetProcAddress for all declared functions

		for (size_t i = 0; i < DeclaredFunctions::AllDeclaredFunctions.size(); i++)
		{
			sOutput += DeclaredFunctions::GenerateGetProcAddressCall(DeclaredFunctions::AllDeclaredFunctions[i].DLL, DeclaredFunctions::AllDeclaredFunctions[i].Name);
		}

		// Generate function calls for all function calls

		for (size_t i = 0; i < FunctionCalls::AllFunctionCalls.size(); i++)
		{
			sOutput += FunctionCalls::GenerateFunctionCall(FunctionCalls::AllFunctionCalls[i]);
		}
	}

	return sOutput;
}
