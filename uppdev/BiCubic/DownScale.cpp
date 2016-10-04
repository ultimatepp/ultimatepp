#include "bicubic.h"

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
	int div = nx * ny;
	Buffer<RGBAV> b(tsz.cx);
	ImageBuffer ib(tsz);
	RGBA *it = ~ib;
	int scx0 = ssz.cx / nx * nx;
	for(int yy = 0; yy < ssz.cy; yy += ny) {
		for(int i = 0; i < tsz.cx; i++)
			b[i].Clear();
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const RGBA *s = img[yy + yi];
				const RGBA *e = s + scx0;
				const RGBA *e2 = s + ssz.cx;
				RGBAV *t = ~b;
				while(s < e) {
					for(int n = nx; n--;)
						t->Put(*s++);
					t++;
				}
				while(s < e2)
					t->Put(*s++);
			}
		}
		const RGBAV *s = ~b;
		for(int x = 0; x < tsz.cx; x++)
			*it++ = (s++)->Get(div);
	}
	return ib;
}
