#include <Core/Core.h>

using namespace Upp;

#define LLOG(x) RLOG(x)
#define LDUMP(x) RDUMP(x)

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	HttpRequest::Trace();
	const Tuple2<const char *, const char *> x[] = {
		{ "https://www.servis24.cz", "" },
		{ "rcmania.cz", "</html>" },
	//	{ "http://www.facebook.com/pages/Upp", "" },
		{ "www.oexchange.org", "" },
		{ "http://pagead2.googlesyndication.com/pagead/show_ads.js", "" },
		{ "www.ultimatepp.org", "</script></BODY>" },
		{ "www.idnes.cz", "</html>" },
		{ "www.google.com", "</script>" },
		{ "http://wattsupwiththat.com/", "</html>" },
		{ "http://www.rcalbum.com", "</html>" },
		{ "www.cekas.cz", "<meta http-equiv" },
	};
	for(int nd = 0; nd < 1; nd++)
		for(int i = 0; i < __countof(x); i++) {
			LLOG("=============================================");
			LLOG("URL: " << x[i].a);
			HttpRequest h(x[i].a);
			if(nd)
				h.Timeout(0);
			if(IsNull(h.Execute())) {
				DUMP(~h);
				LLOG("Error:\n" << h.GetErrorDesc());
				NEVER();
			}
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
