#include "Theming.h"

String ThemeProperties::Raw(const char *id, const String& s)
{
	if(loading) {
		int q = data.Find(id);
		return q < 0 ? s : data[q]; 
	}
	else {
		data.GetAdd(id) = s;
		return s;
	}
}

int ThemeProperties::operator()(const char *id, int p)
{
	return CParser(Raw(id, AsString(p))).ReadInt();
}

Color ThemeProperties::operator()(const char *id, Color p)
{
	String s = Format("#%02x%02x%02x", p.GetR(), p.GetG(), p.GetB());
	s = Raw(id, s);
	if(s.GetCount()) {
		dword w = stou(~s + 1, NULL, 16);
		return Color(LOBYTE(HIWORD(w)), HIBYTE(w), LOBYTE(w));
	}
	return p;
}

Color ThemeProperties::operator()(Color p)
{
	return operator()("Color", p);
}

String ThemeProperties::operator()(const char *id, const String& text)
{
	return CParser(Raw(id, AsCString(text))).ReadString();
}

String ThemeProperties::operator()(const String& text)
{
	return operator()("Text", text);
}

Font ThemeProperties::operator()(const char *id, Font fnt)
{
	String s;
	s << AsCString(fnt.GetFaceName()) << ' ' << fnt.GetHeight();
	if(fnt.IsBold())
		s << " bold";
	if(fnt.IsItalic())
		s << " italic";
	if(fnt.IsUnderline())
		s << " underline";	
	s = Raw(id, s);
	CParser p(s);
	fnt = StdFont();
	fnt.FaceName(p.ReadString());
	fnt.Height(p.ReadInt());
	while(p) {
		if(p.Id("bold"))
			fnt.Bold();
		else
		if(p.Id("italic"))
			fnt.Italic();
		else
		if(p.Id("underline"))
			fnt.Underline();
		else
			p.ThrowError("Invalid font");
	}
	return fnt;
}

Font ThemeProperties::operator()(Font fnt)
{
	return operator()("Font", fnt);
}

Image ThemeProperties::operator()(const char *id, const Image& img)
{
	if(loading)
		return image.Get(id, img);
	image.GetAdd(id) = img;
	return img;
}

Image ThemeProperties::operator()(const Image& img)
{
	return operator()("Image", img);
}
