#include <Core/Core.h>

using namespace Upp;

#ifdef PLATFORM_POSIX
__thread int threadid;
#else
__declspec(thread) int threadid;
#endif

#define LLOG(x)  LOG((threadid) << " " << x << ", count " << count)

RWMutex          rwlock;
VectorMap<int, String> cache;

String Fn(int x)
{
	return AsString(sin(sqrt((double)x)));
}

void CheckResult(int x, const String& r)
{
	if(r != Fn(x)) {
		DUMP(r);
		DUMP(Fn(x));
		Panic("Failure! " + AsString(threadid));
	}
}

int writes, removes;

void WorkThread(int id)
{
	threadid = id;
	for(int i = 0; i < 1000000; i++) {
		if(i % 1000000 == 0)
			INTERLOCKED
				Cout() << id << ": " << i << ", writes: " << writes << ", removes: " << removes << "\n";
		int x = rand() & 0x7fff;
		rwlock.EnterRead();
		int q = cache.Find(x);
		if(q >= 0) {
			String r = cache[q];
			CheckResult(x, r);
	//		for(int i = 0; i < 100; i++)
	//			Fn(x);
			rwlock.LeaveRead();
		}
		else {
			rwlock.LeaveRead();
			rwlock.EnterWrite();
			RTIMING("Write!");
			q = cache.Find(x);
			if(q >= 0)
				CheckResult(x, cache[q]);
			else {
				writes++;
				if(cache.GetCount() >= 0x7ff0) {
					removes++;
					cache.Remove(0, 100);
				}
				cache.Add(x, Fn(x));
			}
			rwlock.LeaveWrite();
		}
	}
}

CONSOLE_APP_MAIN
{
	RTIMING("All");
	Thread t[20];
	for(int i = 0; i < 9; i++)
		t[i].Run(callback1(WorkThread, i + 1));
	WorkThread(0);
	for(int i = 0; i < 9; i++)
		t[i].Wait();
	RDUMP(writes);
	RDUMP(removes);
}
