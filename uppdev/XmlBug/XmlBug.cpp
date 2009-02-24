#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	try {
		XmlNode xn = ParseXML(LoadFile(GetDataFile("XmlBug.xml")));
		LOG(xn["Settings"]["Cycle"].Attr("Time"));
		LOG(xn["Settings"]["Cycle"].Attr("test1"));
		LOG(xn["Settings"]["Cycle"].Attr("test2"));
		LOG(xn["Settings"]["Cycle"].Attr("test3"));
		LOG(xn["Settings"]["OrderList"].Attr("Url"));
	}
	catch(XmlError e)
	{
		LOG("Error " << e);
	}
}

