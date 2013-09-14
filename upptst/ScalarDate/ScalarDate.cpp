#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Date d;
	
	d.Set(Null);
	ASSERT(IsNull(d));
	
	d.Set(0);
	ASSERT(d == Date(0, 1, 1));
	
	for(int i = 0; i < 800000; i++) {
		d.Set(i);
		Date d1 = d;
		d.Set(d.Get());
		ASSERT(d1 == d);
	}
}
