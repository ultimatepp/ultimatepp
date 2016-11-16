#include <Core/Core.h>
#include <plugin/zip/zip.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	FileUnZip unzip(GetDataFile("test.zip"));
	
	String s = unzip.ReadFile();
	
	ASSERT(s.GetCount() > 12000000);

	StringStream ss(s);
	
	XmlParser p(ss);
	
	try {
		ParseXML(p);
	}
	catch(XmlError e) {
		DLOG(e);
		NEVER();
	}
	
	LOG("============= OK");
}
