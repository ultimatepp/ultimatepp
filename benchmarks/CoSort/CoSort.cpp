#include <Core/Core.h>

using namespace Upp;

#ifdef _DEBUG
#define N 10000
#else
#define N 100000000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	Vector<String> h;
	for(int i = 0; i < N; i++)
		h.Add(AsString(Random()));
	
	{
		auto h1 = clone(h);
		TimeStop tm;
		Sort(h1);
		RLOG("Sort " << tm);
	}

	{
		auto h1 = clone(h);
		TimeStop tm;
		CoSort(h1);
		RLOG("CoSort " << tm);
	}

#if 0
	{
		CoWork::SetPoolSize(400);
		auto h1 = clone(h);
		TimeStop tm;
		CoSort(h1);
		RLOG("CoSort (big pool) " << tm);
	}
#endif
	RLOG("Exit");
}
