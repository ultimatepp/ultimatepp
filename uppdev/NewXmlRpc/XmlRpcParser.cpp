#include "XmlRpc.h"

Value ParseXmlRpcValue(XmlParser& p)
{
	Value r;
	p.PassTag("value");
	if(p.Tag("int") || p.Tag("i4")) {
		int n = StrInt(p.ReadText());
		if(IsNull(n))
			throw XmlError("integer expected");
		r = n;
	}
	else
	if(p.Tag("boolean")) {
		int n = StrInt(p.ReadText());
		if(n != 0 && n != 1)
			throw XmlError("boolean expected");
		r = n;
	}
	else
	if(p.Tag("string") || p.Tag("base64"))
		r = p.ReadText();
	else
	if(p.Tag("dateTime.iso8601")) {
		String s = TrimBoth(p.ReadText());
		// 19980717T14:08:55
		// 01234567890123456
		if(s.GetCount() != 16 || s[8] != 'T' || s[11] != ':' || s[14] != ':')
			throw XmlError("invalid dateTime format");
		Time tm;
		tm.year = atoi(s.Mid(0, 4));
		tm.month = atoi(s.Mid(4, 2));
		tm.day = atoi(s.Mid(6, 2));
		tm.hour = atoi(s.Mid(9, 2));
		tm.minute = atoi(s.Mid(12, 2));
		tm.second = atoi(s.Mid(15, 2));
		r = tm;
	}
	else
	if(p.Tag("array")) {
		ValueArray va;
		p.PassTag("data");
		while(!p.End())
			va.Add(ParseXmlRpcValue(p));
		r = va;
	}
	else
	if(p.Tag("struct")) {
		ValueMap vm;
		while(p.Tag("member")) {
			p.PassTag("name");
			String name = p.ReadText();
			p.PassEnd();
			vm.Add((Value)name, ParseXmlRpcValue(p));
			p.PassEnd();
		}
		r = vm;
	}
	else
		throw XmlError("unknown value type");
	p.PassEnd();
	p.PassEnd();
	return r;
}

Value ParseXmlRpcParam(XmlParser& p)
{
	p.PassTag("param");
	Value v = ParseXmlRpcValue(p);
	p.PassEnd();
	return v;
}
