#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int pass = 0; pass < 3; pass++) {
		ValueMap a;
		a.Add(1, "1");
		a.Add(2, "2");
		
		ValueMap b;
		b.Add(2, "2");
		b.Add(1, "1");
		if(pass == 1)
			b.Add(2, 2);
		
		ValueArray va1;
		va1.Add(1);
		va1.Add(a);

		ValueArray va2;
		va2.Add(pass == 2 ? 2 : 1);
		va2.Add(b);
	
		Value v1 = va1;
		Value v2 = va2;
		
		LOG("--------------- " << pass);
		DUMP(v1);
		DUMP(v2);
		DUMP(v1 == v2);
		DUMP(v2 == v1);
		DUMP(v1.IsSame(v2));
		DUMP(v2.IsSame(v1));
		
		ASSERT(v1 != v2);
		ASSERT(v2 != v1);
		ASSERT(v1.IsSame(v2) == !pass);
		ASSERT(v2.IsSame(v1) == !pass);

		LOG("---");

		va2.Add(va1);
		v2 = va2;
		DUMP(v2);
		DUMP(v1.IsSame(v2));
		DUMP(v2.IsSame(v1));
		
		ASSERT(!v1.IsSame(v2));
		ASSERT(!v2.IsSame(v1));
	}
	
	LOG("========== OK");
}
