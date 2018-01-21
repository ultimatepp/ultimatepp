#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(auto n : { 100, 20000 }) {
		Vector<int> x;
		for(int i = 0; i < n; i++)
			x.Add(i);
		
		DUMP(Sum(x));
		DUMP(CoSum(x));
		ASSERT(CoSum(x) == Sum(x));
		
		int sum = 0;
		CoPartition(x,
			[&sum](const SubRangeOf<Vector<int>>& x) {
				int s = Sum(x);
				CoWork::FinLock();
				sum += s;
		});
		
		DUMP(sum);
		ASSERT(sum == Sum(x));
	}
	
	LOG("================ OK");
}
