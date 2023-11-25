#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE | LOG_COUT);

	for(const char *xml : { " <b> </b> ", "</b>"  })
		try {
			DDUMP(xml);
			ParseXML(xml);
		}
		catch(XmlError e)
		{
			DDUMP(e);
		}
	
	CheckLogEtalon();
}
