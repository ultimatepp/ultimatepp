#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

namespace Upp {

Value ParseXmlRpcValue(XmlParser& p)
{
	Value r;
	p.PassTag("value");
	if(p.Tag("int") || p.Tag("i4")) {
		String s = p.ReadText();
		CParser p(s);
		if(!p.IsInt())
			throw XmlError("integer expected");
		r = p.ReadInt();
	}
	else
	if(p.Tag("i8")) {
		String s = p.ReadText();
		CParser p(s);
		if(!p.IsInt())
			throw XmlError("integer expected");
		r = p.ReadInt64();
	}
	else
	if(p.Tag("boolean")) {
		int n = StrInt(p.ReadText());
		if(n != 0 && n != 1)
			throw XmlError("boolean expected");
		r = (bool)n;
	}
	else
	if(p.Tag("double")) {
		String s = p.ReadText();
		CParser p(s);
		if(!p.IsDouble())
			throw XmlError("double expected");
		r = p.ReadDouble();
	}
	else
	if(p.Tag("string") || p.Tag("base64"))
		r = p.ReadText();
	else
	if(p.TagE("nil")) {
		p.PassEnd();
		return r;
	}
	else
	if(p.Tag("dateTime.iso8601")) {
		String s = TrimBoth(p.ReadText());
		// 19980717T14:08:55
		// 01234567890123456
		if(s.GetCount() < 17 || s[8] != 'T' || s[11] != ':' || s[14] != ':') // <17 to allow zone ignored zone info
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
			p.PassEnd(); // name
			vm.Add((Value)name, ParseXmlRpcValue(p));
			p.PassEnd(); // member
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

ValueArray ParseXmlRpcParams(XmlParser& p)
{
	ValueArray va;
	if(p.Tag("params"))
		while(!p.End())
			va.Add(ParseXmlRpcParam(p));
	return va;
}

String FormatXmlRpcValue(const Value& _v, bool supports_i8)
{
	String r;
	Value v = _v;
	if(v.GetType() == INT64_V && !supports_i8) {
		int64 x = v;
		if((int)x == x)
			v = (int)x;
	}
	if(IsNull(v) && !IsString(v) && !IsValueArray(v))
		r = XmlTag("nil")();
	else
	if(v.GetType() == INT64_V && supports_i8)
		r = XmlTag("i8")(AsString((int64)v));
	else
	if(v.GetType() == INT_V)
		r = XmlTag("int")(Format("%d", (int)v));
	else
	if(v.GetType() == BOOL_V)
		r = XmlTag("boolean")(AsString((int)(bool)v));
	else
	if(IsNumber(v))
		r = XmlTag("double")(Format("%.16g", (double)v));
	else
	if(IsDateTime(v)) {
		Time t = v;
		r = XmlTag("dateTime.iso8601")
					(Format("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d",
					        t.year, t.month, t.day, t.hour, t.minute, t.second));
	}
	else
	if(v.GetType() == VALUEMAP_V) {
		r = "<struct>";
		ValueMap vm = v;
		const Index<Value>& k = vm.GetKeys();
		ValueArray va = vm.GetValues();
		for(int i = 0; i < k.GetCount(); i++)
			r << XmlTag("member")(XmlTag("name")(k[i]) + FormatXmlRpcValue(va[i], supports_i8));
		r << "</struct>";
	}
	else
	if(v.GetType() == VALUEARRAY_V) {
		r = "<array><data>";
		ValueArray va = v;
		for(int i = 0; i < va.GetCount(); i++)
			r << FormatXmlRpcValue(va[i], supports_i8);
		r << "</data></array>";
	}
	else
	if(v.Is<RawJsonText>())
		r = XmlTag("string").Text(v.To<RawJsonText>().json);
	else
		r = XmlTag("string").Text(v);
	return XmlTag("value")(r);
}

String FormatXmlRpcParam(const Value& param, bool supports_i8)
{
	return XmlTag("param")(FormatXmlRpcValue(param, supports_i8));
}

String FormatXmlRpcParams(const ValueArray& params, bool supports_i8)
{
	String r;
	r = "<params>";
	for(int i = 0; i < params.GetCount(); i++)
		r << FormatXmlRpcParam(params[i], supports_i8);
	r << "</params>";
	return r;
}

String FormatXmlRpcError(int code, const char *text)
{
	String r;
	r << XmlHeader() <<
		"<methodResponse>"
		  "<fault>"
		    "<value>"
		      "<struct>"
		        "<member>"
		          "<name>faultCode</name>"
		          "<value><int>" << code << "</int></value>"
		        "</member>"
		        "<member>"
		          "<name>faultString</name>"
		          "<value><string>" << DeXml(text) << "</string></value>"
		        "</member>"
		      "</struct>"
		    "</value>"
		  "</fault>"
		"</methodResponse>"
	;
	return r;
}

}
