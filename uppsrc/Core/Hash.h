#ifndef _Core_Hash_h_
#define _Core_Hash_h_

/* MD5 context. */
typedef struct {
	dword state[4];                                   /* state (ABCD) */
	dword count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} UPP_MD5_CTX;

class Md5Stream : public OutStream {
	UPP_MD5_CTX context;

	virtual  void  Out(const void *data, dword size);

public:
	void   Finish(byte *hash16);
	String FinishString();
	String FinishStringS();
	void   Reset();
	
	Md5Stream();
	~Md5Stream();
};

void    MD5(byte *hash16, const void *data, dword size);
void    MD5(byte *hash16, const String& data);
String  MD5String(const void *data, dword size);
String  MD5String(const String& data);
String  MD5StringS(const void *data, dword size);
String  MD5StringS(const String& data);

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} UPP_SHA1_CTX;

class Sha1Stream : public OutStream {
	UPP_SHA1_CTX ctx[1];

	virtual  void  Out(const void *data, dword size);

	void  Cleanup()                      { memset(ctx, 0, sizeof(ctx)); }

public:
	void   Finish(byte *hash20);
	String FinishString();
	String FinishStringS();

	void   Reset();
	void   New()                         { Reset(); }
	
	Sha1Stream();
	~Sha1Stream();
};

void    SHA1(byte *hash20, const void *data, dword size);
void    SHA1(byte *hash20, const String& s);
String  SHA1String(const void *data, dword size);
String  SHA1String(const String& data);
String  SHA1StringS(const void *data, dword size);
String  SHA1StringS(const String& data);

class Sha256Stream : public OutStream {
	byte  buffer[128];

	virtual  void  Out(const void *data, dword size);

	void  Cleanup();

public:
	void   Finish(byte *hash32);
	String FinishString();
	String FinishStringS();

	void   Reset();
	void   New()                         { Reset(); }
	
	Sha256Stream(); 
	~Sha256Stream();
};

void    SHA256(byte *hash32, const void *data, dword size);
void    SHA256(byte *hash32, const String& s);
String  SHA256String(const void *data, dword size);
String  SHA256String(const String& data);
String  SHA256StringS(const void *data, dword size);
String  SHA256StringS(const String& data);

class xxHashStream : public OutStream {
	byte context[8 * 8];
	
	virtual  void  Out(const void *data, dword size);

public:
	int Finish();
	
	void Reset(dword seed = 0);
	
	xxHashStream(dword seed = 0);
};

int xxHash(const void *data, size_t len);
int xxHash(const String& s);

class xxHash64Stream : public OutStream {
	byte context[12 * 8];
	
	virtual  void  Out(const void *data, dword size);

public:
	int64 Finish();
	
	void Reset(dword seed = 0);
	
	xxHash64Stream(dword seed = 0);
};

int64 xxHash64(const void *data, size_t len);
int64 xxHash64(const String& s);

#endif
