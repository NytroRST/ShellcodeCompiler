#ifndef SEHUTILIS_H
#define SEHUTILIS_H
#include <windows.h>
#include <exception>

struct seh_exception : public std::exception
{
	seh_exception(const char* message, DWORD code) : exception(message), m_code(code) {}

	DWORD Code() const { return m_code; }

private:
	const DWORD m_code;
};

void CxxTranslateSehException(unsigned int, EXCEPTION_POINTERS* pointers)
{
	DWORD exceptionCode = pointers->ExceptionRecord->ExceptionCode;
	const char* msg = nullptr;

	switch (exceptionCode) {
	case EXCEPTION_ACCESS_VIOLATION:
		msg = "Access violation.";
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		msg = "Out of bound access.";
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		msg = "Illegal instruction.";
		break;
	default:
		break;
	}
	throw seh_exception(msg, exceptionCode);
}


#endif
