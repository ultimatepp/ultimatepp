#include "RC4.h"

NAMESPACE_UPP

RC4::RC4() : Cypher(1 /* stream cypher */)
{
	memset(sbox, 0, 256);
	si = sj = 0;
}

RC4::RC4(String const &key, String const &nonce) : Cypher(1 /* stream cypher */)
{
	SetKey(key, nonce);
}

RC4::RC4(byte const *keyBuf, size_t keyLen, byte const *nonce, size_t nonceLen) : Cypher(1 /* stream cypher */)
{
	SetKey(keyBuf, keyLen, nonce, nonceLen);
}

bool RC4::CypherKey(byte const *kBuf, size_t keyLen, byte const *nonce, size_t nonceLen)
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

// encode/decode buffer, dest on different buffer
void RC4::CypherCypher(const byte *src, byte *dst, size_t len)
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

END_UPP_NAMESPACE;
