#include <Core/Core.h>

using namespace Upp;

#define N 50000000
#define NMIX 100000000
#define MIX 30

void *ptr[N];
int   len[NMIX];

void MixTest()
{
	void *mix[MIX];
	for(int i = 0; i < MIX; i++)
		mix[i] = NULL;
	for(int i = 0; i < NMIX; i++) {
		int ii = i % MIX;
		if(mix[ii])
			delete[] mix[ii];
		mix[ii] = new byte[len[i]];
	}
	for(int i = 0; i < MIX; i++)
		delete[] mix[i];
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	{
		TimeStop tm;
		for(int i = 0; i < N; i++)
			ptr[i] = new byte[32];
		RLOG("Alloc " << tm);
	}
	{
		TimeStop tm;
		for(int i = 0; i < N; i++)
			delete[] ptr[i];
		RLOG("Free " << tm);
	}
	{
		for(int i = 0; i < NMIX; i++)
			len[i] = Random(500);
		TimeStop tm;
		MixTest();
		RLOG("Mix " << tm);
	}
	Vector<int> h = { 1, 2, 3, 4, 5, 6, 7, 8, 12, 16 };
	for(auto n : h) {
		TimeStop tm;
		Buffer<Thread> t(n);
		for(int i = 0; i < n; i++)
			t[i].Run(callback(MixTest));
		for(int i = 0; i < n; i++)
			t[i].Wait();
		RLOG(n << " threads Mix " << tm);
	}
}
