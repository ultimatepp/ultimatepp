#include "Core.h"

namespace Upp {

#ifdef CPU_X86
void huge_memsetd(void *p, dword c, size_t len)
{ // bypasses the cache, good for >4MB
	dword *t = (dword *)p;
}

String AsString(__m128i x)
{
	dword h[4];
	memcpy(h, &x, 16);
	word w[8];
	memcpy(w, &x, 16);
	return Sprintf("_%08x_%08x_%08x_%08x (%d, %d, %d, %d) : (%d, %d, %d, %d)",
	               h[3], h[2], h[1], h[0], w[6], w[5], w[4], w[7], w[2], w[1], w[0], w[3]);
}

void memset8__(void *p, __m128i data, size_t len)
{
	ASSERT(len >= 16);
	byte *t = (byte *)p;
	auto Set4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), data); };
	Set4(len - 16); // fill tail
	Set4(0); // align up on the next 16 bytes boundary
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

#endif

};