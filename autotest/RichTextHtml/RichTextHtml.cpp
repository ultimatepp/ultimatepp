#include <Core/Core.h>
#include <RichText/RichText.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	Index<String> css;
	VectorMap<String, String> links;
	
	String body = EncodeHtml(ParseQTF(LoadDataFile("Test.qtf")), css, links, Null);

	String outfn = GetHomeDirFile("Out.html");

	SaveFile(outfn, "<html><STYLE TYPE=\"text/css\"><!--\r\n" + AsCss(css) + "\r\n-->\r\n</STYLE>\r\n"
		            "<body>" + body + "</body></html>");
		            
	ASSERT(LoadDataFile("Out.html") == LoadFile(outfn));
	
	LOG("============ OK");
}
