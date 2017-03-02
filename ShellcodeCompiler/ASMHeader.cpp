
#include "ASMHeader.h"

// Function that returns the default ASM Header

string ASMHeader::GetASMHeader()
{
	string sContent =
		"; Shellcode generated using Shellcode Compiler                         \r\n"
		"; https://github.com/NytroRST/ShellcodeCompiler                    \r\n\r\n"
		"BITS 32                                                                \r\n"
		"SECTION .text                                                          \r\n"
		"global main                                                            \r\n"
		"main:                                                              \r\n\r\n"
		"xor ecx, ecx                                                           \r\n"
		"mov eax, [fs:ecx + 0x30]               ; EAX = PEB                     \r\n"
		"mov eax, [eax + 0xc]                   ; EAX = PEB->Ldr                \r\n"
		"mov esi, [eax + 0x14]                  ; ESI = PEB->Ldr.InMemOrder     \r\n"
		"lodsd                                  ; EAX = Second module           \r\n"
		"xchg eax, esi                          ; EAX = ESI, ESI = EAX          \r\n"
		"lodsd                                  ; EAX = Third(kernel32)         \r\n"
		"mov ebx, [eax + 0x10]                  ; EBX = Base address            \r\n"
		"mov edx, [ebx + 0x3c]                  ; EDX = DOS->e_lfanew           \r\n"
		"add edx, ebx                           ; EDX = PE Header               \r\n"
		"mov edx, [edx + 0x78]                  ; EDX = Offset export table     \r\n"
		"add edx, ebx                           ; EDX = Export table            \r\n"
		"mov esi, [edx + 0x20]                  ; ESI = Offset namestable       \r\n"
		"add esi, ebx                           ; ESI = Names table             \r\n"
		"xor ecx, ecx                           ; EXC = 0                       \r\n\r\n"

		"Get_Function:                                                          \r\n\r\n"

		"inc ecx                                ; Increment the ordinal         \r\n"
		"lodsd                                  ; Get name offset               \r\n"
		"add eax, ebx                           ; Get function name             \r\n"
		"cmp dword [eax], 0x50746547            ; GetP                          \r\n"
		"jnz Get_Function                                                       \r\n"
		"cmp dword [eax + 0x4], 0x41636f72      ; rocA                          \r\n"
		"jnz Get_Function                                                       \r\n"
		"cmp dword [eax + 0x8], 0x65726464      ; ddre                          \r\n"
		"jnz Get_Function                                                       \r\n"
		"mov esi, [edx + 0x24]                  ; ESI = Offset ordinals         \r\n"
		"add esi, ebx                           ; ESI = Ordinals table          \r\n"
		"mov cx, [esi + ecx * 2]                ; Number of function            \r\n"
		"dec ecx                                                                \r\n"
		"mov esi, [edx + 0x1c]                  ; Offset address table          \r\n"
		"add esi, ebx                           ; ESI = Address table           \r\n"
		"mov edx, [esi + ecx * 4]               ; EDX = Pointer(offset)         \r\n"
		"add edx, ebx                           ; EDX = GetProcAddress          \r\n\r\n"

		"xor ecx, ecx                           ; ECX = 0                       \r\n"
		"push ebx                               ; Kernel32 base address         \r\n"
		"push edx                               ; GetProcAddress                \r\n"
		"push ecx                               ; 0                             \r\n"
		"push 0x41797261                        ; aryA                          \r\n"
		"push 0x7262694c                        ; Libr                          \r\n"
		"push 0x64616f4c                        ; Load                          \r\n"
		"push esp                               ; LoadLibrary                   \r\n"
		"push ebx                               ; Kernel32 base address         \r\n"
		"call edx                               ; GetProcAddress(LL)            \r\n\r\n"

		"add esp, 0xc                           ; pop LoadLibrary               \r\n"
		"pop ecx                                ; ECX = 0                       \r\n"
		"push eax                               ; EAX = LoadLibrary             \r\n\r\n";

	return sContent;
}
