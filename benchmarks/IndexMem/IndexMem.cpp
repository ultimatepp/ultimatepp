#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

#ifdef _DEBUG
	const int v_num = 10000;
#else
	const int v_num = 1000000;
#endif

	RDUMP(sizeof(Index<int>));

	Vector<Index<int> > v;
	v.SetCount(v_num);
	RDUMP(MemoryUsedKb());
	const int isize = 100;
	TimeStop ts;
	for (int i = 0; i < isize; ++i) {
		const int jsize = v_num;
		for (int j = 0; j < jsize; ++j)
			v[j].FindAdd(i);
	}
	RLOG("Index<int> FindAdd: " << ts);
	RDUMP(MemoryUsedKb());
	RLOG(MemoryProfile());
	ts.Reset();
	for (int i = 0; i < isize; ++i) {
		const int jsize = v_num;
		for (int j = 0; j < jsize; ++j)
			v[j].UnlinkKey(i);
	}
	RLOG("Index<int> UnlinkKey: " << ts);
	ts.Reset();
	const int jsize = v_num;
	for (int j = 0; j < jsize; ++j) {
		v[j].Sweep();
		v[j].Shrink();
	}
	RLOG("Index<int> Sweep: " << ts);
	RDUMP(MemoryUsedKb());
	RLOG(MemoryProfile());
	MemoryDumpHuge();
}
