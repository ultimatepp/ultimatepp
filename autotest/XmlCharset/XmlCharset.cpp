#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < 2; i++) {
		XmlNode xml = ParseXML(LoadFile(GetDataFile(AsString(i) + ".xml")));
		DDUMP(TrimBoth(xml["test"]["tag"].GatherText()));
		DDUMP(xml["test"]["tag"].Attr(0));

		ASSERT(TrimBoth(xml["test"]["tag"].GatherText()) == "Úplně nevinný tag");
		ASSERT(xml["test"]["tag"].Attr(0) == "Nějaký šikovný attribut");
	}
	
	LOG("============== OK");
}
