#include "telupp.h"

void TelDraw::Put16(int x)
{
	result.Cat(LOBYTE(x));
	result.Cat(HIBYTE(x));
}

void TelDraw::Put(Point p)
{// TODO: Clamp?
	Put16(p.x);
	Put16(p.y);
}

void TelDraw::Put(Size sz)
{
	Put((Point)sz);
}

void TelDraw::Put(const Rect& r)
{
	Put(r.TopLeft());
	Put(r.GetSize());
}

void TelDraw::PutImage(Point p, const Image& img, const Rect& src)
{
}

void TelDraw::PutRect(const Rect& r, Color color)
{
	Put8(DRAW_RECT);
	Put(r);
	Put8(color.GetR());
	Put8(color.GetG());
	Put8(color.GetB());
}
