#include "Core.h"

NAMESPACE_UPP

/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain

Test Vectors (from FIPS PUB 180-1)
"abc"
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

/* Hash a single 512-bit block. This is the core of the algorithm. */
/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */

#ifdef COMPILER_MSC
#define rol(value, bits) _rotl(value, bits)
#else
#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
#endif

#define blk0(i) (block[i] = (rol(block[i],24)&0xFF00FF00)|(rol(block[i],8)&0x00FF00FF))
#define blk(i) (block[i&15] = rol(block[(i+13)&15]^block[(i+8)&15] \
               ^block[(i+2)&15]^block[i&15],1))
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

void SHA1Transform(dword state[5], byte buffer[64])
{
	dword a, b, c, d, e;
    dword *block = (dword *)buffer;
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
	a = b = c = d = e = 0;
}

void SHA1Init(dword state[5])
{
    state[0] = 0x67452301;
    state[1] = 0xEFCDAB89;
    state[2] = 0x98BADCFE;
    state[3] = 0x10325476;
    state[4] = 0xC3D2E1F0;
}

void SHA1Size(byte buffer[64], dword size)
{
	buffer[63] = byte(size << 3);
	buffer[62] = byte(size >> 5);
	buffer[61] = byte(size >> 13);
	buffer[60] = byte(size >> 21);
	buffer[59] = byte(size >> 29);
	buffer[58] = 0;
	buffer[57] = 0;
	buffer[56] = 0;
}

void SHA1Result(byte *hash20, dword state[5])
{
    for(int i = 0; i < 5; i++) {
		const byte *h = (const byte *)&state[i];
		hash20[0] = h[3];
		hash20[1] = h[2];
		hash20[2] = h[1];
		hash20[3] = h[0];
		hash20 += 4;
	}
}

void Sha1Stream::Cleanup()
{
	pos = size = 0;
	memset(buffer, 0, sizeof(buffer));
	memset(state, 0, sizeof(state));
}

void Sha1Stream::Out(const void *data, dword length)
{
	const byte *s = (const byte *)data;
	size += length;
	while(pos + length >= 64) {
		int n = 64 - pos;
		memcpy(buffer + pos, s, n);
		SHA1Transform(state, buffer);
		s += n;
		length -= n;
		pos = 0;
	}
	memcpy(buffer + pos, s, length);
	pos += length;
}

void Sha1Stream::Finish(byte *hash20)
{
	Flush();
	memset(buffer + pos, 0, 64 - pos);
	buffer[pos] = 128;
	if(pos > 55) {
		SHA1Transform(state, buffer);
		memset(buffer, 0, 64);
	}
	SHA1Size(buffer, size);
	SHA1Transform(state, buffer);
	SHA1Result(hash20, state);
	Cleanup();
}

String Sha1Stream::FinishString()
{
	byte hash[20];
	Finish(hash);
	return HexString(hash, 20);
}

String Sha1Stream::FinishStringS()
{
	byte hash[20];
	Finish(hash);
	return HexString(hash, 20, 4);
}

void Sha1Stream::New() {
	SHA1Init(state);
	pos = 0;
	size = 0;
}

Sha1Stream::Sha1Stream()
{
	New();
}

Sha1Stream::~Sha1Stream()
{
	Cleanup();
}

void SHA1(byte *hash20, const void *data, dword size)
{
	Sha1Stream sha1;
	sha1.Put(data, size);
	sha1.Finish(hash20);
}

void SHA1(byte *hash20, const String& s)
{
	return SHA1(hash20, s, s.GetLength());
}

String SHA1String(const void *data, dword size)
{
	Sha1Stream sha1;
	sha1.Put(data, size);
	return sha1.FinishString();
}

String SHA1String(const String& data)
{
	return SHA1String(~data, data.GetLength());
}

String  SHA1StringS(const void *data, dword size)
{
	Sha1Stream sha1;
	sha1.Put(data, size);
	return sha1.FinishStringS();
}

String  SHA1StringS(const String& data)
{
	return SHA1StringS(~data, data.GetLength());
}

END_UPP_NAMESPACE
