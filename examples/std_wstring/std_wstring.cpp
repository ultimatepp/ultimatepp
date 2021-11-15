#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DDUMP(sizeof(wchar_t));
	
	WString x;
	x.Cat(48);
	x.Cat(48000);
	x.Cat(70000);
	
	std::wstring w = x;
	
	DDUMP(w.size());
	
	WString y = w;
	
	ASSERT(x == y);
	
	DLOG("============= OK");
}
