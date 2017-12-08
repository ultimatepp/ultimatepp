#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	XmlNode xml = ParseXML(LoadFile(GetDataFile("1.xml")));
	DDUMP(xml["test"]["tag"].GatherText());
	DDUMP(xml["test"]["tag"].Attr(0));
}
