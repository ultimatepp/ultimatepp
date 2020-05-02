#include <Core/Core.h>

#ifdef CPU_ARM
#define N 1000 * 5
#else
#define N 1000 * 100
#endif

#define THREADS 5

using namespace Upp;

Mutex           mutex;
Semaphore       todo;
BiVector<int *> queue;

void ProducerThread()
{
	for(int i = 0; i < N; i++) {
		if(i % 1000 == 0)
			LOG(i);
		mutex.Enter();
		queue.AddHead() = new int[Random(20) == 0 ? Random(100000) : Random(500)];
		mutex.Leave();
		todo.Release();
		MemoryCheckDebug();
	}
	LOG("Producer shutdown");
}

void ConsumerThread()
{
	for(;;) {
		todo.Wait();
		Mutex::Lock __(mutex);
		if(queue.GetCount()) {
			int *ptr = queue.Tail();
			if(!ptr) break;
			queue.DropTail();
			delete[] ptr;
		}
		MemoryCheckDebug();
	}
	LOG("Consumer shutdown");
}

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT);
	Thread producer[THREADS];
	Thread consumer[THREADS];
	for(int i = 0; i < THREADS; i++) {
		producer[i].Run(callback(ProducerThread));
		consumer[i].Run(callback(ConsumerThread));
	}
	for(int i = 0; i < THREADS; i++) {
		producer[i].Wait();
		LOG("Producer #" << i << " terminated");
	}
	mutex.Enter();
	queue.AddHead(NULL);
	mutex.Leave();
	for(int i = 0; i < THREADS; i++)
		todo.Release();
	for(int i = 0; i < THREADS; i++) {
		consumer[i].Wait();
		LOG("Consumer #" << i << " terminated");
	}
	MemoryCheckDebug();
	LOG("App shutdown");
	LOG("Passed");
}
