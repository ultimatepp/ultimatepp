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
