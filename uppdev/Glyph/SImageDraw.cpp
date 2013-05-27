#include "glyph.h"

void SImageDraw::PutImage(Point p, const Image& m, const Rect& src)
{
	Over(b, p, m, src);
}

void SImageDraw::PutRect(const Rect& r, Color color)
{
	Upp::Fill(b, r, color);
}

SImageDraw::SImageDraw(Size sz)
{
	Init(sz);
	b.Create(sz);
}

SImageDraw::SImageDraw(int cx, int cy)
{
	Init(Size(cx, cy));
	b.Create(cx, cy);
}

SImageDraw::~SImageDraw()
{
}
