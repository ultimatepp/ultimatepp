#include "CtrlCore.h"

namespace Upp {

PackedData& Ctrl::Attrs()
{
	if(layout_id_literal) {
		String layout_id((const char *)attrs.GetRawPtr());
		attrs.SetRawPtr(nullptr);
		attrs.SetString(ATTR_LAYOUT_ID, layout_id);
		layout_id_literal = false;
	}
	return attrs;
}

void Ctrl::SetTextAttr(int ii, const char *s)
{
	Attrs().SetString(ii, s);
}

void Ctrl::SetTextAttr(int ii, const String& s)
{
	Attrs().SetString(ii, s);
}

String Ctrl::GetTextAttr(int ii) const
{
	return layout_id_literal ? String() : attrs.GetString(ii);
}

Ctrl& Ctrl::Tip(const char *txt)
{
	SetTextAttr(ATTR_TIP, txt);
	return *this;
}

Ctrl& Ctrl::HelpLine(const char *txt)
{
	SetTextAttr(ATTR_HELPLINE, txt);
	return *this;
}

Ctrl& Ctrl::Description(const char *txt)
{
	SetTextAttr(ATTR_DESCRIPTION, txt);
	return *this;
}

Ctrl& Ctrl::HelpTopic(const char *txt)
{
	SetTextAttr(ATTR_HELPTOPIC, txt);
	return *this;
}

Ctrl& Ctrl::LayoutId(const char *txt)
{
	SetTextAttr(ATTR_LAYOUT_ID, txt);
	return *this;
}

Ctrl& Ctrl::LayoutIdLiteral(const char *txt)
{
	if(attrs.GetRawPtr() && !layout_id_literal)
		LayoutId(txt);
	else {
		attrs.SetRawPtr((void *)txt);
		layout_id_literal = true;
	}
	return *this;
}

String Ctrl::GetLayoutId() const
{
	if(layout_id_literal)
		return (const char *)attrs.GetRawPtr();
	return GetTextAttr(ATTR_LAYOUT_ID);
}

String Ctrl::GetTip() const
{
	return GetTextAttr(ATTR_TIP);
}

String Ctrl::GetHelpLine() const
{
	return GetTextAttr(ATTR_HELPLINE);
}

String Ctrl::GetDescription() const
{
	return GetTextAttr(ATTR_DESCRIPTION);
}

String Ctrl::GetHelpTopic() const
{
	return GetTextAttr(ATTR_HELPTOPIC);
}

void Ctrl::ClearInfo()
{
	if(layout_id_literal)
		attrs.SetRawPtr(nullptr);
	layout_id_literal = false;
	attrs.Clear();
}

void Ctrl::SetColorAttr(int ii, Color c)
{
	Attrs();
	if(IsNull(c))
		attrs.SetNull(ii);
	else
		attrs.SetDword(ii, c.GetRaw());
}

Color Ctrl::GetColorAttr(int ii) const
{
	if(layout_id_literal)
		return Null;
	static dword nullval = Color(Null).GetRaw();
	return Color::FromRaw(attrs.GetDword(ii, nullval));
}

void Ctrl::SetFontAttr(int ii, Font fnt)
{
	Attrs();
	if(IsNull(fnt))
		attrs.SetNull(ii);
	else
		attrs.SetInt64(ii, fnt.AsInt64());
}

Font Ctrl::GetFontAttr(int ii) const
{
	if(layout_id_literal)
		return Null;
	static int64 nullval = Font(Null).AsInt64();
	return Font::FromInt64(attrs.GetInt64(ii, nullval));
}

void Ctrl::SetIntAttr(int ii, int val)
{
	Attrs().SetInt(ii, val);
}

int Ctrl::GetIntAttr(int ii, int def) const
{
	if(layout_id_literal)
		return def;
	return attrs.GetInt(ii, def);
}

void Ctrl::SetInt64Attr(int ii, int64 val)
{
	Attrs().SetInt64(ii, val);
}

int64 Ctrl::GetInt64Attr(int ii, int64 def) const
{
	if(layout_id_literal)
		return def;
	return attrs.GetInt64(ii, def);
}


void Ctrl::SetVoidPtrAttr(int ii, const void *ptr)
{
	if(ptr)
		Attrs().SetPtr(ii, (void *)ptr);
	else
		Attrs().SetNull(ii);
}

void *Ctrl::GetVoidPtrAttr(int ii) const
{
	if(layout_id_literal)
		return NULL;
	return attrs.GetPtr(ii);
}

};