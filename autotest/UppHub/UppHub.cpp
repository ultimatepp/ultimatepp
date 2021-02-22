#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	HttpRequest r("https://raw.githubusercontent.com/ultimatepp/UppHub/main/nests.json");
		
	String s = r.Execute();
	
	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		v = ParseJSON(s);
	}
	
	ASSERT(!v.IsError());
	
	LOG("========== OK");
}
