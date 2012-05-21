#include <Core/Core.h>

using namespace Upp;

struct Data {
	Size                      sz;
	String                    text;
	Vector<Size>              vector;
	VectorMap<String, int>    score;
	VectorMap<String, String> map;
	ArrayMap<int, Size>       sizemap;
	double                    number;
	bool                      option;

	void Jsonize(JsonIO& json);
	void Xmlize(XmlIO& xio)           { XmlizeByJsonize(xio, *this); }
};

void Data::Jsonize(JsonIO& json)
{
	json
		("Dimension", sz)
		("text", text)
		("vector", vector)
		("Scores", score)
		("map", map)
		("sizemap", sizemap)
		("number", number)
		("option", option)
	;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Data x;
	x.sz.cx = 10;
	x.sz.cy = 20;
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

	String json = StoreAsJson(x, true);

	LOG(json);
	LOG("=====================");
	Data y;
	DUMP(LoadFromJson(y, json));
	LOG(StoreAsJson(y));

	LOG("=====================");
	String xml = StoreAsXML(x, "test");
	DUMP(xml);
	Data z;
	DUMP(LoadFromXML(z, xml));
	LOG(StoreAsXML(z, "test"));
}
