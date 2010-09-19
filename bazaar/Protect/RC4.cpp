#include "RC4.h"

RC4::RC4()
{
	memset(sbox, 0, 256);
	si = sj = 0;
}

RC4::RC4(String const &key)
{
	SetKey(key);
}

void RC4::SetKey(String const &key)
{
	byte const *b = (byte const *)~key;
	int keyLen = key.GetCount();

	int i, j;
	unsigned char keyarr[256], swap;
   
	si = sj = 0;
	for (i = j = 0;  i < 256;  i++, j = (j + 1) % keyLen)
	{
		sbox[i] = i;
		keyarr[i] = b[j];
	}
	for (i = j = 0;  i < 256;  i++)
	{
		j += sbox[i] + keyarr[i];
		j %= 256;
		swap = sbox[i];
		sbox[i] = sbox[j];
		sbox[j] = swap;
	}
}

void RC4::Crypt(byte *src, byte *dst, size_t len)
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
