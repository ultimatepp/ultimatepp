#include "glyph.h"

void FDraw::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	Point p = Point(x, y) + state.Top().offset;
	Rect sr = src + p;
	const Vector<Rect>& clip = state.Top().clip;
	for(int i = 0; i < clip.GetCount(); i++) {
		Rect cr = clip[i] & r;
		
	}
}

void FDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	SysDrawImageOp(x, y, img, img.GetSize(), color);
}

void FDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	RectC r(x, y, cx, cy);
	r += state.Top().offset;
	const Vector<Rect>& clip = state.Top().clip;
	for(int i = 0; i < clip.GetCount(); i++) {
		Rect cr = clip[i] & r;
		if(!cr.IsEmpty())
			PutRect(cr, color);
	}
}

