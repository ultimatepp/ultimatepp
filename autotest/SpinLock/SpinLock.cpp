#include <Core/Core.h>

using namespace Upp;

SpinLock data_lock;
dword    data;

void AddData()
{
	SpinLock::Lock __(data_lock);
	data = data + 1;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	RTIMING("Test");
	{
		CoWork co;
		for(int i = 0; i < 10000000; i++)
			co & callback(AddData);
	}
	RDUMP(data);
	ASSERT(data == 10000000);
	LOG("================ OK");
}
