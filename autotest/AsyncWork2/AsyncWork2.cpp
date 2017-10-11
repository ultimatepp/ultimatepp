#include <Core/Core.h>

using namespace Upp;

Atomic h;

void Wait(int c)
{
	while(c--)
		h++;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < 100000; i++) {
		if(i % 1000 == 0)
			LOG(i);
		bool b = false;
		auto x = Async([] { Wait(Random(3000)); throw "error"; });
		try {
			Wait(Random(3000));
			x.Get();
		}
		catch(...) {
			b = true;
		}
		ASSERT(b);
	}
	
	LOG("============ OK");
}
