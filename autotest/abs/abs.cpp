#include <Core/Core.h>

using namespace Upp;

bool FltMatch(double x, double y)
{
	return abs(x - y) < min(abs(x), abs(y)) / 1e5;
}

bool DblMatch(double x, double y)
{
	return abs(x - y) < min(abs(x), abs(y)) / 1e10;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	int n = -1;
	ASSERT(abs(n) == 1);
	
	ASSERT(abs(0) == 0);
	ASSERT(abs(1) == 1);
	ASSERT(abs(-1) == 1);
	ASSERT(abs(-0x40000000) == 0x40000000);
	ASSERT(abs((int16)-0x4000) == 0x4000);
	ASSERT(abs((int64)-0x400000000000) == 0x400000000000);
	ASSERT(abs((int8)-0x40) == 0x40);
	ASSERT(FltMatch(abs((float)-1.1), 1.1));
	ASSERT(DblMatch(abs((double)-1.1), 1.1));
	
	ASSERT(abs(Complex(3, -4)) == 5);

	LOG("=============== OK");
}
