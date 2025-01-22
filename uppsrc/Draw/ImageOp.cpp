#include "Draw.h"

namespace Upp {
	
void  SetHotSpots(Image& m, Point hotspot, Point hotspot2)
{
	ImageBuffer b(m);
	b.SetHotSpot(hotspot);
	b.Set2ndSpot(hotspot2);
	m = b;
}

Image WithHotSpots(const Image& m, Point hotspot, Point hotspot2)
{
	Image h = m;
	SetHotSpots(h, hotspot, hotspot2);
	return h;
}

Image WithHotSpots(const Image& m, int x1, int y1, int x2, int y2)
{
	return WithHotSpots(m, Point(x1, y1), Point(x2, y2));
}

Image WithHotSpot(const Image& m, int x1, int y1)
{
	Image h = m;
	ImageBuffer b(h);
	b.SetHotSpot(Point(x1, y1));
	return Image(b);
}

Image CreateImage(Size sz, const RGBA& rgba)
{
	ImageBuffer ib(sz);
	Fill(~ib, rgba, ib.GetLength());
	return Image(ib);
}

Image CreateImage(Size sz, Color color)
{
	return CreateImage(sz, (RGBA)color);
}

void ScanOpaque(Image& m)
{
	ImageBuffer ib(m);
	ib.SetKind(ib.ScanKind());
	m = ib;
}

force_inline Size DstSrc(ImageBuffer& dest, Point& p, const Image& src, Rect& sr)
{
	if(p.x < 0) {
		sr.left += -p.x;
		p.x = 0;
	}
	if(p.y < 0) {
		sr.top += -p.y;
		p.y = 0;
	}
	sr = sr & src.GetSize();
	Size sz = dest.GetSize() - p;
	sz.cx = min(sz.cx, sr.GetWidth());
	sz.cy = min(sz.cy, sr.GetHeight());
	return sz;
}

void DstSrcOp(ImageBuffer& dest, Point p, const Image& src, const Rect& srect,
                           void (*op)(RGBA *t, const RGBA *s, int n), bool co)
{
	Rect sr = srect;
	Size sz = DstSrc(dest, p, src, sr);
	if(sz.cx > 0) {
		if(co)
			CoFor(sz.cy, [&](int i) {
				(*op)(dest[p.y + i] + p.x, src[sr.top + i] + sr.left, sz.cx);
			});
		else
			while(--sz.cy >= 0)
				(*op)(dest[p.y++] + p.x, src[sr.top++] + sr.left, sz.cx);
	}
}

void Copy(ImageBuffer& dest, Point p, const Image& src, const Rect& srect, bool co)
{
	DstSrcOp(dest, p, src, srect, Copy);
}

void Over(ImageBuffer& dest, Point p, const Image& src, const Rect& srect, bool co)
{
	DstSrcOp(dest, p, src, srect, AlphaBlend);
}

Image GetOver(const Image& dest, const Image& src)
{
	Image r = dest;
	Over(r, src);
	return r;
}

Image Copy(const Image& src, const Rect& srect, bool co)
{
	ImageBuffer ib(srect.GetSize());
	Copy(ib, Point(0, 0), src, srect);
	return Image(ib);
}

void Fill(ImageBuffer& dest, const Rect& rect, RGBA color)
{
	Rect r = dest.GetSize() & rect;
	int cx = r.GetWidth();
	if(cx)
		for(int y = r.top; y < r.bottom; y++)
			Fill(dest[y] + r.left, color, cx);
}

void OverStraightOpaque(ImageBuffer& dest, Point p, const Image& src, const Rect& srect, bool co)
{
	DstSrcOp(dest, p, src, srect, AlphaBlendStraightOpaque);
}

void  Copy(Image& dest, Point p, const Image& _src, const Rect& srect, bool co)
{
	Image src = _src;
	ImageBuffer b(dest);
	Copy(b, p, src, srect, co);
	dest = b;
}

void  Over(Image& dest, Point p, const Image& _src, const Rect& srect, bool co)
{
	Image src = _src;
	ImageBuffer b(dest);
	Over(b, p, src, srect, co);
	dest = b;
}

void  Over(Image& dest, const Image& _src, bool co)
{
	Over(dest, Point(0, 0), _src, _src.GetSize(), co);
}

void Fill(Image& dest, const Rect& rect, RGBA color)
{
	ImageBuffer b(dest);
	Fill(b, rect, color);
	dest = b;
}

void  OverStraightOpaque(Image& dest, Point p, const Image& _src, const Rect& srect, bool co)
{
	Image src = _src;
	ImageBuffer b(dest);
	OverStraightOpaque(b, p, src, srect, co);
	dest = b;
}

void Crop(RasterEncoder& tgt, Raster& img, const Rect& rc)
{
	Rect r = rc & img.GetSize();
	tgt.Create(r.Size(), img);
	for(int y = r.top; y < r.bottom; y++)
		tgt.WriteLine(~img[y] + r.left);
}

Image Crop(const Image& img, const Rect& rc)
{
	if(rc.left == 0 && rc.top == 0 && rc.Size() == img.GetSize())
		return img;
	if((rc & img.GetSize()).IsEmpty())
		return Image();
	ImageRaster  src(img);
	ImageEncoder tgt;
	Crop(tgt, src, rc);
	return tgt;
}

Image AddMargins(const Image& img, int left, int top, int right, int bottom, RGBA color)
{
	Size sz = img.GetSize();
	ImageBuffer ib(sz.cx + left + right, sz.cy + top + bottom);
	Fill(ib, color, ib.GetLength());
	Copy(ib, Point(left, top), img, img.GetSize());
	return Image(ib);
}

Image Crop(const Image& img, int x, int y, int cx, int cy)
{
	return Crop(img, RectC(x, y, cx, cy));
}

bool IsUniform(const RGBA *s, RGBA c, int add, int n)
{
	while(n-- > 0) {
		if(*s != c)
			return false;
		s += add;
	}
	return true;
}

Rect FindBounds(const Image& m, RGBA bg)
{
	Size isz = m.GetSize();
	Rect r = isz;
	for(r.top = 0; r.top < isz.cy && IsUniform(m[r.top], bg, 1, isz.cx); r.top++)
		;
	for(r.bottom = isz.cy; r.bottom > r.top && IsUniform(m[r.bottom - 1], bg, 1, isz.cx); r.bottom--)
		;
	if(r.bottom <= r.top)
		return Null;
	int h = r.GetHeight();
	const RGBA *p = m[r.top];
	for(r.left = 0; r.left < isz.cy && IsUniform(p + r.left, bg, isz.cx, h); r.left++)
		;
	for(r.right = isz.cx; r.right > r.left && IsUniform(p + r.right - 1, bg, isz.cx, h); r.right--)
		;
	return r;
}

void AutoCrop(Image *m, int count, RGBA bg)
{
	if(!count)
		return;
	Rect r = FindBounds(m[0]);
	for(int i = 1; i < count; i++)
		r.Union(FindBounds(m[i], bg));
	for(int i = 0; i < count; i++) {
		Point p1 = m[i].GetHotSpot() - r.TopLeft();
		Point p2 = m[i].Get2ndSpot() - r.TopLeft();
		m[i] = WithHotSpots(Crop(m[i], r), p1.x, p1.y, p2.x, p2.y);
	}
}

Image AutoCrop(const Image& m, RGBA bg)
{
	Image mm = m;
	AutoCrop(&mm, 1, bg);
	return mm;
}

void ClampHotSpots(Image& m)
{
	Point p1 = m.GetHotSpot();
	Point p2 = m.Get2ndSpot();
	Rect clamp = m.GetSize();
	Point p1a = clamp.Bind(p1);
	Point p2a = clamp.Bind(p2);
	if(p1 != p1a || p2 != p2a)
		SetHotSpots(m, p1a, p2a);
}

Image ColorMask(const Image& src, Color key)
{
	ImageBuffer ib(src.GetSize());
	const RGBA *s = src;
	const RGBA *e = src + src.GetLength();
	RGBA *t = ~ib;
	byte kr = key.GetR();
	byte kg = key.GetG();
	byte kb = key.GetB();
	while(s < e) {
		if(s->r == kr && s->g == kg && s->b == kb)
			*t++ = RGBAZero();
		else
			*t++ = *s;
		s++;
	}
	ib.SetHotSpots(src);
	return Image(ib);
}

void CanvasSize(RasterEncoder& tgt, Raster& img, int cx, int cy)
{
	tgt.Create(cx, cy, img);
	int ccx = min(img.GetWidth(), cx);
	int ccy = min(img.GetHeight(), cy);
	for(int y = 0; y < ccy; y++) {
		memcpy(~tgt, img[y], ccx * sizeof(RGBA));
		memset(~tgt + ccx, 0, (cx - ccx) * sizeof(RGBA));
		tgt.WriteLine();
	}
	for(int y = cy - ccy; --y >= 0;) {
		memset(~tgt, 0, cx * sizeof(RGBA));
		tgt.WriteLine();
	}
}

Image CanvasSize(const Image& img, int cx, int cy)
{
	ImageRaster  src(img);
	ImageEncoder tgt;
	CanvasSize(tgt, src, cx, cy);
	return tgt;
}

Image AssignAlpha(const Image& img, const Image& alpha)
{
	Size sz = Size(min(img.GetWidth(), alpha.GetWidth()),
	               min(img.GetHeight(), alpha.GetHeight()));
	if(sz.cx == 0 || sz.cy == 0)
		return Image();
	ImageBuffer ib(sz);
	for(int y = 0; y < sz.cy; y++) {
		const RGBA *s = img[y];
		const RGBA *e = s + sz.cx;
		const RGBA *a = alpha[y];
		RGBA *t = ib[y];
		while(s < e) {
			*t = *s++;
			(t++)->a = (a++)->a;
		}
	}
	ib.SetHotSpots(img);
	return Image(ib);
}

int   EqualightCh(int c, int l, int h)
{
	return Saturate255((c - l) * 255 / (h - l) + l);
}

Image Equalight(const Image& img, int thold)
{
	int histogram[256];
	ZeroArray(histogram);
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	while(s < e) {
		histogram[Grayscale(*s)]++;
		s++;
	}
	size_t n = (thold * img.GetLength()) >> 8;
	int h = 255;
	int l = 0;
	while(l < h) {
		if(n < 0)
			break;
		n -= histogram[l++];
		if(n < 0)
			break;
		n -= histogram[h--];
	}
	if(l >= h)
		return img;
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	s = ~img;
	while(s < e) {
		t->r = EqualightCh(s->r, l, h);
		t->g = EqualightCh(s->g, l, h);
		t->b = EqualightCh(s->b, l, h);
		t->a = s->a;
		s++;
		t++;
	}
	w.SetHotSpots(img);
	return Image(w);
}

Image Grayscale(const Image& img)
{
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	while(s < e) {
		int q = Grayscale(*s);
		t->r = q;
		t->g = q;
		t->b = q;
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpots(img);
	return Image(w);
}

Image Grayscale(const Image& img, int amount)
{
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	int na = 256 - amount;
	while(s < e) {
		int q = Grayscale(*s);
		t->r = Saturate255((amount * q + na * s->r) >> 8);
		t->g = Saturate255((amount * q + na * s->g) >> 8);
		t->b = Saturate255((amount * q + na * s->b) >> 8);
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpots(img);
	return Image(w);
}

Image Colorize(const Image& img, Color color, int alpha)
{
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	Unmultiply(w);
	RGBA *t = w;
	byte r = color.GetR();
	byte g = color.GetG();
	byte b = color.GetB();
	alpha = alpha + (alpha >> 7);
	while(s < e) {
		int ga = Grayscale(*s);
		ga = ga + (ga >> 7);
		t->r = (alpha * (((ga * r) >> 8) - s->r) >> 8) + s->r;
		t->g = (alpha * (((ga * g) >> 8) - s->g) >> 8) + s->g;
		t->b = (alpha * (((ga * b) >> 8) - s->b) >> 8) + s->b;
		t->a = s->a;
		t++;
		s++;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	return Image(w);
}

Image DarkTheme(const Image& img)
{
	if(IsNull(img))
		return img;

	Image simg = Unmultiply(img);
	const RGBA *s = simg.begin();
	const RGBA *e = simg.end();

	ImageBuffer ib(img.GetSize());
	RGBA *t = ~ib;
	
	while(s < e) {
		RGBA h = *s++;
		byte a = h.a;
		h.a = 255;
		h = DarkTheme(Color(h));
		h.a = a;
		*t++ = h;
	}
	
	Premultiply(ib);

	ib.SetHotSpots(img);

	return Image(ib);
}

inline
byte ContrastCh(int amount, int ch)
{
	return Saturate255(128 + (amount * (ch - 128) >> 8));
}

Image Contrast(const Image& img, int amount)
{
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	Unmultiply(w);
	RGBA *t = w;
	while(s < e) {
		t->r = ContrastCh(amount, s->r);
		t->g = ContrastCh(amount, s->g);
		t->b = ContrastCh(amount, s->b);
		t->a = s->a;
		t++;
		s++;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	return Image(w);
}

void sLine(RGBA *t, int cx, const RasterLine l[3], ImageFilter9& filter)
{
	RGBA h[3][3];
	const RGBA *x[3];
	x[0] = h[0];
	x[1] = h[1];
	x[2] = h[2];
	if(cx == 1) {
		h[0][0] = l[0][0]; h[0][1] = l[0][0]; h[0][2] = l[0][0];
		h[1][0] = l[1][0]; h[1][1] = l[1][0]; h[1][2] = l[1][0];
		h[2][0] = l[2][0]; h[2][1] = l[2][0]; h[2][2] = l[2][0];
		*t = filter(x);
		return;
	}
	h[0][0] = l[0][0]; h[0][1] = l[0][0]; h[0][2] = l[0][1];
	h[1][0] = l[1][0]; h[1][1] = l[1][0]; h[1][2] = l[1][1];
	h[2][0] = l[2][0]; h[2][1] = l[2][0]; h[2][2] = l[2][1];
	*t++ = filter(x);
	for(int i = 1; i < cx - 1; i++) {
		x[0] = ~l[0] + i - 1;
		x[1] = ~l[1] + i - 1;
		x[2] = ~l[2] + i - 1;
		*t++ = filter(x);
	}
	h[0][0] = l[0][cx - 2]; h[0][1] = l[0][cx - 1]; h[0][2] = l[0][cx - 1];
	h[1][0] = l[1][cx - 2]; h[1][1] = l[1][cx - 1]; h[1][2] = l[1][cx - 1];
	h[2][0] = l[2][cx - 2]; h[2][1] = l[2][cx - 1]; h[2][2] = l[2][cx - 1];
	x[0] = h[0];
	x[1] = h[1];
	x[2] = h[2];
	*t++ = filter(x);
}

void  Filter(RasterEncoder& target, Raster& src, ImageFilter9& filter)
{
	Size sz = src.GetSize();
	target.Create(sz, src);
	if(sz.cy < 1)
		return;
	RasterLine l[3];
	if(sz.cy == 1) {
		l[0] = src[0];
		l[1] = src[0];
		l[2] = src[0];
		sLine(target, sz.cx, l, filter);
		return;
	}
	l[0] = src[0];
	l[1] = src[0];
	l[2] = src[1];
	sLine(target, sz.cx, l, filter);
	target.WriteLine();
	for(int y = 1; y < sz.cy - 1; y++) {
		l[0] = pick(l[1]);
		l[1] = pick(l[2]);
		l[2] = src[y + 1];
		sLine(target, sz.cx, l, filter);
		target.WriteLine();
	}
	l[0] = pick(l[1]);
	l[1] = pick(l[2]);
	l[2] = src[sz.cy - 1];
	sLine(target, sz.cx, l, filter);
	target.WriteLine();
}

Image Filter(const Image& img, ImageFilter9& filter)
{
	ImageEncoder tgt;
	ImageRaster src(img);
	Filter(tgt, src, filter);
	return tgt;
}

struct RGBAI {
	int r, g, b, a;

	RGBAI() { r = g = b = a= 0; }
};

static void sGetS(RGBA q, RGBAI& p, int mul)
{
	p.r += mul * q.r;
	p.g += mul * q.g;
	p.b += mul * q.b;
	p.a += mul * q.a;
}

struct sSharpenFilter : ImageFilter9 {
	int amount;

	virtual RGBA operator()(const RGBA **mx);
};

RGBA sSharpenFilter::operator()(const RGBA **mx)
{
	RGBAI q;
	sGetS(mx[0][0], q, 7);
	sGetS(mx[0][1], q, 9);
	sGetS(mx[0][2], q, 7);
	sGetS(mx[1][0], q, 9);
	sGetS(mx[1][2], q, 9);
	sGetS(mx[2][0], q, 7);
	sGetS(mx[2][1], q, 9);
	sGetS(mx[2][2], q, 7);
	const RGBA& s = mx[1][1];
	RGBA t;
	int na = 256 + amount;
	t.b = Saturate255((na * (s.b << 6) - amount * q.b) >> 14);
	t.g = Saturate255((na * (s.g << 6) - amount * q.g) >> 14);
	t.r = Saturate255((na * (s.r << 6) - amount * q.r) >> 14);
	t.a = Saturate255((na * (s.a << 6) - amount * q.a) >> 14);
	return t;
}

void Sharpen(RasterEncoder& target, Raster& src, int amount)
{
	Size sz = src.GetSize();
	target.Create(sz, src);
	sSharpenFilter f;
	f.amount = amount;
	Filter(target, src, f);
}

Image Sharpen(const Image& img, int amount)
{
	ImageEncoder tgt;
	ImageRaster src(img);
	Sharpen(tgt, src, amount);
	return tgt;
}

struct sEtchFilter : ImageFilter9 {
	virtual RGBA operator()(const RGBA **mx);
};

RGBA sEtchFilter::operator()(const RGBA **mx)
{
	RGBA t;
	RGBA s = mx[1][1];
	if(s.a > 0x80 && s.r + s.g + s.b < 500) {
		t.r = t.g = t.b = 128;
		t.a = s.a;
		return t;
	}
	s = mx[0][0];
	if(s.a > 0x80 && s.r + s.g + s.b < 500) {
		t.r = t.g = t.b = 255;
		t.a = s.a;
		return t;
	}
	Zero(t);
	return t;
}

Image Etched(const Image& img)
{
	sEtchFilter ef;	
	return Premultiply(Filter(Unmultiply(img), ef));
}

Image SetColorKeepAlpha(const Image& img, Color c)
{
	RGBA rgba = c;
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	while(s < e) {
		*t = rgba;
		(t++)->a = (s++)->a;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	return Image(w);
}

Image CreateHorzFadeOut(Size sz, Color color)
{
	ImageBuffer ib(sz);
	RGBA c = color;
	for(int q = 0; q < sz.cx; q++) {
		c.a = q * 255 / sz.cx;
		RGBA *t = ~ib + q;
		for(int n = sz.cy; n > 0; n--) {
			*t = c;
			t += sz.cx;
		}
	}
	Premultiply(ib);
	return Image(ib);
}

struct FadeOutMaker : ImageMaker {
	Size  sz;
	Color color;

	virtual String Key() const {
		char h[sizeof(Size) + sizeof(Color)];
		memcpy(h, &sz, sizeof(sz));
		memcpy(h + sizeof(Size), &color, sizeof(Color));
		return String(h, sizeof(h));
	}

	virtual Image  Make() const {
		return CreateHorzFadeOut(sz, color);
	}
};

Image  HorzFadeOut(Size sz, Color color)
{
	FadeOutMaker m;
	m.sz = sz;
	m.color = color;
	return MakeImage(m);
}

Image HorzFadeOut(int cx, int cy, Color color)
{
	return HorzFadeOut(Size(cx, cy), color);
}

void SetNormalizedHotSpots(ImageBuffer& ib, int x1, int y1, int x2, int y2)
{
	Rect r(x1, y1, x2, y2);
	r.Normalize();
	ib.SetHotSpot(r.TopLeft());
	ib.Set2ndSpot(r.BottomRight());
}

Image  RotateClockwise(const Image& img)
{
	Size sz = img.GetSize();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[sz.cy - y - 1][x];
	Point p1 = img.GetHotSpot();
	Point p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, sz.cy - p1.y - 1, p1.x, sz.cy - p2.y - 1, p2.x);
	return Image(ib);
}

Image  RotateAntiClockwise(const Image& img)
{
	Size sz = img.GetSize();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[y][sz.cx - x - 1];
	
	Point p1 = img.GetHotSpot();
	Point p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, p1.y, sz.cx - p1.x - 1, p2.y, sz.cx - p2.x - 1);
	return Image(ib);
}

Image Rotate180(const Image& orig)
{
	Size sz = orig.GetSize();
	ImageBuffer dest(sz);
	for(int rw = 0; rw < sz.cy; rw++)
		for(int cl = 0; cl < sz.cx; cl++)
			dest[rw][cl] = orig[sz.cy - rw - 1][sz.cx - cl - 1];
	Point p1 = orig.GetHotSpot();
	Point p2 = orig.Get2ndSpot();
	SetNormalizedHotSpots(dest, sz.cy - p1.y - 1, sz.cx - p1.x - 1, sz.cy - p2.y - 1, sz.cx - p2.x - 1);
	return Image(dest);
}

Image Transpose(const Image& img)
{
	Size sz = img.GetSize();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[y][x];
	Point p1 = img.GetHotSpot();
	Point p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, p1.y, p1.x, p2.y, p2.x);
	return Image(ib);
}

Image Transverse(const Image& img)
{
	Size sz = img.GetSize();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[sz.cy - y - 1][sz.cx - x - 1];
	Point p1 = img.GetHotSpot();
	Point p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, sz.cy - p1.y - 1, sz.cx - p1.x - 1, sz.cy - p2.y - 1, sz.cx - p2.x - 1);
	return Image(ib);
}

Image MirrorHorz(const Image& img)
{
	Size sz = img.GetSize();
	Image h = img;
	ImageBuffer ib(h);
	for(int y = 0; y < sz.cy; y++) {
		RGBA *b = ib[y] + 0;
		RGBA *e = b + sz.cx - 1;
		while(b < e) {
			Swap(*b, *e);
			b++;
			e--;
		}
	}
	Point p1 = img.GetHotSpot();
	Point p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, sz.cx - p1.x - 1, p1.y, sz.cx - p2.x - 1, p2.y);
	return Image(ib);
}

Image MirrorVert(const Image& img)
{
	Size sz = img.GetSize();
	Image h = img;
	ImageBuffer ib(h);

	for(int y = 0; y < sz.cy / 2; y++) {
		RGBA *b = ib[y];
		RGBA *e = ib[sz.cy - y - 1];
		for(int x = 0; x < sz.cx; x++) {
			Swap(*b, *e);
			b++;
			e++;
		}
	}
	Point p1 = img.GetHotSpot();
	Point p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, p1.x, sz.cy - p1.y - 1, p2.x, sz.cy - p2.y - 1);
	return Image(ib);
}

Image FlipImage(const Image& m, int mode)
{
	switch(mode) {
	case FLIP_MIRROR_HORZ:
		return MirrorHorz(m);
	case FLIP_ROTATE_180:
		return Rotate180(m);
	case FLIP_MIRROR_VERT:
		return MirrorVert(m);
	case FLIP_TRANSPOSE:
		return Transpose(m);
	case FLIP_ROTATE_CLOCKWISE:
		return RotateClockwise(m);
	case FLIP_TRANSVERSE:
		return Transverse(m);
	case FLIP_ROTATE_ANTICLOCKWISE:
		return RotateAntiClockwise(m);
	}
	return m;
}

static Pointf Cvp(double x, double y, double sina, double cosa)
{
	return Pointf(x * cosa + y * sina, -x * sina + y * cosa);
}

Image Rotate(const Image& m, int angle)
{
	Size isz = m.GetSize();
	Pointf centerf = Pointf(Point(isz)) / 2.0;
	double sina, cosa;
	Draw::SinCos(-angle, sina, cosa);
	Pointf p1 = Cvp(-centerf.x, -centerf.y, sina, cosa);
	Pointf p2 = Cvp(centerf.x, -centerf.y, sina, cosa);
	Size sz2 = Size(2 * (int)max(tabs(p1.x), tabs(p2.x)),
	                2 * (int)max(tabs(p1.y), tabs(p2.y)));
	ImageBuffer ib(sz2);
	Fill(~ib, RGBAZero(), ib.GetLength());
	RGBA *t = ~ib;
	Draw::SinCos(angle, sina, cosa);
	int sini = int(sina * 128);
	int cosi = int(cosa * 128);
	Buffer<int> xmx(sz2.cx);
	Buffer<int> xmy(sz2.cx);
	for(int x = 0; x < sz2.cx; x++) {
		int xx = x + x - sz2.cx;
		xmx[x] = int(xx * cosi);
		xmy[x] = -int(xx * sini);
	}
	for(int y = 0; y < sz2.cy; y++) {
		int yy = y + y - sz2.cy;
		int ymx = int(yy * sini) + (isz.cx << 7);
		int ymy = int(yy * cosi) + (isz.cy << 7);
		for(int x = 0; x < sz2.cx; x++) {
			int xs = (xmx[x] + ymx) >> 8;
			int ys = (xmy[x] + ymy) >> 8;
			*t++ = xs >= 0 && xs < isz.cx && ys >= 0 && ys < isz.cy ? m[ys][xs] : RGBAZero();
		}
	}
	return Image(ib);
}

Image Dither(const Image& m, int dival)
{
	static byte dither[8][8]= {
		{ 1, 49, 13, 61, 4, 52, 16, 64 },
        { 33, 17, 45, 29, 36, 20, 48, 32 },
        { 9, 57, 5, 53, 12, 60, 8, 56 },
        { 41, 25, 37, 21, 44, 28, 40, 24 },
        { 3, 51, 15, 63, 2, 50, 14, 62 },
        { 35, 19, 47, 31, 34, 18, 46, 30 },
        { 11, 59, 7, 55, 10, 58, 6, 54 },
        { 43, 27, 39, 23, 42, 26, 38, 22 },
	};
	
	Size isz = m.GetSize();
	ImageBuffer ib(m.GetSize());
	const RGBA *s = ~m;
	RGBA *t = ~ib;
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
			int g = Grayscale(*s++) * 100 / dival;
			*t++ = g > dither[y & 7][x & 7] ? White() : Black();
		}
	return Image(ib);
}

Image GaussianBlur(const Image& img, int radius, bool co)
{
	// This code is adapted from Ivan Kutskir's fast blur implementation, published under MIT license.
	// See: http://blog.ivank.net/fastest-gaussian-blur.html
	
	// An implementation of well known fast box and gaussian blur
	// approximation algorithms by Wojciech Jarosz and Peter Kovesi.
	// See: https://elynxsdk.free.fr/ext-docs/Blur/Fast_box_blur.pdf
	// See: https://www.peterkovesi.com/papers/FastGaussianSmoothing.pdf
	
	auto ApplyBoxBlur = [&](const Image& src, int r) -> Image
	{
		double avg = 1.0 / (r + r + 1);
	
		Size sz = src.GetSize();
	
		ImageBuffer tmp(sz);
		ImageBuffer out(sz);

		auto DoLine = [&](int i) {
			int ti = 0;
			int li = ti;
			int ri = r;
			const RGBA& fv = src[i][0];
			const RGBA& lv = src[i][sz.cx - 1];
			dword rsum = fv.r * (r + 1);
			dword gsum = fv.g * (r + 1);
			dword bsum = fv.b * (r + 1);
			dword asum = fv.a * (r + 1);
			for(int j = 0; j < r; j++) {
				const RGBA& p = src[i][j];
				rsum += p.r;
				gsum += p.g;
				bsum += p.b;
				asum += p.a;
			}
			for(int j = 0; j <= r; j++) {
				const RGBA& p = src[i][ri++];
				RGBA& q       = tmp[i][ti++];
				q.r = byte((rsum += p.r - fv.r) * avg);
				q.g = byte((gsum += p.g - fv.g) * avg);
				q.b = byte((bsum += p.b - fv.b) * avg);
				q.a = byte((asum += p.a - fv.a) * avg);
			}
			for(int j = r + 1; j < sz.cx - r; j++) {
				const RGBA& p = src[i][ri++];
				const RGBA& q = src[i][li++];
				RGBA& t       = tmp[i][ti++];
				t.r = byte((rsum += p.r - q.r) * avg);
				t.g = byte((gsum += p.g - q.g) * avg);
				t.b = byte((bsum += p.b - q.b) * avg);
				t.a = byte((asum += p.a - q.a) * avg);
			}
			for(int j = sz.cx - r; j < sz.cx ; j++) {
				const RGBA& p = src[i][li++];
				RGBA& q       = tmp[i][ti++];
				q.r = byte((rsum += lv.r - p.r) * avg);
				q.g = byte((gsum += lv.g - p.g) * avg);
				q.b = byte((bsum += lv.b - p.b) * avg);
				q.a = byte((asum += lv.a - p.a) * avg);
			}
		};


		if(co)
			CoFor(sz.cy, [&](int i) { DoLine(i); });
		else
			for(int i = 0; i < sz.cy; i++)
				DoLine(i);

		auto DoColumn = [&](int i) {
			int ti = 0;
			int li = ti;
			int ri = r;
			const RGBA& fv = tmp[ti][i];
			const RGBA& lv = tmp[sz.cy - 1][i];
			dword rsum = fv.r * (r + 1);
			dword gsum = fv.g * (r + 1);
			dword bsum = fv.b * (r + 1);
			dword asum = fv.a * (r + 1);
			for(int j = 0; j < r; j++) {
				const RGBA& p = tmp[j][i];
				rsum += p.r;
				gsum += p.g;
				bsum += p.b;
				asum += p.a;
			}
			for(int j = 0; j <= r; j++) {
				const RGBA& p = tmp[ri++][i];
				RGBA& q       = out[ti++][i];
				q.r = byte((rsum += p.r - fv.r) * avg);
				q.g = byte((gsum += p.g - fv.g) * avg);
				q.b = byte((bsum += p.b - fv.b) * avg);
				q.a = byte((asum += p.a - fv.a) * avg);
			}
			for(int j = r + 1; j < sz.cy - r; j++) {
				const RGBA& p = tmp[ri++][i];
				const RGBA& q = tmp[li++][i];
				RGBA&       t = out[ti++][i];
				t.r = byte((rsum += p.r - q.r) * avg);
				t.g = byte((gsum += p.g - q.g) * avg);
				t.b = byte((bsum += p.b - q.b) * avg);
				t.a = byte((asum += p.a - q.a) * avg);
			}
			for(int j = sz.cy - r; j < sz.cy; j++) {
				const RGBA& p = tmp[li++][i];
				RGBA& q       = out[ti++][i];
				q.r = byte((rsum += lv.r - p.r) * avg);
				q.g = byte((gsum += lv.g - p.g) * avg);
				q.b = byte((bsum += lv.b - p.b) * avg);
				q.a = byte((asum += lv.a - p.a) * avg);
			}
		};

		if(co) {
			std::atomic<int> ii(0);
			CoDo([&] {
				for(int i = 16 * ii++; i < sz.cx; i = 16 * ii++) { // go by cacheline
					int e = min(i + 16, sz.cx);
					for(int j = i; j < e; j++)
						DoColumn(j);
				}
			});
		}
		else
			for(int i = 0; i < sz.cx; i++)
				DoColumn(i);

		out.SetHotSpots(src);
		return Image(out);
	};

	if(radius < 1 || IsNull(img))
		return Image(img);
	
	double wl = ffloor(sqrt((12 * sqr(radius) / 3) + 1));
	if(fmod(wl, 2) == 0) wl--;
	double wu = wl + 2;
	double m = fround((12 * sqr(radius) - 3 * sqr(wl) - 4 * 3 * wl - 3 * 3) / (-4 * wl - 4));
	Vector<int> sizes;
	Image t = img;
	for (int i = 0; i < 3; i++)
		t = ApplyBoxBlur(t, int(((i < m ? wl : wu) - 1) / 2));
	return pick(t);
}

}
