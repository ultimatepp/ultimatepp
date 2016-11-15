#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<int> data;
	for(int i = 0; i < 100000000; i++)
		data.Add(Random());
	
	{
		RTIMING("FindAll");
		RDUMP(FindAll(data, [=](int c) { return c % 177777 == 0; }).GetCount());
	}
	{
		RTIMING("CoFindAll");
		RDUMP(CoFindAll(data, [=](int c) { return c % 177777 == 0; }).GetCount());
	}

	LOG("===================== OK");
}
