#include <Core/Core.h>

#include "shared.h"

using namespace Upp;

void cuda_mul_a(float a, float *x, int count);

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<float> x;
	for(float i = 0; i < 10; i++)
		x << i;
	DDUMP(x);
	cuda_mul_a(10, x.begin(), x.GetCount());
	DDUMP(x);
	
	x.Clear();
	for(int i = 0; i < 123456780; i++)
		x << (float)i;
	auto xx = clone(x);
	for(float& m : xx)
		m *= 13;
	cuda_mul_a(13, x.begin(), x.GetCount());
	ASSERT(x == xx);
}
