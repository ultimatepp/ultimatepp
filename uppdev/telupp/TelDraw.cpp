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

Index<int64> TelDraw::img_index;

int TelDraw::GetImageI(TelDraw& w, const Image& img)
{
	int id = img.GetSerialId();
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

void TelDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	int i = GetImageI(*this, img);
	Put8(IMAGE);
	Put16(i);
	Put(p);
	Put(src);
}

void TelDraw::PutRect(const Rect& r, Color color)
{ // TODO: Support InvertColor
	Put8(RECT);
	Put(r);
	Put8(color.GetR());
	Put8(color.GetG());
	Put8(color.GetB());
}
