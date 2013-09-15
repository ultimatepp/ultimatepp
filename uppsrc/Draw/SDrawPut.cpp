#include "Draw.h"

NAMESPACE_UPP

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

void SDraw::PutImage(Point p, const Image& img, const Rect& src, Color color)
{
	if(!IsNull(color)) {
		sColorize m;
		m.img = img;
		m.color = color;
		PutImage(p, MakeImage(m), src);
	}
	else
		PutImage(p, img, src);
}

void SDraw::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	Rect sr(Point(x, y) + cloff.Top().offset, (src & img.GetSize()).GetSize());
	const Vector<Rect>& clip = cloff.Top().clip;
	for(int i = 0; i < clip.GetCount(); i++) {
		Rect cr = clip[i] & sr;
		if(!cr.IsEmpty())
			PutImage(cr.TopLeft(), img, Rect(cr.TopLeft() - sr.TopLeft() + src.TopLeft(), cr.GetSize()), color);
	}
}

void SDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	SysDrawImageOp(x, y, img, img.GetSize(), color);
}

void SDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
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

void SDraw::PutHorz(int x, int y, int cx)
{
	DrawRect(x, y, cx, 1, docolor);
}

void SDraw::PutVert(int x, int y, int cy)
{
	DrawRect(x, y, 1, cy, docolor);
}

END_UPP_NAMESPACE
