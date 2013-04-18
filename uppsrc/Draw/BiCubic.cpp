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

Image RescaleBicubic(const Image& img, Size sz, const Rect& sr, Gate2<int, int> progress)
{
	ASSERT(sr.top >= 0 && sr.left >= 0 && sr.right <= img.GetWidth() && sr.bottom <= img.GetHeight());
	Size isz = sr.GetSize();
	ImageBuffer ib(sz);
	RGBA *t = ~ib;
	double inv_cx = 1.0 / sz.cx;
	for(int y = 0; y < sz.cy; y++) {
		if(progress(y, sz.cy))
			return ib;
		int sy = y * isz.cy / sz.cy;
		double dy = y * isz.cy / (double)sz.cy - sy;
		for(int x = 0; x < sz.cx; x++) {
			int sx = x * isz.cx / sz.cx;
			double dx = x * isz.cx * inv_cx - sx;
			double red   = 0;
			double green = 0;
			double blue  = 0;
			double alpha = 0;
			for(int yy = -1; yy <= 2; yy++) {
				double ky = BicubicKernel(yy - dy);
				const RGBA *l = img[minmax(sy + yy + sr.top, sr.top, sr.bottom - 1)];
				for(int xx = -1; xx <= 2; xx++) {
					const RGBA& s = l[minmax(sx + xx + sr.left, sr.left, sr.right - 1)];
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

Image RescaleBicubic(const Image& img, int cx, int cy, Gate2<int, int> progress)
{
	return RescaleBicubic(img, Size(cx, cy), img.GetSize(), progress);
}

Image RescaleBicubic(const Image& img, Size sz, Gate2<int, int> progress)
{
	return RescaleBicubic(img, sz, img.GetSize(), progress);
}

END_UPP_NAMESPACE
