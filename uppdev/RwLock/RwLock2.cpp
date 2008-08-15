#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	{
		RWMutex l;
		RTIMING("RWMutex");
		for(int i = 0; i < 10000000; i++) {
			l.EnterRead();
			l.LeaveRead();
		}
	}
	{
		Mutex  m;
		RTIMING("Mutex");
		for(int i = 0; i < 10000000; i++) {
			m.Enter();
			m.Leave();
		}
	}
	{
		Mutex  m;
		RTIMING("Mutex 2x");
		for(int i = 0; i < 10000000; i++) {
			m.Enter();
			m.Enter();
			m.Leave();
			m.Leave();
		}
	}
	{
		Mutex  m;
		RTIMING("Mutex 3x");
		for(int i = 0; i < 10000000; i++) {
			m.Enter();
			m.Enter();
			m.Enter();
			m.Leave();
			m.Leave();
			m.Leave();
		}
	}
	{
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		RTIMING("PTHREAD_MUTEX_INITIALIZER");
		for(int i = 0; i < 10000000; i++) {
			pthread_mutex_lock(&mutex);
			pthread_mutex_unlock(&mutex);
		}
	}
	{
		pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
		RTIMING("PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP");
		for(int i = 0; i < 10000000; i++) {
			pthread_mutex_lock(&mutex);
			pthread_mutex_unlock(&mutex);
		}
	}
}
