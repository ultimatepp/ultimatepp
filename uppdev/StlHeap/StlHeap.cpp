#include <Core/Core.h>
#include <iostream>

using namespace Upp;

class test
{
public:
	typedef test CLASSNAME;
	void loop();
	void start();	
};

void test::start()
{
	Thread().Start(THISBACK(loop));
}

void test::loop()
{
	std::string str;
	while(true)
	{
		Thread().Sleep(200);
		str.append("test");
		std::cout << "loop" << std::endl;
	}
}

CONSOLE_APP_MAIN
{
	(new test)->start();
	
	while(true)
	{
		Thread().Sleep(500);
		std::cout << "Wait..." << std::endl;
	}
}
