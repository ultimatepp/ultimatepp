#include <Core/Core.h>

using namespace Upp;

#define LLOG(x)   LOG(x)
#define LDUMP(x)  DUMP(x)

//#define NEL  1024 * 1024
//#define STEP 2048

#define NEL  128 * 1024
#define STEP 16

double array[NEL];

struct Test {
	typedef Test CLASSNAME;


	void Single() {
		for(int i = 0; i < NEL; i++) {
			array[i] *= 1.000001;
			array[i] = sqrt(array[i]);
		}
	}

	void Job(int x, int n) {
		LLOG(x << ", " << n);
		for(int i = 0; i < 1000; i++)
			for(int i = 0; i < n; i++) {
				array[x + i] *= 1.000001;
				array[x + i] = sqrt(array[x + i]);
			}
	}

	void Multi() {
		LLOG("=======================================================================");
		{
			CoWork work;
			for(int i = 0; i < NEL; i += STEP)
				work.Do(THISBACK2(Job, i, STEP));
		}
		LLOG(".......................................................................");
//		MtBatch::Quit();
	}
};

#ifdef _DEBUG
#define N 10000
#else
#define N 500
#endif

void *Simple(void *)
{
	double x = 1.23;
	for(;;)
		x = sqrt(x);
	return NULL;
}

CONSOLE_APP_MAIN
{
/*	pthread_t h;
	pthread_create(&h, 0, Simple, NULL);
	Simple(NULL);
	return;*/
	Test x;
	for(int i = 0; i < NEL; i++)
		array[i] = 0.0000001;
/*	Cout() << "Single:\n";
	{
		RTIMING("Single");
		for(int i = 0; i < N; i++)
			x.Single();
	}*/
	Cout() << "Multi:\n";
	x.Multi();
	{
		RTIMING("Multi");
		for(int i = 0; i < N; i++) {
			LLOG("RUN " << i);
			x.Multi();
		}
	}
	Cout() << "Finished.\n";
}
