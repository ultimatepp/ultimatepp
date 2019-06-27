#include <Core/Core.h>

int N;

#define THREADS 5

using namespace Upp;

Mutex           mutex;
Semaphore       todo;
BiVector<int *> queue;

void ProducerThread()
{
	LOG("Producer start");
	for(int i = 0; i < N; i++) {
		if(i % 1000 == 0)
			LOG(i);
		mutex.Enter();
		int *x = new int;
		*x = i;
		queue.AddHead() = x;
		mutex.Leave();
		todo.Release();
		MemoryCheckDebug();
	}
	LOG("Producer shutdown");
}

void ConsumerThread()
{
	LOG("Consumer start");
	for(;;) {
		todo.Wait();
		Mutex::Lock __(mutex);
		if(queue.GetCount()) {
			int *ptr = queue.Tail();
			if(!ptr) break;
			queue.DropTail();
			delete ptr;
		}
		MemoryCheckDebug();
	}
	LOG("Consumer shutdown");
	DUMP(queue.GetCount());
}

void DoTest()
{
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
	queue.Clear();
	LOG("Round shutdown");
	DUMP(MemoryUsedKb());
	DUMP(MemoryProfile());
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	N = 1000;
	
	for(int i = 0; i < 100; i++)
		DoTest();
	
	N = 100 * 1000;
	DoTest();
	LOG("----------------- OK");
}
