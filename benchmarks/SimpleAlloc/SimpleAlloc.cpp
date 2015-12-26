#include <Core/Core.h>

using namespace Upp;

#define N     50000000  // number of "fast growth" allocations
#define NMIX  100000000 // number of random allocations (per thread)
#define MIX   300       // number of allocation 'objects'
#define NPASS 10000000  // number of allocations to be 'passed' between threads

void *ptr[N];
int   len[NMIX];

void MixTest()
{
	void *mix[MIX];
	for(int i = 0; i < MIX; i++)
		mix[i] = NULL;
	for(int i = 0; i < NMIX; i++) {
		int ii = i % MIX;
		if(mix[ii])
			delete[] mix[ii];
		mix[ii] = new byte[len[i]];
	}
	for(int i = 0; i < MIX; i++)
		delete[] mix[i];
}

Mutex         mtx;
bool          finished;
Vector<int *> pass;
int           sum;
int           count;

void Consumer()
{
	for(;;) {
		Mutex::Lock __(mtx);
		for(int i = 0; i < pass.GetCount(); i++) {
			if(!pass[i]) {
				pass.Clear();
				return;
			}
			count++;
			sum += *pass[i];
			delete[] pass[i];
		}
		pass.Clear();
	}
}


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
#if 1
	if(0) {
		TimeStop tm;
		for(int i = 0; i < N; i++)
			ptr[i] = new byte[32];
		RLOG("Alloc " << tm);
	}
	if(0) {
		TimeStop tm;
		for(int i = 0; i < N; i++)
			delete[] ptr[i];
		RLOG("Free " << tm);
	}
	if(1) {
		for(auto m : { 50, 200, 500, 5000 }) {
			for(int i = 0; i < NMIX; i++)
				len[i] = Random(m);
			for(auto n : { 1, 2, 3, 4, 8, 16 }) {
				TimeStop tm;
				Buffer<Thread> t(n);
				for(int i = 0; i < n; i++)
					t[i].Run(callback(MixTest));
				for(int i = 0; i < n; i++)
					t[i].Wait();
				RLOG(n << " threads Mix " << m << ": " << tm);
			}
		}
	}
#endif
	if(1) {
		TimeStop tm;
		for(int i = 0; i < NPASS; i++) {
			for(int i = 0; i < 10; i++) {
				int *p = new int;
				pass.Add(p);
				*p = i;
			}
			for(int i = 0; i < pass.GetCount(); i++) {
				sum += *pass[i];
				delete[] pass[i];
			}
			pass.Clear();
		}
		RLOG("Pass Ref " << tm);
		RDUMP(sum);
	}
	if(1) {
		sum = 0;
		TimeStop tm;
		Thread t;
		t.Run(callback(Consumer));
		for(int i = 0; i < NPASS; i++) {
			Mutex::Lock __(mtx);
			for(int i = 0; i < 10; i++) {
				int *p = new int;
				pass.Add(p);
				*p = i;
			}
		}
		{
			Mutex::Lock __(mtx);
			pass.Add(NULL);
		}
		t.Wait();
		RLOG("Pass " << tm);
		RDUMP(sum);
		RDUMP(count);
		RDUMP(MemoryUsedKb());
	}
}
