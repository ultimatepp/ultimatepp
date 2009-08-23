#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

//#include <Core/Core.h>

#ifndef TLS_PRE_ALLOC
#define TLS_PRE_ALLOC	false
#endif

#ifndef NUM_THREADS
#define NUM_THREADS	10
#endif

#ifndef NUM_TESTS
#define NUM_TESTS	1<<28
#endif

pthread_key_t p_key;

struct tls_ {
	int 	id;
	long 	check;
};

#if TLS_PRE_ALLOC
static tls_ tls[NUM_THREADS];
#endif

bool update_tls() {
	tls_ *tmp = (tls_*)pthread_getspecific(p_key);
	volatile long chk = tmp->check;
	tmp->check += 1;
	if (chk+1 == tmp->check)
		return true;
	else
		return false;
}

void* work(void* p_tls) {
	pthread_setspecific(p_key, p_tls);
	printf("Starting thread id: %d, v: %d\n", ((tls_*)pthread_getspecific(p_key))->id, ((tls_*)pthread_getspecific(p_key))->check);
	int test_nr = 0;
	do {
		if (!update_tls())
			return NULL;
	} while (NUM_TESTS < 0 || ++test_nr < NUM_TESTS);
	tls_ *tmp = (tls_*)pthread_getspecific(p_key);
	printf("Leaving thread %d value %d ...\n", tmp->id, tmp->check);
#if !TLS_PRE_ALLOC
	free(tmp);
	pthread_setspecific(p_key, NULL);
#endif
	return (void*)-1;
}

int main(int argc, const char *argv[])
{
	clock_t clk = clock();
	pthread_key_create(&p_key, NULL);
	pthread_t threads[NUM_THREADS];
	long ret;
	void* ret_ptr = &ret;
	int thread_id = 0;
	while (thread_id < NUM_THREADS) {
#if TLS_PRE_ALLOC
		printf("Thread specific data is pre allocated ...\n", thread_id);
		tls[thread_id].id = thread_id;
		tls[thread_id].check = thread_id;
		if (pthread_create(&threads[thread_id], NULL, work, &tls[thread_id]) != 0) {
			printf("Cannot create thread %d\n", thread_id);
			break;
		}
#else
		printf("Allocating thread specific data for thread %d ...\n", thread_id);
		tls_ *tls = new tls_;
		tls->id = thread_id;
		tls->check = thread_id;
		if (pthread_create(&threads[thread_id], NULL, work, tls) != 0) {
			printf("Cannot create thread %d\n", thread_id);
			break;
		}
#endif
		thread_id++;
	}
	for (int i=0; i < thread_id; i++) {
		pthread_join(threads[i], &ret_ptr);
		if (!ret_ptr)
			printf("thread %d TSS update has errors !!!\n", i);
	}
	pthread_key_delete(p_key);
	printf("CPU time: %f\n", (float)(clock()-clk)/CLOCKS_PER_SEC);
	return 0;
}

