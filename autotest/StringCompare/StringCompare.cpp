#include <Core/Core.h>

#define LDUMP(x)
#define LLOGHEXDUMP(a, b)

using namespace Upp;

#define N 10000000

CONSOLE_APP_MAIN
{
	for(int q = 0; q < N; q++) {
		LDUMP(q);
		if(q % 10000 == 0)
			Cout() << q << '\n';
		int eq = rand() % 40;
		int l1 = rand() % 40;
		int l2 = rand() % 40;
		String a, b;
		for(int i = 0; i < eq; i++)
			a.Cat(rand() % 254 + 1);
		b = a;
		ASSERT(a.Compare(b) == 0);
		for(int i = 0; i < l1; i++)
			a.Cat(rand() % 254 + 1);
		for(int i = 0; i < l2; i++)
			b.Cat(rand() % 254 + 1);
		int l = min(l1, l2) + eq;
		LLOGHEXDUMP(a, a.GetLength());
		LLOGHEXDUMP(b, b.GetLength());
		LDUMP(a.Compare(b));
		LDUMP(strcmp(a, b));
		LDUMP(strcmp(~a, ~b));
		ASSERT(sgn(strcmp(a, b)) == sgn(a.Compare(b)));
		ASSERT(a.Compare(a) == 0);
		ASSERT(b.Compare(b) == 0);
	}
}
