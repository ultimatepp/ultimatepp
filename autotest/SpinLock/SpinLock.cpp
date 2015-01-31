#include <Core/Core.h>

using namespace Upp;

SpinLock data_lock;
dword    data;

void AddData()
{
	data_lock.Enter();
	data = data + 1;
	data_lock.Leave();
}

CONSOLE_APP_MAIN
{
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
