#include <Core/Core.h>

using namespace Upp;

#ifdef CPU_ARM
#define N 1000000
#else
#define N 10000000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < N; i++) {
		if(i % 1000000 == 0)
			LOG(i);
		String a('x', Random(10));
		String b('x', Random(10));
		a << FormatIntBase(Random(), 2) << FormatIntBase(Random(), 2) << FormatIntBase(Random(), 2);
		b << FormatIntBase(Random(), 2) << FormatIntBase(Random(), 2) << FormatIntBase(Random(), 2);
		
		int n = min(a.GetLength(), b.GetLength());
		
		ASSERT(sgn(inline_memcmp_aligned(~a, ~b, n)) == sgn(memcmp(~a, ~b, n)));

		ASSERT(sgn(a.Compare(b)) == sgn(strcmp(~a, ~b)));

		ASSERT(sgn(a.Compare(a)) == 0);
	}
	
	LOG("------------ OK");
}
