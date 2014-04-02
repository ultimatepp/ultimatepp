#include <Core/Core.h>

using namespace Upp;

ConditionVariable cv;
Mutex             mtx;
int               count;
bool              texit;

void WorkThread()
{
	for(;;) {
		Mutex::Lock __(mtx);
		LOG("Before wait");
		if(texit) break;
		cv.Wait(mtx);
		LOG("After wait");
		if(texit) break;
		count++;
	}
	LOG("Exiting work thread");
}

CONSOLE_APP_MAIN
{
	Thread t[10];
	for(int i = 0; i < 10; i++)
		t[i].Run(callback(WorkThread));
	for(int i = 0; i < 1000; i++) {
		Mutex::Lock __(mtx);
		cv.Signal();
	}
	{
		Mutex::Lock __(mtx);
		texit = true;
	}	
	texit = true;
	cv.Broadcast();
	for(int i = 0; i < 10; i++)
		t[i].Wait();
	LOG(count);
	LOG("======== OK");
}
