#include <Core/Core.h>

using namespace Upp;

void TestFn(int a, int b, int c)
{
	LOG("TestFn" << a << b << c);
}

struct Test : Pte<Test> {
	void TestMethod(int a, int b, int c) {
		LOG("TestMethod" << a << b << c);
	}
	
	Callback cb1;
	Callback cb2;
	Callback cb3;
	
	void Perform()
	{
		cb1();
		cb2();
		cb3();
	}

	typedef Test CLASSNAME;

	Test() {
		cb1 = THISBACK3(TestMethod, 1, 2, 3);
		cb2 = PTEBACK3(TestMethod, 3, 2, 1);
		cb3 = callback3(TestFn, 4, 5, 6);
	}
};

CONSOLE_APP_MAIN
{
	Test().Perform();
}
