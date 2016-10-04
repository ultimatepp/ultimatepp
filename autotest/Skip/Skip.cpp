#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String fn = GetHomeDirFile("test.txt");
	{
		FileOut out(fn);
		for(int i = 0; i < 10000; i++)
			out.Put('0');
		out.Put("test");
	}
	{
		FileIn in(fn);
		ASSERT(in.Skip(10000) == 10000);
		ASSERT(in.Get(1000) == "test");
		ASSERT(in.Skip(10) == 0);
	}
	{
		FileIn in(fn);
		ASSERT(in.Skip(20000) == 10004);
	}

	FileDelete(fn);
	LOG("============= OK");
}
