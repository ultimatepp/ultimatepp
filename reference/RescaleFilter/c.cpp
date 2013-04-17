#include "bicubic.h"

#if 1

#define LDUMP(x)

#define up(x)  ((x) << 8)
#define dn(x)  ((x + (1 << 7)) >> 8)
#define dn2(x)  ((x + (1 << 15)) >> 16)

double BiCubicKernel3Imp(double x)
{
	double r = 0;

	double xp2 = x + 2;
	double xp1 = x + 1;
	double xm1 = x - 1;

	if(xp2 > 0)
		r += xp2 * xp2 * xp2;
	if(xp1 > 0)
		r -= 4 * xp1 * xp1 * xp1;
	if(x > 0)
		r += 6 * x * x * x;
	if(xm1 > 0)
		r -= 4 * xm1 * xm1 * xm1;

	return(r / 6.0);
}

static int kernel[up(4)];

force_inline
int BiCubicKernel3(int x)
{
	x += up(2);
	ASSERT(x >= 0 && x <= up(4) + 1);
	return kernel[x];
}

Image RescaleBicubic3(Image& img, int cx, int cy)
{
	ONCELOCK {
		for(int i = 0; i < up(4); i++) {
			kernel[i] = int(up(1) * BiCubicKernel3Imp((double)i / up(1) - 2));
		}
	}
	Size isz = img.GetSize();
	ImageBuffer ib(cx, cy);
	RGBA *t = ~ib;
	for(int y = 0; y < cy; y++) {
		int sy = y * isz.cy / cy;
		int dy = up(y * isz.cy) / cy - up(sy);
		for(int x = 0; x < cx; x++) {
			int sx = x * isz.cx / cx;
			int dx = up(x * isz.cx) / cx - up(sx);
			int red   = 0;
			int green = 0;
			int blue  = 0;
			int alpha = 0;
			int w = 0;
			for(int yy = -1; yy <= 2; yy++) {
				int ky = BiCubicKernel3(up(yy) - dy);
				const RGBA *l = img[minmax(sy + yy, 0, isz.cy - 1)];
				for(int xx = -1; xx <= 2; xx++) {
					const RGBA& s = l[minmax(sx + xx, 0, isz.cx - 1)];
					int weight = ky * BiCubicKernel3(up(xx) - dx);
					LDUMP(weight);
					red   += weight * s.r;
					green += weight * s.g;
					blue  += weight * s.b;
					LDUMP((int)s.a);
					alpha += weight * s.a;
					LDUMP(alpha);
					w += weight;
				}
    		}
			t->r = dn2(red);
			t->g = dn2(green);
			t->b = dn2(blue);
			t->a = dn2(alpha);
			LDUMP((int)t->a);
			LDUMP(w);
	//		return ib;
			t++;
		}
	}
	return ib;
}

#endif