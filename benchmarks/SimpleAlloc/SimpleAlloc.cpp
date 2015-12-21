#include <Core/Core.h>

using namespace Upp;

#define N 20000000

void *ptr[N];

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
}
