#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String test;
	for(int i = 0; i < 10000; i++)
		test << i << " " << i * 12345678 << "\n";
	
	int sz = test.GetCount();
	
	DUMP(sz);
	
	String path = GetHomeDirFile("mapped");
	
//	SaveFile(path, String('0', sz));
	
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
	
	{
		FileMapping m(path);
		for(int i = 0; i < sz; i++)
			ASSERT(*m.Map(i, 1) == test[i]);
	}
	
	{
		FileMapping m;
		m.Open(path, FileStream::READWRITE);
		byte *h = m.Map();
		h[3] = 'X';
		h[4] = 'Y';
	}
	
	test.Set(3, 'X');
	test.Set(4, 'Y');
	
	ASSERT(LoadFile(path) == test);
	
	DeleteFile(path);
	
	LOG("============ OK");
}
