#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	double h = 0;
	{
		RTIMING("sin");
		for(double x = 0; x < 1000000; x += 0.1)
			h += sin(x);
	}
	{
		RTIMING("asin");
		for(double x = 0; x < 1000000; x += 0.1)
			h += asin(x);
	}
	{
		RTIMING("acos");
		for(double x = 0; x < 1000000; x += 0.1)
			h += acos(x);
	}
	{
		RTIMING("add");
		for(double x = 0; x < 1000000; x += 0.1)
			h += x;
	}
	{
		RTIMING("mul");
		for(double x = 0; x < 1000000; x += 0.1)
			h *= x;
	}
	{
		RTIMING("sqrt");
		for(double x = 0; x < 1000000; x += 0.1)
			h += sqrt(x);
	}
	RDUMP(h);
}

