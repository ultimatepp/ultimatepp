#include "glyph.h"

struct sColorize : public ImageMaker
{
	Image img;
	Color color;

	virtual String Key() const {
		StringBuffer h;
		RawCat(h, color);
		RawCat(h, img.GetSerialId());
		return h;
	}

	virtual Image Make() const {
		return SetColorKeepAlpha(img, color);
	}

};

void FDraw::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	if(!IsNull(color)) {
		sColorize m;
		m.img = img;
		m.color = color;
		SysDrawImageOp(x, y, MakeImage(m), src, Null);
		return;
	}
	Rect sr(Point(x, y) + cloff.Top().offset, (src & img.GetSize()).GetSize());
	const Vector<Rect>& clip = cloff.Top().clip;
	for(int i = 0; i < clip.GetCount(); i++) {
		Rect cr = clip[i] & sr;
		if(!cr.IsEmpty())
			PutImage(cr.TopLeft(), img, Rect(cr.TopLeft() - sr.TopLeft() + src.TopLeft(), cr.GetSize()));
	}
}

void FDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	SysDrawImageOp(x, y, img, img.GetSize(), color);
}

void FDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	Rect r = RectC(x, y, cx, cy);
	r += cloff.Top().offset;
	const Vector<Rect>& clip = cloff.Top().clip;
	for(int i = 0; i < clip.GetCount(); i++) {
		Rect cr = clip[i] & r;
		if(!cr.IsEmpty())
			PutRect(cr, color);
	}
}

void FDraw::PutHorz(int x, int y, int cx)
{
	RTIMING("PutHorz");
	DrawRect(x, y, cx, 1, docolor);
}

void FDraw::PutVert(int x, int y, int cy)
{
	RTIMING("PutVert");
	DrawRect(x, y, 1, cy, docolor);
}
