
#ifndef FUNCTIONCALLS_H
#define FUNCTIONCALLS_H

#include <string>
#include <vector>

#include "DeclaredFunctions.h"
#include "StringOffsetAddress.h"
#include "FunctionOffsetAddress.h"
#include "Platform.h"
#include "LinuxSyscalls.h"

using namespace std;

class FunctionCalls
{
public:

	static const int PARAMETER_TYPE_INT;
	static const int PARAMETER_TYPE_STRING;

// Struct used to save a parameter

struct Parameter
{
	size_t Type;
	size_t IntValue;
	string StringValue;
};

// Struct to save data for a function call

struct FunctionCall
{
	string Name;
	vector<Parameter> Parameters;
};

// All function calls and index

static size_t AllFunctionCallsNr;
static vector<FunctionCall> AllFunctionCalls;
static bool FirstFunctionCall;

// Constructor

FunctionCalls()
{
	AllFunctionCallsNr = 0;
	FirstFunctionCall  = true;
}

// Put a string (function parameter) on the stack

static string GeneratePutStringToStack_x86(string p_sString);
static string GeneratePutStringToStack_x64(string p_sString);

// Generate a function call / syscall

static string GenerateFunctionCall(FunctionCalls::FunctionCall p_oFunctionCall);
static string GenerateFunctionCall_x86(FunctionCalls::FunctionCall p_oFunctionCall);
static string GenerateFunctionCall_x64(FunctionCalls::FunctionCall p_oFunctionCall);

static string GenerateLinuxSyscall_x86(FunctionCalls::FunctionCall p_oFunctionCall);
static string GenerateLinuxSyscall_x64(FunctionCalls::FunctionCall p_oFunctionCall);

// Add function call name

static void AddFunctionCallName(string p_sFunctionName);

// Add function call int parameter

static void AddFunctionCallIntParameter(string p_sIntParameter);

// Add function call string parameter

static void AddFunctionCallStringParameter(string p_sStringParameter);

// Check if a function exists

static bool FunctionExists(string p_sFunctionName);

};

#endif
