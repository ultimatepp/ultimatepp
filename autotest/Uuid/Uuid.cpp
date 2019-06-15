#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Uuid id;
	id.New();
	DDUMP(id);
	String s = AsString(id);
	DDUMP(s);
	Uuid id2;
	id2 = ScanUuid(s);
	DDUMP(id2);
	ASSERT(id2 == id);
	id.New();
	DDUMP(id);
	s = FormatWithDashes(id);
	DDUMP(s);
	id2 = ScanUuid(s);
	ASSERT(id2 == id);
	LOG("=================== OK");
}
