#include "Protect.h"

#include <Xed/Xed.h>

#ifndef COMPILER_GCC
	#include <intrin.h>
	#pragma intrinsic(_ReturnAddress)
#endif

NAMESPACE_UPP

// uncomment this if you want debug messages
//#define PROTECT_DEBUG

//////////////////////////////////////////////////////////////
// 6 calls sequences to encode various CODE encrypted parts:
//
// CODE ENCRYPT START : 1 11122
// CODE ENCRYPT END   : 1 11121
// OBFUSCATE START    : 1 22111
// OBFUSCATE END      : 1 22112
//
// 2 data sequences to encode DATA encrypted parts
// DATA_ENCRYPT_START : "\xba\xad\xde\xad\xce\xfa"
// DATA_ENCRYPT_END   : "\xc0\xde\xde\xad\xfe\xed"
//
// ALL OTHER SEQUENCES ARE CONSIDERED INVALID
//////////////////////////////////////////////////////////////
const char *PROTECT_CODE_START		= "111122";
const char *PROTECT_CODE_END		= "111121";
const char *PROTECT_OBFUSCATE_START	= "122111";
const char *PROTECT_OBFUSCATE_END	= "122112";

// 2 dummy functions used to mark encrypted code blocks
// the volatile dummy data are there to avoid that the linker
// collapses the 2 dummy functions in a single one or that
// the optimizer throw them away
volatile int __dummy1_val = 0;
void PROTECT_DUMMY1(void)
{
	__dummy1_val++;
}

volatile int __dummy2_val = 0;
void PROTECT_DUMMY2(void)
{
	__dummy2_val++;
}

volatile bool __PROTECT_DUMMY_BOOL = false;

// nonce used in cryptography
String PROTECT_NONCE = "12345678";

// check if pattern at buffer's start is one of the PROTECT markers
CodeSequence PROTECT_CheckPattern(uint8_t const *buf)
{
	// check for data start pattern
	// we can't to at once with the whole pattern, otherwise the encryptor will detect it
	if(!memcmp(buf, PROTECT_DATA_START_1, 3) && !memcmp(buf + 3, PROTECT_DATA_START_2, 3))
		return SEQ_DATA_START;

	// check for data end pattern
	// we can't to at once with the whole pattern, otherwise the encryptor will detect it
	if(!memcmp(buf, PROTECT_DATA_END_1, 3) && !memcmp(buf + 3, PROTECT_DATA_END_2, 3))
		return SEQ_DATA_END;

	// verify that we've got a 6 calls sequence
	for(int i = 0; i < 6; i++)
		if(buf[5 * i] != 0xe8)
			return SEQ_NONE;
		
	// first call is always an 1
	uint32_t firstAddr = *(uint32_t *)(buf + 1);
	uint32_t secondAddr = 0;
	char foundSeq[7];
	memset(foundSeq, 0, 7);
	foundSeq[0] = '1';
	for(int i = 1; i < 6; i++)
	{
		// get current call address and correct it with position
		uint32_t addr = *(uint32_t *)(buf + 5 * i + 1) + 5 * i;
		
		if(addr == firstAddr)
			foundSeq[i] = '1';
		else if(secondAddr == 0)
		{
			secondAddr = addr;
			foundSeq[i] = '2';
		}
		else if(addr == secondAddr)
			foundSeq[i] = '2';
		else
			return SEQ_NONE;
	}
	
	// now check which sequence we got
	if(!strcmp(foundSeq, PROTECT_CODE_START))
		return SEQ_CODE_START;
	else if(!strcmp(foundSeq, PROTECT_CODE_END))
		return SEQ_CODE_END;
	else if(!strcmp(foundSeq, PROTECT_OBFUSCATE_START))
		return SEQ_OBFUSCATE_START;
	else if(!strcmp(foundSeq, PROTECT_OBFUSCATE_END))
		return SEQ_OBFUSCATE_END;
	else
		return SEQ_NONE;
}

// locate a pattern starting from given buffer's start
// return distance from buffer's start, or 0 if not found or bad pattern found
uint32_t PROTECT_FindPattern(uint8_t const *start, CodeSequence requested, uint32_t scanLimit)
{
	uint8_t const *buf = start;
	uint8_t const *bufEnd = buf + scanLimit - PROTECT_PatternSize(requested);

	while(buf <= bufEnd)
	{
		// check for a sequence
		CodeSequence endSeq = PROTECT_CheckPattern(buf);
		
		// if no sequence found, go on
		if(endSeq == SEQ_NONE)
		{
			buf++;
			continue;
		}
		
		// it the requested end sequence was found, all ok
		else if(endSeq == requested)
			return buf - start;
		
		// unexpected sequence... return null sequence
#ifdef PROTECT_DEBUG
		RLOG("Unexpected sequence '" << (int)endSeq << " requested was " << (int)requested);
#endif
		return 0;
	}
	return 0;
}

// get size of patterns
int PROTECT_PatternSize(CodeSequence seq)
{
	switch(seq)
	{
		case SEQ_CODE_START:
		case SEQ_CODE_END:
		case SEQ_OBFUSCATE_START:
		case SEQ_OBFUSCATE_END:
			return 30;
			
		case SEQ_DATA_START:
		case SEQ_DATA_END:
			return 6;
			
		default:
			return 0;
	}
}

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#include <unistd.h>

bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access)
{
	// round start and size to page size -- needed for mprotect
	long pSize = sysconf(_SC_PAGESIZE );
	long delta =  (long)start % pSize;
	if(delta)
	{
		start -= delta;
		size += delta;
	}
	return mprotect(start, size, access ? PROT_READ | PROT_WRITE | PROT_EXEC : PROT_READ | PROT_EXEC) == 0;
}
#else

bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access)
{
	dword oldProt;
	bool res = VirtualProtect(start, size, access ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ, &oldProt);
	
	return res;
}
#endif

void PROTECT_DECRYPT_CODE(volatile bool &decrypted, One<Cypher>(*GetCypher)(uint8_t const *nonce, int nonceSize))
{
	INTERLOCKED {
#ifdef PROTECT_DEBUG
		RLOG("START DECRYPT");
#endif
	
		// if already decrypted, just return
		if(decrypted)
			return;
	
	// get function return address
	// this should be the start of code to be decrypted
#ifdef COMPILER_GCC
		uint8_t *start = (uint8_t *)__builtin_return_address(0);
#else
		uint8_t *start = (uint8_t *)_ReturnAddress();
#endif

		// calculate nonce size
		int nonceSize = PROTECT_PatternSize(SEQ_CODE_START) - 2;
	
		// to be sure, just check we've got the relative jmp at code start
		// otherwise something went very wrong
		if(*start != 0xeb || *(start + 1) != nonceSize)
		{
			// on MSC (and maybe others) the compiler put some stuff AFTER
			// function return, so the JMP is not exactly at return address
			// we try to find it looking some 20 bytes forward
			bool found = false;
			for(int i = 0; i < 20; i++)
			{
				start++;
				if(*start == 0xeb && *(start + 1) == nonceSize)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
#ifdef PROTECT_DEBUG
				RLOG("JMP NOT FOUND");
#endif
				return;
			}
		}
		
		// chunk size is encoded as a 32 bit unsigned integer
		// after the JMP
		uint32_t size = *(uint32_t const *)(start + 2);
		
		// NONCE is located after the JMP, and includes the size field
		uint8_t const *nonce = start + 2;
		
		// go to code to be decrypted
		start += nonceSize + 2;
		
		// create the decrypter
		One<Cypher>cypher = GetCypher(nonce, nonceSize);
	
		if(!PROTECT_WRITE_ACCESS(start, size, true))
			return;
	
		// code must be processed byte by byte
		uint8_t *end = start + size;
		uint8_t *cur = start;
		while(cur < end)
		{
			(*cypher)(cur, 1);
			int len = XED.InstructionLength(cur);
#ifdef PROTECT_DEBUG
			RLOG(Format("LEN:%2d - %s", len, XED.DisassembleInstruction(cur, true)));
#endif
			cur += len;
		}
				
		PROTECT_WRITE_ACCESS(start, size, false);
	
		// signal that we've already decrypted this chunk
		decrypted = true;
	}
}

String PROTECT_DECRYPT_DATA(One<Cypher>(*GetCypher)(uint8_t const *nonce, int nonceSize), const char *data)
{

#ifdef flagPROTECT

	// extract data size (it's on data beginning)
	uint32_t size = *(uint32_t *)data;
	
	// get nonce size
	int nonceSize = PROTECT_PatternSize(SEQ_DATA_START);

	// go to encrypted data
	data += nonceSize;

	// and its lenght is PROTECT_PatternSize(SEQ_DATA_START)
	uint8_t const *nonce = (uint8_t const *)(data + size);

	// copy data to be decrypted (don't do in place)
	char *dec = (char *)malloc(size + 1);
	memcpy(dec, data, size);
	dec[size] = 0;
	(*GetCypher(nonce, nonceSize))((uint8_t *)dec, size);
	String res = dec;
	free(dec);

#else

	// skip starting pattern
	data += PROTECT_PatternSize(SEQ_DATA_START);
	
	// look for closing pattern
	uint32_t size = PROTECT_FindPattern((uint8_t const *)data, SEQ_DATA_END, 100000);
	
	if(!size)
		return "";
	
	char *dec = (char *)malloc(size + 1);
	memcpy(dec, data, size);
	dec[size] = 0;
	String res = dec;
	free(dec);
	
#endif

	return res;
}

void PROTECT_DEOBFUSCATE(uint8_t *&hdr)
{
#ifdef PROTECT_DEBUG
	RLOG("START DE-OBFUSCATE");
#endif
	
	// get function return address
	// this should be the start of code to be decrypted or
	// end of code to be encrypted
#ifdef COMPILER_GCC
	hdr = (uint8_t *)__builtin_return_address(0);
#else
	hdr = (uint8_t *)_ReturnAddress();
#endif
	
	// calculate nonce size
	int nonceSize = PROTECT_PatternSize(SEQ_CODE_START) - 2;

#ifdef PROTECT_DEBUG
	RLOG("------------------------------");
	RLOG("SOME BYTES AROUND HEADER START");
	int i = -10;
	while(i < 40)
	{
		RLOG(XED.DisassembleInstruction(hdr + i, true));
		i += XED.InstructionLength(hdr + i);
	}
	RLOG("------------------------------");
#endif

	// there must be a JMP to end of header here
	// otherwise something was very wrong
	if(*hdr != 0xeb || *(hdr + 1) != nonceSize)
	{
		// on MSC (and maybe others) the compiler put some stuff AFTER
		// function return, so the JMP is not exactly at return address
		// we try to find it looking some 20 bytes forward
		bool found = false;
		for(int i = 0; i < 20; i++)
		{
			hdr++;
			if(*hdr == 0xeb && *(hdr + 1) == nonceSize)
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
#ifdef PROTECT_DEBUG
			RLOG("JMP NOT FOUND");
#endif
			return;
		}
	}
	
	// chunk size is encoded as a 32 bit unsigned integer
	// after the JMP
	uint32_t size = *(uint32_t const *)(hdr + 2);

	// NONCE is located after the JMP, and includes the size field
	uint8_t const *nonce = hdr + 2;
	
	// go to code to be decrypted
	uint8_t *start = hdr + nonceSize + 2;
	
	// get pointer to end header
	uint8_t *endHdr = start + size;
	
	// get key
	int keySize = 16;
	uint8_t const *key = endHdr + 2 + (nonceSize - keySize);
	
	// create the decrypter - we always use snow for obfuscation
	Snow2 snow2(key, keySize, nonce, nonceSize);

	if(!PROTECT_WRITE_ACCESS(start, size, true))
		return;

	// code must be processed byte by byte
	uint8_t *cur = start;
	while(cur < endHdr)
	{
		// decrypt current instruction
		snow2(cur, 1);
		
		// now it's possible to get its correct length
		int len = XED.InstructionLength(cur);
		
#ifdef PROTECT_DEBUG
		RLOG(Format("LEN:%2d - %s", len, XED.DisassembleInstruction(cur, true)));
#endif

		// go to next instruction
		cur += len;
	}
			
	PROTECT_WRITE_ACCESS(start, size, false);
#ifdef PROTECT_DEBUG
	RLOG("END DE-OBFUSCATE");
#endif
}


void PROTECT_OBFUSCATE(uint8_t *&hdr)
{
#ifdef PROTECT_DEBUG
	RLOG("START OBFUSCATE");
#endif
	
	// the hdr field has been already calculated by de-obfuscation code
	// so we just use it
	// get function return address
	
	// calculate nonce size
	int nonceSize = PROTECT_PatternSize(SEQ_CODE_START) - 2;

	// chunk size is encoded as a 32 bit unsigned integer
	// after the JMP
	uint32_t size = *(uint32_t const *)(hdr + 2);

	// NONCE is located after the JMP, and includes the size field
	uint8_t const *nonce = hdr + 2;
	
	// go to code to be decrypted
	uint8_t *start = hdr + nonceSize + 2;
	
	// get pointer to end header
	uint8_t *endHdr = start + size;
	
	// get key
	int keySize = 16;
	uint8_t const *key = endHdr + 2 + (nonceSize - keySize);
	
	// create the decrypter - we always use snow for obfuscation
	Snow2 snow2(key, keySize, nonce, nonceSize);

	if(!PROTECT_WRITE_ACCESS(start, size, true))
		return;

	// code must be processed byte by byte
	uint8_t *cur = start;
	while(cur < endHdr)
	{
		// get instruction length before obfuscation
		int len = XED.InstructionLength(cur);
#ifdef PROTECT_DEBUG
		RLOG(Format("LEN:%2d - %s", len, XED.DisassembleInstruction(cur, true)));
#endif

		// encrypt the instruction
		snow2(cur, 1);
		
		// go to next instruction
		cur += len;
	}

	PROTECT_WRITE_ACCESS(start, size, false);
#ifdef PROTECT_DEBUG
	RLOG("END OBFUSCATE");
#endif
}

END_UPP_NAMESPACE