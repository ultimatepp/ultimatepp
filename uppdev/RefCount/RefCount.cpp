#include <Core/Core.h>

#ifdef _DEBUG
#define N 1
#else
#define N 500000000
#endif

namespace Upp {
#include <Core/HeapImp.h>
};

using namespace Upp;

Atomic       x;
volatile int y;
String       t;

void TestThread(const char *q)
{
	String s = q;
	for(int i = 0; i < N; i++)
		String t = s;
}

void Test(String s)
{
	Thread a, b;
	a.Run(callback1(TestThread, ~s));
	b.Run(callback1(TestThread, ~s));
	a.Wait();
	b.Wait();
}

void TestThreadMA()
{
	String s = "01234567890123456789";
	for(int i = 0; i < N; i++) {
		volatile qword *d = (qword *)MemoryAlloc32();
		const qword *q = (const qword *)~s;
		d[0] = q[0]; d[1] = q[1]; d[2] = q[2]; d[3] = q[3];
		MemoryFree32((void *)d);
	}
}

volatile int cnt;

void TestThreadINC()
{
	for(int i = 0; i < N; i++)
		cnt++;	
}

void TestThreadAtomicINC()
{
	for(int i = 0; i < N; i++)
		AtomicInc(x);	
}

void TestThreadRD()
{
	int q = 0;
	for(int i = 0; i < N; i++)
		q += cnt;	
}

CONSOLE_APP_MAIN
{
	String x = "01234567890123456789";
	
	{
		RTIMING("MA ST");
		TestThreadMA();
	}
	{
		RTIMING("MA MT");
		Thread a, b;
		a.Run(callback(TestThreadMA));
		b.Run(callback(TestThreadMA));
		a.Wait();
		b.Wait();
	}
	{
		RTIMING("INC ST");
		TestThreadINC();
	}
	{
		RTIMING("INC MT");
		Thread a, b;
		a.Run(callback(TestThreadINC));
		b.Run(callback(TestThreadINC));
		a.Wait();
		b.Wait();
	}
	{
		RTIMING("AtomicINC ST");
		TestThreadAtomicINC();
	}
	{
		RTIMING("AtomicINC MT");
		Thread a, b;
		a.Run(callback(TestThreadAtomicINC));
		b.Run(callback(TestThreadAtomicINC));
		a.Wait();
		b.Wait();
	}
	{
		RTIMING("RD ST");
		TestThreadRD();
	}
	{
		RTIMING("RD MT");
		Thread a, b;
		a.Run(callback(TestThreadRD));
		b.Run(callback(TestThreadRD));
		a.Wait();
		b.Wait();
	}
	{
		RTIMING("20 ST");
		TestThread(~x);
	}
	{
		RTIMING("20 MT");
		Test(x);
	}
	x << x;
	{
		RTIMING("40 single");
		TestThread(x);
	}
	{
		RTIMING("40");
		Test(x);
	}
	x << x;
	{
		RTIMING("80 single");
		TestThread(x);
	}
	{
		RTIMING("80");
		Test(x);
	}
	return;
/*
	Atomic *x = new Atomic;
	{
		RTIMING("Atomic");
		for(int i = 0; i < N; i++)
			AtomicInc(*x);
	}
	int *p = new int;
	{
		RTIMING("Normal");
		for(int i = 0; i < N; i++)
			*p++;
	}
	{
		RTIMING("Smart");
		for(int i = 0; i < N; i++) {
			if(IsInHeap(p))
				*p++;
		}
	}
	delete p;
*/
}
