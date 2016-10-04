#include <Core/Core.h>

//#include <intrin.h>

enum { COUNT = 600 };

const char *s[COUNT];

LONG x[100000];

inline void Inc(long& i)
{
	i++;
}

#define ATOMIC_INCREMENT_32(_ptr) \
	void *p = _ptr; \
	__asm mov ecx, p; \
	__asm mov eax, 1; \
	__asm lock xadd dword ptr [ECX], EAX; \


__declspec(thread) LONG *queue[100];
__declspec(thread) int   queue_i;

struct Qfast {
	LONG *ptr[100];
	int   i;

	Qfast() { i = 0; }
};

__declspec(thread) Qfast *fq;


template <int N>
struct SSO {
	byte x[N];
};

SSO<16> a[100000];
SSO<16> b[100000];

SSO<32> a32[100000];
SSO<32> b32[100000];

struct SSOF {
	dword a, b, c, d;

	void operator=(const SSOF& o) { a = o.a; b = o.b; c = o.c; d = o.d; }
};

SSOF af[100000];
SSOF bf[100000];

#define N 500000

CONSOLE_APP_MAIN
{
	fq = new Qfast;
	{
		TimeStop raw_inc;
		for(int j = 0; j < 10000; j++)
			for(int i = 0; i < 10000; i++)
				Inc(x[i]);
		RDUMP(raw_inc);
	}
	for(int i = 0; i < 10000; i += 5)
		a[i].x[15] = a32[i].x[31] = 255;
	{
		TimeStop que;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++) {
				if(queue_i >= 100) {
					for(int i = 0; i < 100; i++)
						++*queue[i];
					queue_i = 0;
				}
				queue[queue_i++] = &x[i];
			}
		RDUMP(que);
	}
	{
		TimeStop qfast;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++) {
				Qfast *q = fq;
				if(q->i >= 100) {
					LONG **p = q->ptr;
					LONG **e = q->ptr + 100;
					while(p < e) {
						++*p;
						p++;
					}
					q->i = 0;
				}
				q->ptr[q->i++] = &x[i];
			}
		RDUMP(qfast);
	}
//	{
//		TimeStop _Interlocked;
//		for(int j = 0; j < N; j++)
//			for(int i = 0; i < 100; i++)
//				_InterlockedIncrement(&x[i]);
//		RDUMP(_Interlocked);
//	}
	{
		TimeStop Interlocked;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++)
				InterlockedIncrement(&x[i]);
		RDUMP(Interlocked);
	}
	{
		TimeStop raw_inc;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++)
				Inc(x[i]);
		RDUMP(raw_inc);
	}
	{
		TimeStop SSO16;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++)
				if(b[i].x[15] < 15)
					a[i] = b[i];
		RDUMP(SSO16);
	}
	{
		TimeStop SSO32;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++)
				if(b32[i].x[31] < 15)
					a32[i] = b32[i];
		RDUMP(SSO32);
	}
	{
		TimeStop SSOF;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++)
				af[i] = bf[i];
		RDUMP(SSOF);
	}
	{
		TimeStop HEAP;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++) {
				byte *b = new byte[i];
				delete b;
			}
		RDUMP(HEAP);
	}
/*	{
		TimeStop MALLOC;
		for(int j = 0; j < N; j++)
			for(int i = 0; i < 100; i++) {
				void *b = malloc(i);
				free(b);
			}
		RDUMP(MALLOC);
	}*/
	delete fq;
}
