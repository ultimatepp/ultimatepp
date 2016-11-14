#include <Core/Core.h>

using namespace Upp;

#define XML(x) LOG(#x << " ------------\n" << x << "\n-------\n\n")

void Check(const String& s)
{
	String xml = XmlTag("outer")(XmlTag("inner").Text(s));
	LOG("==================== " << AsCString(s));
	LOG(xml);
	XmlParser p(xml);
	p.Tag("outer");
	p.Tag("inner");
	String h = p.ReadText();
	LOG("-----");
	LOG(AsCString(h));
	ASSERT(h == s);

//	String h = ParseXML(xml)["outer"]["inner"].GatherText();
}

CONSOLE_APP_MAIN
{
//	String h = XmlTag("test").Text("ahoj\nkamarade\n");
//	LOG(h);
//	LOG("=============");
//	LOG(XmlTag("hhh")(h));

	XML(XmlTag("a")(XmlTag("b")()));
	XML(XmlTag("c")(XmlTag("a")(XmlTag("b")())));
	XML(XmlTag("a")(XmlTag("b").Text("text/")));
	XML(XmlTag("simple")("text"));
	XML(XmlTag("tag2").Text("text"));
	XML(XmlTag("tag2").Text("text\n"));
	XML(XmlTag("tag2").Text("\n"));
	XML(XmlTag("tag2").Text("\n\n"));
	XML(XmlTag("tag")(XmlTag("tag2").Text("text")));
	XML(XmlTag("tag")(XmlTag("tag2").Text("text\n")));
	XML(XmlTag("tag")(XmlTag("tag2").Text("\n")));
	XML(XmlTag("tag")(XmlTag("tag2").Text("\n\n")));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\n"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\n"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\ntext"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\ntext\n"))));
	XML(XmlTag("methodCall")(XmlTag("methodName")("METHOD") +
	                         XmlTag("params")(
	                         	XmlTag("param")(
	                         		XmlTag("int")("123")))));
	XML(XmlTag("methodCall")(XmlTag("methodName")("METHOD") +
	                         XmlTag("params")(
	                         	XmlTag("param")(
	                         		XmlTag("string")("\n")))));
	XML(XmlTag("param")(XmlTag("int")("123")));
	XML(XmlTag("params")(XmlTag("param")(XmlTag("int")("123"))));
	XML(XmlTag("params")(XmlTag("param")(XmlTag("nil")())));
	XML(XmlTag("tag2")(XmlTag("tag")("attrib","bad\n<value").Text("text")));
	XML(XmlTag("yyy")(XmlTag("xxx")("")));

	LOG("----------------------------------------------------");
	
	Check("text");
	Check("text\n");
	Check("\ntext");
	Check("\ntext\n");
	Check("\nte\nxt\n");
	Check("\n");
	Check("\n\n");
	Check("\n\n\n");
	
	return;
	String h = XmlTag("int")("123");
	LOG(h);
	DUMPHEX(h);
	h = XmlTag("param")(h);
	LOG(h);
}
