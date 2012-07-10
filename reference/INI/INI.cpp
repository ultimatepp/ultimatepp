#include <Core/Core.h>

using namespace Upp;

namespace Config {

INI_BOOL(flag1, false, "This is bool parameter 1")
INI_BOOL(flag2, true, "This is bool parameter 2")

INI_STRING(text, "default text", "Text parameter");

INI_INT(number, 123456, "Number parameter");

INI_DOUBLE(fp_number, 0.1, "Floating point parameter");

};

extern void ExternalFn();

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	SetIniFile(GetDataFile("test.ini"));
	
	DDUMP(Config::flag1);
	DDUMP(Config::flag2);
	DDUMP(Config::text);
	DDUMP(Config::number);
	DDUMP(Config::fp_number);
	
	LOG(GetIniInfoFormatted());

	Config::number = 321;
	DDUMP(Config::number);
	
	LOG(GetIniInfoFormatted());
	
	ExternalFn();
}
