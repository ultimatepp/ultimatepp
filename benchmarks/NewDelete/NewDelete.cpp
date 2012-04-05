#include <Core/Core.h>

using namespace Upp;

class Test  {
public:
	Test (int c) {count = c;}
//	virtual ~Test() {  }
	int count;
}; 

CONSOLE_APP_MAIN
{
	RTIMING("NewDelete");
	for (int i=0; i<=100000000; i++) {
		Test *test = new Test(i);
		delete test;
	}
}

