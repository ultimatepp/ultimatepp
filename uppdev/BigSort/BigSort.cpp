#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<int> x;

#if 1
	FileIn in(GetDataFile("data.txt"));
	if(!in)
		return;
	while(!in.IsEof()) {
		x.Add(ScanInt(in.GetLine()));
	}
#else
	for(int i = 0; i < 200; i++)
		x.Add(Random(25));
#endif

//	DDUMPC(x);
	Sort(x);
	LOG("------------------------");
	DDUMPC(x);
}
