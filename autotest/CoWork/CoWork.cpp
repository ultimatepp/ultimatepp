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
	if(1) {
		CoWork co;
		String out;
		for(int i = 0; i < 4000; i++)
			co & [=, &out] { Sleep(1); INTERLOCKED { out << i << ", "; }};
		co.Cancel();
		co.Finish();
		co.Finish();
		DUMP(out);
	}

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
