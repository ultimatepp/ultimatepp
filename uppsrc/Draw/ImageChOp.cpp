#include "Draw.h"

namespace Upp {

int Diff(RGBA a, RGBA b)
{
	return max(abs(a.a - b.a), max(abs(a.r - b.r), max(abs(a.b - b.b), abs(a.g - b.g))));
}

struct ButtonDecomposer {
	Image src;
	Image dst;
	int   aa;
	int   maxdiff;
	RGBA  color;
	int   gdiff;
	int   gcount;

	void Do() {
		Size sz = src.GetSize();
		int a = min(5, min(sz.cx, sz.cy) / 5);
		maxdiff = gdiff = gcount = 0;
		color = SColorDisabled();
		dst = src;
		ImageBuffer b(dst);
		for(int y = a; y < sz.cy - a; y++) {
			RGBA *p = b[y];
			RGBA c1 = p[a - 1];
			RGBA c2 = p[sz.cx - a + 1];
			int n = sz.cx - 2 * a;
			p += a;
			for(int x = 0; x < n; x++) {
				RGBA c;
				c.r = ((n - x) * c1.r + x * c2.r) / n;
				c.g = ((n - x) * c1.g + x * c2.g) / n;
				c.b = ((n - x) * c1.b + x * c2.b) / n;
				c.a = ((n - x) * c1.a + x * c2.a) / n;
				int d = Diff(c, *p);
				if(d > 30) {
					gcount++;
					gdiff += d;
					if(d >= maxdiff) {
						maxdiff = d;
						color = *p;
					}
				}
				*p++ = c;
			}
		}
		b.SetHotSpot(Point(a, a));
		dst = b;
	}
};

Image Unglyph(const Image& m, Color& c, double& gfactor)
{
	ButtonDecomposer b;
	b.src = Unmultiply(m);
	b.Do();
	c = b.color;
	gfactor = (double)b.gdiff / b.gcount;
	return Premultiply(b.dst);
}

Image Unglyph(const Image& m, Color& c)
{
	double dummy;
	return Unglyph(m, c, dummy);
}

Image Unglyph(const Image& m)
{
	Color dummy;
	return Unglyph(m, dummy);
}

Image VertBlend(Image img1, Image img2, int y0, int y1)
{
	Size sz = img1.GetSize();
	Size sz2 = img2.GetSize();
	sz.cx = min(sz.cx, sz2.cx);
	sz.cy = min(sz.cy, sz2.cy);
	ImageBuffer b(sz);
	for(int y = 0; y < sz.cy; y++)
		if(y >= y1)
			memcpy(b[y], img2[y], sz.cx * sizeof(RGBA));
		else
		if(y >= y0 && y1 > y0) {
			int alpha = 256 * (y - y0) / (y1 - y0);
			RGBA *t = b[y];
			const RGBA *s1 = img1[y];
			const RGBA *s2 = img2[y];
			const RGBA *e = s1 + sz.cx;
			while(s1 < e) {
				t->r = s1->r + ((alpha * (s2->r - s1->r)) >> 8);
				t->g = s1->g + ((alpha * (s2->g - s1->g)) >> 8);
				t->b = s1->b + ((alpha * (s2->b - s1->b)) >> 8);
				t->a = s1->a + ((alpha * (s2->a - s1->a)) >> 8);
				s1++;
				s2++;
				t++;
			}
		}
		else
			memcpy(b[y], img1[y], sz.cx * sizeof(RGBA));
	b.SetHotSpot(img1.GetHotSpot());
	b.Set2ndSpot(img1.Get2ndSpot());
	return Image(b);
}

Image HorzBlend(Image img1, Image img2, int x0, int x1)
{
	Image m = RotateAntiClockwise(VertBlend(RotateClockwise(img1), RotateClockwise(img2), x0, x1));
	ImageBuffer b(m);
	b.SetHotSpot(img1.GetHotSpot());
	b.Set2ndSpot(img1.Get2ndSpot());
	return Image(b);
}

Image HorzSymm(Image src) {
	ImageBuffer b(src);
	Size sz = b.GetSize();
	for(int y = 0; y < sz.cy; y++) {
		RGBA *l = b[y];
		for(int x = 0; x < sz.cx / 2; x++)
			l[sz.cx - x - 1] = l[x];
	}
	b.SetHotSpot(src.GetHotSpot());
	b.Set2ndSpot(src.Get2ndSpot());
	return Image(b);
}

bool EqLine(const Image& m, int l1, int l2, int x, int width)
{
	return !memcmp(m[l1] + x, m[l2] + x, width * sizeof(RGBA));
}

bool EqLine(const Image& m, int l1, int l2)
{
	return EqLine(m, l1, l2, 0, m.GetWidth());
}

bool IsSingleColor(const Image& m, const Rect& rect_)
{
	Rect rect = rect_;
	rect.Intersect(m.GetSize());
	if(IsNull(rect))
		return true;
	RGBA c = m[rect.top][rect.left];
	for(int y = rect.top; y < rect.bottom; y++) {
		const RGBA *line = m[y];
		for(int x = rect.left; x < rect.right; x++)
			if(line[x] != c)
				return false;
	}
	return true;
}

Image RecreateAlpha(const Image& overwhite, const Image& overblack)
{
	Size sz = overwhite.GetSize();
	ASSERT(overblack.GetSize() == sz);
	ImageBuffer r(sz);
	const RGBA *ws = overwhite;
	const RGBA *bs = overblack;
	RGBA *t = r;
	RGBA *e = t + r.GetLength();
	while(t < e) {
		t->a = Saturate255(bs->r - ws->r + 255);
		if(t->a) {
			t->r = Saturate255(bs->r * 255 / t->a);
			t->g = Saturate255(bs->g * 255 / t->a);
			t->b = Saturate255(bs->b * 255 / t->a);
		}
		else
			t->r = t->g = t->b = 0;
		t++;
		bs++;
		ws++;
	}
	Premultiply(r);
	return Image(r);
}

int ImageMargin(const Image& _m, int p, int dist)
{
	Image m = Unmultiply(_m);
	Color c = m[p][p];
	int d;
	Size sz = m.GetSize();
	for(d = p; d >= 0; d--)
		if(Diff(m[d][d], c) > dist || Diff(m[sz.cx - d - 1][sz.cy - d - 1], c) > dist)
			break;
	return d + 1;
}

int ImageMarginV(const Image& _m, int p, int dist)
{
	Image m = Unmultiply(_m);
	Size sz = m.GetSize();
	Color c = m[sz.cx / 2][p];
	int d;
	for(d = p; d >= 0; d--)
		if(Diff(m[sz.cx / 2][d], c) > dist || Diff(m[sz.cx / 2][sz.cy - d - 1], c) > dist)
			break;
	return d + 1;
}

Rect GetImageMargins(const Image& m, RGBA margin_color)
{
	Rect r;
	Size isz = m.GetSize();
	for(int pass = 0; pass < 2; pass++) {
		int& y = pass ? r.bottom : r.top;
		for(y = 0; y < isz.cy; y++) {
			const RGBA *s = m[pass ? isz.cy - y - 1 : y];
			for(int x = 0; x < isz.cx; x++)
				if(*s++ != margin_color)
					goto foundy;
		}
	foundy:
		int& x = pass ? r.right : r.left;
		for(x = 0; x < isz.cx; x++) {
			const RGBA *s = m[0] + (pass ? isz.cx - x - 1 : x);
			for(int y = 0; y < isz.cy; y++) {
				if(*s != margin_color)
					goto foundx;
				s += isz.cx;
			}
		}
	foundx:;
	}
	return r;
}

ChPartMaker::ChPartMaker(const Image& m)
{
	image = m;
	border = SColorShadow();
	bg = Null;
	ResetShape();
}

void ChPartMaker::ResetShape()
{
	t = b = l = r = true;
	tl = tr = bl = br = 0;
}

Image ChPartMaker::Make() const
{
	Size sz = image.GetSize();
	ASSERT(sz.cx >= 6 && sz.cy >= 6);
	Image h = image;
	ImageBuffer ib(h);
	for(int x = 0; x < sz.cx; x++) {
		if(t)
			ib[0][x] = x >= tl && x < sz.cx - tr ? border : bg;
		if(b)
			ib[sz.cy - 1][x] = x >= bl && x < sz.cx - br ? border : bg;
	}
	for(int y = 0; y < sz.cy; y++) {
		if(l)
			ib[y][0] = y >= tl && y < sz.cy - bl ? border : bg;
		if(r)
			ib[y][sz.cx - 1] = y >= tr && y < sz.cy - br ? border : bg;
	}
	if(tl == 2)
		ib[1][1] = border;
	if(tr == 2)
		ib[1][sz.cx - 2] = border;
	if(bl == 2)
		ib[sz.cy - 2][1] = border;
	if(br == 2)
		ib[sz.cy - 2][sz.cx - 2] = border;
	int q = max(max(tl, tr), max(br, bl));
	ib.SetHotSpot(Point(q, q));
	ib.Set2ndSpot(Point(sz.cx - q - 1, sz.cy - q - 1));
	return Image(ib);
}

}
