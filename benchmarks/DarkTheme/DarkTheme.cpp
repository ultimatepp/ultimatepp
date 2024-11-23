#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Color c = Color(100, 140, 200);
	for(int i = 0; i < 100000000; i++) {
		RTIMING("DarkTheme");
		DarkTheme(c);
	}
	for(int i = 0; i < 500000000; i++) {
		RTIMING("DarkThemeCached");
		DarkThemeCached(c);
	}
	Mutex m;
	for(int i = 0; i < 100000000; i++) {
		RTIMING("Mutex");
		Mutex::Lock __(m);
	}
}
