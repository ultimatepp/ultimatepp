#include <Core/Core.h>

using namespace Upp;

void Check0(const String& xml0, bool full)
{
	dword astyle = full ? 0 : XML_HEADER|XML_DOCTYPE;
	dword pstyle = full ? 0 : XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS;
	String xml = AsXML(ParseXML(xml0, pstyle), astyle);
	String p = GetHomeDirFile("XML.xml");
	SaveFile(p, xml);
	String xml2 = AsXML(ParseXMLFile(p, pstyle), astyle);
	SaveFile(GetHomeDirFile("XML2.xml"), xml2);
	String xml1 = AsXML(ParseXML(xml, pstyle), astyle);
	SaveFile(GetHomeDirFile("XML1.xml"), xml1);
	
	ASSERT(xml == xml1);
	ASSERT(xml == xml2);
}

void Check(const String& xml)
{
	Check0(xml, true);
	String xml2 = AsXML(ParseXML(xml));
	Check0(xml2, false);
}

String GenID()
{
	return "A" + AsString(Random(1000000));
}

void GenNode(XmlNode& n)
{
	bool doc = n.GetType() == XML_DOC;
	XmlNode& nn = n.Add();
	nn.CreateTag(GenID());
	int N = Random(4);
	for(int i = 0; i < N; i++)
		nn.SetAttr(GenID(), AsString(Random()));
	if(Random(10) == 0)
		nn.Add().CreatePI(GenID());
	if(Random(10) == 0)
		nn.Add().CreateComment(GenID());
	if(Random(10) == 0)
		nn.Add().CreateDecl(GenID());
	N = Random(2) * Random(4);
	if(nn.GetCount() == 0 && Random(3) == 0)
		nn.Add().CreateText(GenID());
	else		
		for(int i = 0; i < N; i++)
			GenNode(nn);
}

void CheckFile(const char *path)
{
	Cout() << path << '\n';
	Check(AsXML(ParseXML(LoadFile(path), 0)));
}

CONSOLE_APP_MAIN
{
	SeedRandom();

	for(int i = 0; i < 100; i++) {
		Cout() << i << '\n';
		XmlNode n;
		XmlNode& nn = n.Add();
		nn.CreateTag(GenID());
		for(int i = 0; i < 100; i++) {
			GenNode(nn);
			String xml = AsXML(n);
			LOG("============");
			LOG(xml);
			Check(AsXML(n));
		}
	}

	for(FindFile ff(GetDataFile("*.xml")); ff; ff.Next())
		CheckFile(ff.GetPath());
}
