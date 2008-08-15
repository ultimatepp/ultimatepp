#include <CppBase/CppBase.h>

using namespace Upp;

void Error(int line, const String& x)
{
	RDUMP(line);
	RDUMP(x);
}

CONSOLE_APP_MAIN
{
	TimeStop tm;
	for(int i = 0; i < 1; i++) {
		{
			CppBase base;
			FileIn in(GetDataFile("test.txt"));
			Parse(in, Vector<String>(), base, "", callback(Error));
			RDUMP(base.GetCount());
			for(int i = 0; i < base.GetCount(); i++)
				RDUMP(base.GetKey(i));
		}
	}
	RDUMP(tm.Elapsed());
}
