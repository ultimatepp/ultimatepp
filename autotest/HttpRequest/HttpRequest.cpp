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
		{ "www.oexchange.org", "" },
		{ "http://pagead2.googlesyndication.com/pagead/show_ads.js", "" },
		{ "www.ultimatepp.org", "</script></BODY>" },
		{ "www.idnes.cz", "</html>" },
		{ "www.google.com", "</script>" },
		{ "http://wattsupwiththat.com/", "</html>" },
		{ "http://www.rcalbum.cz", "</html>" },
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
	if(GetSysDate() > Date(2018, 10, 1)) { // Because "Resource temporarily unavailable", but let us recheck later
		HttpRequest h("http://dev.alt.cloudappsportal.com/_api/web/lists");
		h.KeepAlive();
		h.Execute();
		ASSERT(h.GetStatusCode() == 401);
		DUMP(h.GetReasonPhrase());
		DUMP(h.GetContent());
		ASSERT(h.GetContent().EndsWith(".</m:message></m:error>"));
	}
	{
		HttpRequest r("http://httpbin.org/basic-auth/user/passw0rd");
		r.User("user", "passw0rd");
		String h = r.Execute();
		DUMP(h);
		ASSERT(h.Find("\"authenticated\": true") >= 0);
	}
	{
		HttpRequest r("http://httpbin.org/digest-auth/auth/usr/pwd");
		r.Digest("usr", "pwd");
		String h = r.Execute();
		DUMP(r.GetContent());
		ASSERT(h.Find("\"authenticated\": true") >= 0);
	}
	{
		HttpRequest r("https://httpbin.org/bytes/0");
		ASSERT(r.Execute().GetCount() == 0);
		ASSERT(r.IsSuccess());
	}
	
	{
		HttpRequest h("https://journeyman.cz/");
		String s = h.Execute();
		DUMP(h.GetReasonPhrase());
		DUMP(h.GetContent());
		ASSERT(s.Find("<script type=\"text/javascript\" src=\"//c.imedia.cz/js/retargeting.js\"></script>") >= 0);
	}

	{
		HttpRequest h("https://journeyman.cz/");
		h.Timeout(0);
		String s = h.Execute();
		DUMP(h.GetReasonPhrase());
		DUMP(h.GetContent());
		ASSERT(s.Find("<script type=\"text/javascript\" src=\"//c.imedia.cz/js/retargeting.js\"></script>") >= 0);
	}

	LLOG("*********** Everything is OK");
}
