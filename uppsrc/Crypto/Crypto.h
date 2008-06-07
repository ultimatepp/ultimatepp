#ifndef CRYPTO_H
#define CRYPTO_H

#include <Core/Core.h>

NAMESPACE_UPP

void   SHA1Transform(dword state[5], byte buffer[64]);
void   SHA1Init(dword state[5]);
void   SHA1Size(byte buffer[64], dword size);
String SHA1(const void *s, dword size);
String SHA1(const String& s);

class Sha1 {
	dword state[5];
	byte  buffer[64];
	int   pos;
	dword size;

public:
	void    Put(const void *data, dword length);
	void    Put(const String& s)                      { Put(s, s.GetLength()); }
	String  Finish();
	dword   GetSize()                                 { return size; }

	Sha1();
	~Sha1();
};

String EncodeBscF(const String& src);
String DecodeBscF(const String& src);

END_UPP_NAMESPACE

#endif
