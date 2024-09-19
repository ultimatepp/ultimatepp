#include "IconDes.h"

template <class Eq>
force_inline
void FloodFill(const Image& src, ImageBuffer& ib, RGBA fill, Point p, const Eq& eq)
{
	Size isz = ib.GetSize();
	if(!Rect(isz).Contains(p))
		return;

	Vector<Point> stack { p };
	Buffer<byte> filled(isz.cx * isz.cy, 0);
	auto Filled = [&](int y) -> byte * { return ~filled + y * isz.cx; };
	
	RGBA color = src[p.y][p.x];
	while(stack.GetCount()) {
		Point p = stack.Pop();
		const RGBA *l = src[p.y];
		byte *f = Filled(p.y);
		int xl = p.x;
		while(xl > 0 && !f[xl - 1] && eq(l[xl - 1], color))
			xl--;
		int xh = p.x;
		while(xh < isz.cx - 1 && !f[xh + 1] && eq(l[xh + 1], color))
			xh++;
		Fill(ib[p.y] + xl, fill, xh - xl + 1);
		memset(f + xl, 1, xh - xl + 1);
		Point q;
		for(q.y = p.y - 1; q.y <= p.y + 1; q.y += 2)
			if(q.y >= 0 && q.y < isz.cy) {
				const RGBA *ql = src[q.y];
				byte *qf = Filled(q.y);
				bool flag = false;
				for(q.x = xl; q.x <= xh; q.x++) {
					bool f = !qf[q.x] && eq(ql[q.x], color);
					if(f != flag) {
						flag = f;
						if(f) stack.Add(q);
					}
				}
			}
	}
}

void FloodFill(const Image& source, ImageBuffer& target, RGBA color, Point pt, int tolerance)
{
	if(tolerance == 0)
		FloodFill(source, target, color, pt,
		          [](RGBA a, RGBA b) { return a == b; });
	else
	if(tolerance < 0)
		FloodFill(source, target, RGBAZero(), pt,
		          [&](RGBA a, RGBA b) { return a != color; });
	else
		FloodFill(source, target, color, pt,
		          [&](RGBA a, RGBA b) { return abs(a.a - b.a) <= tolerance &&
		                                       abs(a.r - b.r) + abs(a.g - b.g) + abs(a.b - b.b) <= tolerance; });
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

void MirrorDiag(Image& img, const Rect& rect, bool symm)
{
	ImageBuffer ib(img);
	Size isz = ib.GetSize();
	Rect rc = rect & isz;
	int sz = min(rc.GetWidth(), rc.GetHeight());
	for(int y = 0; y < sz - 1; y++) {
		RGBA *h = ib[y + rect.top] + rect.left + y + 1;
		RGBA *e = ib[y + rect.top] + rect.left + sz;
		RGBA *v = ib[y + rect.top + 1] + rect.left + y;
		while(h < e) {
			if(symm)
				*h = *v;
			else
				Swap(*h, *v);
			h++;
			v += isz.cx;
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
	return String(block);
}

String PackImlData(const Vector<ImageIml>& image)
{
	return ZCompress(PackImlDataUncompressed(image));
}
