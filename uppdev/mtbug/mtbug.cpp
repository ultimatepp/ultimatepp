#include <Core/Core.h>

using namespace Upp;

static const int ITEMS = 10000;
static const int PASSES = 5;
static const int THREADS = 4;

volatile Atomic items[ITEMS];

static void ThreadProc(volatile Atomic *item, bool *finish)
{
	AtomicInc(*item);
	*finish = true;
}

static void TryMain()
{
	for(int pass = 1; pass <= PASSES; pass++) {
		puts(NFormat("pass #%d", pass));
		fflush(stdout);
		for(int i = 0; i < ITEMS; i += THREADS) {
			if(i % (100 * THREADS) == 0) {
				puts(NFormat("i = %d", i));
				fflush(stdout);
			}
			bool thread_finished[THREADS];
			for(int t = 0; t < THREADS; t++) {
				if(i + t < ITEMS) {
					thread_finished[t] = false;
					Thread().Run(callback2(&ThreadProc, &items[i + t], &thread_finished[t]));
				}
				else
					thread_finished[t] = true;
			}
			for(;;) {
				bool done = true;
				for(int t = 0; t < THREADS; t++)
					if(!thread_finished[t]) {
						done = false;
						break;
					}
				if(done)
					break;
				Sleep(10);
			}
		}
		for(int i = 0; i < ITEMS; i++)
			if(items[i] != pass)
				RLOG("pass #" << pass << ": error at [" << i << "] = " << (int)items[i]);
	}
}

CONSOLE_APP_MAIN {
	TryMain();
}
