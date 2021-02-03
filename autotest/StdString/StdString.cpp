#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	{
		String h = "Hello world";
		std::string sh = h.ToStd();
		DDUMP(sh);
		h = sh;
		DDUMP(h);
		Value v = sh;
		DDUMP(v);
		sh = v.ToStd();
		DDUMP(sh);
	}

	{
		WString h = "Hello world";
		std::wstring sh = h.ToStd();
		DDUMP(sh);
		h = sh;
		DDUMP(h);
		Value v = sh;
		DDUMP(v);
		sh = v.ToWStd();
		DDUMP(sh);
	}
	CheckLogEtalon();
}
