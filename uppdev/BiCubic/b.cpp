#include "bicubic.h"

#if 1
double BiCubicKernel2(double x)
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

	return (1 / 6.0) * r;
}

double MagicKernel(double x)
{
	x = abs(x);
	return x > 1.3 ? 0 : 1.5 - x;
}

double Kernel(double x)
{
	return MagicKernel(x);
}

Image RescaleBicubic2(Image& img, int cx, int cy)
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
			double w = 0;
			for(int yy = -2; yy <= 2; yy++) {
				double ky = Kernel(yy - dy);
				const RGBA *l = img[minmax(sy + yy, 0, isz.cy - 1)];
				for(int xx = -2; xx <= 2; xx++) {
					const RGBA& s = l[minmax(sx + xx, 0, isz.cx - 1)];
//					double weight = ky * BiCubicKernel2(xx - dx);
					double weight = ky * Kernel(xx - dx);
					red   += weight * s.r;
					green += weight * s.g;
					blue  += weight * s.b;
					alpha += weight * s.a;
					w += weight;
				}
    		}
			t->r = (int)(red / w);
			t->g = (int)(green / w);
			t->b = (int)(blue / w);
			t->a = (int)(alpha / w);
			t++;
		}
	}
	return ib;
}
#endif