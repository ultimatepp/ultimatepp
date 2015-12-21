#include <Core/Core.h>

using namespace Upp;

#define N 50000000
#define MIX 30

void *ptr[N];
void *mix[MIX];
int   len[N];

CONSOLE_APP_MAIN
{
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
		for(int i = 0; i < N; i++)
			len[i] = Random(1000 + 500);
		TimeStop tm;
		for(int i = 0; i < N / MIX; i++) {
			int ii = i % MIX;
			if(mix[ii])
				delete[] mix[ii];
			mix[ii] = new byte[len[i]];
		}
		RLOG("Mix " << tm);
	}
}
