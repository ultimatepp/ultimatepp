#include <Core/Core.h>

using namespace Upp;

#define N 10000000

CONSOLE_APP_MAIN
{
	String s;
	s << GetSysDate() << "Hello world!!!";
	int q = 0;
	{
		RTIMING("Find 2");
		for(int i = 0; i < N; i++)
			q += s.FindFirstOf("rd");
	}
	{
		RTIMING("Find 3");
		for(int i = 0; i < N; i++)
			q += s.FindFirstOf("!ab");
	}
	{
		RTIMING("Find 4");
		for(int i = 0; i < N; i++)
			q += s.FindFirstOf("!abc");
	}
	{
		RTIMING("Find 5");
		for(int i = 0; i < N; i++)
			q += s.FindFirstOf("!abcd");
	}
	RDUMP(q);
}
