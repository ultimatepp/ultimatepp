#include "Telpp.h"

NAMESPACE_UPP

void SystemDraw::Put16(int x)
{
	result.Cat(LOBYTE(x));
	result.Cat(HIBYTE(x));
}

void SystemDraw::Put32(int x)
{
	Put16(LOWORD(x));
	Put16(HIWORD(x));
}

void SystemDraw::Put(Point p)
{// TODO: Clamp?
	Put16(p.x);
	Put16(p.y);
}

void SystemDraw::Put(Size sz)
{
	Put((Point)sz);
}

void SystemDraw::Put(const Rect& r)
{
	Put(r.TopLeft());
	Put(r.GetSize());
}

void SystemDraw::Put(const String& s)
{
	Put32(s.GetLength());
	result.Cat(s);
}

Index<int64> SystemDraw::img_index;

int SystemDraw::GetImageI(SystemDraw& w, const Image& img)
{
	int64 id = img.GetSerialId();
	int q = img_index.Find(id);
	if(q < 0) { // TODO: Implement some sort of victim elimination
		q = img_index.GetCount();
		img_index.Add(id);
		w.Put8(SETIMAGE);
		w.Put16(q);
		w.Put(img.GetSize());
		const RGBA *end = ~img + img.GetLength();
		for(const RGBA *s = ~img; s < end; s++) {
			w.Put8(s->r);
			w.Put8(s->g);
			w.Put8(s->b);
			w.Put8(s->a);
		}
	}
	return q;
}

void SystemDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	int i = GetImageI(*this, img);
	Put8(IMAGE);
	Put16(i);
	Put(p);
	Put(src);
}

void SystemDraw::PutRect(const Rect& r, Color color)
{ // TODO: Support InvertColor
	if(color == InvertColor()) {
		Put8(INVERTRECT);
		Put(r);
	}
	else {
		Put8(RECT);
		Put(r);
		Put8(color.GetR());
		Put8(color.GetG());
		Put8(color.GetB());
	}
}

END_UPP_NAMESPACE
