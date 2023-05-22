#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(String s : { "1;", "9;", "a;", "f;", "a0;" }) {
		DDUMP(s);
		try { DDUMP(CParser(s).ReadNumber(2)); } catch(CParser::Error) { LOG("Error"); }
		try { DDUMP(CParser(s).ReadNumber(8)); } catch(CParser::Error) { LOG("Error"); }
		try { DDUMP(CParser(s).ReadNumber(10)); } catch(CParser::Error) { LOG("Error"); }
		try { DDUMP(CParser(s).ReadNumber(16)); } catch(CParser::Error) { LOG("Error"); }
	}
	
	CheckLogEtalon();
}
