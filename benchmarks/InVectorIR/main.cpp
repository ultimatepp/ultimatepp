#include <Core/Core.h>

using namespace Upp;

template <class T>
void Benchmark(int n)
{
	T o;
	for(int i = 0; i < n; i++)
		o.Add(AsString(i));
	String h = "0";
	TimeStop tm;
	for(int i = 0; i < 100; i++) {
		for(int j = 0; j < 10000; j++)
			o.Insert(0, h);
		o.Remove(0, 10000);
	}
	RLOG(n << ": " << tm.Elapsed() << " ms");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	RLOG("-----------------------------------");
	RLOG("InVector: ");
	for(int i = 1000; i <= 1000000; i *= 10) {
		Benchmark<InVector<String> >(i);
		Benchmark<InVector<String> >(2 * i);
		Benchmark<InVector<String> >(5 * i);
	}
	RLOG("-----------------------------------");
	RLOG("Vector: ");
	for(int i = 1000; i <= 10000; i *= 10) {
		Benchmark<Vector<String> >(i);
		Benchmark<Vector<String> >(2 * i);
		Benchmark<Vector<String> >(5 * i);
	}
}