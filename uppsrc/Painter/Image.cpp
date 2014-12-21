#include "Painter.h"

NAMESPACE_UPP

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

Image DownScale(const Image& img, int nx, int ny)
{
	ASSERT(nx > 0 && ny > 0);
	Size ssz = img.GetSize();
	Size tsz = Size((ssz.cx + nx - 1) / nx, (ssz.cy + ny - 1) / ny);
	Buffer<RGBAV> b(tsz.cx);
	ImageBuffer ib(tsz);
	RGBA *it = ~ib;
	int scx0 = ssz.cx / nx * nx;
	Buffer<int> div(tsz.cx);
	for(int yy = 0; yy < ssz.cy; yy += ny) {
		for(int i = 0; i < tsz.cx; i++)
			b[i].Clear();
		memset(div, 0, tsz.cx * sizeof(int));
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const RGBA *s = img[yy + yi];
				const RGBA *e = s + scx0;
				const RGBA *e2 = s + ssz.cx;
				RGBAV *t = ~b;
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
		const RGBAV *s = ~b;
		const int *dv = div;
		for(int x = 0; x < tsz.cx; x++)
			*it++ = (s++)->Get(*dv++);
	}
	return ib;
}

struct PainterImageSpan : SpanSource {
	LinearInterpolator interpolator;
	int         ax, ay, cx, cy, maxx, maxy;
	byte        style;
	byte        hstyle, vstyle;
	bool        fast;
	bool        fixed;
	Image       image;
	
	void Set(const Xform2D& m, const Image& img) {
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
			interpolator.Set(Inverse(m));
		else {
			image = DownScale(image, nx, ny);
			interpolator.Set(Inverse(m) * Xform2D::Scale(1.0 / nx, 1.0 / ny));			
		}
		cx = image.GetWidth();
		cy = image.GetHeight();
		maxx = cx - 1;
		maxy = cy - 1;
		ax = 6000000 / cx * cx * 2;
		ay = 6000000 / cy * cy * 2;
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
		interpolator.Begin(x, y, len);
		fixed = hstyle && vstyle;
		while(len--) {
			Point h = interpolator.Get();
	//		h -= 128;
			Point l = h >> 8;
			if(hstyle == FILL_HREPEAT)
				l.x = (l.x + ax) % cx;
			if(vstyle == FILL_VREPEAT)
				l.y = (l.y + ay) % cy;
			if(fast) {
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy)
					*span = Pixel(l.x, l.y);
				else
				if(style == 0 && (l.x < -1 || l.x > cx || l.y < -1 || l.y > cy))
					*span = RGBAZero();
				else
					*span = GetPixel(l.x, l.y);
			}
			else {
				RGBAV v;
				v.Set(0);
	//			v.Set(256 * 256 / 2);
				h.x &= 255;
				h.y &= 255;
				Point u = -h + 256;
				if(l.x > 0 && l.x < maxx && l.y > 0 && l.y < maxy) {
					v.Put(u.x * u.y, Pixel(l.x, l.y));
					v.Put(h.x * u.y, Pixel(l.x + 1, l.y));
					v.Put(u.x * h.y, Pixel(l.x, l.y + 1));
					v.Put(h.x * h.y, Pixel(l.x + 1, l.y + 1));
				}
				else
				if(style == 0 && (l.x < -1 || l.x > cx || l.y < -1 || l.y > cy))
					v.Set(0);
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
	PainterImageSpan ss;
	ss.style = byte(flags & 15);
	ss.hstyle = byte(flags & 3);
	ss.vstyle = byte(flags & 12);
	ss.fast = flags & FILL_FAST;
	Xform2D m = transsrc * pathattr.mtx;
	ss.Set(m, image);
	RenderPath(width, &ss, RGBAZero());
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

END_UPP_NAMESPACE
