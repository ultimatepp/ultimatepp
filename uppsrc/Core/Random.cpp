#include "Core.h"

namespace Upp {

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* This is xoshiro256** 1.0, our all-purpose, rock-solid generator. It has
   excellent (sub-ns) speed, a state (256 bits) that is large enough for
   any parallel application, and it passes all tests we are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

static force_inline uint64_t s_rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k)); // GCC/CLANG/MSC happily optimize this
}

force_inline
static uint64 sNext(uint64 *s)
{
	const uint64_t result_starstar = s_rotl(s[1] * 5, 7) * 9;

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = s_rotl(s[3], 45);

	return result_starstar;
}

never_inline
static void sSeed(uint64 *s)
{
#ifdef PLATFORM_POSIX
	int fd = open("/dev/urandom", O_RDONLY);
	if(fd != -1) {
		IGNORE_RESULT(
			read(fd, s, 4 * sizeof(uint64))
		);
		close(fd);
	}
#else
	for(int pass = 0; pass < 4; pass++) {
		for(int i = 0; i < 4; i++) {
			CombineHash h;
			h << GetSysTime().Get() << usecs() << msecs();
			for(int p = 0; p < 2; p++) {
				Uuid uuid;
				CoCreateGuid((GUID *)&uuid); // GUID is basically a random number...
				h << uuid.v[0] << uuid.v[1];
			}
			s[i] ^= h;
		}
	}
#endif
}

force_inline
static uint64 *sState()
{
#ifdef MINGW_TLS_PATCH
	static FastMingwTls<uint64 *> s;
#else
	thread_local uint64 *s;
#endif
	if(!s) {
		thread_local uint64 state[4];
		s = state;
		sSeed(s);
#ifdef PLATFORM_POSIX
		if(Thread::IsMain()) // non-main threads do not work with fork anyway
			pthread_atfork(NULL, NULL, [] { sSeed(s); }); // reseed random generator after fork
#endif
	}
	return s;
}

dword Random()
{
	return (dword)sNext(sState());
}

uint64 Random64()
{
	return sNext(sState());
}

void Random64(uint64 *t, int n) {
	uint64 *s = sState();
	while(n-- > 0)
		*t++ = sNext(s);
}

dword Random(dword n) {
	ASSERT(n);
	uint64 *s = sState();
	dword mask = (1 << SignificantBits(n)) - 1;
	dword r;
	do
		r = (dword)sNext(s) & mask;
	while(r >= n);
	return r;
}

uint64 Random64(uint64 n) {
	ASSERT(n);
	uint64 *s = sState();
	uint64 mask = ((uint64)1 << SignificantBits64(n)) - 1;
	uint64 r;
	do
		r = sNext(s) & mask;
	while(r >= n);
	return r;
}

double Randomf()
{
	return (sNext(sState()) >> 11) * (1. / (UINT64_C(1) << 53));
}

void SeedRandom()
{
	sSeed(sState());
}

void SeedRandom(dword seed) {
	uint64 *s = sState();
	for(int i = 0; i < 4; i++)
		s[i] = 12345678 + seed + i; // xoshiro does not work well if all is zero
}

}
