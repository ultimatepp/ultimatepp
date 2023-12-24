#include "Painter.h"

namespace Upp {

#ifdef CPU_SIMD

force_inline
int IntAndFraction(f32x4 x, f32x4& fraction)
{
	const int ishift = 1000000;
	x = x + f32all(ishift); // Truncate truncates toward 0, need to fix negatives
	i32x4 m = Truncate(x);
	fraction = x - ToFloat(m);
	return (int)m - ishift;
}

force_inline
int Int(f32x4 x)
{
	return (int)Truncate(x + f32all(8000)) - 8000;
}

#endif

struct PainterImageSpan : SpanSource {
	int         ax, ay, cx, cy, maxx, maxy;
	byte        style;
	byte        hstyle, vstyle;
	bool        fast;
	bool        fixed;
	Image       image;
	Xform2D     xform;
	bool        dofilter = false;
	ImageFilterKernel kx, ky;

	struct RGBAF {
		double r, g, b, a;
	
		void Put(double weight, const RGBA& src) {
			r += weight * src.r;
			g += weight * src.g;
			b += weight * src.b;
			a += weight * src.a;
		}
		RGBA Get(double div) const {
			RGBA c;
			c.r = Saturate255(int(r / div));
			c.g = Saturate255(int(g / div));
			c.b = Saturate255(int(b / div));
			c.a = Saturate255(int(a / div));
			return c;
		}
		
		RGBAF() { r = g = b = a = 0; }
	};
	

	PainterImageSpan(dword flags, const Xform2D& m, const Image& img, bool co, bool imagecache, int filter) {
		style = byte(flags & 15);
		hstyle = byte(flags & 3);
		vstyle = byte(flags & 12);
		fast = (flags & FILL_FAST) || filter == FILTER_NEAREST;
		image = img;
		int nx = 1;
		int ny = 1;
		if(!fast) {
			Pointf sc = m.GetScaleXY();
			if(sc.x >= 0.01 && sc.y >= 0.01) {
				nx = (int)max(1.0, 1.0 / sc.x);
				ny = (int)max(1.0, 1.0 / sc.y);
			}

			if(filter != FILTER_BILINEAR) {
				kx.Init(filter, nx, 1);
				ky.Init(filter, ny, 1);
				dofilter = true;
				fast = false;
			}
		}
		if(nx == 1 && ny == 1 || dofilter)
			xform = Inverse(m);
		else {
			if(!fast)
				image = (imagecache ? MinifyCached : Minify)(image, nx, ny, nx * ny > 20000 && co);
			xform = Inverse(m) * Xform2D::Scale(1.0 / nx, 1.0 / ny);
		}
		cx = image.GetWidth();
		cy = image.GetHeight();
		maxx = cx - 1;
		maxy = cy - 1;
		ax = 6000000 / cx * cx * 2;
		ay = 6000000 / cy * cy * 2;
		fixed = hstyle && vstyle;
	}

	const RGBA *Pixel(int x, int y) const { return &image[y][x]; }

	const RGBA *GetPixel(int x, int y) const {
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

	void GetFilter(RGBA *span, Pointf p0, Pointf dd, unsigned len) const
	{
		int ii = 0;
		while(len--) {
			const int ishift = 1000000; // to avoid problems with negatives
			Pointf p = (p0 + ii++ * dd) + Pointf(ishift, ishift);
			Point l = p;
			Pointf h = p - Pointf(l);
			l -= Point(ishift, ishift);
			int mx = int(kx.mul * h.x);
			int my = int(ky.mul * h.y);
		
		#ifdef CPU_SIMD
			f32x4 rgbaf = 0;
			f32x4 w = 0;
			for(int yy = -ky.n + 1; yy <= ky.n; yy++) {
				int wy = ky.Get(yy, my);
				for(int xx = -kx.n + 1; xx <= kx.n; xx++) {
					f32x4 s = LoadRGBAF(GetPixel(l.x + xx, l.y + yy));
					f32x4 weight = f32all(wy * kx.Get(xx, mx));
					rgbaf += weight * s;
					w += weight;
				}
			}
			StoreRGBAF(span++, ClampRGBAF(rgbaf / w));
		#else
			RGBAF  rgbaf;
			double w = 0;
			for(int yy = -ky.n + 1; yy <= ky.n; yy++) {
				int wy = ky.Get(yy, my);
				for(int xx = -kx.n + 1; xx <= kx.n; xx++) {
					double weight = wy * kx.Get(xx, mx);
					rgbaf.Put(weight, *GetPixel(l.x + xx, l.y + yy));
					w += weight;
				}
			}
			*span++ = rgbaf.Get(w);
		#endif
		}
	}

	virtual void Get(RGBA *span, int x, int y, unsigned len) const
	{
		PAINTER_TIMING("ImageSpan::Get");

		Pointf p0, dd;
		if(fast) {
			p0 = xform.Transform(Pointf(x, y));
			dd = xform.Transform(Pointf(x + 1, y)) - p0;
		}
		else {
			p0 = xform.Transform(Pointf(x, y) + Pointf(0.5, 0.5)) - Pointf(0.5, 0.5);
			dd = xform.Transform(Pointf(x + 1, y) + Pointf(0.5, 0.5))  - Pointf(0.5, 0.5) - p0;
		}

		if(dofilter)
			return GetFilter(span, p0, dd, len);

#ifdef CPU_SIMD
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
#else
		int ii = 0;
		if(hstyle + vstyle == 0 && fast) {
			while(len--) {
				Pointf p = p0 + ii++ * dd;
				Point l = p;
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
			const int ishift = 1000000; // to avoid problems with negatives
			Pointf p = (p0 + ii++ * dd) + Pointf(ishift, ishift);
			Point l = p;
			Point h = Pointf(256 * (p - Pointf(l)));
			l -= Point(ishift, ishift);
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
				RGBAV v;
				v.Set(0);
				Point u = -h + 256;
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy) {
					v.Put(u.x * u.y, *Pixel(l.x, l.y));
					v.Put(h.x * u.y, *Pixel(l.x + 1, l.y));
					v.Put(u.x * h.y, *Pixel(l.x, l.y + 1));
					v.Put(h.x * h.y, *Pixel(l.x + 1, l.y + 1));
				}
				else {
					v.Put(u.x * u.y, *GetPixel(l.x, l.y));
					v.Put(h.x * u.y, *GetPixel(l.x + 1, l.y));
					v.Put(u.x * h.y, *GetPixel(l.x, l.y + 1));
					v.Put(h.x * h.y, *GetPixel(l.x + 1, l.y + 1));
				}
				span->r = byte(v.r >> 16);
				span->g = byte(v.g >> 16);
				span->b = byte(v.b >> 16);
				span->a = byte(v.a >> 16);
			}
			++span;
		}
#endif
    }
};

void BufferPainter::RenderImage(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	current = Null;
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return;
	One<SpanSource> ss;
	ss.Create<PainterImageSpan>(flags, transsrc * pathattr.mtx, image, co, imagecache, pathattr.filter);
	RenderPath(width, ss, RGBAZero());
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

}