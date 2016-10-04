#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Array< Vector<int> > a;
	a.Add() << 1 << 2 << 3;
	
	auto b = clone(a);
	
	DUMPCC(a);
	DUMPCC(b);
	
	Vector<int> h;
	h.Insert(0, {1, 2, 3});
	h.Insert(1, {5, 4, 3});
	DDUMPC(h);
}
