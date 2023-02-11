#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Vector<int> a, b;
	int tm0 = msecs();
	for(int i = 0; i < 200000; i++) {
		if(i % 10000 == 0)
			LOG(i);
		int q = Random();
		a.Add(q);
		ASSERT(!IsEqualBySerialize(a, b));
		ASSERT(!IsEqualBySerialize(b, a));
		b.Add(Random());
		ASSERT(!IsEqualBySerialize(a, b));
		ASSERT(!IsEqualBySerialize(b, a));
		b.Top() = q;
		ASSERT(IsEqualBySerialize(a, b));
		ASSERT(IsEqualBySerialize(b, a));
	}
	DLOG("=========== OK in " << (tm0 - msecs()) / 1000.0 << " seconds");
}
