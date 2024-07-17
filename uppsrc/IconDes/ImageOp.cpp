#include "IconDes.h"

namespace Upp {

struct sFloodFill {
	Size         sz;
	Buffer<byte> flag;
	Image        source;
	ImageBuffer& target;
	RGBA         scolor;
	RGBA         color;
	bool         done;
	int          tolerance;

	RGBA& At(int x, int y)         { return target[y][x]; }
	bool  Eq(int x, int y);
	byte& Flag(int x, int y) { return flag[y * sz.cx + x]; }
	void  Fill(RGBA color, Point pt, int tolerance_);
	void  Try(int x, int y);

	sFloodFill(const Image& source, ImageBuffer& target) : source(source), target(target) { tolerance = 0; }
};

force_inline
bool sFloodFill::Eq(int x, int y)
{
	const RGBA& q = source[y][x];
	if(tolerance < 0 && q.a)
		return q.a != scolor.a || q.r != scolor.r || q.g != scolor.g || q.b != scolor.b;
	if((q.a | scolor.a) == 0) return true;
	return abs(q.a - scolor.a) <= tolerance && abs(q.r - scolor.r) + abs(q.g - scolor.g) + abs(q.b - scolor.b) <= tolerance;
}

force_inline
void sFloodFill::Try(int x, int y)
{
	if(x >= 0 && x < sz.cx && y >= 0 && y < sz.cy && Flag(x, y) == 0 && Eq(x, y)) {
		Flag(x, y) = 1;
		target[y][x] = color;
		done = false;
	}
}

void sFloodFill::Fill(RGBA _color, Point pt, int tolerance_)
{
	tolerance = tolerance_;
	scolor = tolerance < 0 ? _color : source[pt.y][pt.x];
	color = tolerance < 0 ? RGBAZero() : _color;
	sz = source.GetSize();
	flag.Alloc(sz.cx * sz.cy, 0);
	Flag(pt.x, pt.y) = 1;
	target[pt.y][pt.x] = color;
	do {
		done = true;
		for(int y = 0; y < sz.cy; y++)
			for(int x = 0; x < sz.cx; x++) {
				if(Flag(x, y) == 1) {
					Flag(x, y) = 2;
					Try(x + 1, y);
					Try(x - 1, y);
					Try(x, y + 1);
					Try(x, y - 1);
				}
			}
	}
	while(!done);
}

void FloodFill(const Image& source, ImageBuffer& target, RGBA color, Point pt, int tolerance)
{
	sFloodFill(source, target).Fill(color, pt, tolerance);
}

struct InterpolateFilter : ImageFilter9 {
	int todo;

	virtual RGBA operator()(const RGBA **mx);
};

RGBA InterpolateFilter::operator()(const RGBA **mx)
{
	RGBA t = mx[1][1];
	int na = mx[0][1].a, wa = mx[1][0].a, ea = mx[1][2].a, sa = mx[2][1].a;
	int suma = na + wa + ea + sa, half = suma >> 1;
	if(suma > 0) {
		t.r = (mx[0][1].r * na + mx[1][0].r * wa + mx[1][2].r * ea + mx[2][1].r * sa + half) / suma;
		t.g = (mx[0][1].g * na + mx[1][0].g * wa + mx[1][2].g * ea + mx[2][1].g * sa + half) / suma;
		t.b = (mx[0][1].b * na + mx[1][0].b * wa + mx[1][2].b * ea + mx[2][1].b * sa + half) / suma;
		t.a = max(max(na, sa), max(wa, ea));
		if(t != mx[1][1])
			todo++;
	}
	return t;
}

void InterpolateImage(Image& img, const Rect& _rc)
{
	Rect rc = _rc & img.GetSize();
	Image m = Crop(img, rc);
	TimeStop tm;
	Image imp = CreateImage(rc.GetSize(), Null);
	Over(imp, Point(0, 0), m, m.GetSize());
	Progress pi("Interpolating...");
	for(int qq = 0; qq < 2000; qq++) {
		InterpolateFilter f;
		f.todo = 0;
		imp = Filter(imp, f);
		Over(imp, Point(0, 0), m, m.GetSize());
		if(f.todo == 0)
			break;
		if(pi.SetCanceled(qq, 2000))
			break;
	}
	Copy(img, rc.TopLeft(), imp, imp.GetSize());
}

void MirrorHorz(Image& img, const Rect& rect)
{
	ImageBuffer ib(img);
	Rect rc = rect & ib.GetSize();
	for(int y = rc.top; y < rc.bottom; y++) {
		RGBA *b = ib[y] + rc.left;
		RGBA *e = ib[y] + rc.right - 1;
		while(b < e) {
			Swap(*b, *e);
			b++;
			e--;
		}
	}
	img = ib;
}

void MirrorVert(Image& img, const Rect& rect)
{
	ImageBuffer ib(img);
	Rect rc = rect & ib.GetSize();
	int b = rc.top;
	int e = rc.bottom - 1;
	int n = rc.right - rc.left;
	if(n > 0) {
		Buffer<RGBA> h(n);
		n *= sizeof(RGBA);
		while(b < e) {
			memcpy(h, ib[b] + rc.left, n);
			memcpy(ib[b] + rc.left, ib[e] + rc.left, n);
			memcpy(ib[e] + rc.left, h, n);
			b++;
			e--;
		}
	}
	img = ib;
}

String PackImlDataUncompressed(const Vector<ImageIml>& image)
{
	StringBuffer block;
	for(const ImageIml& m : image) {
		const Image& img = m.image;
		StringStream ss;
		ss.Put(m.flags);
		Size sz = img.GetSize();
		ss.Put16le(sz.cx);
		ss.Put16le(sz.cy);
		Point p = img.GetHotSpot();
		ss.Put16le(p.x);
		ss.Put16le(p.y);
		p = img.Get2ndSpot();
		ss.Put16le(p.x);
		ss.Put16le(p.y);
		block.Cat(ss.GetResult());
		const RGBA *s = img;
		const RGBA *e = s + img.GetLength();
		while(s < e) {
			block.Cat(s->r);
			block.Cat(s->g);
			block.Cat(s->b);
			block.Cat(s->a);
			s++;
		}
	}
	return block;
}

String PackImlData(const Vector<ImageIml>& image)
{
	return ZCompress(PackImlDataUncompressed(image));
}

}
