#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TimeStop tm;

	StdLogSetup(LOG_COUT|LOG_FILE);

	SeedRandom();

#ifdef CPU_ARM
	for(int ii = 0; ii < 10; ii++)
#else
	for(int ii = 0; ii < 200; ii++)
#endif
	{
		DUMP(ii);
		int N = Random(20000000);
		int mod = 6 * Random(50000) + 1;
		int from = Random(N);
		DUMP(N);
		DUMP(mod);
		DUMP(from);
		Vector<int> data;
		for(int i = 0; i < N; i++)
			data.Add(Random());
		
		auto f = FindAll(data, [=](int c) { return c % mod == 0; }, from);
		auto cf = CoFindAll(data, [=](int c) { return c % mod == 0; }, from);
		DUMP(f.GetCount());
		DUMP(cf.GetCount());
		ASSERT(f == cf);
		LOG("================");
	}
	
	LOG("===================== OK in " << tm);
}
