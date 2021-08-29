#include <Core/Core.h>

using namespace Upp;

Atomic val;

struct Data {
	int64 val[4];
	int   xxx;
	
	void Zero() { val[0] = val[1] = val[2] = val[3] = 0; }
};

Data src[16], dst[16];

#define N 10000000

int count;

/*
force_inline void SSEZero16(void *t)
{
	_mm_storeu_si128((__m128i*)t, _mm_setzero_si128());
}

force_inline void SSEZero32(void *t)
{
	_mm_storeu_si128((__m128i*)t, _mm_setzero_si128());
	_mm_storeu_si128((__m128i*)t + 1, _mm_setzero_si128());
}
*/

byte b10000[10000];
String es;

CONSOLE_APP_MAIN
{
	{
		RTIMING("Atomic Inc+Dec");
		for(int i = 0; i < N; i++) {
			AtomicInc(val);
			AtomicDec(val);
		}
	}
	{
		RTIMING("Atomic Inc+Dec+test (unstable)");
		for(int i = 0; i < N; i++) {
			if(i & 1) {
				AtomicInc(val);
				AtomicInc(val);
			}
			if(AtomicDec(val))
				count++;
		}
	}
	{
		RTIMING("Read (no locking)");
		for(int i = 0; i < N; i++) {
			count += src[i & 15].val[0];
		}
	}
	{
		RTIMING("static variable");
		for(int i = 0; i < N; i++) {
			static String h;
			count += src[i & 15].val[0];
		}
	}
/*	{
		RTIMING("Barrier read");
		for(int i = 0; i < N; i++) {
			count += ReadWithBarrier(src[i & 15].val[0]);
		}
	}*/
	{
		static StaticMutex mtx;
		RTIMING("StaticMutex");
		for(int i = 0; i < N; i++) {
			Mutex::Lock __(mtx);
		}
	}
	{
		static Mutex mtx;
		for(int i = 0; i < N; i++) {
			RTIMING("Mutex");
			Mutex::Lock __(mtx);
		}
	}
	{
		static SpinLock lock;
		for(int i = 0; i < N; i++) {
			RTIMING("SpinLock");
			SpinLock::Lock __(lock);
		}
	}
	{
		for(int i = 0; i < N; i++) {
			RTIMING("Alloc/Free");
			delete[] new byte[32];
		}
	}
	{
		for(int i = 0; i < N; i++) {
			RTIMING("Alloc32/Free32");
			MemoryFree32(MemoryAlloc32());
		}
	}
	{
		char h[32] = { 0 };
		for(int i = 0; i < N; i++) {
			RTIMING("String 32");
			String s(h, 30);
			es = s;
		}
	}
	{
		for(int i = 0; i < 100; i++) {
			RTIMING("Alloc/Free huge");
			delete[] new byte[300000];
		}
	}
	if(0) { // very slow
		for(int i = 0; i < N; i++) {
			RTIMING("Alloc/Free 128K");
			delete[] new byte[128*1024];
		}
	}
	{
		RTIMING("Alloc/Free large");
		for(int i = 0; i < N; i++) {
			delete[] new byte[3000];
		}
	}
	{
		RTIMING("malloc/free 32");
		for(int i = 0; i < N; i++) {
			free(malloc(32));
		}
	}
	{
		RTIMING("malloc/free 3000");
		for(int i = 0; i < N; i++) {
			free(malloc(3000));
		}
	}
	{
		RTIMING("Zero32");
		for(int i = 0; i < N; i++) {
			dst[i & 15].Zero();
		}
	}
/*	{
		RTIMING("SSEZero32");
		for(int i = 0; i < N; i++) {
			SSEZero32(&dst[i & 15]);
		}
	}
	{
		RTIMING("SSEZero16");
		for(int i = 0; i < N; i++) {
			SSEZero16(&dst[i & 15]);
		}
	}
*/	{
		RTIMING("Copy32");
		for(int i = 0; i < N; i++) {
			dst[i & 15] = src[i & 15];
		}
	}
	int cnt = max((int)Random(10), 32);
	{
		RTIMING("memcpy variable 32");
		for(int i = 0; i < N; i++) {
			memcpy(&dst[i & 15], &src[i & 15], cnt);
		}
	}
	{
		RTIMING("memcpy fixed 32");
		for(int i = 0; i < N; i++) {
			memcpy(&dst[i & 15], &src[i & 15], 32);
		}
	}
	{
		RTIMING("Copy64");
		for(int i = 0; i < N; i++) {
			dst[i & 15] = src[i & 15];
			dst[i & 7] = src[i & 7];
		}
	}
	{
		RTIMING("Copy96");
		for(int i = 0; i < N; i++) {
			dst[i & 15] = src[i & 15];
			dst[i & 7] = src[i & 7];
			dst[i & 3] = src[i & 3];
		}
	}
	{
		RTIMING("Copy128");
		for(int i = 0; i < N; i++) {
			dst[i & 1] = src[i & 1];
			dst[i & 15] = src[i & 15];
			dst[i & 7] = src[i & 7];
			dst[i & 3] = src[i & 3];
		}
	}
	{
		int n = Random();
		RTIMING("Divide");
		for(int i = 0; i < N; i++) {
			dst[i & 15].xxx = src[i & 15].xxx / n;
		}
	}
	{
		int n = Random();
		RTIMING("Mul");
		for(int i = 0; i < N; i++) {
			dst[i & 15].xxx = src[i & 15].xxx * n;
		}
	}

	{
		int n = Random();
		RTIMING("Add");
		for(int i = 0; i < N; i++) {
			dst[i & 15].xxx = src[i & 15].xxx + n;
		}
	}

	{
		RTIMING("Zero 10000");
		for(int i = 0; i < N; i++)
			memset(b10000, 0, 10000);
	}
	{
		RTIMING("new 10000, Zero, delete");
		for(int i = 0; i < N; i++) {
			byte *b = new byte[10000];
			memset(b, 0, 10000);
			delete[] b;
		}
	}
	{
		RTIMING("new 10000, delete");
		for(int i = 0; i < N; i++) {
			byte *b = new byte[10000];
			delete[] b;
		}
	}
}
