#include "Core.h"

namespace Upp {

#ifdef CPU_SIMD

void memset8__(void *p, i16x8 data_, size_t len)
{
	i16x8 data = data_;
	ASSERT(len >= 16);
	byte *t = (byte *)p;
	auto Set4 = [&](size_t at) { data.Store(t + at); };
	Set4(len - 16); // fill tail
	Set4(0); // align up on the next 16 bytes boundary
	if(len <= 32)
		return;
	const byte *e = t + len;
	t = (byte *)(((uintptr_t)t | 15) + 1);
	len = e - t;
	e -= 128;
#if 0 // streaming does not seem to be benefical anymore
#ifdef CPU_SSE2
	if(len >= 1024*1024 && 0) { // for really huge data, bypass the cache
		auto Set4S = [&](int at) { data.Stream(t + at); };
		while(len >= 64) {
			Set4S(0*16); Set4S(1*16); Set4S(2*16); Set4S(3*16);
			t += 64;
			len -= 64;
		}
		_mm_sfence();
		e = t - 1;
	}
#endif
#endif
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

	if(len > 4*1024*1024) { // for really huge data, call memcpy to use possible CPU magic
		memcpy(t, s, len);
		return;
	}

	auto Copy128 = [&](size_t at) { i16x8(s + at).Store(t + at); };
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
bool memeq128(const void *p, const void *q, size_t len) { return inline_memeq128_aligned(p, q, len); }

#endif

#ifdef CPU_UNALIGNED

#ifdef HASH64

hash_t memhash(const void *ptr, size_t len)
{
	const byte *s = (byte *)ptr;
	uint64 val = HASH_CONST1;
	if(len >= 8) {
		if(len >= 32) {
			uint64 val1, val2, val3, val4;
			val1 = val2 = val3 = val4 = HASH_CONST1;
			while(len >= 32) {
				val1 = HASH_CONST2 * val1 + Peek64(s);
				val2 = HASH_CONST2 * val2 + Peek64(s + 8);
				val3 = HASH_CONST2 * val3 + Peek64(s + 16);
				val4 = HASH_CONST2 * val4 + Peek64(s + 24);
				s += 32;
				len -= 32;
			}
			val = HASH_CONST2 * val + val1;
			val = HASH_CONST2 * val + val2;
			val = HASH_CONST2 * val + val3;
			val = HASH_CONST2 * val + val4;
		}
		const byte *e = s + len - 8;
		while(s < e) {
			val = HASH_CONST2 * val + Peek64(s);
			s += 8;
		}
		return HASH_CONST2 * val + Peek64(e);
	}
	if(len > 4) {
		val = HASH_CONST2 * val + Peek32(s);
		val = HASH_CONST2 * val + Peek32(s + len - 4);
		return val;
	}
	if(len >= 2) {
		val = HASH_CONST2 * val + Peek16(s);
		val = HASH_CONST2 * val + Peek16(s + len - 2);
		return val;
	}
	return len ? HASH_CONST2 * val + *s : val;
}

#else

never_inline
hash_t memhash(const void *ptr, size_t len)
{
	const byte *s = (byte *)ptr;
	dword val = HASH_CONST1;
	if(len >= 4) {
		if(len >= 48) {
			dword val1, val2, val3, val4;
			val1 = val2 = val3 = val4 = HASH_CONST1;
			while(len >= 16) {
				val1 = HASH_CONST2 * val1 + Peek32(s);
				val2 = HASH_CONST2 * val2 + Peek32(s + 4);
				val3 = HASH_CONST2 * val3 + Peek32(s + 8);
				val4 = HASH_CONST2 * val4 + Peek32(s + 12);
				s += 16;
				len -= 16;
			}
			val = HASH_CONST2 * val + val1;
			val = HASH_CONST2 * val + val2;
			val = HASH_CONST2 * val + val3;
			val = HASH_CONST2 * val + val4;
		}
		const byte *e = s + len - 4;
		while(s < e) {
			val = HASH_CONST2 * val + Peek32(s);
			s += 4;
		}
		return HASH_CONST2 * val + Peek32(e);
	}
	if(len >= 2) {
		val = HASH_CONST2 * val + Peek16(s);
		val = HASH_CONST2 * val + Peek16(s + len - 2);
		return val;
	}
	return len ? HASH_CONST2 * val + *s : val;
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