#include "Core.h"

namespace Upp {

template<>
void Xmlize(XmlIO& xml, String& var)
{
	if(xml.IsLoading()) {
		for(int i = 0; i < xml->GetCount(); i++)
			if(xml->Node(i).IsText()) {
				var = xml->Node(i).GetText();
				return;
			}
		var.Clear();
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
		var = ToUtf32(h);
}

template<> void XmlAttrLoad(WString& var, const String& text)
{
	var = ToUtf32(text);
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

template<> String XmlAttrStore(const float& var)
{
	return AsString(var);
}

template<> void XmlAttrLoad(float& var, const String& text)
{
	var = ScanFloat(text);
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
VALUE_XMLIZE(float);
VALUE_XMLIZE(bool);
VALUE_XMLIZE(int16);
VALUE_XMLIZE(int64);
VALUE_XMLIZE(byte);
VALUE_XMLIZE(Time);
VALUE_XMLIZE(Date);

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

String DoStoreAsXML(Event<XmlIO> xmlize, const char *name)
{
	String n = Filter(name ? name : "app", CharFilterAlpha);
	XmlNode node;
	xmlize(XmlIO(node(n), false, Value()));
	return AsXML(node);
}

bool LoadFromXML0(Event<XmlIO> xmlize, const String& xml)
{
	XmlNode node = ParseXML(xml);
	if(node.GetCount() == 0)
		return false;
	for(int i = 0; i < node.GetCount(); i++)
		if(node.Node(i).IsTag()) {
			Value dummy;
			xmlize(XmlIO(node.At(i), true, dummy));
			break;
		}
	return true;
}

bool DoLoadFromXML(Event<XmlIO> xmlize, const String& xml)
{
	try {
		return LoadFromXML0(xmlize, xml);
	}
	catch(ValueTypeError) {}
	catch(XmlError) {}
	return false;
}

bool DoTryLoadFromXML(Event<XmlIO> xmlize, const String& xml)
{
	return LoadFromXML0(xmlize, xml);
}

static String sXMLFile(const char *file)
{
	return file ? String(file) : ConfigFile(GetExeTitle() + ".xml");
}

bool StoreAsXMLFile(Event<XmlIO> xmlize, const char *name, const char *file)
{
	return SaveFile(sXMLFile(file), DoStoreAsXML(xmlize, name ? (String)name : GetExeTitle()));
}

bool LoadFromXMLFile(Event<XmlIO> xmlize, const char *file)
{
	return DoLoadFromXML(xmlize, LoadFile(sXMLFile(file)));
}

bool TryLoadFromXMLFile(Event<XmlIO> xmlize, const char *file)
{
	return DoTryLoadFromXML(xmlize, LoadFile(sXMLFile(file)));
}

void StoreJsonValue(XmlIO& xio, const Value& v)
{
	if(v.GetType() == VALUEMAP_V) {
		ValueMap m = v;
		ValueArray va = m.GetValues();
		for(int i = 0; i < m.GetCount(); i++) {
			Value h = m.GetValue(i);
			XmlIO io = xio.Add((String)m.GetKey(i));
			StoreJsonValue(io, h);
		}
		return;
	}
	else
	if(v.GetType() == VALUEARRAY_V) {
		ValueArray va = v;
		for(int i = 0; i < va.GetCount(); i++) {
			XmlIO io = xio.Add("item");
			Value h = va[i];
			StoreJsonValue(io, h);
		}
	}
	else
	if(v.GetType() == BOOL_V) {
		bool b = v;
		Xmlize(xio, b);
	}
	else
	if(v.GetType() == FLOAT_V) {
		float f = v;
		Xmlize(xio, f);
	}
	else
	if(IsNumber(v)) {
		double h = v;
		Xmlize(xio, h);
		return;
	}
	else
	if(IsString(v)) {
		String h = v;
		Xmlize(xio, h);
	}
	else
	if(v.Is<Date>()) {
		Date h = v;
		Xmlize(xio, h);
	}
	else
	if(v.Is<Time>()) {
		Time h = v;
		Xmlize(xio, h);
	}
	else
		NEVER();
}

Value LoadJsonValue(const XmlNode& n)
{
	String h = n.Attr("value");
	if(h.GetCount())
		return ScanDouble(h);
	ValueMap m;
	String text;
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].IsTag())
			m.Add(n[i].GetTag(), LoadJsonValue(n[i]));
		else
			return n[i].GetText();
	return m;
}

}