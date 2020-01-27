#include <RichText/RichText.h>

#define TOPICFILE <QtfHtml/app.tpp/all.i>
#include <Core/topic_group.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	RichText txt = ParseQTF(GetTopic("QtfHtml", "app", "Test_en-us").text);
	DDUMP(txt.GetLength());

	Index<String> css;
	VectorMap<String, String> links;

	String outdir = GetExeDirFile("html");
	RealizeDirectory(outdir);
	
	String body = EncodeHtml(txt, css, links, outdir);
	
	String html = AppendFileName(outdir, "test.html");

	SaveFile(html,
	         "<html><STYLE TYPE=\"text/css\"><!--\r\n" + AsCss(css) + "\r\n-->\r\n</STYLE>\r\n"
		            "<body>" + body + "</body></html>");

	LaunchWebBrowser(html);
}
