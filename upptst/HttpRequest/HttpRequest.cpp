#include <Core/Core.h>

using namespace Upp;

#define LLOG(x) RLOG(x)
#define LDUMP(x) RDUMP(x)

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	const Tuple2<const char *, const char *> x[] = {
		{ "www.ultimatepp.org", "</script></BODY>" },
		{ "www.idnes.cz", "</html>" },
		{ "www.google.com", "</script>" },
		{ "http://wattsupwiththat.com/", "</html>" },
		{ "http://www.rcalbum.com", "</html>" },
		{ "rcmania.cz", "</html>" },
	};
	for(int i = 0; i < __countof(x); i++) {
		LLOG("=============================================");
		LLOG("URL: " << x[i].a);
		HttpRequest h(x[i].a);
		ASSERT(!IsNull(h.Execute()));
		if((~h).Find(x[i].b) < 0) {
			LLOG("Content:\n" << ~h);
			NEVER();
		}
	}
	{
		HttpRequest h("www.idnes.cz");
		h.MaxContentSize(10000);
		h.Execute();
		ASSERT(h.IsError());
		LDUMP(h.GetError());
	}
	LLOG("*********** Everything is OK");
}
