#include <Core/Core.h>

using namespace Upp;

struct Data {
	Size                      sz;
	int                       lang;
	String                    text;
	Vector<Size>              vector;
	VectorMap<String, int>    score;
	VectorMap<String, String> map;
	ArrayMap<int, Size>       sizemap;
	double                    number;
	bool                      option;

	void Dump();
	void Xmlize(XmlIO& xml);
};

void Data::Xmlize(XmlIO& xml)
{
	xml
		("Dimension", sz)
		("text", text)
		.List("vector", "size", vector)
		("Scores", score)
		("map", map)
		("sizemap", sizemap)
		("number", number)
		("option", option)
	;
	XmlizeLang(xml, "Language", lang);
}

void   Data::Dump()
{
	DUMP(sz);
	DUMP(text);
	LOG("lang: " << LNGAsText(lang));
	DUMPC(vector);
	LOG("score: ");
	for(int i = 0; i < score.GetCount(); i++)
		LOG("   " << score.GetKey(i) << " ..... " << score[i]);
	LOG("map: ");
	for(int i = 0; i < map.GetCount(); i++)
		LOG("   " << map.GetKey(i) << " ..... " << map[i]);
	LOG("sizemap: ");
	for(int i = 0; i < sizemap.GetCount(); i++)
		LOG("   " << sizemap.GetKey(i) << " ..... " << sizemap[i]);
	DUMP(number);
	DUMP(option);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Data x;
	x.sz.cx = 10;
	x.sz.cy = 20;
	x.lang = LNGFromText("CS-CZ");
	x.text = "Some text";
	x.vector.Add(Size(1, 2));
	x.vector.Add(Size(3, 4));
	x.score.Add("peter", 100);
	x.score.Add("john", 10);
	x.score.Add("frank", 20);
	x.map.Add("keytext", "valuetext");
	x.map.Add("key1text", "value1text");
	x.sizemap.Add(1, Size(2, 3));
	x.sizemap.Add(10, Size(20, 30));
	x.number = -1.235e-30;
	x.option = true;

	LOG(StoreAsXML(x));
	LOG("====");

	StoreAsXMLFile(x);

	Data y;
	LoadFromXMLFile(y);
	y.Dump();
}
