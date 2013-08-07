#include "Core.h"

NAMESPACE_UPP

/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/

#include <stdio.h>

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

struct MTrand {
	dword mt[N];
	int   mti;         /* mti==N+1 means mt[N] is not initialized */
	dword mag01[2];
#ifdef PLATFORM_POSIX
	int   pid;
#endif
	
	void seed();
	void init_genrand(dword s);
	void init_by_array(dword *init_key, int key_length);
	dword genrand();

	MTrand();
};

/* initializes mt[N] with a seed */
void MTrand::init_genrand(dword s)
{
	mt[0]= s & 0xffffffffUL;
	for (mti=1; mti<N; mti++) {
	    mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
	    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
	    /* In the previous versions, MSBs of the seed affect   */
	    /* only MSBs of the array mt[].                        */
	    /* 2002/01/09 modified by Makoto Matsumoto             */
	    mt[mti] &= 0xffffffffUL;
	    /* for >32 bit machines */
	}
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void MTrand::init_by_array(dword *init_key, int key_length)
{
	int i, j, k;
	init_genrand(19650218UL);
	i=1; j=0;
	k = (N>key_length ? N : key_length);
	for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
	for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
	}

	mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
dword MTrand::genrand()
{
    dword y;

	if (mti >= N) { /* generate N words at one time */
		int kk;

		for (kk=0;kk<N-M;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<N-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
		
		mti = 0;
    }

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

#ifdef PLATFORM_WIN32

#define Ptr Ptr_
#include <objbase.h>
#undef Ptr

#pragma comment(lib, "ole32.lib")

#endif

MTrand::MTrand()
{
	mti = N + 1;
	mag01[0] = 0;
	mag01[1] = MATRIX_A;
	seed();
#ifdef PLATFORM_POSIX
	pid = getpid();
#endif
}

void MTrand::seed()
{
	dword seed[1024];
#ifdef PLATFORM_POSIX
	int fd = open("/dev/urandom", O_RDONLY);
	if(fd != -1) {
		IGNORE_RESULT(
			read(fd, seed, sizeof(seed))
		);		
		close(fd);
	}
#else
	for(int i = 0; i < 1024; i++) {
		Uuid uuid;
		CoCreateGuid((GUID *)&uuid);
		seed[i] = GetHashValue(uuid);
	}
#endif
	init_by_array(seed, 1024);
}

#ifdef CPU_BLACKFIN
//on blackfin toolchain we dont have TLS support, but we can live here without
MTrand *sRng;
byte    sRb[sizeof(MTrand)];
#else
thread__ MTrand *sRng;
thread__ byte    sRb[sizeof(MTrand)];
#endif

void SeedRandom(dword *seed, int len){
	if(!sRng) {
		sRng = new(sRb) MTrand;
	}
	sRng->init_by_array(seed, len);
}

void SeedRandom(dword seed){
	if(!sRng) {
		sRng = new(sRb) MTrand;
	}
	sRng->init_genrand(seed);
}

dword Random()
{
	if(!sRng) {
		sRng = new(sRb) MTrand;
	}
#ifdef PLATFORM_POSIX // Be fork safe...
	int pid = getpid();
	if(sRng->pid != pid) {
		sRng->seed();
		sRng->pid = pid;
	}
#endif
	return sRng->genrand();
}

dword Random(dword n)
{
	ASSERT(n);
	dword mask = n;
	mask |= mask >> 1;
	mask |= mask >> 2;
	mask |= mask >> 4;
	mask |= mask >> 8;
	mask |= mask >> 16;
	
	dword r;
	do
		r = Random() & mask;
	while(r >= n);
	return r;
}

qword Random64()
{
	return MAKEQWORD(Random(), Random());
}

qword Random64(qword n)
{
	qword mask = n, r;
	mask |= mask >> 1;	mask |= mask >> 2;
	mask |= mask >> 4;	mask |= mask >> 8;
	mask |= mask >> 16;	mask |= mask >> 32;

	do
		r = Random64() & mask;
	while(r >= n);
	return r;
}

double Randomf()
{
	return Random64(I64(4503599627370496)) / 4503599627370496.0;
}

END_UPP_NAMESPACE
