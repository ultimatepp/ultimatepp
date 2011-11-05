#include "Core/Core.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	Value js = ParseJSON(LoadFile(GetDataFile("test.json")));
	DUMP(js);
	DUMP(js["age"]);
	Value phone_number = js["phoneNumber"];
	DUMP(phone_number);
	DUMP(phone_number.GetCount());
	for(int i = 0; i < phone_number.GetCount(); i++) {
		DUMP(phone_number[i]["type"]);
		DUMP(phone_number[i]["number"]);
	}

	DUMP(AsJSON(js, true));
	DUMP(AsJSON(js));

	LOG("- Partial parsing");
	ValueArray va;
	va << js << js << js;
	String s = AsJSON(va);
	DUMP(s);
	CParser p2(s);
	p2.PassChar('[');
	if(!p2.Char(']')) {
		do {
			DUMP(ParseJSON(p2)["firstName"]);
		}
		while(p2.Char(','));
		p2.PassChar(']');
	}
	
	LOG("- Composing JSON using support classes");
	Json json;
	json
		("firstName", "Andrew")
		("secondName", "Smith")
		("age", 28)
		("address", Json("streetAddress", "23 3rd Street")("city", "New York"))
	;
	JsonArray pn;
	pn << Json("type", "home")("number", "12312345")
	   << Json("type", "work")("number", "87126388");
	json("phoneNumber", pn);
	DUMP(~json);
	DUMP(AsJSON(ParseJSON(~json), true));
}
