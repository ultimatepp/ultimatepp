#include "Core.h"

namespace Upp {

#ifdef CPU_X86

void memset8__(void *p, __m128i data, size_t len)
{
	ASSERT(len >= 16);
	byte *t = (byte *)p;
	auto Set4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), data); };
	Set4(len - 16); // fill tail
	Set4(0); // align up on the next 16 bytes boundary
	if(len <= 32)
		return;
	const byte *e = t + len;
	t = (byte *)(((uintptr_t)t | 15) + 1);
	len = e - t;
	e -= 128;
	if(len >= 1024*1024) { // for really huge data, bypass the cache
		auto Set4S = [&](int at) { _mm_stream_si128((__m128i *)(t + at), data); };
		while(len >= 64) {
			Set4S(0*16); Set4S(1*16); Set4S(2*16); Set4S(3*16);
			t += 64;
			len -= 64;
		}
		_mm_sfence();
		e = t - 1;
	}
	while(t <= e) {
		Set4(0*16); Set4(1*16); Set4(2*16); Set4(3*16);
		Set4(4*16); Set4(5*16); Set4(6*16); Set4(7*16);
		t += 128;
	}
	if(len & 64) {
		Set4(0*16); Set4(1*16); Set4(2*16); Set4(3*16);
		t += 64;
	}
	if(len & 32) {
		Set4(0*16); Set4(1*16);
		t += 32;
	}
	if(len & 16)
		Set4(0);
}

never_inline
void memcpy8__(void *p, const void *q, size_t len)
{
	ASSERT(len >= 16);
	
	byte *t = (byte *)p;
	const byte *s = (const byte *)q;

	if(len > 4*1024*1024) { // for really huge data, call memcpy to bypass the cache
		memcpy(t, s, len);
		return;
	}

	auto Copy128 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };
	Copy128(len - 16); // copy tail
	Copy128(0); // align target data up on the next 16 bytes boundary
	if(len <= 32)
		return;
	const byte *e = t + len;
	byte *t1 = (byte *)(((uintptr_t)t | 15) + 1);
	s += t1 - t;
	t = t1;
	len = e - t;
	e -= 64;
	while(t <= e) {
		Copy128(0*16); Copy128(1*16); Copy128(2*16), Copy128(3*16);
		t += 64;
		s += 64;
	}
	if(len & 32) {
		Copy128(0*16); Copy128(1*16);
		t += 32;
		s += 32;
	}
	if(len & 16)
		Copy128(0*16);
}

bool memeq8(const void *p, const void *q, size_t len) { return inline_memeq8_aligned(p, q, len); }
bool memeq16(const void *p, const void *q, size_t len) { return inline_memeq16_aligned(p, q, len); }
bool memeq32(const void *p, const void *q, size_t len) { return inline_memeq32_aligned(p, q, len); }
bool memeq64(const void *p, const void *q, size_t len) { return inline_memeq64_aligned(p, q, len); }

#endif

#ifdef CPU_UNALIGNED

#if 1

#ifdef HASH64

hash_t memhash(const void *ptr, size_t len)
{
	const byte *s = (byte *)ptr;
	uint64 val = HASH64_CONST1;
	if(len >= 8) {
		if(len >= 32) {
			uint64 val1, val2, val3, val4;
			val1 = val2 = val3 = val4 = HASH64_CONST1;
			while(len >= 32) {
				val1 = HASH64_CONST2 * val1 + *(qword *)(s);
				val2 = HASH64_CONST2 * val2 + *(qword *)(s + 8);
				val3 = HASH64_CONST2 * val3 + *(qword *)(s + 16);
				val4 = HASH64_CONST2 * val4 + *(qword *)(s + 24);
				s += 32;
				len -= 32;
			}
			val = HASH64_CONST2 * val + val1;
			val = HASH64_CONST2 * val + val2;
			val = HASH64_CONST2 * val + val3;
			val = HASH64_CONST2 * val + val4;
		}
		const byte *e = s + len - 8;
		while(s < e) {
			val = HASH64_CONST2 * val + *(qword *)(s);
			s += 8;
		}
		return HASH64_CONST2 * val + *(qword *)(e);
	}
	if(len > 4) {
		val = HASH64_CONST2 * val + *(dword *)(s);
		val = HASH64_CONST2 * val + *(dword *)(s + len - 4);
		return val;
	}
	if(len >= 2) {
		val = HASH64_CONST2 * val + *(word *)(s);
		val = HASH64_CONST2 * val + *(word *)(s + len - 2);
		return val;
	}
	return len ? HASH64_CONST2 * val + *s : val;
}

#else

hash_t memhash(const void *ptr, size_t len)
{
	const byte *s = (byte *)ptr;
	dword val = HASH32_CONST1;
	if(len >= 4) {
		if(len >= 16) {
			dword val1, val2;
			val1 = val2 = HASH32_CONST1;
			while(len >= 8) {
				val1 = HASH32_CONST2 * val1 + *(dword *)(s);
				val2 = HASH32_CONST2 * val2 + *(dword *)(s + 4);
				s += 8;
				len -= 8;
			}
			val = HASH32_CONST2 * val + val1;
			val = HASH32_CONST2 * val + val2;
		}
		const byte *e = s + len - 4;
		while(s < e) {
			val = HASH32_CONST2 * val + *(dword *)(s);
			s += 4;
		}
		return HASH32_CONST2 * val + *(dword *)(e);
	}
	if(len >= 2) {
		val = HASH32_CONST2 * val + *(word *)(s);
		val = HASH32_CONST2 * val + *(word *)(s + len - 2);
		return val;
	}
	return len ? HASH32_CONST2 * val + *s : val;
}

#endif

#else

NOUBSAN // CPU supports unaligned memory access
hash_t memhash(const void *ptr, size_t count)
{
	unsigned hash = 1234567890U;

	const unsigned *ds = (unsigned *)ptr;
	const unsigned *de = ds + (count >> 2);
	while(ds < de)
		hash = ((hash << 5) - hash) ^ *ds++;

	const byte *s = (byte *)ds;
	const byte *e = s + (count & 3);
	while(s < e)
		hash = ((hash << 5) - hash) ^ *s++;

	return hash;
}

#endif

#else

hash_t memhash(const void *ptr, size_t count)
{
	unsigned hash = 1234567890U;

	const byte *s = (byte *)ptr;
	const byte *e = s + count;
	while(s < e)
		hash = ((hash << 5) - hash) ^ *s++;

	return hash;
}

#endif

};