#ifndef _Protect_h_
#define _Protect_h_

#include <Core/Core.h>
#include "RC4.h"

using namespace Upp;

#define PROTECT_START_MARKER	"\xba\xad\xde\xad\xfa\xce"
#define PROTECT_END_MARKER		"\xc0\xde\xde\xad\xfe\xed"

#ifdef flagPROTECT

#ifdef PLATFORM_POSIX

#define PROTECT_START_FUNC(decrFunc) \
	static bool __decrypted = false; \
	void (*ptr)(void); \
	dword size; \
	if(!__decrypted) \
	{ \
		decrFunc((byte *)&&__start, (byte *)&&__end - (byte *)&&__start); \
		__decrypted = true; \
		asm volatile ( \
			"\txor %%eax, %%eax\n" \
			"\tcpuid\n" \
			: \
			: \
			: "eax", "ebx", "ecx", "edx" \
	    ); \
	} \
	if(!__decrypted) \
		goto __end; \
	asm volatile( \
		"\tjmp 1f\n" \
		"\t.ascii \""PROTECT_START_MARKER"\"\n" \
		"1:\n" \
	); \
	__start: \
	{ \

#define PROTECT_END_FUNC \
	}  \
	__end: \
	asm volatile ( \
		"\tjmp 2f\n" \
		"\t.ascii \""PROTECT_END_MARKER"\"\n" \
		"2:\n" \
	);
	
#else

#define _PROTECT_START_MARKER	__asm _emit 0xba __asm _emit 0xad __asm _emit 0xde __asm _emit 0xad __asm _emit 0xfa __asm _emit 0xce
#define _PROTECT_END_MARKER		__asm _emit 0xc0 __asm _emit 0xde __asm _emit 0xde __asm _emit 0xad __asm _emit 0xfe __asm _emit 0xed

#define PROTECT_START_FUNC(decrFunc) \
	static bool __decrypted = false; \
	byte *start, *end; \
	if(!__decrypted) \
	{ \
		__asm \
		{ \
			__asm lea eax, __start \
			__asm mov start, eax \
			__asm lea eax, __end \
			__asm mov end, eax \
		}; \
		decrFunc(start, end - start); \
		__decrypted = true; \
		__asm \
		{ \
			__asm push eax \
			__asm push ebx \
			__asm push ecx \
			__asm push edx \
			__asm xor eax, eax \
			__asm cpuid \
			__asm pop edx \
			__asm pop ecx \
			__asm pop ebx \
			__asm pop eax \
			__asm lea eax, __start \
			__asm mov start, eax \
			__asm lea eax, __end \
			__asm mov end, eax \
		}; \
	} \
	if(!__decrypted) \
		goto __end; \
	__asm { \
		__asm jmp next \
		_PROTECT_START_MARKER \
		__asm next: \
	} \
	__start: \
	{ \

#define PROTECT_END_FUNC \
	} \
	__end: \
	__asm { __asm jmp next2 } \
	_PROTECT_END_MARKER; \
	__asm next2:

#endif

#else

#define PROTECT_START_FUNC(decrFunc)
#define PROTECT_END_FUNC

#endif

bool PROTECT_DECRYPT(byte *start, size_t size, String const &key);

#endif
