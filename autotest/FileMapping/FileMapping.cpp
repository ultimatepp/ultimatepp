#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String test;
	for(int i = 0; i < 100; i++)
		test << i << " " << i * 12345678 << "\n";
	
	int sz = test.GetCount();
	
	String path = GetHomeDirFile("mapped");
	
	{
		FileMapping m;
		m.Create(path, sz);
		memcpy(m.Map(), ~test, sz);
	}
	
	ASSERT(LoadFile(path) == test);
	
	{
		FileMapping m(path);
		ASSERT(memcmp(m.Map(), ~test, sz) == 0);
	}
	
	DeleteFile(path);
	
	LOG("============ OK");
}
