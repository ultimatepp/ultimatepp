#include <Core/Core.h>
#include <vector>

using namespace Upp;

#define N 1000000

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);

	Vector<String> a;
	std::vector<std::string> b;
	for(int i = 0; i < N; i++) {
		String h = FormatIntBase(Random(10000) * Random(10000), 2);
		a.Add(h);
		b.push_back(h.ToStd());
	}
	Sort(a);
	std::sort(b.begin(), b.end());
	for(int i = 0; i < N; i++)
		ASSERT(a[i].ToStd() == b[i]);
	
	LOG("----------------- OK");
}
