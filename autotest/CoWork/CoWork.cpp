#include <Core/Core.h>

using namespace Upp;

void Do() {
	Vector<int> h;
	CoWork co;
	for(int i = 0; i < 1000000; i++)
		co & [&] { INTERLOCKED { h.Add(Random()); } };
	LOG("Exit Do");
}

CONSOLE_APP_MAIN
{
	Thread a, b, c;
	a.Run(callback(Do));
	b.Run(callback(Do));
	c.Run(callback(Do));
	Do();

	a.Wait();
	b.Wait();
	c.Wait();

	LOG("Exit main");
	LOG("=============== OK");
}
