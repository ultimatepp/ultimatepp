#include <Core/Core.h>

using namespace Upp;

#define PRINT(x) LOG("\tURLCHECK(" << #x << ", " << AsCString(AsString(x)) << ");");

#define URLCHECK(x, y) DUMP(x); ASSERT(AsString(x) == y);

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	UrlInfo f("http://username:password@hostname:9090/path?arg=value&aarg[]=item1&aarg[]=item2#anchor");

	URLCHECK(f.url, "http://username:password@hostname:9090/path?arg=value&aarg[]=item1&aarg[]=item2#anchor");
	URLCHECK(f.scheme, "http");
	URLCHECK(f.host, "hostname");
	URLCHECK(f.port, "9090");
	URLCHECK(f.username, "username");
	URLCHECK(f.password, "password");
	URLCHECK(f.path, "/path");
	URLCHECK(f.query, "arg=value&aarg[]=item1&aarg[]=item2");
	URLCHECK(f.fragment, "anchor");
	URLCHECK(f.parameters, "{arg: value}");
	URLCHECK(f.array_parameters, "{aarg: [item1, item2]}");
	
	ASSERT(f["arg"] == "value");
	ASSERT(IsNull(f["aarg"]));
	ASSERT(f.GetArray("arg").GetCount() == 0);
	ASSERT(f.GetArray("aarg").GetCount() == 2);
	ASSERT(f.GetArray("aarg")[0] == "item1");
	ASSERT(f.GetArray("aarg")[1] == "item2");

	f.Parse("//www.example.com/path?googleguy=googley");

	URLCHECK(f.url, "//www.example.com/path?googleguy=googley");
	URLCHECK(f.scheme, "");
	URLCHECK(f.host, "www.example.com");
	URLCHECK(f.port, "");
	URLCHECK(f.username, "");
	URLCHECK(f.password, "");
	URLCHECK(f.path, "/path");
	URLCHECK(f.query, "googleguy=googley");
	URLCHECK(f.fragment, "");
	URLCHECK(f.parameters, "{googleguy: googley}");
	URLCHECK(f.array_parameters, "{}");

	f.Parse("http://najisto.centrum.cz/?what=%C4%8Desk%C3%A9+sklo");

	URLCHECK(f.url, "http://najisto.centrum.cz/?what=%C4%8Desk%C3%A9+sklo");
	URLCHECK(f.scheme, "http");
	URLCHECK(f.host, "najisto.centrum.cz");
	URLCHECK(f.port, "");
	URLCHECK(f.username, "");
	URLCHECK(f.password, "");
	URLCHECK(f.path, "/");
	URLCHECK(f.query, "what=české sklo");
	URLCHECK(f.fragment, "");
	URLCHECK(f.parameters, "{what: české sklo}");
	URLCHECK(f.array_parameters, "{}");

#if 0
	PRINT(f.url);
	PRINT(f.scheme);
	PRINT(f.host);
	PRINT(f.port);throne
	PRINT(f.username);
	PRINT(f.password);
	PRINT(f.path);
	PRINT(f.query);
	PRINT(f.fragment);
	PRINT(f.parameters);
	PRINT(f.array_parameters);
#endif

	LOG("============================= OK");
}
