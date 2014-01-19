#include <CtrlCore/CtrlCore.h>

#ifdef GUI_TURTLE

NAMESPACE_UPP

void SystemDraw::Put16(int x)
{
	Put8(LOBYTE(x));
	Put8(HIBYTE(x));
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
	turtle_stream.Put(s);
}

Index<int64> SystemDraw::img_index[3];

int SystemDraw::GetImageI(int from, Index<int64>& img_index, int maxcount, const Image& img)
{
	int64 id = img.GetSerialId();
	int q = img_index.Find(id);
	if(q < 0) {
		if(img_index.GetCount() < maxcount) {
			q = img_index.GetCount();
			img_index.Add(id);
		}
		else {
			q = Random(maxcount);
			img_index.Set(q, id);
		}
		Put8(SETIMAGE);
		Put16(q + from);
		Put(img.GetSize());
		const RGBA *end = ~img + img.GetLength();
		for(const RGBA *s = ~img; s < end; s++) {
			Put8(s->r);
			Put8(s->g);
			Put8(s->b);
			Put8(s->a);
		}
	}
	return q + from;
}

int SystemDraw::GetImageI(const Image& img)
{
	int area = img.GetWidth() * img.GetHeight();
	return area <= 64*64     ? GetImageI(0, img_index[0], 2048, img) :
	       area <= 512 * 512 ? GetImageI(2048, img_index[1], 64, img) :
	                           GetImageI(2048 + 64, img_index[2], 8, img);
}

void SystemDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	int i = GetImageI(img);
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

#endif