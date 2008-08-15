#include <Core/Core.h>

CONSOLE_APP_MAIN
{
	String q = "Ahoj kamarade";
	WString w;
	{
		TimeStop s;
		for(int i = 0; i < 10000000; i++)
			w = ToUnicode(q, CHARSET_WIN1250);
		RLOG("normal: " << s);
	}
	{
		TimeStop s;
		for(int i = 0; i < 10000000; i++)
			w = ToUnicode(q, CHARSET_UTF8);
		RLOG("utf8: " << s);
	}

}
