#include <Core/Core.h>

CONSOLE_APP_MAIN
{
	Parallel m;
	for(int i = 0; i < 100; i++)
		m * callback1(Test, i);
	m.Wait();
}
