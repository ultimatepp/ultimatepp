#include <Core/Core.h>

using namespace Upp;

void SomeThread()
{
	String h('x', 200);
	Thread::Exit();
	ASSERT(0);
}

CONSOLE_APP_MAIN
{
	Thread t;
	t.Start([] { SomeThread(); });
	t.Wait();
	LOG("------------- OK");
}
