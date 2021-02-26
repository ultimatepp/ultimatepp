#include <RichText/RichText.h>

#define TOPICFILE <EncodeHtml/Test.tpp/all.i>
#include <Core/topic_group.h>

using namespace Upp;


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String path = GetHomeDirFile("test.html");
	Index<String> css;
	String html = EncodeHtml(ParseQTF(GetTopic("topic://EncodeHtml/Test/Test_en-us").text), css,
	                         VectorMap<String, String>(), VectorMap<String, String>(),
	                         GetFileFolder(path));

	html = MakeHtml("Test", AsCss(css), html);
	SaveFile(path, html);
	
	auto NoCr = [](String s) { s.Replace("\r", ""); return s; };

	ASSERT(NoCr(LoadDataFile("test.html")) == NoCr(html));
	
	LOG("============== OK");
}
