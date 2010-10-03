#include "RC4.h"

NAMESPACE_UPP

RC4::RC4() : CypherBase()
{
	memset(sbox, 0, 256);
	si = sj = 0;
}

RC4::RC4(String const &key, String const &nonce) : CypherBase()
{
	SetKey(key, nonce);
}

RC4::RC4(byte const *keyBuf, size_t keyLen, byte const *nonce, size_t nonceLen)
{
	SetKey(keyBuf, keyLen, nonce, nonceLen);
}

bool RC4::SetKey(String const &key, String const &nonce)
{
	byte const *b = (byte const *)~key;
	int keyLen = key.GetCount();

	return SetKey(b, keyLen, (byte const *)~nonce, nonce.GetCount());
}

bool RC4::SetKey(byte const *kBuf, size_t keyLen, byte const *nonce, size_t nonceLen)
{
	int i, j;
	unsigned char keyarr[256], swap;

	// appends NONCE at key -- avoids repeated encoding of same soruce stream
	Buffer<byte>keyBuf(keyLen + nonceLen);
	memcpy(keyBuf, kBuf, keyLen);
	memcpy(keyBuf + keyLen, nonce, nonceLen);
	keyLen += nonceLen;
   
	si = sj = 0;
	for (i = j = 0;  i < 256;  i++, j = (j + 1) % keyLen)
	{
		sbox[i] = i;
		keyarr[i] = keyBuf[j];
	}
	for (i = j = 0;  i < 256;  i++)
	{
		j += sbox[i] + keyarr[i];
		j %= 256;
		swap = sbox[i];
		sbox[i] = sbox[j];
		sbox[j] = swap;
	}
	
	// discards first 256 stream values -- see wikipedia for details
	for(i = 0; i < 256; i++)
	{
		sj += sbox[++si];
		swap = sbox[si];
		sbox[si] = sbox[sj];
		sbox[sj] = swap;
		swap = sbox[si] + sbox[sj];
	}
	
	return true;
}

// encode/decode string
String RC4::operator()(String const &s)
{
	unsigned char swap;
	int i = 0;
	int len = s.GetCount();
	String res;
	
	while (len--)
	{
		sj += sbox[++si];
		swap = sbox[si];
		sbox[si] = sbox[sj];
		sbox[sj] = swap;
		swap = sbox[si] + sbox[sj];
		res += s[i++] ^ sbox[swap];
	}
	return res;
}

// encode/decode buffer, dest on different buffer
void RC4::operator()(const byte *src, byte *dst, size_t len)
{
	unsigned char swap;
	
	while (len--)
	{
		sj += sbox[++si];
		swap = sbox[si];
		sbox[si] = sbox[sj];
		sbox[sj] = swap;
		swap = sbox[si] + sbox[sj];
		*dst++ = *src++ ^ sbox[swap];
	}
}

// encode/decode buffer in place
void RC4::operator()(byte *buf, size_t len)
{
	operator()(buf, buf, len);
}

// stream support
String RC4::StreamOut(void)
{
	// returns encoded buffer from FIFO
	String res;
	FIFO.Get(res);
	return res;
}

void RC4::StreamIn(String const &s)
{
	// en/decode string and put into FIFO
	FIFO.Put(operator()(s));
}

void RC4::Flush(void)
{
	// Snow2 is already a stream Cypher,
	// no need to pad last block
};


END_UPP_NAMESPACE;
