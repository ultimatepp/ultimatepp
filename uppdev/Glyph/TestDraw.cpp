#include "glyph.h"

#define LTIMING(x) RTIMING(x)

void TestDraw::PutImage(Point p, const Image& m, const Rect& src)
{
	draw->DrawImage(p.x, p.y, m, src);
}

void TestDraw::PutRect(const Rect& r, Color color)
{
	draw->DrawRect(r, color);
}

Image TestDraw::RenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz)
{
	ImageBuffer ib(sz);
	BufferPainter sw(ib);
	sw.Clear(RGBAZero());
	sw.DrawText(at.x, at.y, angle, WString(chr, 1), fnt, color);
	return ib;
}
