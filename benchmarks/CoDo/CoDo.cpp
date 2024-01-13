#include <Core/Core.h>

using namespace Upp;

int m;
std::atomic<int> ii(0);

Function<void ()> x;

Mutex mtx;

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 100000; i++) {
		RTIMING("CoDo");
		CoDo([&] {});
	}
	for(int i = 0; i < 100000; i++) {
		RTIMING("Function");
		x = [&] {};
	}
	for(int i = 0; i < 1000000; i++) {
		RTIMING("atomic++");
		ii++;
	}
	for(int i = 0; i < 1000000; i++) {
		RTIMING("int++");
		m++;
	}
	for(int i = 0; i < 1000000; i++) {
		RTIMING("Mutex");
		Mutex::Lock __(mtx);
	}
}
