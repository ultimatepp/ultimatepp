#include <Core/Core.h>
#include <plugin/zstd/zstd.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	TimeStop tm;

	Vector<int> x;
	for(int i = 0; i < 100000000; i++)
		x.Add(i);
	int sum = 0;
	CoPartition(x, [&sum](const SubRangeOf<Vector<int>>& range) {
		int sum1 = 0;
		for(const auto& m : range)
			sum1 += m;
		CoWork::FinLock();
		sum += sum1;
	});

	ASSERT(sum = Sum(x));

	ASSERT(sum = CoSum(x));

	sum = 0;
	
	CoPartition(0, x.GetCount(), [&sum, &x](int from, int to) {
		int sum1 = 0;
		while(from < to)
			sum1 += x[from++];
		CoWork::FinLock();
		sum += sum1;
	});

	ASSERT(sum = Sum(x));
	
	SeedRandom();
	
	for(int ii = 0; ii < 200; ii++) {
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
