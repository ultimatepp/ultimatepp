#include <Core/Core.h>

using namespace Upp;

Vector<String> result;

void Process(CoWork& co, String h, int n)
{
	int x = atoi(h);
	x++;
	if(n < 100)
		co.Pipe(n + 1, [=, &co] { Process(co, AsString(atoi(h) + 1), n + 1); });
	else
		result.Add(h);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	CoWork co;
	for(int i = 0; i < 10000; i++)
		co.Pipe(0, [=, &co] { Process(co, AsString(i), 0); });
	co.Finish();
	
	for(int i = 0; i < 10000; i++)
		ASSERT(result[i] == AsString(i + 100));

	Thread::ShutdownThreads();
	
	LOG("============== OK");
}
