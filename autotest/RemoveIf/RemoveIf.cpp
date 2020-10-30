#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<String> x { "1", "4", "5", "5", "7", "5", "2", "1", "7", "9" };
	
	DUMP(x);
	x.RemoveIf([&](int i) { return x[i] == "8"; });
	DUMP(x);
	x.RemoveIf([&](int i) { return x[i] == "9"; });
	DUMP(x);
	x.RemoveIf([&](int i) { return x[i] == "5"; });
	DUMP(x);
	x.RemoveIf([&](int i) { return true; });
	DUMP(x);
	
	CheckLogEtalon();
}
