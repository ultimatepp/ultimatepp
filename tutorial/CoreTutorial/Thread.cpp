#include "Tutorial.h"

void ThreadTutorial()
{
	/// .`Thread`
	
	/// Since C++11, there is now a reasonable support for threads in standard library.
	/// There are however reasons to use U++ threads instead. One of them is that U++ high
	/// performance memory allocator needs a cleanup call at the the thread exit, which is
	/// naturally implemented into `Upp::Thread`. Second 'hard' reason is that Microsoft
	/// compiler is using Win32 API function for condition variable that are not available for
	/// Windows XP, while U++ has alternative implementation for Windows XP, thus making
	/// executable compatible with it.
	
	/// Then of course we believe U++ multithreading / parallel programming support is easier
	/// to use and leads to higher performance...
	
	/// `Thread` class can start the thread and allows launching thread to `Wait` for its
	/// completion:
	
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
	
	/// `Thread` destructor calls `Detach` method with 'disconnects' `Thread` from the thread.
	/// Thread continues running.
	
	/// `Thread::Start` static method launches a thread without possibility to wait for its
	/// completion; if you need to wait, you have to use some other method:
	
	bool x = false;
	
	Thread::Start([&x] { LOG("In the Started thread"); x = true; });
	
	LOG("About to wait for thread to finish");
	while(!x) { Sleep(1); } // Do not do this in real code!
	LOG("Wait for thread done");
	
	/// (method used here is horrible, but should demonstrate the point).
}
