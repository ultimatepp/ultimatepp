#include <Core/Core.h>

using namespace Upp;

struct Data {
	Size                   sz;
	int                    lang;
	String                 text;
	VectorMap<String, int> score;
	double                 number;

	void   Dump();
	String ToXML();
	void   LoadXML(XmlParser& p);
};

void   Data::Dump()
{
	DUMP(sz);
	DUMP(text);
	LOG("lang: " << LNGAsText(lang));
	LOG("score: ");
	for(int i = 0; i < score.GetCount(); i++)
		LOG("   " << score.GetKey(i) << " ..... " << score[i]);
	DUMP(number);
}

String Data::ToXML()
{
	String scores;
	for(int i = 0; i < score.GetCount(); i++)
		scores << XmlTag("Player")("name", score.GetKey(i))("score", score[i])();
	return
		XmlHeader() +
		XmlTag("Configuration") (
			XmlTag("Language")("id", LNGAsText(lang))() +
			XmlTag("Dimension")("x", sz.cx)("y", sz.cy)() +
			XmlTag("text").Text(text) +
			XmlTag("Scores") (scores) +
			XmlTag("Number")("value", number)()
		);
}

void   Data::LoadXML(XmlParser& p)
{
	while(!p.IsTag())
		p.Skip();
	p.PassTag("Configuration");
	while(!p.End())
		if(p.TagE("Language"))
			lang = LNGFromText(p["id"]);
		else
		if(p.TagE("Dimension")) {
			sz.cx = minmax(p.Int("x"), 1, 32);
			sz.cy = minmax(p.Int("y"), 1, 32);
		}
		else
		if(p.Tag("Scores"))
			while(!p.End()) {
				if(p.TagE("Player")) {
					int& x = score.GetAdd(p["name"]);
					x = p.Int("score");
				}
				else
					p.Skip();
			}
		else
		if(p.Tag("text")) {
			text = p.ReadText();
			p.PassEnd();
		}
		else
		if(p.TagE("Number")) {
			number = p.Double("value");
		}
		else
			p.Skip();
}

CONSOLE_APP_MAIN
{
	Data x;
	x.sz.cx = 10;
	x.sz.cy = 20;
	x.lang = LNGFromText("CS-CZ");
	x.text = "Some text";
	x.score.Add("peter", 100);
	x.score.Add("john", 10);
	x.score.Add("frank", 20);
	x.number = 1.2345e100;

	x.Dump();
	String xml = x.ToXML();
	LOG("\n---- XmlTag (encoding) -----------------");
	LOG(xml);

	LOG("\n---- XmlParser -----------------");
	Data y;
	XmlParser p(xml);
	y.LoadXML(p);
	y.Dump();

	LOG("\n---- XmlNode -----------------");
	XmlNode xn = ParseXML(xml);
	LOG("Dimension x: " << xn["Configuration"]["Dimension"].Attr("x"));
	xn("Configuration").Remove("Scores");
	XmlNode& n = xn("Configuration")("TEST");
	n.SetAttr("test_attr", "23");
	LOG(AsXML(xn));
	LOG("Missing tag: " << xn["Configuration"]["MISSING_TAG"].Attr("doesnotmatter"));
}
