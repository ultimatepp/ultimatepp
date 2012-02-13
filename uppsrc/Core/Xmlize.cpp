#include "Core.h"

NAMESPACE_UPP

template<>
void Xmlize(XmlIO& xml, String& var)
{
	if(xml.IsLoading()) {
		for(int i = 0; i < xml->GetCount(); i++)
			if(xml->Node(i).IsText()) {
				var = xml->Node(i).GetText();
				return;
			}
	}
	else {
		for(int i = 0; i < xml->GetCount(); i++)
			if(xml->Node(i).IsText()) {
				xml->At(i).CreateText(var);
				return;
			}
		xml->Add().CreateText(var);
	}
}

template<>
void Xmlize(XmlIO& xml, WString& var)
{
	String h;
	if(xml.IsStoring())
		h = ToUtf8(var);
	Xmlize(xml, h);
	if(xml.IsLoading())
		var = FromUtf8(h);
}

template<> void XmlAttrLoad(WString& var, const String& text)
{
	var = FromUtf8(text);
}

template<> String XmlAttrStore(const WString& var)
{
	return ToUtf8(var);
}

template<> void XmlAttrLoad(int& var, const String& text)
{
	var = ScanInt(text);
}

template<> String XmlAttrStore(const int& var)
{
	return AsString(var);
}

template<> void   XmlAttrLoad(dword& var, const String& text)
{
	CParser p(text);
	var = p.IsNumber() ? p.ReadNumber() : 0;
}

template<> String XmlAttrStore(const double& var)
{
	return AsString(var);
}

template<> void XmlAttrLoad(double& var, const String& text)
{
	var = ScanDouble(text);
}

template<> String XmlAttrStore(const dword& var)
{
	return AsString(var);
}

template<> void XmlAttrLoad(bool& var, const String& text)
{
	var = text == "1";
}

template<> String XmlAttrStore(const bool& var)
{
	return var ? "1" : "0";
}

template<> void XmlAttrLoad(int16& var, const String& text) {
	var = ScanInt(text);
}

template<> String XmlAttrStore(const int16& var) {
	return AsString(var);
}

template<> void XmlAttrLoad(int64& var, const String& text) {
	var = ScanInt64(text);
}

template<> String XmlAttrStore(const int64& var) {
	return AsString(var);
}

template<> void XmlAttrLoad(byte& var, const String& text) {
	var = ScanInt(text);
}

template<> String XmlAttrStore(const byte& var) {
	return AsString((int)var);
}

template<> void XmlAttrLoad(Date& var, const String& text) {
	if(text.GetCount() > 6) {
		var.year = ScanInt(text.Left(4));
		var.month = ScanInt(text.Mid(4, 2));
		var.day = ScanInt(text.Mid(6));
		if(var.IsValid())
			return;
	}
	var = Null;
}

template<> String XmlAttrStore(const Date& var) {
	return Format("%04d%02d%02d", var.year, var.month, var.day);
}

template<> void XmlAttrLoad(Time& var, const String& text) {
	if(text.GetCount() > 15) {
		var.year = ScanInt(text.Left(4));
		var.month = ScanInt(text.Mid(4, 2));
		var.day = ScanInt(text.Mid(6, 2));
		var.hour = ScanInt(text.Mid(9, 2));
		var.minute = ScanInt(text.Mid(12, 2));
		var.second = ScanInt(text.Mid(15));
		if(var.IsValid())
			return;
	}
	var = Null;
}

template<> String XmlAttrStore(const Time& var) {
	return Format("%04d%02d%02d`T%02d`:%02d`:%02d", var.year, var.month, var.day, var.hour, var.minute, var.second);
}

#define VALUE_XMLIZE(type) template <> void Xmlize(XmlIO& xml, type& var) { xml.Attr("value", var); }

VALUE_XMLIZE(int);
VALUE_XMLIZE(dword);
VALUE_XMLIZE(double);
VALUE_XMLIZE(bool);
VALUE_XMLIZE(int16);
VALUE_XMLIZE(int64);
VALUE_XMLIZE(byte);
VALUE_XMLIZE(Time);
VALUE_XMLIZE(Date);

template <class T>
void XmlizePoint_(XmlIO& xml, T& p)
{
	xml
		.Attr("x", p.x)
		.Attr("y", p.y)
	;
}

template<> void Xmlize(XmlIO& xml, Point& p)   { XmlizePoint_(xml, p); }
template<> void Xmlize(XmlIO& xml, Point16& p) { XmlizePoint_(xml, p); }
template<> void Xmlize(XmlIO& xml, Point64& p) { XmlizePoint_(xml, p); }
template<> void Xmlize(XmlIO& xml, Pointf& p)  { XmlizePoint_(xml, p); }

template<class T>
void XmlizeSize_(XmlIO& xml, T& sz)
{
	xml
		.Attr("cx", sz.cx)
		.Attr("cy", sz.cy)
	;
}

template<> void Xmlize(XmlIO& xml, Size& p)   { XmlizeSize_(xml, p); }
template<> void Xmlize(XmlIO& xml, Size16& p) { XmlizeSize_(xml, p); }
template<> void Xmlize(XmlIO& xml, Size64& p) { XmlizeSize_(xml, p); }
template<> void Xmlize(XmlIO& xml, Sizef& p)  { XmlizeSize_(xml, p); }

template<class T>
void XmlizeRect_(XmlIO& xml, T& r)
{
	xml
		.Attr("left", r.left)
		.Attr("top", r.top)
		.Attr("right", r.right)
		.Attr("bottom", r.bottom)
	;
}

template<> void Xmlize(XmlIO& xml, Rect& p)   { XmlizeRect_(xml, p); }
template<> void Xmlize(XmlIO& xml, Rect16& p) { XmlizeRect_(xml, p); }
template<> void Xmlize(XmlIO& xml, Rect64& p) { XmlizeRect_(xml, p); }
template<> void Xmlize(XmlIO& xml, Rectf& p)  { XmlizeRect_(xml, p); }

template<>
void Xmlize(XmlIO& xml, Color& c)
{
	int r = c.GetR();
	int g = c.GetG();
	int b = c.GetB();
	xml
		.Attr("red", r)
		.Attr("green", g)
		.Attr("blue", b)
	;
	c = Color(r, g, b);
}

typedef void (*ValueXmlizer)(XmlIO& xml, Value& v);

VectorMap<dword, ValueXmlizer>& ValueXmlizeMap()
{
	static VectorMap<dword, ValueXmlizer> x;
	return x;
}

Index<String>& ValueXmlizeName()
{
	static Index<String> x;
	return x;
}

INITBLOCK {
	ValueXmlizeMap();
	ValueXmlizeName();
}

void RegisterValueXmlize(dword type, void (*xmlize)(XmlIO& xml, Value& v), const char *name)
{
	ASSERT(ValueXmlizeMap().Find(type) < 0);
	ASSERT(ValueXmlizeName().Find(name) < 0);
	ValueXmlizeMap().Add(type, xmlize);
	ValueXmlizeName().Add(name);
}

REGISTER_VALUE_XMLIZE(String);
REGISTER_VALUE_XMLIZE(WString);
REGISTER_VALUE_XMLIZE(int);
REGISTER_VALUE_XMLIZE(double);
REGISTER_VALUE_XMLIZE(int64);
REGISTER_VALUE_XMLIZE(Date);
REGISTER_VALUE_XMLIZE(Time);
REGISTER_VALUE_XMLIZE(Point);
REGISTER_VALUE_XMLIZE(Point64);
REGISTER_VALUE_XMLIZE(Pointf);
REGISTER_VALUE_XMLIZE(Size);
REGISTER_VALUE_XMLIZE(Size64);
REGISTER_VALUE_XMLIZE(Sizef);
REGISTER_VALUE_XMLIZE(Rect);
REGISTER_VALUE_XMLIZE(Rect64);
REGISTER_VALUE_XMLIZE(Rectf);
REGISTER_VALUE_XMLIZE(Color);
REGISTER_VALUE_XMLIZE(ValueArray);
REGISTER_VALUE_XMLIZE(ValueMap);

static String s_binary("serialized_binary");

template<> void Xmlize(XmlIO& xml, Value& v)
{
	if(xml.IsStoring()) {
		dword typeno = v.GetType();
		int q = ValueXmlizeMap().Find(typeno);
		if(q < 0) {
			xml.SetAttr("type", s_binary);
			String s = HexString(StoreAsString(v));
			Xmlize(xml, s);
		}
		else {
			xml.SetAttr("type", ValueXmlizeName()[q]);
			(*ValueXmlizeMap()[q])(xml, v);
		}
	}
	else {
		String name = xml.GetAttr("type");
		if(name == s_binary) {
			String s;
			Xmlize(xml, s);
			try {
				LoadFromString(v, ScanHexString(s));
			}
			catch(LoadingError) {
				throw XmlError("serialized_binary Error");
			}
		}
		else {
			int q = ValueXmlizeName().Find(name);
			if(q < 0)
				throw XmlError("invalid Value type");
			(*ValueXmlizeMap()[q])(xml, v);
		}
	}
}

template<> void Xmlize(XmlIO& xml, ValueArray& v)
{
	if(xml.IsStoring())
		XmlizeStore(xml, v.Get());
	else {
		Vector<Value> vv;
		Xmlize(xml, vv);
		v = ValueArray(vv);
	}
}

template<> void Xmlize(XmlIO& xml, ValueMap& v)
{
	if(xml.IsStoring()) {
		XmlizeStore(xml, v.GetKeys());
		XmlizeStore(xml, v.GetValues());
	}
	else {
		Index<Value> vv;
		Xmlize(xml, vv);
		ValueArray va;
		Xmlize(xml, va);
		ASSERT(vv.GetCount() == va.GetCount());
		v.Clear();
		for(int i = 0; i < vv.GetCount(); i++)
			v.Add(vv[i], va[i]);
	}
}

void XmlizeLangAttr(XmlIO& xml, int& lang, const char *id)
{
	String l;
	if(xml.IsStoring())
		l = LNGAsText(lang);
	xml.Attr(id, l);
	if(xml.IsLoading() && !IsNull(l))
		lang = LNGFromText(l);
}

void XmlizeLang(XmlIO& xml, const char *tag, int& lang, const char *id)
{
	XmlIO n(xml, tag);
	XmlizeLangAttr(n, lang, id);
}

String StoreAsXML(Callback1<XmlIO> xmlize, const char *name)
{
	String n = Filter(name, CharFilterAlpha);
	if(IsNull(n))
		n = "app";
	XmlNode node;
	xmlize(XmlIO(node(n), false, Value()));
	return AsXML(node);
}

bool LoadFromXML(Callback1<XmlIO> xmlize, const String& xml)
{
	try {
		XmlNode node = ParseXML(xml);
		for(int i = 0; i < node.GetCount(); i++)
			if(node.Node(i).IsTag()) {
				xmlize(XmlIO(node.At(i), true, Value()));
				break;
			}
		return true;
	}
	catch(XmlError) {}
	return false;
}

static String sXMLFile(const char *file)
{
	return file ? String(file) : ConfigFile(GetExeTitle() + ".xml");
}

bool StoreAsXMLFile(Callback1<XmlIO> xmlize, const char *name, const char *file)
{
	return SaveFile(sXMLFile(file), StoreAsXML(xmlize, name ? (String)name : GetExeTitle()));
}

bool LoadFromXMLFile(Callback1<XmlIO> xmlize, const char *file)
{
	return LoadFromXML(xmlize, LoadFile(sXMLFile(file)));
}

END_UPP_NAMESPACE
