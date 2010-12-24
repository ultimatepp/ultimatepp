#include <Core/Core.h>

using namespace Upp;

void Test(const char *text)
{
	Time tm;
	const char *q = StrToTime(tm, text);
	if(q)
		LOG(text << " -> " << tm << ", follows: " << q);
	else
		LOG("FAILED: " << text);
}

CONSOLE_APP_MAIN
{
	SetLanguage(LNG_CZECH);
	Test("15.12.2005 23:10:20 hello!");
	Test("15.12.2005 hello!");
	Test("15.12.2005 12:10 foo!");
	Test("15.12.2005 12 foo!");
	Test("15.12.2005 foo!");
	Test("uck!");
}

