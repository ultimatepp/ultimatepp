#include "IconDes.h"

NAMESPACE_UPP

struct sFloodFill {
	Rect         rc;
	Size         sz;
	Buffer<byte> flag;
	ImageBuffer& ib;
	RGBA         scolor;
	RGBA         color;
	bool         done;

	RGBA& At(int x, int y)         { return ib[y + rc.top][x + rc.left]; }
	bool  Eq(int x, int y, RGBA c) { RGBA& q = At(x, y); return (q.a | c.a) == 0 || c == q; }
	byte& Flag(int x, int y) { return flag[y * sz.cx + x]; }
	void  Fill(RGBA color, Point pt, const Rect& rc);
	void  Try(int x, int y);

	sFloodFill(ImageBuffer& ib) : ib(ib) {}
};

void sFloodFill::Try(int x, int y)
{
	if(x >= 0 && x < sz.cx && y >= 0 && y < sz.cy && Flag(x, y) == 0 && Eq(x, y, scolor)) {
		Flag(x, y) = 1;
		At(x, y) = color;
		done = false;
	}
}

void sFloodFill::Fill(RGBA _color, Point pt, const Rect& _rc)
{
	rc = _rc & ib.GetSize();
	if(!rc.Contains(pt))
		return;
	scolor = ib[pt.y][pt.x];
	color = _color;
	sz = rc.GetSize();
	flag.Alloc(sz.cx * sz.cy, 0);
	pt -= rc.TopLeft();
	Flag(pt.x, pt.y) = 1;
	At(pt.x, pt.y) = color;
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

void FloodFill(ImageBuffer& img, RGBA color, Point pt, const Rect& rc)
{
	sFloodFill(img).Fill(color, pt, rc);
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

String PackImlData(const Vector<Image>& image)
{
	StringBuffer block;
	for(int i = 0; i < image.GetCount(); i++) {
		const Image& img = image[i];
		StringStream ss;
		ss.Put(img.GetResolution() << 6);
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
	return ZCompress(block);
}

Image DownSample3x(const Image& src)
{
	Size tsz = src.GetSize() / 3;
	ImageBuffer ib(tsz);
	int w = src.GetSize().cx;
	int w2 = 2 * w;
	for(int y = 0; y < tsz.cy; y++) {
		RGBA *t = ib[y];
		RGBA *e = t + tsz.cx;
		const RGBA *s = src[3 * y];
		while(t < e) {
			int r, g, b, a;
			const RGBA *q;
			r = g = b = a = 0;
#define S__SUM(delta) q = s + delta; r += q->r; g += q->g; b += q->b; a += q->a;
			S__SUM(0) S__SUM(1) S__SUM(2)
			S__SUM(w + 0) S__SUM(w + 1) S__SUM(w + 2)
			S__SUM(w2 + 0) S__SUM(w2 + 1) S__SUM(w2 + 2)
#undef  S__SUM
			t->a = a / 9;
			t->r = r / 9;
			t->g = g / 9;
			t->b = b / 9;
			t++;
			s += 3;
		}
	}
	return ib;
}

END_UPP_NAMESPACE
