#include "RC4.h"

NAMESPACE_UPP

RC4::RC4()
{
	memset(sbox, 0, 256);
	si = sj = 0;
}

RC4::RC4(String const &key)
{
	SetKey(key);
}

RC4::RC4(byte const *keyBuf, int keyLen)
{
	SetKey(keyBuf, keyLen);
}

bool RC4::SetKey(String const &key)
{
	byte const *b = (byte const *)~key;
	int keyLen = key.GetCount();

	return SetKey(b, keyLen);
}

bool RC4::SetKey(byte const *keyBuf, int keyLen)
{
	int i, j;
	unsigned char keyarr[256], swap;
   
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
	return true;
}

void RC4::Encode(const byte *src, byte *dst, size_t len)
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

void RC4::Encode(byte *buf, size_t len)
{
	Encode(buf, buf, len);
}

String RC4::Encode(String const &s)
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

END_UPP_NAMESPACE;
