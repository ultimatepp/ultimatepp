#include <CtrlLib/CtrlLib.h>

#define TOPICFILE <EncodeHtml/Test.tpp/all.i>
#include <Core/topic_group.h>

using namespace Upp;

GUI_APP_MAIN
{
	String path = GetExeDirFile("test.html");
	Index<String> css;
	String html = EncodeHtml(ParseQTF(GetTopic("topic://EncodeHtml/Test/Test_en-us").text), css,
	                         VectorMap<String, String>(), VectorMap<String, String>(),
	                         GetFileFolder(path));
	SaveFile(path, MakeHtml("Test", AsCss(css), html));
	
	DDUMP(SHA1StringS(LoadFile(path)));
	
	CheckLogEtalon();
}
