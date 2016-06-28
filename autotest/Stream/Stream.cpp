#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	SizeStream ss;
	
	ASSERT(ss.IsStoring());
	
	ss.Put(0);
	
	ASSERT(ss.GetSize() == 1);
	
	int len = 1;
	
	SeedRandom();
	for(int i = 0; i < 100; i++) {
		byte h[201];
		int l = Random(200);
		ss.Put(h, l);
		len += l;
		ASSERT(len == ss.GetSize());
	}
	
	LOG("============= OK");
}
