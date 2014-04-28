#include "Draw.h"

NAMESPACE_UPP

Image WithHotSpots(const Image& m, int x1, int y1, int x2, int y2)
{
	Image h = m;
	ImageBuffer b(h);
	b.SetHotSpot(Point(x1, y1));
	b.Set2ndSpot(Point(x2, y2));
	return b;
}

Image WithHotSpot(const Image& m, int x1, int y1)
{
	Image h = m;
	ImageBuffer b(h);
	b.SetHotSpot(Point(x1, y1));
	return b;
}

Image CreateImage(Size sz, const RGBA& rgba)
{
	ImageBuffer ib(sz);
	Fill(~ib, rgba, ib.GetLength());
	return ib;
}

Image CreateImage(Size sz, Color color)
{
	return CreateImage(sz, (RGBA)color);
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
                           void (*op)(RGBA *t, const RGBA *s, int n))
{
	Rect sr = srect;
	Size sz = DstSrc(dest, p, src, sr);
	if(sz.cx > 0)
		while(--sz.cy >= 0)
			(*op)(dest[p.y++] + p.x, src[sr.top++] + sr.left, sz.cx);
}

void Copy(ImageBuffer& dest, Point p, const Image& src, const Rect& srect)
{
	DstSrcOp(dest, p, src, srect, Copy);
}

void Over(ImageBuffer& dest, Point p, const Image& src, const Rect& srect)
{
	DstSrcOp(dest, p, src, srect, AlphaBlend);
}

void Fill(ImageBuffer& dest, const Rect& rect, RGBA color)
{
	Rect r = dest.GetSize() & rect;
	int cx = r.GetWidth();
	if(cx)
		for(int y = r.top; y < r.bottom; y++)
			Fill(dest[y] + r.left, color, cx);
}

void OverStraightOpaque(ImageBuffer& dest, Point p, const Image& src, const Rect& srect)
{
	DstSrcOp(dest, p, src, srect, AlphaBlendStraightOpaque);
}

void  Copy(Image& dest, Point p, const Image& _src, const Rect& srect)
{
	Image src = _src;
	ImageBuffer b(dest);
	Copy(b, p, src, srect);
	dest = b;
}

void  Over(Image& dest, Point p, const Image& _src, const Rect& srect)
{
	Image src = _src;
	ImageBuffer b(dest);
	Over(b, p, src, srect);
	dest = b;
}

void Fill(Image& dest, const Rect& rect, RGBA color)
{
	ImageBuffer b(dest);
	Fill(b, rect, color);
	dest = b;
}

void  OverStraightOpaque(Image& dest, Point p, const Image& _src, const Rect& srect)
{
	Image src = _src;
	ImageBuffer b(dest);
	OverStraightOpaque(b, p, src, srect);
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

Image AutoCrop(const Image& m, RGBA c)
{
	Size isz = m.GetSize();
	Rect r = isz;
	for(r.top = 0; r.top < isz.cy && IsUniform(m[r.top], c, 1, isz.cx); r.top++)
		;
	for(r.bottom = isz.cy; r.bottom > r.top && IsUniform(m[r.bottom - 1], c, 1, isz.cx); r.bottom--)
		;
	if(r.bottom <= r.top)
		return Null;
	int h = r.GetHeight();
	const RGBA *p = m[r.top];
	for(r.left = 0; r.left < isz.cy && IsUniform(p + r.left, c, isz.cx, h); r.left++)
		;
	for(r.right = isz.cx; r.right > r.left && IsUniform(p + r.right - 1, c, isz.cx, h); r.right--)
		;
	Point p1 = m.GetHotSpot() - r.TopLeft();
	Point p2 = m.Get2ndSpot() - r.TopLeft();
	return WithHotSpots(Crop(m, r), p1.x, p1.y, p2.x, p2.y);
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
	return ib;
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
	return ib;
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
	int n = (thold * img.GetLength()) >> 8;
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
	return w;
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
	return w;
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
	return w;
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
	return w;
}

Image AdjustForDarkBk(const Image& img)
{
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	while(s < e) {
		RGBA h = *s;
		Unmultiply(&h, &h, 1);
		int q = Grayscale(*s);
		if(q < 40) {
			int lvl0 = max(s->r, max(s->g, s->b));
			int l = 255 - lvl0;
			t->r = Saturate255(l + s->r);
			t->g = Saturate255(l + s->g);
			t->b = Saturate255(l + s->b);
		}
		else
		if(q > 216) {
			int lvl0 = min(s->r, min(s->g, s->b));
			int l = 255 - lvl0;
			t->r = Saturate255(l + s->r - lvl0);
			t->g = Saturate255(l + s->g - lvl0);
			t->b = Saturate255(l + s->b - lvl0);
		}
		else {
			t->r = s->r;
			t->g = s->g;
			t->b = s->b;
		}
		t->a = s->a;
		t++;
		s++;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	return w;
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
	return w;
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
	return w;
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
	return ib;
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
	return ib;
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
	return ib;
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
	return dest;
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
	return ib;
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
	return ib;
}

Image Magnify(const Image& img, int nx, int ny)
{
	if(nx == 1 && ny == 1)
		return img;
	if(nx == 0 || ny == 0)
		return Image();
	Size sz = img.GetSize();
	bool xdown = nx < 0;
	nx = abs(nx);
	int ncx = xdown ? sz.cx / nx : sz.cx * nx;
	ImageBuffer b(ncx, sz.cy * ny);
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	RGBA *t = ~b;
	while(s < e) {
		RGBA *q = t;
		const RGBA *le = s + sz.cx;
		while(s < le) {
			Fill(q, *s, nx);
			q += nx;
			s++;
		}
		for(int n = ny - 1; n--;) {
			memcpy(q, t, ncx * sizeof(RGBA));
			q += ncx;
		}
		t = q;
	}
	return b;
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
	return ib;
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
	return ib;
}

END_UPP_NAMESPACE
