#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<int> h = { 1, 2, 3 };
	
	WithDeepCopy<Vector<int>> h2, h3;
	
	h2 = h;
	DUMP(h2);
	ASSERT(h2.GetCount() == 3);
	ASSERT(h2 == h);
	
	h2 = pick(h);
	DUMP(h2);
	ASSERT(h2.GetCount() == 3);
	ASSERT(h.GetCount() == 0);
	
	h3 = h2;
	DUMP(h3);
	ASSERT(h3.GetCount() == 3);
	ASSERT(h2 == h3);
	
	h2 = pick(h3);
	DUMP(h2);
	ASSERT(h2.GetCount() == 3);
	ASSERT(h3.GetCount() == 0);
	
	LOG("=================== OK");
}
