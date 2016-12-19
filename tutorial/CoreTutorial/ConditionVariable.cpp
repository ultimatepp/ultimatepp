#include "Tutorial.h"

void ConditionVariableTutorial()
{
	/// .`ConditionVariable`
	
	/// `ConditionVariable` in general is a synchronization primitive used to block/awaken the
	/// thread. `ConditionVariable` is associated with `Mutex` used to protect some data; in
	/// the thread that is to be blocked, `Mutex` has to locked; call to `Wait` atomically
	/// unlocks the `Mutex` and puts the thread to waiting. Another thread then can resume the
	/// thread by calling `Signal`, which also causes `Mutex` to lock again. Multiple threads
	/// can be waiting on single `ConditionVariable`; `Signal` resumes single waiting thread,
	/// `Brodcast` resumes all waitng threads.
	
	bool  stop = false;
	BiVector<int> data;
	Mutex m;
	ConditionVariable cv;
	
	Thread t;
	t.Run([&stop, &data, &m, &cv] {
		Mutex::Lock __(m);
		for(;;) {
			while(data.GetCount()) {
				int q = data.PopTail();
				LOG("Data received: " << q);
			}
			if(stop)
				break;
			cv.Wait(m);
		}
	});
	
	for(int i = 0; i < 10; i++) {
		{
			Mutex::Lock __(m);
			data.AddHead(i);
		}
		cv.Signal();
		Sleep(1);
	}
	stop = true;
	cv.Signal();
	t.Wait();
	
	/// Important note: rarely thread can be resumed from `Wait` even if no other called
	/// `Signal`. This is not a bug, but ^https://en.wikipedia.org/wiki/Spurious_wakeup:design
	/// decision for performance reason^. In practice it only means that situation has to be
	/// (re)checked after resume.
}