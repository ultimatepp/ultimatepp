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
	
	Md5Stream();
	~Md5Stream();
};

void    MD5(byte *hash16, const void *data, dword size);
void    MD5(byte *hash16, const String& data);
String  MD5String(const void *data, dword size);
String  MD5String(const String& data);
String  MD5StringS(const void *data, dword size);
String  MD5StringS(const String& data);

class Sha1Stream : public OutStream {
	dword state[5];
	byte  buffer[64];
	int   pos;
	dword size;

	virtual  void  Out(const void *data, dword size);

	void  Cleanup();

public:
	void   Finish(byte *hash20);
	String FinishString();
	String FinishStringS();

	void   New();
	
	Sha1Stream(); 
	~Sha1Stream();
};

void    SHA1(byte *hash20, const void *data, dword size);
void    SHA1(byte *hash20, const String& s);
String  SHA1String(const void *data, dword size);
String  SHA1String(const String& data);
String  SHA1StringS(const void *data, dword size);
String  SHA1StringS(const String& data);

#endif
