#include "RichText.h"

namespace Upp {

struct RichDiagram : public RichObjectType {
	virtual String GetTypeName(const Value& v) const;
	virtual Value  Read(const String& s) const;
	virtual String Write(const Value& v) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;

	virtual void   Paint(const Value& data, Draw& w, Size sz, Color, void *) const;
	virtual Image  ToImage(int64, const Value& data, Size sz, Color, void *) const;

	static Image GetImage(const Value &data, Size sz);
	static bool  Load(Diagram& m, const Value& zqdf);
};

String RichDiagram::GetTypeName(const Value& v) const
{
	return "qdf";
}

Value RichDiagram::Read(const String& s) const
{
	return s;
}

String RichDiagram::Write(const Value& v) const
{
	return v;
}

bool RichDiagram::Load(Diagram &m, const Value &zqdf)
{
	String qdf = ZDecompress(~zqdf);
	try {
		CParser p(qdf);
		m.Load(p);
	}
	catch(CParser::Error) {
		return false;
	}
	return true;
}

Size RichDiagram::GetPhysicalSize(const Value& data) const
{
	Diagram m;
	if(Load(m, data)) {
		Zoom z = GetRichTextStdScreenZoom();
		return z.d * (Size)m.GetSize() / z.m;
	}
	return Size(0, 0);
}

Size RichDiagram::GetPixelSize(const Value& data) const
{
	Diagram m;
	return Load(m, ~data) ? m.GetSize() : Size(0, 0);
}

Image RichDiagram::GetImage(const Value& data, Size sz)
{
	Diagram m;
	if(Load(m, data)) {
		Size isz = m.GetSize();
		ImagePainter sw(sz);
		sw.Clear();
		sw.Scale((double)sz.cx / isz.cx, (double)sz.cy / isz.cy);
		Diagram::PaintInfo pi;
		m.Paint(sw, pi);
		return sw;
	}
	return Null;
}


void RichDiagram::Paint(const Value& data, Draw& w, Size sz, Color ink, void *) const
{
	w.DrawImage(0, 0, GetImage(data, sz));
}

Image RichDiagram::ToImage(int64 serial_id, const Value& data, Size sz, Color ink, void *) const
{
	struct DiagramLoader : ImageMaker {
		int64  serial_id;
		String data;
		Size   sz;

		virtual String Key() const  { String x; RawCat(x, serial_id); RawCat(x, data); RawCat(x, sz); return x; }
		virtual Image  Make() const {
			return GetImage(data, sz);
		}
	} loader;
	loader.serial_id = serial_id;
	loader.data = ~data;
	loader.sz = sz;
	return MakeImage(loader);
}

INITBLOCK {
	RichObject::Register("qdf", &Single<RichDiagram>());
};

};