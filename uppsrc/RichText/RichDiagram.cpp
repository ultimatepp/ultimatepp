#include "RichText.h"

namespace Upp {

struct RichDiagram : public RichObjectType {
	String GetTypeName(const Value& v) const override;
	Value  Read(const String& s) const override;
	String Write(const Value& v) const override;
	Size   GetPhysicalSize(const Value& data) const override;
	Size   GetPixelSize(const Value& data) const override;

	void   Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const override;
	Image  ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo& pi) const override;

	static Image GetImage(const Value &data, Size sz, bool dark);
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

Image RichDiagram::GetImage(const Value& data, Size sz, bool dark)
{
	Diagram m;
	if(Load(m, data)) {
		Size isz = m.GetSize();
		ImagePainter sw(sz);
		sw.Clear();
		sw.Scale((double)sz.cx / isz.cx, (double)sz.cy / isz.cy);
		Diagram::PaintInfo pi;
		pi.dark = dark;
		m.Paint(sw, pi);
		return sw;
	}
	return Null;
}


void RichDiagram::Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const
{
	w.DrawImage(0, 0, ToImage(0, data, sz, pi));
}

Image RichDiagram::ToImage(int64 serial_id, const Value& data, Size sz, const RichObjectPaintInfo& pi) const
{
	struct DiagramLoader : ImageMaker {
		int64  serial_id;
		String data;
		Size   sz;
		bool   dark;

		virtual String Key() const  { String x; RawCat(x, serial_id); RawCat(x, data); RawCat(x, sz); RawCat(x, dark); return x; }
		virtual Image  Make() const {
			return GetImage(data, sz, dark);
		}
	} loader;
	loader.serial_id = serial_id;
	loader.data = ~data;
	loader.sz = sz;
	loader.dark = pi.dark;
	return MakeImage(loader);
}

INITBLOCK {
	RichObject::Register("qdf", &Single<RichDiagram>());
};

};