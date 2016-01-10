#include <OpenMP_demo/OpenMP_demo.h>

double timeIt(void (*fun)()) {
	double timeh = GetTickCount();
	fun();
	return (GetTickCount() - timeh)/1000.;
}