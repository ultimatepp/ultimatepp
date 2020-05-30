#ifdef CPU_X86

inline
bool memeq128(const void *p, const void *q)
{
	qword *s = (qword *)p;
	qword *t = (qword *)q;
	return s[0] == t[0] && s[1] == t[1];
//	return _mm_movemask_epi8(_mm_cmpeq_epi32(_mm_loadu_si128((__m128i *)p), _mm_loadu_si128((__m128i *)q))) == 0xffff;
}

void memset8__(void *t, __m128i data, size_t len);

inline
void memset8(void *p, byte data, size_t len)
{
	byte *t = (byte *)p;
	if(len < 2) {
		if(len)
			t[0] = data;
		return;
	}
	dword val4 = 0x1010101 * data;
	if(len <= 4) {
		*(word *)t = *(word *)(t + len - 2) = (word)val4;
		return;
	}
	if(len > 16) {
		memset8__(t, _mm_set1_epi32(val4), len);
		return;
	}
	*(dword *)t = *(dword *)(t + len - 4) = val4;
	if(len > 8)
		*(dword *)(t + 4) = *(dword *)(t + len - 8) = val4;
}

inline
void memset16(void *p, word data, size_t len)
{
	word *t = (word *)p;
	if(len < 2) {
		if(len)
			t[0] = data;
		return;
	}
	dword val4 = 0x10001 * data;
	if(len >= 16) {
		memset8__(t, _mm_set1_epi32(val4), 2 * len);
		return;
	}
	*(dword *)(t + len - 2) = val4;
	if(len & 8) {
		_mm_storeu_si128((__m128i *)t, _mm_set1_epi32(val4));
		t += 8;
	}
	if(len & 4) {
		*(dword *)t = val4;
		*(dword *)(t + 2) = val4;
		t += 4;
	}
	if(len & 2)
		*(dword *)t = val4;
}

inline
void memset32(void *p, dword data, size_t len)
{
	dword *t = (dword *)p;
	if(len < 4) {
		if(len & 2) {
			t[0] = t[1] = t[len - 1] = data;
			return;
		}
		if(len & 1)
			t[0] = data;
		return;
	}
	__m128i val4 = _mm_set1_epi32(data);
	if(len >= 16) {
		memset8__(t, val4, 4 * len);
		return;
	}
	auto Set4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), val4); };
	Set4(len - 4); // fill tail
	if(len & 8) {
		Set4(0); Set4(4);
		t += 8;
	}
	if(len & 4)
		Set4(0);
}

inline
void memset64(void *p, qword data, size_t len)
{
	qword *t = (qword *)p;
	if(len < 2) {
		if(len)
			t[0] = data;
		return;
	}
	__m128i val2 = _mm_set1_epi64x(data);
	if(len >= 8) {
		memset8__(t, val2, 8 * len);
		return;
	}
	auto Set128 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), val2); };
	Set128(len - 2); // fill tail
	if(len & 4) {
		Set128(0); Set128(2);
		t += 4;
	}
	if(len & 2)
		Set128(0);
}

void memcpy8__(void *p, const void *q, size_t len);

inline
void memcpy8(void *p, const void *q, size_t len)
{
	byte *t = (byte *)p;
	byte *s = (byte *)q;
	if(len <= 4) {
		if(len < 2) {
			if(len)
				t[0] = s[0];
			return;
		}
		*(word *)t = *(word *)s;
		*(word *)(t + len - 2) = *(word *)(s + len - 2);
		return;
	}
	if(len <= 16) {
		if(len <= 8) {
			*(dword *)(t) = *(dword *)(s);
			*(dword *)(t + len - 4) = *(dword *)(s + len - 4);
			return;
		}
		*(uint64 *)t = *(uint64 *)s;
		*(uint64 *)(t + len - 8) = *(uint64 *)(s + len - 8);
		return;
	}
	memcpy8__(t, s, len);
}

inline
void memcpy16(void *p, const void *q, size_t len)
{
	word *t = (word *)p;
	word *s = (word *)q;
	if(len <= 4) {
		if(len < 2) {
			if(len)
				t[0] = s[0];
			return;
		}
		*(dword *)t = *(dword *)s;
		*(dword *)(t + len - 2) = *(dword *)(s + len - 2);
		return;
	}
	if(len <= 16) {
		if(len <= 8) {
			*(uint64 *)(t) = *(uint64 *)(s);
			*(uint64 *)(t + len - 4) = *(uint64 *)(s + len - 4);
			return;
		}
		auto Copy128 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };
		Copy128(0);
		Copy128(len - 8);
		return;
	}
	memcpy8__(t, s, 2 * len);
}

inline
void memcpy32(void *p, const void *q, size_t len)
{
	dword *t = (dword *)p;
	dword *s = (dword *)q;

#ifdef CPU_64
	if(len <= 4) {
		if(len) {
			if(len > 1) {
				*(int64 *)t = *(int64 *)s;
				*(int64 *)(t + len - 2) = *(int64 *)(s + len - 2);
				return;
			}
			*t = *s;
		}
		return;
	}
#else
	if(len < 4) {
		if(len) {
			if(len > 1) {
				t[0] = s[0];
				t[1] = s[1];
				t[len - 1] = s[len - 1];
				return;
			}
			*t = *s;
		}
		return;
	}
#endif

	auto Copy128 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };

	if(len >= 16) {
		memcpy8__(t, s, 4 * len);
		return;
	}
	Copy128(len - 4); // copy tail
	if(len & 8) {
		Copy128(0); Copy128(4);
		t += 8;
		s += 8;
	}
	if(len & 4)
		Copy128(0);
}

inline
void memcpy64(void *p, const void *q, size_t len)
{
	qword *t = (qword *)p;
	qword *s = (qword *)q;

	if(len <= 2) {
		if(len) {
			if(len > 1) {
				*(int64 *)t = *(int64 *)s;
				*(int64 *)(t + len - 1) = *(int64 *)(s + len - 1);
				return;
			}
			*t = *s;
		}
		return;
	}

	auto Copy4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };

	Copy4(len - 2); // copy tail
	if(len >= 8) {
		memcpy8__(t, s, 8 * len);
		return;
	}
	if(len & 4) {
		Copy4(0); Copy4(2);
		t += 4;
		s += 4;
	}
	if(len & 2)
		Copy4(0);
}

inline
void memcpy128(void *p, const void *q, size_t len)
{
	struct dqword { qword x[2]; };

	static_assert(sizeof(dqword) == 16, "dqword sizeof");

	dqword *t = (dqword *)p;
	dqword *s = (dqword *)q;

	auto Copy4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };

	if(len >= 8) {
		memcpy8__(t, s, 16 * len);
		return;
	}
	if(len & 4) {
		Copy4(0); Copy4(1); Copy4(2); Copy4(3);
		t += 4;
		s += 4;
	}
	if(len & 2) {
		Copy4(0); Copy4(1);
		t += 2;
		s += 2;
	}
	if(len & 1)
		Copy4(0);
}

template <class T>
void memcpy_t(void *t, const T *s, size_t count)
{
#ifdef CPU_X86
	if((sizeof(T) & 15) == 0)
		memcpy128(t, s, count * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		memcpy64(t, s, count * (sizeof(T) >> 3));
	else
#endif
	if((sizeof(T) & 3) == 0)
		memcpy32(t, s, count * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		memcpy16(t, s, count * (sizeof(T) >> 1));
	else
		memcpy8(t, s, count * sizeof(T));
}

force_inline
bool memeq8__(const void *p, const void *q, size_t len)
{
	ASSERT(len >= 16);
	const byte *t = (byte *)p;
	const byte *s = (byte *)q;
	
	auto Cmp128 = [&](int at) { return _mm_cmpeq_epi32(_mm_loadu_si128((__m128i *)(s + at)), _mm_loadu_si128((__m128i *)(t + at))); };
	auto Neq = [](__m128i v) { return _mm_movemask_epi8(v) != 0xffff; };
	auto And = [](__m128i a, __m128i b) { return _mm_and_si128(a, b); };
	
	if(Neq(And(Cmp128(len - 16), Cmp128(0)))) // tail & alignment, also <= 32
		return false;
	
	if(len <= 32)
		return true;

	const byte *e = t + len; // align up

	byte *t1 = (byte *)(((uintptr_t)t | 15) + 1);
	s += t1 - t;
	t = t1;
	len = e - t;
	e -= 32;
	while(t <= e) {
		if(Neq(And(Cmp128(0), Cmp128(1*16))))
			return false;
		s += 32;
		t += 32;
	}
	if(len & 16)
		if(Neq(Cmp128(0)))
			return false;
	return true;
}

force_inline
bool inline_memeq8_aligned(const void *p, const void *q, size_t len)
{
	const byte *t = (const byte *)p;
	const byte *s = (const byte *)q;
	if(len >= 16) { // 15..31 is the most important range for String, make it fastest
		auto Cmp128 = [&](int at) { return _mm_cmpeq_epi32(_mm_loadu_si128((__m128i *)(s + at)), _mm_loadu_si128((__m128i *)(t + at))); };
		return memeq8__(t, s, len);
	}
	if(len > 4) {
		if(len <= 8) // test tail first in case we are searching in sorted list
			return Peek32(s + len - 4) == Peek32(t + len - 4) && Peek32(s) == Peek32(t);
		return Peek64(s + len - 8) == Peek64(t + len - 8) && Peek64(s) == Peek64(t);
	}
	if(len < 2)
		return len ? t[0] == s[0] : true;
	if(Peek16(s + len - 2) != Peek16(t + len - 2))
		return false;
	return len > 2 ? Peek16(s) == Peek16(t) : true;
}

force_inline
bool inline_memeq16_aligned(const void *p, const void *q, size_t len)
{
	const word *t = (const word *)p;
	const word *s = (const word *)q;
	if(len <= 2)
		return len ? Peek16(s + len - 1) == Peek16(t + len - 1) && Peek16(s) == Peek16(t) : true;
	if(len <= 8) {
		if(len <= 4)
			return Peek32(s + len - 2) == Peek32(t + len - 2) && Peek32(s) == Peek32(t);
		return Peek64(s + len - 4) == Peek64(t + len - 4) && Peek64(s) == Peek64(t);
	}
	return memeq8__(t, s, 2 * len);
}

force_inline
bool inline_memeq32_aligned(const void *p, const void *q, size_t len)
{
	const dword *t = (const dword *)p;
	const dword *s = (const dword *)q;
	if(len <= 4) {
		if(len <= 2)
			return len ? Peek32(s + len - 1) == Peek32(t + len - 1) && Peek32(s) == Peek32(t) : true;
		return Peek64(s + len - 2) == Peek64(t + len - 2) && Peek64(s) == Peek64(t);
	}
	return memeq8__(t, s, 4 * len);
}

force_inline
bool inline_memeq64_aligned(const void *p, const void *q, size_t len)
{
	const qword *t = (const qword *)p;
	const qword *s = (const qword *)q;
	if(len <= 2)
		return len ? Peek64(s + len - 1) == Peek64(t + len - 1) && Peek64(s) == Peek64(t) : true;
	return memeq8__(t, s, 8 * len);
}

bool memeq8(const void *p, const void *q, size_t len);
bool memeq16(const void *p, const void *q, size_t len);
bool memeq32(const void *p, const void *q, size_t len);
bool memeq64(const void *p, const void *q, size_t len);

#else

template <class T>
void memset__(void *p, T data, size_t len)
{
	T *t = (T *)p;
	while(len >= 8) {
		t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = data;
		t += 8;
		len -= 8;
	}
	if(len & 4) {
		t[0] = t[1] = t[2] = t[3] = data;
		t += 4;
	}
	if(len & 2) {
		t[0] = t[1] = data;
		t += 2;
	}
	if(len & 1)
		t[0] = data;
}

inline
void memset8(void *p, byte data, size_t len)
{
	if(len >= 64) {
		memset(p, data, len);
		return;
	}
	memset__<byte>(p, data, len);
}

inline
void memset16(void *p, word data, size_t len)
{
	memset__<word>(p, data, len);
}

inline
void memset32(void *p, dword data, size_t len)
{
	memset__<dword>(p, data, len);
}

inline
void memcpy8(void *p, const void *q, size_t len)
{
	memcpy(p, q, len);
}

inline
void memcpy16(void *p, const void *q, size_t len)
{
	memcpy(p, q, 2 * len);
}

inline
void memcpy32(void *p, const void *q, size_t len)
{
	memcpy(p, q, 4 * len);
}

inline
void memcpy64(void *p, const void *q, size_t len)
{
	memcpy(p, q, 8 * len);
}

inline
void memcpy128(void *p, const void *q, size_t len)
{
	memcpy(p, q, 16 * len);
}

template <class T>
void memcpy_t(void *t, const T *s, size_t count)
{
	memcpy8(t, s, count * sizeof(T));
}

inline
bool inline_memeq8_aligned(const void *p, const void *q, size_t len)
{
	const byte *t = (const byte *)p;
	const byte *s = (const byte *)q;
	while(len >= 8) {
		if(*(qword *)t != *(qword *)s)
			return false;
		s += 8;
		t += 8;
		len -= 8;
	}
	if(len & 4) {
		if(*(dword *)t != *(dword *)s)
			return false;
		s += 4;
		t += 4;
	}
	if(len & 2) {
		if(*(word *)t != *(word *)s)
			return false;
		s += 2;
		t += 2;
	}
	if(len & 2) {
		if(*t != *s)
			return false;
	}
	return true;
}

force_inline
bool inline_memeq16_aligned(const void *p, const void *q, size_t len)
{
	return memcmp(p, q, 2 * len) == 0;
}

force_inline
bool inline_memeq32_aligned(const void *p, const void *q, size_t len)
{
	return memcmp(p, q, 4 * len) == 0;
}

force_inline
bool inline_memeq64_aligned(const void *p, const void *q, size_t len)
{
	return memcmp(p, q, 8 * len) == 0;
}

inline
bool memeq8(const void *p, const void *q, size_t len)
{
	return memcmp(p, q, len) == 0;
}

inline
bool memeq16(const void *p, const void *q, size_t len)
{
	memcmp(p, q, 2 * len) == 0;
}

inline
bool memeq32(const void *p, const void *q, size_t len)
{
	memcmp(p, q, 4 * len) == 0;
}

inline
bool memeq64(const void *p, const void *q, size_t len);
{
	memcmp(p, q, 4 * len) == 0;
}

#endif
