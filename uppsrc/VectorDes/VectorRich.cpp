#include "VectorDes.h"
#pragma hdrstop

struct RichObjectTypeVectorImageCls : public RichObjectType
{
	virtual String GetTypeName(const Value&) const;
	virtual String GetCreateName() const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const { return Size(0, 0); }
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual Value  Read(const String& s) const;
	virtual String Write(const Value& v) const;
//	virtual Value  ReadClipboard() const;
//	virtual void   Menu(Bar& bar, RichObjectExchange& ex) const;
	virtual void   DefaultAction(RichObject& ex) const;
	virtual String GetLink(const Value& data, Point pt, Size sz) const;

	struct Data : DeepCopyOption<Data> {
		Data() {}
		Data(const Value& v) { *this = ValueTo<Data>(v); }

		operator           Value() const { return RawToValue(*this); }

		void               Serialize(Stream& stream);

		Color              background;
		VectorImage        image;
	};
};

RichObjectType *RichObjectTypeVectorImage() { return &Single<RichObjectTypeVectorImageCls>(); }

void RichObjectTypeVectorImageCls::Data::Serialize(Stream& stream)
{
	int version = 1;
	stream / version % background;
	image.SerializeVM(stream);
}

String RichObjectTypeVectorImageCls::GetTypeName(const Value&) const
{
	return "VectorImage";
}

String RichObjectTypeVectorImageCls::GetCreateName() const
{
	return "Vector drawing";
}

/*
RichObject CreateVectorImageObject(String text, Font font, Color color)
{
	RichObjectTypeVectorImageCls::Data data;
	data.text = text;
	data.font = font;
	data.color = color;
	RichObject obj(RichObjectTypeVectorImage(), data);
	obj.SetYDelta(data.Parse() -> GetDescent());
	return obj;
}
*/

String RichObjectTypeVectorImageCls::GetLink(const Value& data, Point pt, Size sz) const
{
	if(IsTypeRaw<Data>(data))
		return Data(data).image.GetLink(pt, sz);
	return Value();
}

Size RichObjectTypeVectorImageCls::GetPhysicalSize(const Value& data) const
{
	if(IsTypeRaw<Data>(data))
		return Data(data).image.GetSize() * 6;
	return Size(0, 0);
}

Value RichObjectTypeVectorImageCls::Read(const String& s) const
{
	Data data;
	StringStream ss(s);
	ss % data;
	if(ss.IsError())
		return Value();
	return RawToValue(data);
}

String RichObjectTypeVectorImageCls::Write(const Value& v) const
{
	if(IsTypeRaw<Data>(v)) {
		StringStream ss;
		Data d(v);
		ss % d;
		return ss;
	}
	return Null;
}

void RichObjectTypeVectorImageCls::Paint(const Value& data, Draw& w, Size sz) const
{
	w.DrawRect(sz, White);
	if(IsTypeRaw<Data>(data)) {
		VectorImage form = Data(data).image;
		ScalingDraw sdraw(&w, Scaling(form.GetSize(), sz));
		form.Paint(sdraw);
	}
}

void RichObjectTypeVectorImageCls::DefaultAction(RichObject& ex) const
{
	bool RunDlgVectorImage(String& text, Font& font, Color& color);
	static Data data;
	RichObject obj = ex;
	if(IsTypeRaw<Data>(obj.GetData()))
		data = Data(obj.GetData());
	data.image.Clone();
	if(RunVectorImageEditor(data.image)) {
		RichObject ro(RichObjectTypeVectorImage(), data);
		ex = ro;
	}
}
