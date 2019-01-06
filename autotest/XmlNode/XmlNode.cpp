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

	ASSERT(AsXML(a, XML_HEADER|XML_DOCTYPE) == "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><!DOCTYPE Configuration><Configuration><Language id=\"CS-CZ\"/><Dimension x=\"10\" y=\"20\"/><text>Some text</text><Number value=\"1.2345e100\"/><TEST test_attr=\"23\"/><cdata>&lt;cdata&amp;test&gt;</cdata><empty1/><empty2/><empty_cdata/></Configuration>");
	
	DLOG("================ OK");
}
