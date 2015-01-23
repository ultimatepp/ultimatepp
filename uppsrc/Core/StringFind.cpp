#include "Core.h"

NAMESPACE_UPP

#ifdef CPU_X86 // Use unaligned access

#ifdef CPU_64
inline bool equal_back_8(const char *a, const char *b, int len)
{
	while(len > 8) {
		len -= 8;
		if(Peek64le(a + len) != Peek64le(b + len))
			return false;
	}
	return true;
}
#else
force_inline bool equal_back_4(const char *a, const char *b, int len)
{
	while(len > 4) {
		len -= 4;
		if(Peek32le(a + len) != Peek32le(b + len))
			return false;
	}
	return true;
}
#endif

template <int step> // Template parameter to be a constant
int t_find(const char *ptr, int slen, const char *p, int len, int from)
{
	ASSERT(from >= 0 && from <= slen);
	int l = slen - len - from;
	if(l < 0)
		return -1;
	const char *s = ptr + from;
	const char *e = s + l;
	if(len <= 8) {
		if(len > 4) {
			len -= 4; 
			int32 p0 = Peek32le(p);
			int32 p1 = Peek32le(p + len);
			while(s <= e) {
				if(Peek32le(s) == p0 && Peek32le(s + len) == p1)
					return (int)(s - ptr);
				s += step;
			}
		}
		else
		if(len == 4) {
			int32 p0 = Peek32le(p);
			while(s <= e) {
				if(Peek32le(s) == p0)
					return (int)(s - ptr);
				s += step;
			}
		}
		else
		if(len == 3) {
			int16 p0 = Peek16le(p);
			char p1 = p[2];
			while(s <= e) {
				if(Peek16le(s) == p0 && s[2] == p1)
					return (int)(s - ptr);
				s += step;
			}
		}
		else
		if(len == 2) {
			int16 p0 = Peek16le(p);
			while(s <= e) {
				if(Peek16le(s) == p0)
					return (int)(s - ptr);
				s += step;
			}
		}
		else
		if(len == 1) {
			char p0 = p[0];
			while(s <= e) {
				if(*s == p0)
					return (int)(s - ptr);
				s += step;
			}
		}
		else
			return from;
	}
	else {
#ifdef CPU_64	
		int64 p0 = Peek64le(p);
		if(len <= 16) {
			len -= 8;
			int64 p1 = Peek64le(p + len);
			while(s <= e) {
				if(Peek64le(s) == p0 && Peek64le(s + len) == p1)
					return (int)(s - ptr);
				s += step;
			}
		}
		else
			while(s <= e) {
				if(Peek64le(s) == p0 && equal_back_8(s, p, len))
					return (int)(s - ptr);
				s += step;
			}
#else
		int32 p0 = Peek32le(p);
		while(s <= e) {
			if(Peek32le(s) == p0 && equal_back_4(s, p, len))
				return (int)(s - ptr);
			s += step;
		}
#endif
	}
	return -1;
}

int find(const char *text, int len, const char *needle, int nlen, int from)
{
	return t_find<1>(text, len, needle, nlen, from);
}

int find(const wchar *text, int len, const wchar *needle, int nlen, int from)
{
	int q = t_find<2>((const char *)text, 2 * len, (const char *)needle, 2 * nlen, 2 * from);
	return q < 0 ? q : q / 2;
}

#else

template <class tchar>
int t_find(const tchar *ptr, int plen, const tchar *s, int len, int from)
{
	ASSERT(from >= 0 && from <= plen);
	int l = plen - len - from;
	if(l < 0)
		return -1;
	if(len == 0)
		return from;
	const tchar *p = ptr + from;
	const tchar *e = p + l;
	if(len > 4) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		tchar s2 = s[2];
		tchar sl = s[len - 1];
		int l_1 = len - 1;
		while(p <= e) {
			if(s0 == p[0] && sl == p[l_1] && s1 == p[1] && s2 == p[2] &&
			   svo_memeq(s + 3, p + 3, len - 4))
				return (int)(p - ptr);
			p++;
		}
	}
	else
	if(len == 4) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		tchar s2 = s[2];
		tchar s3 = s[3];
		while(p <= e) {
			if(s0 == p[0] && s3 == p[3] && s1 == p[1] && s2 == p[2])
				return (int)(p - ptr);
			p++;
		}
	}
	else
	if(len == 3) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		tchar s2 = s[2];
		while(p <= e) {
			if(s0 == p[0] && s2 == p[2] && s1 == p[1])
				return (int)(p - ptr);
			p++;
		}
	}
	else
	if(len == 2) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		while(p <= e) {
			if(s0 == p[0] && s1 == p[1])
				return (int)(p - ptr);
			p++;
		}
	}
	else {
		tchar s0 = s[0];
		while(p <= e) {
			if(s0 == *p)
				return (int)(p - ptr);
			p++;
		}
	}
	return -1;
}

int find(const char *text, int len, const char *needle, int nlen, int from)
{
	return t_find(text, len, needle, nlen, from);
}

int find(const wchar *text, int len, const wchar *needle, int nlen, int from)
{
	return t_find(text, len, needle, nlen, from);
}
#endif

END_UPP_NAMESPACE
