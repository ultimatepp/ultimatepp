//////////////////////////////////////////////////////////////////////
// wisapicfg: HTML-based configuration dialog.

#ifndef __tinet_wisapi_wisapicfg__
#define __tinet_wisapi_wisapicfg__

#include <tcore/all.h>
#include <tcore/event.h>
#include <tinet/html.h>
#include <tinet/http.h>

WID(NEW_HOST)
WID(STAT)
WID(HOSTCOUNT)
WID(ADDHOST)
WID(SAVE_CONFIG)
WID(CONFIGURE)

class IsapiCfg
{
public:
	IsapiCfg(IsapiClient& client, const char *url);

	bool         Run(String& header, String& content, String& error);

private:
	Htmls        Body();
	Htmls        Configuration();
	Htmls        Login();
	Htmls        Timing();
	HtmlTag      Edit(Id id, int& value);
	Htmls        Servers();
	Htmls        Commit();

	Htmls        Statistics();

	Htmls        Services();
	Htmls        ServiceTable();

	bool         CanWrite();

private:
	IsapiClient& client;
	WebMap       map;
	String       default_host;
	String       error;
	String       config_url;
	int          can_write;
};

#endif//__tinet_wisapi_wisapicfg__
