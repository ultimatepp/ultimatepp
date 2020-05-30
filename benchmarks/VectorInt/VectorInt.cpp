#include <Core/Core.h>

using namespace Upp;

#define N 1000000
#define SZ 100

CONSOLE_APP_MAIN
{
	RTIMING("Time");
	for(int i = 0; i < N; i++) {
		Vector<int> v;
		for(int n = 0; n < SZ; n++)
			v.Add(n);
	}
}
