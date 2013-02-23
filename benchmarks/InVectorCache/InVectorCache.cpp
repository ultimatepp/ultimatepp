#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	int N = 10;
	InVector<int> x;
	for(int i = 0; i < 1000000; i++)
		x.Add(Random(1000));
	{
		RTIMING("Simple scan");
		int m = 0;
		for(int j = 0; j < N; j++) {
			for(int i = 0; i < x.GetCount(); i++)
				m += x[i];
			for(int i = x.GetCount(); --i >= 0;)
				m += x[i];
		}
		RDUMP(m);
	}
	{
		RTIMING("Single InVector, problematic scan");
		int m = 0;
		for(int j = 0; j < N; j++) {
			for(int i = 0; i < x.GetCount(); i++)
				m += x[i] + x[x.GetCount() - i - 1];
		}
		RDUMP(m);
	}
	{
		InVector<int> y;
		y <<= x;
		RTIMING("Two InVectors");
		int m = 0;
		for(int j = 0; j < N; j++) {
			for(int i = 0; i < x.GetCount(); i++)
				m += x[i] + y[i];
		}
		RDUMP(m);
	}
	{
		InVector<int> y;
		y <<= x;
		RTIMING("Two InVectors, two simple scans");
		int m = 0;
		for(int j = 0; j < N; j++) {
			for(int i = 0; i < x.GetCount(); i++)
				m += x[i];
			for(int i = y.GetCount(); --i >= 0;)
				m += y[i];
		}
		RDUMP(m);
	}
}
