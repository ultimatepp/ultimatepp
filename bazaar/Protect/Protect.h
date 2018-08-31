#ifndef _Protect_Protect_h_
#define _Protect_Protect_h_

#include <Core/Core.h>
#include <Cypher/Cypher.h>

NAMESPACE_UPP

// 2 dummy functions used to mark encrypted code blocks
void PROTECT_DUMMY1(void);
void PROTECT_DUMMY2(void);

extern volatile bool __PROTECT_DUMMY_BOOL;

// nonce used in cryptography
extern String PROTECT_NONCE;

bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access);
void PROTECT_DECRYPT_CODE(volatile bool &decrypted, One<Cypher>(*GetCypher)(uint8_t const *nonce, int nonceSize));
String PROTECT_DECRYPT_DATA(One<Cypher>(*GetCypher)(uint8_t const *nonce, int nonceSize), const char *data);
void PROTECT_DEOBFUSCATE(uint8_t *&hdr);
void PROTECT_OBFUSCATE(uint8_t *&hdr);

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
extern const char *PROTECT_CODE_START;
extern const char *PROTECT_CODE_END;
extern const char *PROTECT_OBFUSCATE_START;
extern const char *PROTECT_OBFUSCATE_END;

// we MUST separate the data patterns, otherwise they will be embedded
// in code and detected by encryptor
#define PROTECT_DATA_START_1	"\xba\xad\xde"
#define PROTECT_DATA_START_2	"\xad\xce\xfa"

#define PROTECT_DATA_END_1		"\xc0\xde\xde"
#define PROTECT_DATA_END_2		"\xad\xfe\xed"

#define PROTECT_DATA_START		PROTECT_DATA_START_1 PROTECT_DATA_START_2
#define PROTECT_DATA_END		PROTECT_DATA_END_1 PROTECT_DATA_END_2

typedef enum
{
	SEQ_NONE,
	SEQ_CODE_START,
	SEQ_CODE_END,
	SEQ_DATA_START,
	SEQ_DATA_END,
	SEQ_OBFUSCATE_START,
	SEQ_OBFUSCATE_END,

} CodeSequence;

// check if pattern at buffer's start is one of the PROTECT markers
CodeSequence PROTECT_CheckPattern(uint8_t const *buf);

// locate a pattern starting from given buffer's start
// return distance from buffer's start, or 0 if not found or bad pattern found
uint32_t PROTECT_FindPattern(uint8_t const *buf, CodeSequence requested, uint32_t scanLimit);

// get size of patterns
int PROTECT_PatternSize(CodeSequence seq);

////////////////////////////////////////////////////////////////////////////////////

#ifdef flagPROTECT

#define PROTECT_START_FUNC(GetCypher) \
	volatile static bool __decrypted = false; \
	PROTECT_DECRYPT_CODE(__decrypted, GetCypher); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY1()
	
#define PROTECT_END_FUNC \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1()

#define OBFUSCATE_START_FUNC \
	Mutex __mutex; \
	__mutex.Enter(); \
	uint8_t *__hdr = NULL; \
	PROTECT_DEOBFUSCATE(__hdr); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1()
	
#define  OBFUSCATE_END_FUNC \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY2(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY1(); \
	PROTECT_DUMMY2(); \
	PROTECT_OBFUSCATE(__hdr); \
	__mutex.Leave()

// Check macro for program startup -- executes next statement if invalid key
#define ON_PROTECT_BAD_KEY(GetCypher) \
	if(PROTECT_DECRYPT_DATA(GetCypher, PROTECT_DATA_START "abracadabra" PROTECT_DATA_END) != "abracadabra")
		
#else

#define PROTECT_START_FUNC(GetCypher)
#define PROTECT_END_FUNC
#define OBFUSCATE_START_FUNC
#define OBFUSCATE_END_FUNC
#define ON_PROTECT_BAD_KEY(GetCypher) if(false)

#endif

END_UPP_NAMESPACE

#endif
