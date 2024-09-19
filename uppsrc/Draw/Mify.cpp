#include "Draw.h"

namespace Upp {
	
#ifdef CPU_SIMD

Image Minify(const Image& img, int nx, int ny, bool co)
{
	ASSERT(nx > 0 && ny > 0);
	Size ssz = img.GetSize();
	Size tsz = Size((ssz.cx + nx - 1) / nx, (ssz.cy + ny - 1) / ny);
	ImageBuffer ib(tsz);
	int scx0 = ssz.cx / nx * nx;
	auto do_line = [&](int ty, f32x4 *b, f32x4 *div) {
		memset((void *)b, 0, tsz.cx * sizeof(f32x4));
		memset((void *)div, 0, tsz.cx * sizeof(f32x4));
		f32x4 v1 = f32all(1);
		f32x4 vnx = f32all(nx);
		int yy = ny * ty;
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const RGBA *s = img[yy + yi];
				const RGBA *e = s + scx0;
				const RGBA *e2 = s + ssz.cx;
				f32x4 *t = b;
				f32x4 *d = div;
				while(s < e) {
					f32x4 px = 0;
					for(int n = nx; n--;)
						px += LoadRGBAF(s++);
					*t += px;
					*d += vnx;
					t++;
					d++;
				}
				if(s < e2) {
					f32x4 px = 0;
					f32x4 dv = 0;
					while(s < e2) {
						px += LoadRGBAF(s++);
						dv += v1;
					}
					*t += px;
					*d += dv;
					t++;
					d++;
				}
				ASSERT(t == b + tsz.cx);
			}
		}
		f32x4 *s = b;
		f32x4 *d = div;
		RGBA *t = ~ib + ty * tsz.cx;
		RGBA *e = t + tsz.cx;
		while(t < e)
			StoreRGBAF(t++, *s++ / *d++);
	};
	if(co) {
		CoWork cw;
		cw * [&] {
			Buffer<f32x4> div(tsz.cx);
			Buffer<f32x4> b(tsz.cx);
			for(;;) {
				int y = cw.Next();
				if(y >= tsz.cy)
					break;
				do_line(y, b, div);
			}
		};
	}
	else {
		Buffer<f32x4> div(tsz.cx);
		Buffer<f32x4> b(tsz.cx);
		for(int y = 0; y < tsz.cy; y++)
			do_line(y, b, div);
	}
	return Image(ib);
}

#else

Image Minify(const Image& img, int nx, int ny, bool co)
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
	return Image(ib);
}

#endif

struct MinifyImageMaker : public ImageMaker {
	Image image;
	int   nx;
	int   ny;
	bool  co;

	virtual String Key() const;
	virtual Image  Make() const;
};

String MinifyImageMaker::Key() const
{
	String key;
	RawCat(key, image.GetSerialId());
	RawCat(key, nx);
	RawCat(key, ny);
	// we are not adding co as that is just a hint for actual image making
	return key;
}

Image MinifyImageMaker::Make() const
{
	return Minify(image, nx, ny, co);
}

Image MinifyCached(const Image& img, int nx, int ny, bool co)
{
	MinifyImageMaker m;
	m.image = img;
	m.nx = nx;
	m.ny = ny;
	m.co = co;
	return MakeImage(m);
}

Image Magnify(const Image& img, const Rect& src_, int nx, int ny, bool co)
{
	Rect src = src_ & img.GetSize();
	bool xdown = nx < 0;
	nx = abs(nx);
	Size ssz = src.GetSize();
	if(ssz.cx <= 0 || ssz.cy <= 0 || nx == 0 || ny == 0)
		return Image();
	int ncx = xdown ? ssz.cx / nx : ssz.cx * nx;
	Size tsz(ncx, ssz.cy * ny);
	ImageBuffer b(tsz);
	CoFor(co, ssz.cy, [&](int y) {
		const RGBA *s = img[src.top + y] + src.left;
		const RGBA *e = s + ssz.cx;
		RGBA *t = ~b + y * ncx * ny;
		RGBA *q = t;
		while(s < e) {
			Fill(q, *s, nx);
			q += nx;
			s++;
		}
		for(int n = ny - 1; n--;) {
			memcpy(q, t, ncx * sizeof(RGBA));
			q += ncx;
		}
	});
	
	auto HotSpot = [&](Point p) {
		p = (p - src.TopLeft()) * Point(nx, ny);
		return Point(clamp(p.x, 0, tsz.cx), clamp(p.y, 0, tsz.cy));
	};
	
	b.SetHotSpot(HotSpot(img.GetHotSpot()));
	b.Set2ndSpot(HotSpot(img.Get2ndSpot()));
	
	return Image(b);
}

Image Magnify(const Image& img, int nx, int ny, bool co)
{
	if(nx == 1 && ny == 1)
		return img;
	return Magnify(img, img.GetSize(), nx, ny, co);
}

Image DownSample3x(const Image& src, bool co)
{
	Size tsz = src.GetSize() / 3;
	if(tsz.cx * tsz.cy == 0)
		return Null;
	ImageBuffer ib(tsz);
	int w = src.GetSize().cx;
	int w2 = 2 * w;
	CoFor(co, tsz.cy, [&](int y) {
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
	});
	ib.SetHotSpot(src.GetHotSpot() / 3);
	ib.Set2ndSpot(src.Get2ndSpot() / 3);
	return Image(ib);
}

Image DownSample2x(const Image& src, bool co)
{
	Size tsz = src.GetSize() / 2;
	if(tsz.cx * tsz.cy == 0)
		return Null;
	ImageBuffer ib(tsz);
	int w = src.GetSize().cx;
	CoFor(co, tsz.cy, [&](int y) {
		RGBA *t = ib[y];
		RGBA *e = t + tsz.cx;
		const RGBA *s = src[2 * y];
		while(t < e) {
			int r, g, b, a;
			const RGBA *q;
			r = g = b = a = 0;
#define S__SUM(delta) q = s + delta; r += q->r; g += q->g; b += q->b; a += q->a;
			S__SUM(0) S__SUM(1)
			S__SUM(w + 0) S__SUM(w + 1)
#undef  S__SUM
			t->a = a / 4;
			t->r = r / 4;
			t->g = g / 4;
			t->b = b / 4;
			t++;
			s += 2;
		}
	});
	ib.SetHotSpot(src.GetHotSpot() / 2);
	ib.Set2ndSpot(src.Get2ndSpot() / 2);
	return Image(ib);
}

};