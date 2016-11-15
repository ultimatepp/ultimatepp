#include <Core/Core.h>
#include <plugin/zstd/zstd.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);

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
	
	LOG("=================== OK");
}
