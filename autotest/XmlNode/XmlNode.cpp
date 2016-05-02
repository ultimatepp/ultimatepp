#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	XmlNode a = ParseXMLFile(GetDataFile("0.xml"));
	XmlNode b = pick(a); // pick copy
	ASSERT(a.GetCount() == 0);
	ASSERT(b.GetCount());
	
	DDUMP(AsXML(b, XML_HEADER|XML_DOCTYPE));
	a = clone(b);
	ASSERT(a.GetCount());
	ASSERT(b.GetCount());
	DDUMP(AsXML(a, XML_HEADER|XML_DOCTYPE));
	DDUMP(AsXML(b, XML_HEADER|XML_DOCTYPE));
	AsXML(b, XML_HEADER|XML_DOCTYPE);
	
	DLOG("================ OK");
}
