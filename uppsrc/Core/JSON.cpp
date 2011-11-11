#include "Core.h"

NAMESPACE_UPP

Value ParseJSON(CParser& p)
{
	p.UnicodeEscape();
	if(p.IsNumber())
		return p.ReadDouble();
	if(p.IsString())
		return p.ReadString();
	if(p.Id("null"))
		return Null;
	if(p.Id("true"))
		return true;
	if(p.Id("false"))
		return false;
	if(p.Char('{')) {
		ValueMap m;
		if(!p.IsChar('}'))
			do {
				String key = p.ReadString();
				p.PassChar(':');
				m.Add(key, ParseJSON(p));
			}
			while(p.Char(','));
		p.PassChar('}');
		return m;
	}
	if(p.Char('[')) {
		ValueArray va;
		if(!p.IsChar(']'))
			do
				va.Add(ParseJSON(p));
			while(p.Char(','));
		p.PassChar(']');
		return va;		
	}
	p.ThrowError("Unrecognized JSON element");
	return Null;
}

Value ParseJSON(const char *s)
{
	try {
		CParser p(s);
		return ParseJSON(p);
	}
	catch(CParser::Error) {
		return ErrorValue();
	}
}

String AsJSON(const Value& v, const String& sep, bool pretty)
{
	String r;
	if(v.GetType() == VALUEMAP_V) {
		r << "{";
		String sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		ValueMap m = v;
		ValueArray va = m.GetValues();
		for(int i = 0; i < m.GetCount(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsCString((String)m.GetKeys()[i]) << (pretty ? ": " : ":")
			  << AsJSON(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "}";
		return r;
	}
	if(v.GetType() == VALUEARRAY_V) {
		r << "[";
		String sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		ValueArray va = v;
		for(int i = 0; i < va.GetCount(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsJSON(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "]";
		return r;
	}
	if(IsNumber(v) && IsNull(v))
		return "null";
	if(v.GetType() == INT_V)
		return Format("%d", (int)v);
	if(v.GetType() == BOOL_V)
		return (bool)v ? "true" : "false";
	if(IsNumber(v))
		return Format("%.16g", (double)v);
	if(IsString(v))
		return AsCString((String)v);
	NEVER();
	return "null";
}

String AsJSON(const Value& v, bool pretty)
{
	return AsJSON(v, String(), pretty);
}

END_UPP_NAMESPACE
