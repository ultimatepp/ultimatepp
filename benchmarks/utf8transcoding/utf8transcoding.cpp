#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	WString x;
	for(int i = 0; i < 1000000; i++)
		x.Cat(Random(20000));
	
	String utf8 = x.ToString();
	
	for(int i = 0; i < 100; i++) {
		RTIMING("conversion");
		utf8.ToWString();
	}
}
