#ifndef _svo_memcpy_svo_memcpy_h_
#define _svo_memcpy_svo_memcpy_h_

#include <Core/Core.h>

using namespace Upp;

force_inline void svo_memcpy(void *dst, const void *src, int n)
{
	if(n < 15) {
		byte *t = (byte *)dst;
		byte *s = (byte *)src;
		if(n & 8) {
			*(uint64 *)t = *(const uint64 *)s;
			t += 8;
			s += 8;
		}
		if(n & 4) {
			*(uint32 *)t = *(const uint32 *)s;
			t += 4;
			s += 4;
		}
		if(n & 2) {
			*(uint16 *)t = *(const uint16 *)s;
			t += 2;
			s += 2;
		}
		if(n & 1)
			*t = *s;
	}
	else
		memcpy(dst, src, n);
}

/*
force_inline void svo_memcpy2(void *tgt, const void *src, int len)
{
	const char *s = (const char *)(src);
	char *t = (char *)(tgt);
	switch(len) {
	case 11: *(uint64 *)t = *(const uint64 *)s; *(uint16 *)(t + 8) = *(const uint16 *)(s + 8); t[10] = s[10]; break;
	case 10: *(uint64 *)t = *(const uint64 *)s; *(uint16 *)(t + 8) = *(const uint16 *)(s + 8); break;
	case  9: *(uint64 *)t = *(const uint64 *)s; t[9] = s[9]; break;
	case  8: *(uint64 *)t = *(const uint64 *)s; break;
	case  7: *(uint32 *)t = *(const uint32 *)s; *(uint16 *)(t + 4) = *(const uint16 *)(s + 4); t[6] = s[6]; break;
	case  6: *(uint32 *)t = *(const uint32 *)s; *(uint16 *)(t + 4) = *(const uint16 *)(s + 4); break;
	case  5: *(uint32 *)t = *(const uint32 *)s; t[4] = s[4]; break;
	case  4: *(uint32 *)t = *(const uint32 *)s; break;
	case  3: *(uint16 *)t = *(const uint16 *)s; t[2] = s[2]; break;
	case  2: *(uint16 *)t = *(const uint16 *)s; break;
	case  1: *t = *s; break;
	case  0: break;
	default:
		memcpy(tgt, src, len);
	}
}
*/
force_inline void svo_memcpy2(void *tgt, const void *src, int len)
{
	const char *s = (const char *)(src);
	char *t = (char *)(tgt);
	switch(len) {
	case 11: t[10] = s[10];
	case 10: *(uint64 *)t = *(const uint64 *)s; *(uint16 *)(t + 8) = *(const uint16 *)(s + 8); break;
	case  9: t[9] = s[9];
	case  8: *(uint64 *)t = *(const uint64 *)s; break;
	case  7: *(uint16 *)(t + 4) = *(const uint16 *)(s + 4); t[6] = s[6];
	case  6: *(uint32 *)t = *(const uint32 *)s; *(uint16 *)(t + 4) = *(const uint16 *)(s + 4); break;
	case  5: t[4] = s[4]; break;
	case  4: *(uint32 *)t = *(const uint32 *)s; break;
	case  3: t[2] = s[2]; break;
	case  2: *(uint16 *)t = *(const uint16 *)s; break;
	case  1: *t = *s; break;
	case  0: break;
	default:
		memcpy(tgt, src, len);
	}
}

void Test1(void *t, void *s, int n);
void Test2(void *t, void *s, int n);
void Test3(void *t, void *s, int n);
void Test4(void *t, void *s, int n);

#endif
