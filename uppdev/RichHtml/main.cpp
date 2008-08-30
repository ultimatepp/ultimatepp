#include <CtrlLib/CtrlLib.h>
#include <RichText/RichText.h>
#include <Web/Web.h>

using namespace Upp;

#define TOPICFILE <RichHtml/tst.tpp/all.i>
#include <Core/topic_group.h>

GUI_APP_MAIN
{
	Index<String> css;
	VectorMap<String, String> links;
	String qtf = GetTopic("topic://RichHtml/tst/Topic$en-us");
	String html = EncodeHtml(ParseQTF(qtf), css, links, "e:\\xxx");
	Htmls content =
		HtmlHeader("Ultimate++", AsCss(css))
	    .BgColor(White)
	    /  html;
	SaveFile(GetHomeDirFile("html.html"), content);
}
