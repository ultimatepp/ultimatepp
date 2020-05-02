#include <Core/Core.h>

#define LDUMP(x)          // DUMP(x)
#define LLOGHEXDUMP(a, b) // LOGHEXDUMP(a, b)

using namespace Upp;

#ifdef CPU_ARM
#define N 2000000
#else
#define N 10000000
#endif

int RandomChar()
{
	return '0' + Random(64);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String a = "1911s";
	String b = "1911–12 in Scottish football";
	ASSERT(a < b);

	for(int q = 0; q < N; q++) {
		LDUMP(q);
		if(q % 10000 == 0)
			Cout() << q << '\n';
		int eq = rand() % 40;
		int l1 = rand() % 40;
		int l2 = rand() % 40;
		String a, b;
		for(int i = 0; i < eq; i++)
			a.Cat(RandomChar());
		b = a;
		ASSERT(a.Compare(b) == 0);
		for(int i = 0; i < l1; i++)
			a.Cat(RandomChar());
		for(int i = 0; i < l2; i++)
			b.Cat(RandomChar());
		int l = min(l1, l2) + eq;
		LDUMP(a);
		LDUMP(b);
		LLOGHEXDUMP(a, a.GetLength());
		LLOGHEXDUMP(b, b.GetLength());
		LDUMP(a.Compare(b));
		LDUMP(strcmp(a, b));
		LDUMP(strcmp(~a, ~b));
		ASSERT(sgn(strcmp(a, b)) == sgn(a.Compare(b)));
		ASSERT(a.Compare(a) == 0);
		ASSERT(b.Compare(b) == 0);
	}

	LOG("------------ OK");
}
