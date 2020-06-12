#include "Painter.h"

namespace Upp {

struct RGBAV {
	dword r, g, b, a;

	void Set(dword v) { r = g = b = a = v; }
	void Clear()      { Set(0); }
	void Put(dword weight, const RGBA& src) {
		r += weight * src.r;
		g += weight * src.g;
		b += weight * src.b;
		a += weight * src.a;
	}
	void Put(const RGBA& src) {
		r += src.r;
		g += src.g;
		b += src.b;
		a += src.a;
	}
	RGBA Get(int div) const {
		RGBA c;
		c.r = byte(r / div);
		c.g = byte(g / div);
		c.b = byte(b / div);
		c.a = byte(a / div);
		return c;
	}
};

Image DownScale(const Image& img, int nx, int ny, bool co)
{
	ASSERT(nx > 0 && ny > 0);
	Size ssz = img.GetSize();
	Size tsz = Size((ssz.cx + nx - 1) / nx, (ssz.cy + ny - 1) / ny);
	ImageBuffer ib(tsz);
	int scx0 = ssz.cx / nx * nx;
	auto do_line = [&](int ty, RGBAV *b, int *div) {
		int yy = ny * ty;
		for(int i = 0; i < tsz.cx; i++)
			b[i].Clear();
		memset(div, 0, tsz.cx * sizeof(int));
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const RGBA *s = img[yy + yi];
				const RGBA *e = s + scx0;
				const RGBA *e2 = s + ssz.cx;
				RGBAV *t = b;
				int *dv = div;
				while(s < e) {
					for(int n = nx; n--;) {
						t->Put(*s++);
						(*dv)++;
					}
					t++;
					dv++;
				}
				while(s < e2) {
					t->Put(*s++);
					(*dv)++;
				}
			}
		}
		const RGBAV *s = b;
		const int *dv = div;
		RGBA *it = ~ib + ty * tsz.cx;
		for(int x = 0; x < tsz.cx; x++)
			*it++ = (s++)->Get(*dv++);
	};
	if(co) {
		CoWork cw;
		cw * [&] {
			Buffer<int> div(tsz.cx);
			Buffer<RGBAV> b(tsz.cx);
			for(;;) {
				int y = cw.Next();
				if(y >= tsz.cy)
					break;
				do_line(y, b, div);
			}
		};
	}
	else {
		Buffer<int> div(tsz.cx);
		Buffer<RGBAV> b(tsz.cx);
		for(int y = 0; y < tsz.cy; y++)
			do_line(y, b, div);
	}
	return ib;
}

struct DownscaleImageMaker : public ImageMaker {
	Image image;
	int   nx;
	int   ny;
	bool  co;

	virtual String Key() const;
	virtual Image  Make() const;
};

String DownscaleImageMaker::Key() const
{
	String key;
	RawCat(key, image.GetSerialId());
	RawCat(key, nx);
	RawCat(key, ny);
	// we are not adding co as that is just a hint for actual image making
	return key;
}

Image DownscaleImageMaker::Make() const
{
	return DownScale(image, nx, ny, co);
}

Image DownScaleCached(const Image& img, int nx, int ny, bool co)
{
	DownscaleImageMaker m;
	m.image = img;
	m.nx = nx;
	m.ny = ny;
	m.co = co;
	return MakeImage(m);
}

#ifdef CPU_X86

never_inline
String AsString(__m128 x)
{
	float f[4];
	memcpy(f, &x, 16);
	return Sprintf("(%f, %f, %f, %f)", f[3], f[2], f[1], f[0]);
}

#if 1
#define DUMPS(x) RLOG(#x << " = " << AsString(x));
#else
#define DUMPS(x)
#endif

force_inline
__m128 LoadRGBAF(const RGBA *s)
{
	return _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_unpacklo_epi8(_mm_set_epi32(0, 0, 0, *(dword *)s), _mm_setzero_si128()), _mm_setzero_si128()));
}

force_inline
void StoreRGBAF(RGBA *t, __m128 s)
{
	_mm_store_ss((float *)t, _mm_castsi128_ps(
		_mm_packus_epi16(
			_mm_packs_epi32(_mm_cvttps_epi32(s), _mm_setzero_si128()),
			_mm_setzero_si128()
		)));
}

force_inline
int IntAndFraction(__m128 x, __m128& fraction)
{
	x = _mm_add_ps(x, _mm_set1_ps(8000)); // cvttps truncates toward 0, need to fix negatives
	__m128i m = _mm_cvttps_epi32(x);
	fraction = _mm_sub_ps(x, _mm_cvtepi32_ps(m));
	return _mm_cvtsi128_si32(m) - 8000;
}

force_inline
int Int(__m128 x)
{
	return _mm_cvtsi128_si32(_mm_cvttps_epi32(_mm_add_ps(x, _mm_set1_ps(8000)))) - 8000;
}

struct PainterImageSpanData {
	int         ax, ay, cx, cy, maxx, maxy;
	byte        style;
	byte        hstyle, vstyle;
	bool        fast;
	bool        fixed;
	Image       image;
	Xform2D     xform;

	PainterImageSpanData(dword flags, const Xform2D& m, const Image& img, bool co, bool imagecache) {
		style = byte(flags & 15);
		hstyle = byte(flags & 3);
		vstyle = byte(flags & 12);
		fast = flags & FILL_FAST;
		image = img;
		int nx = 1;
		int ny = 1;
		if(!fast) {
			Pointf sc = m.GetScaleXY();
			if(sc.x >= 0.01 && sc.y >= 0.01) {
				nx = (int)max(1.0, 1.0 / sc.x);
				ny = (int)max(1.0, 1.0 / sc.y);
			}
		}
		if(nx == 1 && ny == 1)
			xform = Inverse(m);
		else {
			if(!fast)
				image = (imagecache ? DownScaleCached : DownScale)(image, nx, ny, co);
			xform = Inverse(m) * Xform2D::Scale(1.0 / nx, 1.0 / ny);
		}
		cx = image.GetWidth();
		cy = image.GetHeight();
		maxx = cx - 1;
		maxy = cy - 1;
		ax = 6000000 / cx * cx * 2;
		ay = 6000000 / cy * cy * 2;
	}
	
	PainterImageSpanData() {}
};


struct PainterImageSpan : SpanSource, PainterImageSpanData {
	PainterImageSpan(const PainterImageSpanData& f)
	:	PainterImageSpanData(f) {}
	
	const RGBA *Pixel(int x, int y) { return &image[y][x]; }

	const RGBA *GetPixel(int x, int y) {
		if(hstyle == FILL_HPAD)
			x = minmax(x, 0, maxx);
		else
		if(hstyle == FILL_HREFLECT)
			x = (x + ax) / cx & 1 ? (ax - x - 1) % cx : (x + ax) % cx;
		else
		if(hstyle == FILL_HREPEAT)
			x = (x + ax) % cx;
		if(vstyle == FILL_VPAD)
			y = minmax(y, 0, maxy);
		else
		if(vstyle == FILL_VREFLECT)
			y = (y + ay) / cy & 1 ? (ay - y - 1) % cy : (y + ay) % cy;
		else
		if(vstyle == FILL_VREPEAT)
			y = (y + ay) % cy;
		static RGBA zero;
		return fixed || (x >= 0 && x < cx && y >= 0 && y < cy) ? &image[y][x] : &zero;
	}

	virtual void Get(RGBA *span, int x, int y, unsigned len)
	{
		PAINTER_TIMING("ImageSpan::Get");

		Pointf p0 = xform.Transform(Pointf(x, y));
		Pointf dd = xform.Transform(Pointf(x + 1, y)) - p0;
		
		__m128 x0 = _mm_set_ps1((float)p0.x);
		__m128 y0 = _mm_set_ps1((float)p0.y);
		__m128 dx = _mm_set_ps1((float)dd.x);
		__m128 dy = _mm_set_ps1((float)dd.y);
		__m128 ii = _mm_setzero_ps();
		__m128 v1 = _mm_set_ps1(1);
		__m128 ix, iy;

		auto GetIXY = [&] {
			ix = _mm_add_ps(x0, _mm_mul_ps(ii, dx));
			iy = _mm_add_ps(y0, _mm_mul_ps(ii, dy));
			ii = _mm_add_ps(ii, v1);
		};

		fixed = hstyle && vstyle;
		if(hstyle + vstyle == 0 && fast) {
			while(len--) {
				GetIXY();
				Point l(Int(ix), Int(iy));
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy)
					*span = *Pixel(l.x, l.y);
				else
				if(style == 0 && (l.x < -1 || l.x > cx || l.y < -1 || l.y > cy))
					*span = RGBAZero();
				else
					*span = *GetPixel(l.x, l.y);
				++span;
			}
			return;
		}
		while(len--) {
			GetIXY();
			__m128 fx, fy;
			Point l(IntAndFraction(ix, fx), IntAndFraction(iy, fy));
			if(hstyle == FILL_HREPEAT)
				l.x = (l.x + ax) % cx;
			if(vstyle == FILL_VREPEAT)
				l.y = (l.y + ay) % cy;
			if(style == 0 && (l.x < -1 || l.x > cx || l.y < -1 || l.y > cy))
				*span = RGBAZero();
			else
			if(fast) {
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy)
					*span = *Pixel(l.x, l.y);
				else
					*span = *GetPixel(l.x, l.y);
			}
			else {
				__m128 p00, p01, p10, p11;
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy) {
					p00 = LoadRGBAF(Pixel(l.x + 0, l.y + 0));
					p01 = LoadRGBAF(Pixel(l.x + 0, l.y + 1));
					p10 = LoadRGBAF(Pixel(l.x + 1, l.y + 0));
					p11 = LoadRGBAF(Pixel(l.x + 1, l.y + 1));
				}
				else {
					p00 = LoadRGBAF(GetPixel(l.x + 0, l.y + 0));
					p01 = LoadRGBAF(GetPixel(l.x + 0, l.y + 1));
					p10 = LoadRGBAF(GetPixel(l.x + 1, l.y + 0));
					p11 = LoadRGBAF(GetPixel(l.x + 1, l.y + 1));
				}

				p01 = _mm_mul_ps(p01, fy);
				p11 = _mm_mul_ps(p11, fy);
				p10 = _mm_mul_ps(p10, fx);
				p11 = _mm_mul_ps(p11, fx);
				
				fx = _mm_sub_ps(v1, fx);
				fy = _mm_sub_ps(v1, fy);
				
				p00 = _mm_mul_ps(p00, fy);
				p10 = _mm_mul_ps(p10, fy);
				p00 = _mm_mul_ps(p00, fx);
				p01 = _mm_mul_ps(p01, fx);
			
				StoreRGBAF(span, _mm_add_ps(p00, _mm_add_ps(p01, _mm_add_ps(p10, p11))));
			}
			++span;
		}
    }
};

void BufferPainter::RenderImage(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	current = Null;
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return;
	PainterImageSpanData f(flags, transsrc * pathattr.mtx, image, co, imagecache);
	RenderPath(width, [&](One<SpanSource>& s) {
		s.Create<PainterImageSpan>(f);
	}, RGBAZero());
}

void BufferPainter::FillOp(const Image& image, const Xform2D& transsrc, dword flags)
{
	Close();
	RenderImage(-1, image, transsrc, flags);
}

void BufferPainter::StrokeOp(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	RenderImage(width, image, transsrc, flags);
}

#else

struct PainterImageSpanData {
	int         ax, ay, cx, cy, maxx, maxy;
	byte        style;
	byte        hstyle, vstyle;
	bool        fast;
	bool        fixed;
	Image       image;
	Xform2D     xform;

	PainterImageSpanData(dword flags, const Xform2D& m, const Image& img, bool co, bool imagecache) {
		style = byte(flags & 15);
		hstyle = byte(flags & 3);
		vstyle = byte(flags & 12);
		fast = flags & FILL_FAST;
		image = img;
		int nx = 1;
		int ny = 1;
		if(!fast) {
			Pointf sc = m.GetScaleXY();
			if(sc.x >= 0.01 && sc.y >= 0.01) {
				nx = (int)max(1.0, 1.0 / sc.x);
				ny = (int)max(1.0, 1.0 / sc.y);
			}
		}
		if(nx == 1 && ny == 1)
			xform = Inverse(m);
		else {
			if(!fast)
				image = (imagecache ? DownScaleCached : DownScale)(image, nx, ny, co);
			xform = Inverse(m) * Xform2D::Scale(1.0 / nx, 1.0 / ny);
		}
		cx = image.GetWidth();
		cy = image.GetHeight();
		maxx = cx - 1;
		maxy = cy - 1;
		ax = 6000000 / cx * cx * 2;
		ay = 6000000 / cy * cy * 2;
	}
	
	PainterImageSpanData() {}
};


struct PainterImageSpan : SpanSource, PainterImageSpanData {
	LinearInterpolator interpolator;
	
	PainterImageSpan(const PainterImageSpanData& f)
	:	PainterImageSpanData(f) {
		interpolator.Set(xform);
	}
	
	RGBA Pixel(int x, int y) { return image[y][x]; }

	RGBA GetPixel(int x, int y) {
		if(hstyle == FILL_HPAD)
			x = minmax(x, 0, maxx);
		else
		if(hstyle == FILL_HREFLECT)
			x = (x + ax) / cx & 1 ? (ax - x - 1) % cx : (x + ax) % cx;
		else
		if(hstyle == FILL_HREPEAT)
			x = (x + ax) % cx;
		if(vstyle == FILL_VPAD)
			y = minmax(y, 0, maxy);
		else
		if(vstyle == FILL_VREFLECT)
			y = (y + ay) / cy & 1 ? (ay - y - 1) % cy : (y + ay) % cy;
		else
		if(vstyle == FILL_VREPEAT)
			y = (y + ay) % cy;
		return fixed || (x >= 0 && x < cx && y >= 0 && y < cy) ? image[y][x] : RGBAZero();
	}

	virtual void Get(RGBA *span, int x, int y, unsigned len)
	{
		PAINTER_TIMING("ImageSpan::Get");
		interpolator.Begin(x, y, len);
		fixed = hstyle && vstyle;
		if(hstyle + vstyle == 0 && fast) {
			while(len--) {
				Point l = interpolator.Get() >> 8;
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy)
					*span = Pixel(l.x, l.y);
				else
				if(style == 0 && (l.x < -1 || l.x > cx || l.y < -1 || l.y > cy))
					*span = RGBAZero();
				else
					*span = GetPixel(l.x, l.y);
				++span;
			}
			return;
		}
		while(len--) {
			Point h = interpolator.Get();
			Point l = h >> 8;
			if(hstyle == FILL_HREPEAT)
				l.x = (l.x + ax) % cx;
			if(vstyle == FILL_VREPEAT)
				l.y = (l.y + ay) % cy;
			if(style == 0 && (l.x < -1 || l.x > cx || l.y < -1 || l.y > cy))
				*span = RGBAZero();
			else
			if(fast) {
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy)
					*span = Pixel(l.x, l.y);
				else
					*span = GetPixel(l.x, l.y);
			}
			else {
				RGBAV v;
				v.Set(0);
				h.x &= 255;
				h.y &= 255;
				Point u = -h + 256;
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy) {
					v.Put(u.x * u.y, Pixel(l.x, l.y));
					v.Put(h.x * u.y, Pixel(l.x + 1, l.y));
					v.Put(u.x * h.y, Pixel(l.x, l.y + 1));
					v.Put(h.x * h.y, Pixel(l.x + 1, l.y + 1));
				}
				else {
					v.Put(u.x * u.y, GetPixel(l.x, l.y));
					v.Put(h.x * u.y, GetPixel(l.x + 1, l.y));
					v.Put(u.x * h.y, GetPixel(l.x, l.y + 1));
					v.Put(h.x * h.y, GetPixel(l.x + 1, l.y + 1));
				}
				span->r = byte(v.r >> 16);
				span->g = byte(v.g >> 16);
				span->b = byte(v.b >> 16);
				span->a = byte(v.a >> 16);
			}
			++span;
		}
    }
};

void BufferPainter::RenderImage(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	current = Null;
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return;
	PainterImageSpanData f(flags, transsrc * pathattr.mtx, image, co, imagecache);
	RenderPath(width, [&](One<SpanSource>& s) {
		s.Create<PainterImageSpan>(f);
	}, RGBAZero());
}

void BufferPainter::FillOp(const Image& image, const Xform2D& transsrc, dword flags)
{
	Close();
	RenderImage(-1, image, transsrc, flags);
}

void BufferPainter::StrokeOp(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	RenderImage(width, image, transsrc, flags);
}

#endif

}
