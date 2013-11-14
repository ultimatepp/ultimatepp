#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define XML(x) LOG(#x << " ------------\n" << x << "-------\n\n")

CONSOLE_APP_MAIN
{
//	String h = XmlTag("test").Text("ahoj\nkamarade\n");
//	LOG(h);
//	LOG("=============");
//	LOG(XmlTag("hhh")(h));
	
	XML(XmlTag("a")(XmlTag("b")()));
	XML(XmlTag("a")(XmlTag("b").Text("text")));
	XML(XmlTag("simple")("text"));
	XML(XmlTag("tag2").Text("text"));
	XML(XmlTag("tag2").Text("text\n"));
	XML(XmlTag("tag")(XmlTag("tag2").Text("text")));
	XML(XmlTag("tag")(XmlTag("tag2").Text("text\n")));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\n"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\n"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\ntext"))));
	XML(XmlTag("tag3")(XmlTag("tag")(XmlTag("tag2").Text("text\ntext\n"))));
	XML(XmlTag("methodCall")(XmlTag("methodName")("METHOD") +
	                         XmlTag("params")(
	                         	XmlTag("param")(
	                         		XmlTag("int")("123")))));
	XML(XmlTag("param")(XmlTag("int")("123")));
	XML(XmlTag("params")(XmlTag("param")(XmlTag("int")("123"))));
//	XML(XmlTag("tag2")(XmlTag("tag")("attrib","bad\n<value").Text("text")));4
	
	return;
	String h = XmlTag("int")("123");
	LOG(h);
	DUMPHEX(h);
	h = XmlTag("param")(h);
	LOG(h);
}
