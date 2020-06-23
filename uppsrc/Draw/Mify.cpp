#include "Draw.h"

namespace Upp {
	
#ifdef CPU_SSE2

Image Minify(const Image& img, int nx, int ny, bool co)
{
	ASSERT(nx > 0 && ny > 0);
	Size ssz = img.GetSize();
	Size tsz = Size((ssz.cx + nx - 1) / nx, (ssz.cy + ny - 1) / ny);
	ImageBuffer ib(tsz);
	int scx0 = ssz.cx / nx * nx;
	auto do_line = [&](int ty, __m128 *b, __m128 *div) {
		memset(b, 0, tsz.cx * sizeof(__m128));
		memset(div, 0, tsz.cx * sizeof(__m128));
		__m128 v1 = _mm_set1_ps(1);
		__m128 vnx = _mm_set1_ps(nx);
		int yy = ny * ty;
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const RGBA *s = img[yy + yi];
				const RGBA *e = s + scx0;
				const RGBA *e2 = s + ssz.cx;
				__m128 *t = b;
				__m128 *d = div;
				while(s < e) {
					__m128 px = _mm_setzero_ps();
					__m128 dv = _mm_setzero_ps();
					for(int n = nx; n--;) {
						px = _mm_add_ps(px, LoadRGBAF(s++));
						dv = _mm_add_ps(dv, v1);
					}
					*t++ = px;
					*d++ = dv;
				}
				if(s < e2) {
					__m128 px = _mm_setzero_ps();
					__m128 dv = _mm_setzero_ps();
					while(s < e2) {
						px = _mm_add_ps(px, LoadRGBAF(s++));
						dv = _mm_add_ps(px, v1);
					}
					*t++ = px;
					*d++ = dv;
				}
				ASSERT(t == b + tsz.cx);
			}
		}
		__m128 *s = b;
		__m128 *d = div;
		RGBA *t = ~ib + ty * tsz.cx;
		RGBA *e = t + tsz.cx;
		while(t < e)
			StoreRGBAF(t++, _mm_div_ps(*s++, *d++));
	};
	if(co) {
		CoWork cw;
		cw * [&] {
			Buffer<__m128> div(tsz.cx);
			Buffer<__m128> b(tsz.cx);
			for(;;) {
				int y = cw.Next();
				if(y >= tsz.cy)
					break;
				do_line(y, b, div);
			}
		};
	}
	else {
		Buffer<__m128> div(tsz.cx);
		Buffer<__m128> b(tsz.cx);
		for(int y = 0; y < tsz.cy; y++)
			do_line(y, b, div);
	}
	return ib;
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
	return ib;
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
	b.SetResolution(img.GetResolution());
	return b;
}

};