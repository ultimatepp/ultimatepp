#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	// this is just compilation check:
		
	Semaphore s;
	s.Wait(1000);
	
	Mutex m;
	ConditionVariable cv;
	cv.Wait(m, 1000);
}
