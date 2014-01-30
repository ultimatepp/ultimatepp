#include <CtrlCore/CtrlCore.h>

#ifdef GUI_TURTLE

#define LLOG(x) // LOG(x)

NAMESPACE_UPP

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
		LLOG("SetImage " << q + from << ", size: " << img.GetLength());
		Ctrl::Put8(SETIMAGE);
		Ctrl::Put16(q + from);
		Ctrl::Put(img.GetSize());
		const RGBA *end = ~img + img.GetLength();
		for(const RGBA *s = ~img; s < end; s++) {
			Ctrl::Put8(s->r);
			Ctrl::Put8(s->g);
			Ctrl::Put8(s->b);
			Ctrl::Put8(s->a);
		}
		Ctrl::stat_setimage++;
		Ctrl::stat_setimage_len += img.GetLength() * sizeof(RGBA);
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
	LLOG("Ctrl::PutImage " << p << ", size: " << img.GetSize() << ", src: " << src << ", id: " << img.GetSerialId());
	Ctrl::stat_putimage++;
	int i = GetImageI(img);
	if(Rect(img.GetSize()) == src) {
		Point dp = p - pos;
		if(abs(dp.x) < 256 && abs(dp.y) < 256) {
			Ctrl::Put8(dp.x < 0 ? dp.y < 0 ? IMAGENN : IMAGENP : dp.y < 0 ? IMAGEPN : IMAGEPP);
			Ctrl::Put8(abs(dp.x));
			Ctrl::Put8(abs(dp.y));
			Ctrl::Put16(i);
			pos = p;
			return;
		}
	}
	Ctrl::Put8(IMAGE);
	Ctrl::Put16(i);
	Ctrl::Put(p);
	Ctrl::Put(src);
	pos = p;
}

void SystemDraw::PutRect(const Rect& r, Color color)
{
	LLOG("Ctrl::PutRect " << r << ", color " << color);
	Ctrl::stat_putrect++;
	Point p = r.TopLeft();
	if(color == InvertColor()) {
		Ctrl::Put8(INVERTRECT);
		Ctrl::Put(r);
	}
	else {
		Size sz = r.GetSize();
		Point dp = p - pos;
		if(abs(dp.x) < 256 && abs(dp.y) < 256 && sz.cx < 256 && sz.cy < 256 && 0) {
			Ctrl::Put8(dp.x < 0 ? dp.y < 0 ? RECTNN : RECTNP : dp.y < 0 ? RECTPN : RECTPP);
			Ctrl::Put8(abs(dp.x));
			Ctrl::Put8(abs(dp.y));
			Ctrl::Put8(sz.cx);
			Ctrl::Put8(sz.cy);
		}
		else {
			Ctrl::Put8(RECT);
			Ctrl::Put(r);
		}
		Ctrl::Put8(color.GetR());
		Ctrl::Put8(color.GetG());
		Ctrl::Put8(color.GetB());
	}
	pos = p;
}

END_UPP_NAMESPACE

#endif
