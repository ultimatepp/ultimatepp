#include <Core/Core.h>
#include <plugin/pcre/Pcre.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	RegExp r0("(\\w+)\\s(\\w+)\\s(\\w+)");
	if(r0.Match("one two three"))
	{
		for(int i = 0; i < r0.GetCount(); i++)
		    DUMP(r0[i]);
    } else if(r0.IsError())
	    DUMP(r0.GetError());
    ASSERT(!r0.IsError());

	LOG("==================================");

	RegExp r1("(\\w+)", RegExp::UNICODE);
	int i = 0;
	while(r1.GlobalMatch("hello 4 ho 55 uuu iii pp 99 baby too swistak"))
	{
		for(int i = 0; i < r1.GetCount(); i++)
			DUMP(r1[i]);
    }
	if(r1.IsError())
	    DUMP(r1.IsError());
    ASSERT(!r1.IsError());

	RegExp re(
		"(stuff)\\s+(\\d+)?\\s*(stuffx)?\\s*([a-zA-Z_][a-zA-Z0-9_-]*=(?:\".*\"|\'.*\'|[[:graph:]]*) )*\\s*(\".*?\");"
		"|(stuff2)\\s+(\".*?\");"
		"|(stuff3)\\s+(\".*?\".*?);"
		"|(.*?);"
	);
	ASSERT(re.Match("tid nanu;"));
    ASSERT(!re.IsError());
	
	LOG("========================= OK");
}
