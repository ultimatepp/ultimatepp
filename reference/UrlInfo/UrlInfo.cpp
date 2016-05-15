#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	UrlInfo f("http://username:password@hostname:9090/path?arg=value&aarg[]=item1&aarg[]=item2#anchor");

	DUMP(f.url);
	DUMP(f.scheme);
	DUMP(f.host);
	DUMP(f.port);
	DUMP(f.username);
	DUMP(f.password);
	DUMP(f.path);
	DUMP(f.query);
	DUMP(f.fragment);
	DUMP(f.parameters);
	DUMP(f.array_parameters);
}
