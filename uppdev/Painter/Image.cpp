#include "Painter.h"

NAMESPACE_UPP

#if 0 // does not seem to help...

Image MipMap(const Image& img)
{
	Size ssz = img.GetSize() / 2;
	Size msz = (img.GetSize() + 1) / 2;
	ImageBuffer ib(msz);
	for(int y = 0; y < ssz.cy; y++) {
		const RGBA *s1 = img[2 * y];
		const RGBA *s2 = img[2 * y + 1];
		const RGBA *e = s1 + 2 * ssz.cx;
		RGBA *t = ib[y];
		while(s1 < e) {
			t->r = (s1[0].r + s1[1].r + s2[0].r + s2[1].r) >> 2;
			t->g = (s1[0].g + s1[1].g + s2[0].g + s2[1].g) >> 2;
			t->b = (s1[0].b + s1[1].b + s2[0].b + s2[1].b) >> 2;
			t->a = (s1[0].a + s1[1].a + s2[0].a + s2[1].a) >> 2;
			t++;
			s1 += 2;
			s2 += 2;
		}
		if(ssz.cx < msz.cx) {
			t->r = (s1[0].r + s2[0].r) >> 2;
			t->g = (s1[0].g + s2[0].g) >> 2;
			t->b = (s1[0].b + s2[0].b) >> 2;
			t->a = (s1[0].a + s2[0].a) >> 2;
		}
	}
	if(ssz.cy < msz.cy) {
		const RGBA *s1 = img[img.GetSize().cy - 1];
		const RGBA *e = s1 + 2 * ssz.cx;
		RGBA *t = ib[msz.cy - 1];
		while(s1 < e) {
			t->r = (s1[0].r + s1[1].r) >> 2;
			t->g = (s1[0].g + s1[1].g) >> 2;
			t->b = (s1[0].b + s1[1].b) >> 2;
			t->a = (s1[0].a + s1[1].a) >> 2;
			t++;
			s1 += 2;
		}
		if(ssz.cx < msz.cx) {
			t->r = s1[0].r >> 2;
			t->g = s1[0].g >> 2;
			t->b = s1[0].b >> 2;
			t->a = s1[0].a >> 2;
		}
	}
	return ib;
}

Image MakeMipMap(const Image& m, int level);

struct MipMapMaker : ImageMaker {
	int   level;
	Image image;

	virtual String Key() const {
		String h;
		RawCat(h, image.GetSerialId());
		RawCat(h, level);
		return h;
	}
	virtual Image Make() const {
		Size sz = image.GetSize();
		if(sz.cx && sz.cx) {
			if(level <= 0)
				return image;
			return MipMap(MakeMipMap(image, level - 1));
		}
		return Image();
	}
};

Image MakeMipMap(const Image& img, int level)
{
	MipMapMaker m;
	m.image = img;
	m.level = level;
	return MakeImage(m);
}

#endif

struct PainterImageSpan : SpanSource {
	struct RGBAV {
		dword r, g, b, a;
	
		void Set(dword v) { r = g = b = a = v; }
		void Put(dword weight, const RGBA& src) {
			r += weight * src.r;
			g += weight * src.g;
			b += weight * src.b;
			a += weight * src.a;
		}
    };

	LinearInterpolator interpolator;
	int         ax, ay, cx, cy, maxx, maxy;
	byte        style;
	byte        hstyle, vstyle;
	bool        fast;
	bool        fixed;
	Image       image;
	
	void Set(const Xform2D& m, const Image& img) {
		int level = 0;
#if 0 // no mipmap for now
		double q = 1;
		if(!fast) {
			double q = 1;
			Pointf sc = m.GetScaleXY();
			if(sc.x >= 0.01 && sc.y >= 0.01)
				while(sc.x < 0.5 && sc.y < 0.5) {
					level++;
					sc.x *= 2;
					sc.y *= 2;
					q /= 2;
				}
		}
		if(q != 1)
			interpolator.Set(Inverse(m) * Xform2D::Scale(q));
		else
#endif
			interpolator.Set(Inverse(m));
		image = img;
//		image = MakeMipMap(img, level);
		cx = image.GetWidth();
		cy = image.GetHeight();
		maxx = cx - 1;
		maxy = cy - 1;
		ax = 6000000 / cx * cx;
		ay = 6000000 / cy * cy;
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
			int x_hr;
			int y_hr;
			interpolator.Get(x_hr, y_hr);
			x_hr -= 128;
			y_hr -= 128;
			int x_lr = x_hr >> 8;
			int y_lr = y_hr >> 8;
			if(hstyle == FILL_HREPEAT)
				x_lr = (x_lr + ax) % cx;
			if(vstyle == FILL_VREPEAT)
				y_lr = (y_lr + ay) % cy;
			if(fast) {
				if(x_lr > 0 && x_lr < maxx && y_lr > 0 && y_lr < maxy)
					*span = Pixel(x_lr, y_lr);
				else
				if(style == 0 && (x_lr < -1 || x_lr > cx || y_lr < -1 || y_lr > cy))
					*span = RGBAZero();
				else
					*span = GetPixel(x_lr, y_lr);
			}
			else {
				RGBAV v;
				v.Set(256 * 256 / 2);
				x_hr &= 255;
				y_hr &= 255;
				if(x_lr > 0 && x_lr < maxx && y_lr > 0 && y_lr < maxy) {
					v.Put((256 - x_hr) * (256 - y_hr), Pixel(x_lr, y_lr));
					v.Put(x_hr * (256 - y_hr), Pixel(x_lr + 1, y_lr));
					v.Put((256 - x_hr) * y_hr, Pixel(x_lr, y_lr + 1));
					v.Put(x_hr * y_hr, Pixel(x_lr + 1, y_lr + 1));
				}
				else
				if(style == 0 && (x_lr < -1 || x_lr > cx || y_lr < -1 || y_lr > cy))
					v.Set(0);
				else {
					v.Put((256 - x_hr) * (256 - y_hr), GetPixel(x_lr, y_lr));
					v.Put(x_hr * (256 - y_hr), GetPixel(x_lr + 1, y_lr));
					v.Put((256 - x_hr) * y_hr, GetPixel(x_lr, y_lr + 1));
					v.Put(x_hr * y_hr, GetPixel(x_lr + 1, y_lr + 1));
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
	Close();
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return;
	PainterImageSpan ss;
	ss.style = flags & 15;
	ss.hstyle = flags & 3;
	ss.vstyle = flags & 12;
	ss.fast = flags & FILL_FAST;
	ss.Set(transsrc * pathattr.mtx, image);
	RenderPath(width, &ss, RGBAZero());
}

void BufferPainter::FillOp(const Image& image, const Xform2D& transsrc, dword flags)
{
	RenderImage(-1, image, transsrc, flags);
}

void BufferPainter::StrokeOp(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	RenderImage(width, image, transsrc, flags);
}

END_UPP_NAMESPACE
