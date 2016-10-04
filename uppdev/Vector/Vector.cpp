#include <Core/Core.h>

using namespace Upp;

Vector<int> Test()
{
	Vector<int> x;
	x << 1 << 2 << 3;
	return pick(x);
}

CONSOLE_APP_MAIN
{
	Vector<String> test;
	test.Add("1");
	test.Add("2");
	test.Add("3");
	test.Add("5");
	test.Add("4");
	DUMPC(test);
	
	Vector<String> b = pick(test);
	
	for(auto x: b)
		DLOG(x);
	
	DDUMP(__cplusplus);
	
#ifdef CPP_11
	LOG("C++ 11 active");
#endif
}
