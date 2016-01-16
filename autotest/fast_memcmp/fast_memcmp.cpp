#include <Core/Core.h>

using namespace Upp;

#define N 100000000

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < N; i++) {
		if(i % 1000000 == 0)
			LOG(i);
		String a('x', Random(10));
		String b('x', Random(10));
		a << AsString(Random());
		b << AsString(Random());
		
		int n = min(a.GetLength(), b.GetLength());
		
		ASSERT(sgn(fast_memcmp((byte *)~a, (byte *)~b, n)) == sgn(memcmp((byte *)~a, (byte *)~b, n)));
	}
	
	LOG("------------ OK");
}
