#ifdef CPU_SIMD

void memset8__(void *t, i16x8 data, size_t len);

inline void Set2__(void *t, word val)
{
	memcpy(t, &val, 2);
}

inline void Set4__(void *t, dword val)
{
	memcpy(t, &val, 4);
}

inline void Set8__(void *t, uint64 val)
{
	memcpy(t, &val, 8);
}

inline void Cpy2__(void *t, const void *s)
{
	memcpy(t, s, 2);
}

inline void Cpy4__(void *t, const void *s)
{
	memcpy(t, s, 4);
}

inline void Cpy8__(void *t, const void *s)
{
	memcpy(t, s, 8);
}

inline
void memset8(void *p, byte data, size_t count)
{
	byte *t = (byte *)p;
	if(count < 2) {
		if(count)
			t[0] = data;
		return;
	}
	dword val4 = 0x1010101 * data;
	if(count <= 4) {
		Set2__(t, (word)val4);
		Set2__(t + count - 2, (word)val4);
		return;
	}
	if(count > 16) {
		memset8__(t, i32all(val4), count);
		return;
	}
	Set4__(t, val4);
	Set4__(t + count - 4, val4);
	if(count > 8) {
		Set4__(t + 4, val4);
		Set4__(t + count - 8, val4);
	}
}

inline
void memset16(void *p, word data, size_t count)
{
	word *t = (word *)p;
	if(count < 2) {
		if(count)
			Set2__(t, data);
		return;
	}
	dword val4 = 0x10001 * data;
	if(count >= 16) {
		memset8__(t, i32all(val4), 2 * count);
		return;
	}
	Set4__(t + count - 2, val4);
	if(count & 8) {
		i32all(val4).Store(t);
		t += 8;
	}
	if(count & 4) {
		Set4__(t, val4);
		Set4__(t + 2, val4);
		t += 4;
	}
	if(count & 2)
		Set4__(t, val4);
}

inline
void memset32(void *p, dword data, size_t count)
{
	dword *t = (dword *)p;
	if(count < 4) {
		if(count & 2) {
			Set4__(t, data);
			Set4__(t + 1, data);
			Set4__(t + count - 1, data);
			return;
		}
		if(count & 1)
			Set4__(t, data);
		return;
	}
	i32x4 val4 = i32all(data);
	if(count >= 16) {
		memset8__(t, val4, 4 * count);
		return;
	}
	auto Set128 = [&](size_t at) { val4.Store(t + at); };
	Set128(count - 4); // fill tail
	if(count & 8) {
		Set128(0); Set128(4);
		t += 8;
	}
	if(count & 4)
		Set128(0);
}

inline
void memset64(void *p, qword data, size_t count)
{
	qword *t = (qword *)p;
	if(count < 2) {
		if(count)
			Set8__(t, data);
		return;
	}
	i16x8 val2 = i64all(data);
	if(count >= 8) {
		memset8__(t, val2, 8 * count);
		return;
	}
	auto Set128 = [&](size_t at) { val2.Store(t + at); };
	Set128(count - 2); // fill tail
	if(count & 4) {
		Set128(0); Set128(2);
		t += 4;
	}
	if(count & 2)
		Set128(0);
}

inline
void memset128(void *t, m128 data, size_t count)
{
	if(count)
		memset8__(t, i16x8().Load(&data), 16 * count);
}

void memcpy8__(void *p, const void *q, size_t count);

inline
void memcpy8(void *p, const void *q, size_t count)
{
	byte *t = (byte *)p;
	byte *s = (byte *)q;
	if(count <= 4) {
		if(count < 2) {
			if(count)
				t[0] = s[0];
			return;
		}
		Cpy2__(t, s);
		Cpy2__(t + count - 2, s + count - 2);
		return;
	}
	if(count <= 16) {
		if(count <= 8) {
			Cpy4__(t, s);
			Cpy4__(t + count - 4, s + count - 4);
			return;
		}
		Cpy8__(t, s);
		Cpy8__(t + count - 8, s + count - 8);
		return;
	}
	if(count <= 32) { // improves String::LSet
		auto Copy128 = [&](size_t at) { i16x8(s + at).Store(t + at); };
		Copy128(count - 16);
		Copy128(0);
		return;
	}

	memcpy8__(t, s, count);
}

inline
void memcpy16(void *p, const void *q, size_t count)
{
	word *t = (word *)p;
	word *s = (word *)q;
	if(count <= 4) {
		if(count < 2) {
			if(count)
				Cpy2__(t, s);
			return;
		}
		Cpy4__(t, s);
		Cpy4__(t + count - 2, s + count - 2);
		return;
	}
	if(count <= 16) {
		if(count <= 8) {
			Cpy8__(t, s);
			Cpy8__(t + count - 4, s + count - 4);
			return;
		}
		auto Copy128 = [&](size_t at) { i16x8(s + at).Store(t + at); };
		Copy128(0);
		Copy128(count - 8);
		return;
	}
	memcpy8__(t, s, 2 * count);
}

inline
void memcpy32(void *p, const void *q, size_t count)
{
	dword *t = (dword *)p;
	dword *s = (dword *)q;

#ifdef CPU_64
	if(count <= 4) {
		if(count) {
			if(count > 1) {
				Cpy8__(t, s);
				Cpy8__(t + count - 2, s + count - 2);
				return;
			}
			*t = *s;
		}
		return;
	}
#else
	if(count < 4) {
		if(count) {
			if(count > 1) {
				Cpy4__(t, s);
				Cpy4__(t + 1, s + 1);
				Cpy4__(t + count - 1, s + count - 1);
				return;
			}
			*t = *s;
		}
		return;
	}
#endif

	auto Copy128 = [&](size_t at) { i16x8(s + at).Store(t + at); };

	if(count >= 16) {
		memcpy8__(t, s, 4 * count);
		return;
	}
	Copy128(count - 4); // copy tail
	if(count & 8) {
		Copy128(0); Copy128(4);
		t += 8;
		s += 8;
	}
	if(count & 4)
		Copy128(0);
}

inline
void memcpy64(void *p, const void *q, size_t count)
{
	qword *t = (qword *)p;
	qword *s = (qword *)q;

	if(count <= 2) {
		if(count) {
			if(count > 1) {
				Cpy8__(t, s);
				Cpy8__(t + count - 1, s + count - 1);
				return;
			}
			*t = *s;
		}
		return;
	}

	auto Copy128 = [&](size_t at) { i16x8(s + at).Store(t + at); };

	Copy128(count - 2); // copy tail
	if(count >= 8) {
		memcpy8__(t, s, 8 * count);
		return;
	}
	if(count & 4) {
		Copy128(0); Copy128(2);
		t += 4;
		s += 4;
	}
	if(count & 2)
		Copy128(0);
}

inline
void memcpy128(void *p, const void *q, size_t count)
{
	struct dqword { qword x[2]; };

	static_assert(sizeof(dqword) == 16, "dqword sizeof");

	dqword *t = (dqword *)p;
	dqword *s = (dqword *)q;

	auto Copy128 = [&](size_t at) { i16x8(s + at).Store(t + at); };

	if(count >= 8) {
		memcpy8__(t, s, 16 * count);
		return;
	}
	if(count & 4) {
		Copy128(0); Copy128(1); Copy128(2); Copy128(3);
		t += 4;
		s += 4;
	}
	if(count & 2) {
		Copy128(0); Copy128(1);
		t += 2;
		s += 2;
	}
	if(count & 1)
		Copy128(0);
}

template <class T>
void memcpy_t(void *t, const T *s, size_t count)
{
	if((sizeof(T) & 15) == 0)
		memcpy128(t, s, count * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		memcpy64(t, s, count * (sizeof(T) >> 3));
	else
	if((sizeof(T) & 3) == 0)
		memcpy32(t, s, count * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		memcpy16(t, s, count * (sizeof(T) >> 1));
	else
		memcpy8(t, s, count * sizeof(T));
}

force_inline
bool memeq8__(const void *p, const void *q, size_t count)
{
	ASSERT(count >= 16);
	const byte *t = (byte *)p;
	const byte *s = (byte *)q;
	
	auto Cmp128 = [&](size_t at)        { return i16x8(s + at) == i16x8(t + at); };
	
	if(!AllTrue(Cmp128(count - 16) & Cmp128(0))) // tail & alignment, also <= 32
		return false;
	
	if(count <= 32)
		return true;

	const byte *e = t + count; // align up

	byte *t1 = (byte *)(((uintptr_t)t | 15) + 1);
	s += t1 - t;
	t = t1;
	count = e - t;
	e -= 32;
	while(t <= e) {
		if(!AllTrue(Cmp128(0) & Cmp128(1*16)))
			return false;
		s += 32;
		t += 32;
	}
	if(count & 16)
		if(!AllTrue(Cmp128(0)))
			return false;
	return true;
}

force_inline
bool inline_memeq8_aligned(const void *p, const void *q, size_t count)
{
	const byte *t = (const byte *)p;
	const byte *s = (const byte *)q;
	if(count >= 16) // 15..31 is the most important range for String, make it fastest
		return memeq8__(t, s, count);
	if(count > 4) {
		if(count <= 8) // test tail first in case we are searching in sorted list
			return Peek32(s + count - 4) == Peek32(t + count - 4) && Peek32(s) == Peek32(t);
		return Peek64(s + count - 8) == Peek64(t + count - 8) && Peek64(s) == Peek64(t);
	}
	if(count < 2)
		return count ? t[0] == s[0] : true;
	if(Peek16(s + count - 2) != Peek16(t + count - 2))
		return false;
	return count > 2 ? Peek16(s) == Peek16(t) : true;
}

force_inline
bool inline_memeq16_aligned(const void *p, const void *q, size_t count)
{
	const word *t = (const word *)p;
	const word *s = (const word *)q;
	if(count <= 2)
		return count ? Peek16(s + count - 1) == Peek16(t + count - 1) && Peek16(s) == Peek16(t) : true;
	if(count <= 8) {
		if(count <= 4)
			return Peek32(s + count - 2) == Peek32(t + count - 2) && Peek32(s) == Peek32(t);
		return Peek64(s + count - 4) == Peek64(t + count - 4) && Peek64(s) == Peek64(t);
	}
	return memeq8__(t, s, 2 * count);
}

force_inline
bool inline_memeq32_aligned(const void *p, const void *q, size_t count)
{
	const dword *t = (const dword *)p;
	const dword *s = (const dword *)q;
	if(count <= 4) {
		if(count <= 2)
			return count ? Peek32(s + count - 1) == Peek32(t + count - 1) && Peek32(s) == Peek32(t) : true;
		return Peek64(s + count - 2) == Peek64(t + count - 2) && Peek64(s) == Peek64(t);
	}
	return memeq8__(t, s, 4 * count);
}

force_inline
bool inline_memeq64_aligned(const void *p, const void *q, size_t count)
{
	const qword *t = (const qword *)p;
	const qword *s = (const qword *)q;
	if(count <= 2)
		return count ? Peek64(s + count - 1) == Peek64(t + count - 1) && Peek64(s) == Peek64(t) : true;
	return memeq8__(t, s, 8 * count);
}

force_inline
bool inline_memeq128_aligned(const void *t, const void *s, size_t count)
{
	return count == 0 || memeq8__(t, s, 16 * count);
}

bool memeq8(const void *p, const void *q, size_t count);
bool memeq16(const void *p, const void *q, size_t count);
bool memeq32(const void *p, const void *q, size_t count);
bool memeq64(const void *p, const void *q, size_t count);
bool memeq128(const void *p, const void *q, size_t count);

inline
size_t memeq8len(const void *p, const void *q, size_t count)
{
	const byte *t = (byte *)p;
	const byte *t0 = t;
	const byte *s = (byte *)q;

	const byte *et = t0 + (count & ~15);
	
	while(t < et) {
		i8x16 cmp = i8x16(t) == i8x16(s);
		if(!AllTrue(cmp))
			return t - t0 + FirstFalse(cmp);
		t += 16;
		s += 16;
	}
	
	et = t0 + count;
	while(t < et) {
		if(*s != *t)
			return t - t0;
		s++;
		t++;
	}
	return count;
}

#else

template <class T>
void memset__(void *p, T data, size_t count)
{
	T *t = (T *)p;
	while(count >= 8) {
		t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = data;
		t += 8;
		count -= 8;
	}
	if(count & 4) {
		t[0] = t[1] = t[2] = t[3] = data;
		t += 4;
	}
	if(count & 2) {
		t[0] = t[1] = data;
		t += 2;
	}
	if(count & 1)
		t[0] = data;
}

inline
void memset8(void *p, byte data, size_t count)
{
	if(count >= 64) {
		memset(p, data, count);
		return;
	}
	memset__<byte>(p, data, count);
}

inline
void memset16(void *p, word data, size_t count)
{
	memset__<word>(p, data, count);
}

inline
void memset32(void *p, dword data, size_t count)
{
	memset__<dword>(p, data, count);
}

inline
void memset64(void *p, qword data, size_t count)
{
	memset__<qword>(p, data, count);
}

inline
void memset128(void *p, m128 data, size_t count)
{
	memset__<m128>(p, data, count);
}

inline
void memcpy8(void *p, const void *q, size_t count)
{
	memcpy(p, q, count);
}

inline
void memcpy16(void *p, const void *q, size_t count)
{
	memcpy(p, q, 2 * count);
}

inline
void memcpy32(void *p, const void *q, size_t count)
{
	memcpy(p, q, 4 * count);
}

inline
void memcpy64(void *p, const void *q, size_t count)
{
	memcpy(p, q, 8 * count);
}

inline
void memcpy128(void *p, const void *q, size_t count)
{
	memcpy(p, q, 16 * count);
}

template <class T>
void memcpy_t(void *t, const T *s, size_t count)
{
	memcpy8(t, s, count * sizeof(T));
}

inline
bool inline_memeq8_aligned(const void *p, const void *q, size_t count)
{
	const byte *t = (const byte *)p;
	const byte *s = (const byte *)q;
	while(count >= 8) {
		if(*(qword *)t != *(qword *)s)
			return false;
		s += 8;
		t += 8;
		count -= 8;
	}
	if(count & 4) {
		if(*(dword *)t != *(dword *)s)
			return false;
		s += 4;
		t += 4;
	}
	if(count & 2) {
		if(*(word *)t != *(word *)s)
			return false;
		s += 2;
		t += 2;
	}
	if(count & 1) {
		if(*t != *s)
			return false;
	}
	return true;
}

force_inline
bool inline_memeq16_aligned(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 2 * count) == 0;
}

force_inline
bool inline_memeq32_aligned(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 4 * count) == 0;
}

force_inline
bool inline_memeq64_aligned(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 8 * count) == 0;
}

inline
bool memeq8(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, count) == 0;
}

inline
bool memeq16(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 2 * count) == 0;
}

inline
bool memeq32(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 4 * count) == 0;
}

inline
bool memeq64(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 8 * count) == 0;
}

inline
bool memeq128(const void *p, const void *q, size_t count)
{
	return memcmp(p, q, 16 * count) == 0;
}

inline
size_t memeq8len(const void *p, const void *q, size_t count)
{
	const byte *t = (byte *)p;
	const byte *t0 = t;
	const byte *s = (byte *)q;
	const byte *et = t0 + count;
	
	while(t < et) {
		if(*t != *s)
			return t - t0;
		t++;
		s++;
	}
	
	return count;
}

#endif

#if defined(CPU_LE)
force_inline
int inline_memcmp_aligned(const char *a, const char *b, size_t count)
{
#ifdef CPU_64
	while(count >= 8) {
		uint64 a64 = *(uint64 *)a;
		uint64 b64 = *(uint64 *)b;
		if(a64 != b64)
			return SwapEndian64(a64) < SwapEndian64(b64) ? -1 : 1;
		a += 8;
		b += 8;
		count -= 8;
	}
	if(count & 4) {
		uint32 a32 = *(uint32 *)a;
		uint32 b32 = *(uint32 *)b;
		if(a32 != b32)
			return SwapEndian32(a32) < SwapEndian32(b32) ? -1 : 1;
		a += 4;
		b += 4;
	}
#else
	while(count >= 4) {
		uint32 a32 = *(uint32 *)a;
		uint32 b32 = *(uint32 *)b;
		if(a32 != b32)
			return SwapEndian32(a32) < SwapEndian32(b32) ? -1 : 1;
		a += 4;
		b += 4;
		count -= 4;
	}
#endif
	if(count & 2) {
		uint16 a16 = *(uint16 *)a;
		uint16 b16 = *(uint16 *)b;
		if(a16 != b16)
			return SwapEndian16(a16) < SwapEndian16(b16) ? -1 : 1;
		a += 2;
		b += 2;
	}
	if((count & 1) != 0 && *a != *b)
		return (byte)*a < (byte)*b ? -1 : 1;
	return 0;
}
#else
inline
int inline_memcmp_aligned(const char *a, const char *b, size_t count)
{
	return memcmp(a, b, count);
}
#endif

template <class T>
bool memeq_t(const T *p, const T *q, size_t count)
{
	if((sizeof(T) & 15) == 0)
		return memeq128(p, q, count * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		return memeq64(p, q, count * (sizeof(T) >> 3));
	else
	if((sizeof(T) & 3) == 0)
		return memeq32(p, q, count * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		return memeq16(p, q, count * (sizeof(T) >> 1));
	else
		return memeq8(p, q, count * sizeof(T));
}

hash_t memhash(const void *ptr, size_t count);

inline
size_t memcnt8(const void *s, dword value, size_t sz)
{
	const byte *p = (byte *)s;
	const byte *e = p + sz;
	size_t n = 0;

#ifdef CPU_SIMD
	const byte *e16 = p + (sz & ~15);  // Process in 16-byte chunks
	if(p < e16) {
		i8x16 value16 = i8all(value);
		do {
			n += CountTrue(i8x16(p) == value16);
			p += 16;
		}
		while(p < e16);
	}
#endif

	while(p < e) // Process remaining bytes (less than 16)
		n += (*p++ == value);
	return n;
}

inline
size_t memcnt16(const void *s, dword value, size_t sz)
{
	const word *p = (word *)s;
	const word *e = p + sz;
	size_t n = 0;

#ifdef CPU_SIMD
	const word *e16 = p + (sz & ~7);  // Process in 16-byte chunks
	if(p < e16) {
		i16x8 value8 = i16all(value);
		do {
			n += CountTrue(i16x8(p) == value8);
			p += 8;
		}
		while(p < e16);
	}
#endif

	while(p < e) // Process remaining words (less than 8)
		n += (*p++ == value);
	return n;
}

inline
size_t memcnt32(const void *s, dword value, size_t sz)
{
	const dword *p = (dword *)s;
	const dword *e = p + sz;
	size_t n = 0;

#ifdef CPU_SIMD
	const dword *e16 = p + (sz & ~3);  // Process in 16-byte chunks
	if(p < e16) {
		i32x4 value4 = i32all(value);
		do {
			n += CountTrue(i32x4(p) == value4);
			p += 4;
		}
		while(p < e16);
	}
#endif

	while(p < e) // Process remaining dwords (less than 4)
		n += (*p++ == value);
	return n;
}
