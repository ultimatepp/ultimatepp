#include <Core/Core.h>

using namespace Upp;

namespace Config {

INI_STRING(text, "default text", "Text parameter");

};

struct Test {
	int x;
	String a;
};

Test y = { 123, "123" };

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	SetIniFile(GetDataFile("test.ini"));
	
	for(;;) {
		LOG(Config::text);
		LOG(GetIniInfoFormatted());
		getchar();
		SetIniFile(GetDataFile("test.ini"));
	}
}
