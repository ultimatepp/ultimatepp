#include "Core.h"

NAMESPACE_UPP

template<>
void Xmlize(XmlIO xml, String& var)
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

template<>
void Xmlize(XmlIO xml, int& var)
{
	xml.Attr("value", var);
}

template<>
void Xmlize(XmlIO xml, dword& var)
{
	xml.Attr("value", var);
}

template<>
void Xmlize(XmlIO xml, double& var)
{
	xml.Attr("value", var);
}

template<>
void Xmlize(XmlIO xml, bool& var)
{
	xml.Attr("value", var);
}

template<>
void Xmlize(XmlIO xml, Point& p)
{
	xml
		.Attr("x", p.x)
		.Attr("y", p.y)
	;
}

template<>
void Xmlize(XmlIO xml, Size& sz)
{
	xml
		.Attr("cx", sz.cx)
		.Attr("cy", sz.cy)
	;
}

template<>
void Xmlize(XmlIO xml, Rect& r)
{
	xml
		.Attr("left", r.left)
		.Attr("top", r.top)
		.Attr("right", r.right)
		.Attr("bottom", r.bottom)
	;
}

template<>
void Xmlize(XmlIO xml, Color& c)
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

void XmlizeLangAttr(XmlIO xml, int& lang, const char *id)
{
	String l;
	if(xml.IsStoring())
		l = LNGAsText(lang);
	xml.Attr(id, l);
	if(xml.IsLoading() && !IsNull(l))
		lang = LNGFromText(l);
}

void XmlizeLang(XmlIO xml, const char *tag, int& lang, const char *id)
{
	XmlIO n(xml, tag);
	XmlizeLangAttr(n, lang, id);
}

String StoreAsXML(Callback1<XmlIO> xmlize, const char *name)
{
	XmlNode node;
	xmlize(XmlIO(node(name), false));
	return AsXML(node);
}

bool LoadFromXML(Callback1<XmlIO> xmlize, const String& xml)
{
	try {
		XmlNode node = ParseXML(xml);
		for(int i = 0; i < node.GetCount(); i++)
			if(node.Node(i).IsTag()) {
				xmlize(XmlIO(node.At(i), true));
				break;
			}
		return true;
	}
	catch(XmlError) {}
	return false;
}

static String sXMLFile(const char *file)
{
	if(file)
		return IsFullPath(file) ? String(file) : ConfigFile(file);
	else
		return ConfigFile(GetExeTitle() + ".xml");
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
