#include <Core/Core.h>

class Mutex {
	int    count;
	void  *threadid;
	
	void Enter() {
		if(self_thread == threadid) {
			count++;
			return;
		}
		Enter...
		threadid = self_thread
		count = 1;
	}
	
	void Leave() {
		if(--count != 0) return;
		Leave...
	}
};

CONSOLE_APP_MAIN
{
	
}
