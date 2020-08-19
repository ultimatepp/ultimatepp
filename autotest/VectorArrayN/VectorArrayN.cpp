#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	{ Vector<int> x(20); DUMP(x.GetCount()); }
	{ Vector<int> x(20, 123); DUMP(x); }
	{ Vector<String> x(20, "123"); DUMP(x); }

	{ Array<int> x(20); DUMP(x.GetCount()); }
	{ Array<int> x(20, 123); DUMP(x); }
	{ Array<String> x(20, "123"); DUMP(x); }

	CheckLogEtalon();
}
