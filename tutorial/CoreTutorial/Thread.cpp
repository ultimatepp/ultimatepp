#include "Tutorial.h"

void ThreadTutorial()
{
	/// .Thread
	
	Thread t;
	t.Run([] {
		for(int i = 0; i < 10; i++) {
			LOG("In the thread " << i);
			Sleep(100);
		}
		LOG("Thread is ending...");
	});
	for(int i = 0; i < 5; i++) {
		LOG("In the main thread " << i);
		Sleep(100);
	}
	LOG("About to wait for thread to finish");
	t.Wait();
	LOG("Wait for thread done");
	
	///
}