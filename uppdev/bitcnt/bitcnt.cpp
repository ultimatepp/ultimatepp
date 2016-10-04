#include <Core/Core.h>

using namespace Upp;

int BitMatch0(dword *a, dword *b, int count)
{
	int pop = 0;
	while(count--) {
		dword x = *a++ & *b++;
		for(int n = 32; n--;) {
			pop += x & 1;
			x >>= 1;
		}
	}
	return pop;
}

static byte sBitCnt[] = { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8 };

int BitMatch1(dword *_a, dword *_b, int count)
{
	int pop = 0;
	byte *a = (byte *)_a;
	byte *b = (byte *)_b;
	count <<= 2;
	while(count--)
		pop += sBitCnt[*a++ & *b++];
	return pop;
}

extern byte sPop16__[];

int BitMatch2(dword *_a, dword *_b, int count)
{
	int pop = 0;
	word *a = (word *)_a;
	word *b = (word *)_b;
	count <<= 1;
	while(count--)
		pop += sPop16__[*a++ & *b++];
	return pop;
}

int BitMatch4(dword *a, dword *b, int count)
{
	int pop = 0;
	while(count--) {
		dword x = *a++ & *b++;
		pop += sPop16__[(word)x] + sPop16__[(word)SwapEndian32(x)];
	}
	return pop;
}

int BitMatch21(dword *a, dword *b, int count)
{
	int pop = 0;
	while(count--) {
		dword x = *a++ & *b++;
		x = ((x & 0xaaaaaaaa) >> 1) + (x & 0x55555555);
		x = ((x & 0xcccccccc) >> 2) + (x & 0x33333333);
		x = ((x & 0xf0f0f0f0) >> 4) + (x & 0x0f0f0f0f);
		x = ((x & 0xff00ff00) >> 8) + (x & 0x00ff00ff);
		x = ((x & 0xffff0000) >> 16) + (x & 0x0000ffff);
		pop += x;
	}
	return pop;
}

int BitMatch3(dword *a, dword *b, int count)
{
	int pop = 0;
	while(count--) {
		dword n = *a++ & *b++;
		n = n - ((n >> 1) & 0x55555555);
		n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
		n = (n + (n >> 4)) & 0x0F0F0F0F;
		n *= 0x1010101;
		pop += n >> 24;
	}
	return pop;
}

int BitMatch41(dword *a, dword *b, int count)
{
	int pop = 0;
	while(count--) {
		dword n = *a++ & *b++;
		n = n - ((n >> 1) & 0x55555555);
		n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
		n = (n + (n >> 4)) & 0x0F0F0F0F;
		n = n + (n >> 8);
		n = n + (n >> 16);
		pop += n & 31;
	}
	return pop;
}

void TestPopCount()
{
	dword a[1024];
	dword b[1024];
	for(int i = 0; i < 500 * 1000; i++) {
		for(int q = 0; q < 1024; q++) {
			a[q] = MAKELONG(rand(), rand());
			b[q] = MAKELONG(rand(), rand());
		}
		if(i % 10000 == 0)
			Cout() << i << '\n';
		ASSERT(BitMatch0(a, b, 1024) == BitMatch4(a, b, 1024));
		ASSERT(BitMatch2(a, b, 1024) == BitMatch4(a, b, 1024));
	}
}

void BenchmarkPopCount()
{
	dword a[1024];
	dword b[1024];
	for(int q = 0; q < 1024; q++) {
		a[q] = MAKELONG(rand(), rand());
		b[q] = MAKELONG(rand(), rand());
	}
	int cnt = 0;
	{
		RTIMING("2");
		for(int i = 0; i < 100 * 1000; i++)
			cnt += BitMatch2(a, b, 1024);
	}
	{
		RTIMING("4");
		for(int i = 0; i < 100 * 1000; i++)
			cnt += BitMatch4(a, b, 1024);
	}
	RDUMP(cnt);
}

void GenTable()
{
	String r;
	for(int i = 0; i < 65536; i++) {
		int n = 0;
		for(int j = 0; j < 16; j++)
			if(i & (1 << j)) n++;
		if((i & 31) == 0)
			r << '\n';
		r << n << ',';
	}
	LOG(r);
}

CONSOLE_APP_MAIN
{
//	TestPopCount(); return;
	BenchmarkPopCount();
}

