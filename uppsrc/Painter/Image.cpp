#include "Painter.h"

namespace Upp {

#ifdef CPU_SIMD

force_inline
int IntAndFraction(f32x4 x, f32x4& fraction)
{
	x = x + f32all(8000); // Truncate truncates toward 0, need to fix negatives
	i32x4 m = Truncate(x);
	fraction = x - ToFloat(m);
	return (int)m - 8000;
}

force_inline
int Int(f32x4 x)
{
	return (int)Truncate(x + f32all(8000)) - 8000;
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
				image = (imagecache ? MinifyCached : Minify)(image, nx, ny, co);
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
		
		f32x4 x0 = f32all(p0.x);
		f32x4 y0 = f32all(p0.y);
		f32x4 dx = f32all(dd.x);
		f32x4 dy = f32all(dd.y);
		f32x4 ii = 0;
		f32x4 v1 = f32all(1);
		f32x4 ix, iy;

		auto GetIXY = [&] {
			ix = x0 + ii * dx;
			iy = y0 + ii * dy;
			ii += v1;
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
			f32x4 fx, fy;
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
				f32x4 p00, p01, p10, p11;
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

				p01 = p01 * fy;
				p11 = p11 * fy;
				p10 = p10 * fx;
				p11 = p11 * fx;
				
				fx = v1 - fx;
				fy = v1 - fy;
				
				p00 = p00 * fy;
				p10 = p10 * fy;
				p00 = p00 * fx;
				p01 = p01 * fx;
			
				StoreRGBAF(span, p00 + p01 + p10 + p11);
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
				image = (imagecache ? MinifyCached : Minify)(image, nx, ny, co);
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
