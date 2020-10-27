#include <Core/Core.h>
#include <vector>

using namespace Upp;

#ifdef _DEBUG
#define N 100000
#else
#define N 1000000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	RDUMP(CPU_Cores());

	Vector<String> a;
	std::vector<std::string> c;
	for(int i = 0; i < N; i++) {
		String s = AsString(Random());
		a.Add(s);
		c.push_back(s.ToStd());
	}

#ifdef _DEBUG
	Vector<String> b = clone(a);
	Sort(a);
	CoSort(b);
	ASSERT(a == b);
#else
	RDUMP(N);
	for(int i = 0; i < max(1, 10000000 / N); i++) {
		{
			Vector<String> b = clone(a);
			RTIMING("Sort");
			Sort(b);
		}
		{
			Vector<String> b = clone(a);
			RTIMING("CoSort");
			CoSort(b);
		}
		if(0) {
			std::vector<std::string> d = c;
			RTIMING("std::sort");
			std::sort(d.begin(), d.end());
		}
		if(0) {
			{
				std::vector<std::string> d = c;
				RTIMING("CoSort<string>");
				CoSort(SubRange(d.begin(), d.end()), StdLess<std::string>());
			}
			{
				std::vector<std::string> d = c;
				RTIMING("Sort<string>");
				Sort(SubRange(d.begin(), d.end()), StdLess<std::string>());
			}
		}
	}
#endif
}
