#include "Draw.h"

NAMESPACE_UPP

double BicubicKernel(double x)
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
	return 1 / 6.0 * r;
}

Image RescaleBicubic(const Image& img, int cx, int cy)
{
	Size isz = img.GetSize();
	ImageBuffer ib(cx, cy);
	RGBA *t = ~ib;
	double inv_cx = 1.0 / cx;
	for(int y = 0; y < cy; y++) {
		int sy = (y * isz.cy) / cy;
		double dy = y * isz.cy / (double)cy - sy;
		for(int x = 0; x < cx; x++) {
			int sx = (x * isz.cx) / cx;
			double dx = x * isz.cx * inv_cx - sx;
			double red   = 0;
			double green = 0;
			double blue  = 0;
			double alpha = 0;
			for(int yy = -1; yy <= 2; yy++) {
				double ky = BicubicKernel(yy - dy);
				const RGBA *l = img[minmax(sy + yy, 0, isz.cy - 1)];
				for(int xx = -1; xx <= 2; xx++) {
					const RGBA& s = l[minmax(sx + xx, 0, isz.cx - 1)];
					double weight = ky * BicubicKernel(xx - dx);
					red   += weight * s.r;
					green += weight * s.g;
					blue  += weight * s.b;
					alpha += weight * s.a;
				}
    		}
			t->r = (int)fround(red);
			t->g = (int)fround(green);
			t->b = (int)fround(blue);
			t->a = (int)fround(alpha);
			t++;
		}
	}
	return ib;
}

Image RescaleBicubic(const Image& src, Size sz)
{
	return RescaleBicubic(src, sz.cx, sz.cy);
}

END_UPP_NAMESPACE
