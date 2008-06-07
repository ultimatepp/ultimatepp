#include "TCtrlLibHelp.h"
#pragma hdrstop

#include <TDraw/formula.h>

NAMESPACE_UPP

struct RichObjectTypeFormulaCls : public RichObjectType
{
	virtual String GetTypeName(const Value&) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const { return Size(0, 0); }
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual Value  Read(const String& s) const;
	virtual String Write(const Value& v) const;
//	virtual Value  ReadClipboard() const;
//	virtual void   Menu(Bar& bar, RichObjectExchange& ex) const;
	virtual void   DefaultAction(RichObject& ex) const;

	struct Data {
		Data() : font(Roman(100)), color(SBlack) {}
		Data(const Value& v) { *this = ValueTo<Data>(v); }

		operator           Value() const { return RawToValue(*this); }

		void               Serialize(Stream& stream);
		RefCon<Formula>    Parse() const;

		String             text;
		Font               font;
		Color              color;
	};
};

RichObjectType *RichObjectTypeFormula() { return &Single<RichObjectTypeFormulaCls>(); }

void RichObjectTypeFormulaCls::Data::Serialize(Stream& stream)
{
	int version = 1;
	stream % version % text % font % color;
}

RefCon<Formula> RichObjectTypeFormulaCls::Data::Parse() const
{
	return ParseFormula(text, font, color);
}

String RichObjectTypeFormulaCls::GetTypeName(const Value&) const
{
	return "Formula";
}

/*
RichObject CreateFormulaObject(String text, Font font, Color color)
{
	RichObjectTypeFormulaCls::Data data;
	data.text = text;
	data.font = font;
	data.color = color;
	RichObject obj(RichObjectTypeFormula(), data);
	obj.SetYDelta(data.Parse() -> GetDescent());
	return obj;
}
*/

Size RichObjectTypeFormulaCls::GetPhysicalSize(const Value& data) const
{
	if(IsTypeRaw<Data>(data))
		return Data(data).Parse() -> GetSize();
	return Size(0, 0);
}

Value RichObjectTypeFormulaCls::Read(const String& s) const
{
	Data data;
	StringStream ss(s);
	ss % data;
	if(ss.IsError())
		return Value();
	return RawToValue(data);
}

String RichObjectTypeFormulaCls::Write(const Value& v) const
{
	if(IsTypeRaw<Data>(v)) {
		StringStream ss;
		Data d(v);
		ss % d;
		return ss;
	}
	return Null;
}

void RichObjectTypeFormulaCls::Paint(const Value& data, Draw& w, Size sz) const
{
	w.DrawRect(sz, White);
	if(IsTypeRaw<Data>(data)) {
		RefCon<Formula> form = Data(data).Parse();
		DrawingDraw dd(form -> GetSize());
		form -> Paint(dd, 0, form -> GetAscent());
		w.DrawDrawing(Rect(sz), dd);
	}
}

bool RunDlgFormula(String& text, Font& font, Color& color);

void RichObjectTypeFormulaCls::DefaultAction(RichObject& ex) const
{
	static Data data;
	data.text = Null;
	RichObject obj = ex;
	if(IsTypeRaw<Data>(obj.GetData()))
		data = Data(obj.GetData());
	if(RunDlgFormula(data.text, data.font, data.color)) {
		RichObject ro(RichObjectTypeFormula(), data);
		RefCon<Formula> form = data.Parse();
		ro.SetYDelta(form -> GetDescent() + Formula::GetTextDeltaY(data.font) + 15);
		ex = ro;
	}
}

END_UPP_NAMESPACE
