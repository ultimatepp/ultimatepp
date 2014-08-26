#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Index<String> index;
	TimeStop tm;
	RTIMESTOP("Total runtime");
	for(int i = 0; i < 1000000; i++) {
		int x;
		String q;
		{
			RTIMING("rand");
			x = rand();
		}
		{
			RTIMING("AsString");
			q = AsString(x);
		}
		{
			RTIMING("Index::FindAdd");
			index.FindAdd(q);
		}
	}
	RDUMP(index.GetCount());
	RDUMP(tm.Elapsed());
}
