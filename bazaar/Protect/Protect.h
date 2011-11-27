#ifndef _Protect_h_
#define _Protect_h_

#include <Core/Core.h>
#include <Cypher/Cypher.h>

using namespace Upp;

#define PROTECT_START_MARKER	"\xba\xad\xde\xad\xfa\xce"
#define PROTECT_END_MARKER		"\xc0\xde\xde\xad\xfe\xed"

#define OBFUSCATE_START_MARKER	"\xba\xad\xde\xad\xfa\xde"
#define OBFUSCATE_END_MARKER	"\xc0\xde\xde\xad\xfe\xde"

#ifdef flagPROTECT

#ifdef PLATFORM_POSIX

#ifdef flagMT
#define PROTECT_START_FUNC1(decrFunc) \
	static volatile bool __decrypted = false; \
	INTERLOCKED \
	{ \
		if(!__decrypted) \
		{ \
			PROTECT_WRITE_ACCESS((byte *)&&__start, (byte *)&&__end - (byte *)&&__start, true); \
			decrFunc((byte *)&&__start, (byte *)&&__end - (byte *)&&__start, (byte *)&&__init + 2, 6 /* sizeof(PROTECT_START_MARK) */); \
			PROTECT_WRITE_ACCESS((byte *)&&__start, (byte *)&&__end - (byte *)&&__start, false); \
			__decrypted = true; \
			asm volatile ( \
				"\txor %%eax, %%eax\n" \
				"\tcpuid\n" \
				: \
				: \
				: "eax", "ebx", "ecx", "edx" \
		    ); \
		} \
	}
	
/* THIS BECAUSE THEIDE HANGS... */
#define PROTECT_START_FUNC2 \
	if(__decrypted) \
	{ \
		if(!__decrypted) \
		{ \
			__init: \
				asm volatile( \
					"\tjmp 1f\n" \
					"\t.ascii \""PROTECT_START_MARKER"\"\n" \
					"1:\n" \
				); \
		} \
		__start:

#define PROTECT_START_FUNC(DecrFunc) PROTECT_START_FUNC1(DecrFunc) PROTECT_START_FUNC2
#else
#define PROTECT_START_FUNC1(decrFunc) \
	static volatile bool __decrypted = false; \
	if(!__decrypted) \
	{ \
		PROTECT_WRITE_ACCESS((byte *)&&__start, (byte *)&&__end - (byte *)&&__start, true); \
		decrFunc((byte *)&&__start, (byte *)&&__end - (byte *)&&__start, (byte *)&&__init + 2, 6 /* sizeof(PROTECT_START_MARK) */); \
		PROTECT_WRITE_ACCESS((byte *)&&__start, (byte *)&&__end - (byte *)&&__start, false); \
		__decrypted = true; \
		asm volatile ( \
			"\txor %%eax, %%eax\n" \
			"\tcpuid\n" \
			: \
			: \
			: "eax", "ebx", "ecx", "edx" \
	    ); \
	}
	
/* THIS BECAUSE THEIDE HANGS... */
#define PROTECT_START_FUNC2 \
	if(__decrypted) \
	{ \
		if(!__decrypted) \
		{ \
			__init: \
				asm volatile( \
					"\tjmp 1f\n" \
					"\t.ascii \""PROTECT_START_MARKER"\"\n" \
					"1:\n" \
				); \
		} \
		__start:

#define PROTECT_START_FUNC(DecrFunc) PROTECT_START_FUNC1(DecrFunc) PROTECT_START_FUNC2
#endif

#define PROTECT_END_FUNC \
	}  \
	else \
	{ \
		__end: \
		asm volatile ( \
			"\tjmp 2f\n" \
			"\t.ascii \""PROTECT_END_MARKER"\"\n" \
			"2:\n" \
			::: \
		); \
		goto __start; \
	}
	
#define OBFUSCATE_START_FUNC \
	static volatile bool __xxx = false; \
	PROTECT_OBFUSCATE((byte *)&&__start, (byte *)&&__end - (byte *)&&__start + 2, (byte *)&&__init + 2, 16 /* sizeof(OBFUSCATE_START_MARKER) + sizeof("0123456789") */); \
	asm volatile ( \
		"\txor %%eax, %%eax\n" \
		"\tcpuid\n" \
		: \
		: \
		: "eax", "ebx", "ecx", "edx" \
    ); \
    __xxx = true; \
		if(__xxx) \
		{ \
			__init: \
			asm __volatile__( \
				"\tjmp 1f\n" \
				"\t.ascii \""OBFUSCATE_START_MARKER"\"\n" \
				"\t.ascii \"0123456789\"\n" \
				"1:\n" \
			); \
		} \
		__start: \
		if(__xxx) \
			__xxx++;

#define OBFUSCATE_END_FUNC \
		if(__xxx) \
			__xxx++; \
		__end: \
			asm __volatile__( \
				"\tjmp 1f\n" \
				"\t.ascii \""OBFUSCATE_END_MARKER"\"\n" \
				"1:\n" \
			); \
			PROTECT_OBFUSCATE((byte *)&&__start, (byte *)&&__end - (byte *)&&__start + 2, (byte *)&&__init + 2, 16 /* sizeof(OBFUSCATE_START_MARKER) + sizeof("0123456789") */);

#else

#define _PROTECT_START_MARKER	__asm _emit 0xba __asm _emit 0xad __asm _emit 0xde __asm _emit 0xad __asm _emit 0xfa __asm _emit 0xce
#define _PROTECT_END_MARKER		__asm _emit 0xc0 __asm _emit 0xde __asm _emit 0xde __asm _emit 0xad __asm _emit 0xfe __asm _emit 0xed

#define _OBFUSCATE_START_MARKER	__asm _emit 0xba __asm _emit 0xad __asm _emit 0xde __asm _emit 0xad __asm _emit 0xfa __asm _emit 0xde
#define _OBFUSCATE_END_MARKER	__asm _emit 0xc0 __asm _emit 0xde __asm _emit 0xde __asm _emit 0xad __asm _emit 0xfe __asm _emit 0xde
#define _OBFUSCATE_KEYPLACER	__asm _emit 0x00 __asm _emit 0x01 __asm _emit 0x02 __asm _emit 0x03 __asm _emit 0x04 __asm _emit 0x05 __asm _emit 0x06 __asm _emit 0x07 __asm _emit 0x08 __asm _emit 0x09

#ifdef flagMT
#define PROTECT_START_FUNC(decrFunc) \
	static volatile bool __decrypted = false; \
	INTERLOCKED \
	{ \
		byte *__startPtr, *__endPtr, *__noncePtr; \
		if(!__decrypted) \
		{ \
			__asm \
			{ \
				__asm push eax \
				__asm lea eax, __start \
				__asm mov __startPtr, eax \
				__asm lea eax, __end \
				__asm mov __endPtr, eax \
				__asm lea eax, __init \
				__asm inc eax \
				__asm inc eax \
				__asm mov __noncePtr, eax \
				__asm pop eax \
			}; \
			PROTECT_WRITE_ACCESS(__startPtr, __endPtr - __startPtr, true); \
			decrFunc(__startPtr, __endPtr - __startPtr, __noncePtr, 6); \
			PROTECT_WRITE_ACCESS(__startPtr, __endPtr - __startPtr, false); \
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
			}; \
		} \
	} \
	if(!__decrypted) \
		goto __end; \
	__init: \
	__asm { \
		__asm jmp __next \
		_PROTECT_START_MARKER \
		__asm __next: \
	} \
	__start: \
	{
#else
#define PROTECT_START_FUNC(decrFunc) \
	static bool __decrypted = false; \
	byte *__startPtr, *__endPtr, *__noncePtr; \
	if(!__decrypted) \
	{ \
		__asm \
		{ \
			__asm push eax \
			__asm lea eax, __start \
			__asm mov __startPtr, eax \
			__asm lea eax, __end \
			__asm mov __endPtr, eax \
			__asm lea eax, __init \
			__asm inc eax \
			__asm inc eax \
			__asm mov __noncePtr, eax \
			__asm pop eax \
		}; \
		PROTECT_WRITE_ACCESS(__startPtr, __endPtr - __startPtr, true); \
		decrFunc(__startPtr, __endPtr - __startPtr, __noncePtr, 6); \
		PROTECT_WRITE_ACCESS(__startPtr, __endPtr - __startPtr, false); \
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
		}; \
	} \
	if(!__decrypted) \
		goto __end; \
	__init: \
	__asm { \
		__asm jmp __next \
		_PROTECT_START_MARKER \
		__asm __next: \
	} \
	__start: \
	{
#endif

#define PROTECT_END_FUNC \
	} \
	__end: \
	__asm { \
		__asm jmp __next2 \
		_PROTECT_END_MARKER \
		__asm __next2: \
	};

#define OBFUSCATE_START_FUNC \
	byte *__startPtr, *__endPtr, *__keyPtr; \
	__asm \
	{ \
		__asm push eax \
		__asm lea eax, __start \
		__asm mov __startPtr, eax \
		__asm lea eax, __end \
		__asm mov __endPtr, eax \
		__asm lea eax, __init \
		__asm inc eax \
		__asm inc eax \
		__asm mov __keyPtr, eax \
		__asm pop eax \
	}; \
	PROTECT_OBFUSCATE(__startPtr, __endPtr - __startPtr, __keyPtr, 16); \
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
	}; \
	__init: \
	__asm { \
		__asm jmp __next \
		_OBFUSCATE_START_MARKER \
		_OBFUSCATE_KEYPLACER \
		__asm __next: \
	} \
	__start: \

#define OBFUSCATE_END_FUNC \
	__asm { \
		__asm jmp __next2 \
		_OBFUSCATE_END_MARKER \
		__asm __next2: \
	}; \
	__end: \
	PROTECT_OBFUSCATE(__startPtr, __endPtr - __startPtr, __keyPtr, 16)


#endif

// Check macro for program startup -- executes next statement if invalid key
#define ON_PROTECT_BAD_KEY(decrFunc) \
	bool __keyOk; \
	{ \
		const char *crypted = PROTECT_START_MARKER "abracadabra" PROTECT_END_MARKER; \
		const int len = strlen("abracadabra"); \
		Buffer<byte>buf(len); \
		Buffer<byte>nonce(6); \
		/* can't use memcpy, it gets optimized out */ \
	    for(int i = 0; i < len; i++) buf[i] = crypted[i + 6 /* sizeof(PROTECT_START_MARKER)*/]; \
	    for(int i = 0; i < 6 /* sizeof(PROTECT_START_MARKER)*/; i++) nonce[i] = crypted[i]; \
		decrFunc(buf, len, nonce, 6); \
		__keyOk = !memcmp(buf, "abracadabra", len); \
	} \
	if(!__keyOk)

#else

#define PROTECT_START_FUNC(decrFunc)
#define PROTECT_END_FUNC
#define OBFUSCATE_START_FUNC
#define OBFUSCATE_END_FUNC
#define ON_PROTECT_BAD_KEY(decrFunc) if(false)

#endif

bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access);
void PROTECT_DECRYPT(byte *start, size_t size, String const &key, byte const *nonce, size_t nonceLen);
void PROTECT_OBFUSCATE(byte *start, size_t len, byte *key, size_t keyLen);

#endif
