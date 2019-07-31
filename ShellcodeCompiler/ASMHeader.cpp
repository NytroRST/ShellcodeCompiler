
#include "ASMHeader.h"

// Function that returns the ASM Header

string ASMHeader::GetASMHeader()
{
	if (Platform::GetPlatform() == PLATFORM_TYPE_WINDOWS_X64) return GetASMHeader_x64();
	else return GetASMHeader_x86();
}

// Function that returns the ASM Header for x86

string ASMHeader::GetASMHeader_x86()
{
	string sContent =
		"; Shellcode generated using Shellcode Compiler                         \r\n"
		"; https://github.com/NytroRST/ShellcodeCompiler                    \r\n\r\n"

		"xor ecx, ecx                                                           \r\n"
		"mov eax, fs:[ecx + 0x30]               ; EAX = PEB                     \r\n"
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

// Function that returns the ASM Header for x64

string ASMHeader::GetASMHeader_x64()
{
	string sContent =
		"; Shellcode generated using Shellcode Compiler\r\n"
		"; https://github.com/NytroRST/ShellcodeCompiler \r\n\r\n"

		"sub   RSP, 0x28                        ; 40 bytes of shadow space                  \r\n"
		"and   RSP, 0FFFFFFFFFFFFFFF0h          ; Align the stack to a multiple of 16 bytes \r\n\r\n"

		"xor rcx, rcx                           ; RCX = 0                                   \r\n"
		"mov rax, gs:[rcx + 0x60]               ; RAX = PEB                                 \r\n"
		"mov rax, [rax + 0x18]                  ; RAX = PEB->Ldr                            \r\n"
		"mov rsi, [rax + 0x20]                  ; RSI = PEB->Ldr.InMemOrder                 \r\n"
		"lodsq                                  ; RAX = Second module                       \r\n"
		"xchg rax, rsi                          ; RAX = RSI, RSI = RAX                      \r\n"
		"lodsq                                  ; RAX = Third(kernel32)                     \r\n"
		"mov rbx, [rax + 0x20]                  ; RBX = Base address                        \r\n\r\n"

		"xor r8, r8                             ; Clear r8                                  \r\n"
		"mov r8d, [rbx + 0x3c]                  ; R8D = DOS->e_lfanew offset                \r\n"
		"mov rdx, r8                            ; RDX = DOS->e_lfanew                       \r\n"
		"add rdx, rbx                           ; RDX = PE Header                           \r\n"
		"xor rcx, rcx                           ; RCX = 0                                   \r\n"
		"mov cl, 0x88                           ; RCX = 0x88 - Offset export table          \r\n"
		"add rcx, rdx                           ; RCX = PE Header + Offset export table     \r\n"
		"mov r8d, [rcx]                         ; R8D = Offset export table                 \r\n"
		"add r8, rbx                            ; R8 = Export table                         \r\n"
		"xor rsi, rsi                           ; Clear RSI                                 \r\n"
		"mov esi, [r8 + 0x20]                   ; RSI = Offset namestable                   \r\n"
		"add rsi, rbx                           ; RSI = Names table                         \r\n"
		"xor rcx, rcx                           ; RCX = 0                                   \r\n"
		"mov r9, 0x41636f7250746547             ; GetProcA                                  \r\n\r\n"

		"Get_Function:                                                                      \r\n\r\n"

		"inc rcx                                ; Increment the ordinal                     \r\n"
		"xor rax, rax                           ; RAX = 0                                   \r\n"
		"mov eax, [rsi + rcx * 4]               ; Get name offset                           \r\n"
		"add rax, rbx                           ; Get function name                         \r\n"
		"cmp [rax], r9                          ; GetProcA ?                                \r\n"
		"jnz Get_Function                                                                   \r\n"
		"xor rsi, rsi                           ; RSI = 0                                   \r\n"
		"mov esi, [r8 + 0x24]                   ; ESI = Offset ordinals                     \r\n"
		"add rsi, rbx                           ; RSI = Ordinals table                      \r\n"
		"mov cx, [rsi + rcx * 2]                ; Number of function                        \r\n"
		"xor rsi, rsi                           ; RSI = 0                                   \r\n"
		"mov esi, [r8 + 0x1c]                   ; Offset address table                      \r\n"
		"add rsi, rbx                           ; ESI = Address table                       \r\n"
		"xor rdx, rdx                           ; RDX = 0                                   \r\n"
		"mov edx, [rsi + rcx * 4]               ; EDX = Pointer(offset)                     \r\n"
		"add rdx, rbx                           ; RDX = GetProcAddress                      \r\n"
		"mov rdi, rdx                           ; Save GetProcAddress in RDI                \r\n\r\n"

		"mov ecx, 0x41797261                    ; aryA                                      \r\n"
		"push rcx                               ; Push on the stack                         \r\n"
		"mov rcx, 0x7262694c64616f4c            ; LoadLibr                                  \r\n"
		"push rcx                               ; Push on stack                             \r\n"
		"mov rdx, rsp                           ; LoadLibraryA                              \r\n"
		"mov rcx, rbx                           ; kernel32.dll base address                 \r\n"
		"sub rsp, 0x30                                                                      \r\n"
		"call rdi                               ; GetProcAddress                            \r\n"
		"add rsp, 0x30                                                                      \r\n"
		"add rsp, 0x10                          ; Clean space for LoadLibrary string        \r\n"
		"mov rsi, rax                           ; LoadLibrary saved in RSI                  \r\n\r\n ";

	return sContent;
}
