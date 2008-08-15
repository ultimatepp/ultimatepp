#include <Core/Core.h>

#ifdef _DEBUG
#define N 1000
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
		mutex.Enter();
		queue.AddHead() = new int;
		mutex.Leave();
		todo.Release();
	}
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
			delete ptr;
		}
	}
}

CONSOLE_APP_MAIN {
	Thread producer[THREADS];
	Thread consumer[THREADS];
	for(int i = 0; i < THREADS; i++) {
		producer[i].Run(callback(ProducerThread));
		consumer[i].Run(callback(ConsumerThread));
	}
	for(int i = 0; i < THREADS; i++)
		producer[i].Wait();
	mutex.Enter();
	queue.AddHead(NULL);
	mutex.Leave();
	for(int i = 0; i < THREADS; i++)
		todo.Release();
	for(int i = 0; i < THREADS; i++)
		consumer[i].Wait();
}
