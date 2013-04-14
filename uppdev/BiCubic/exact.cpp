#include "bicubic.h"

#if 1

static int sGetA(int a, int src, int tgt, int method)
{
	if(method != DOWNSCALE_WIDE)
		return a;
	return max(src / tgt, 1) * a;
}

Image RescaleWithKernelE(const Image& _img, int cx, int cy, double (*kernel)(double x), int a,
                         int method)
{
	Image img = _img;
	Size isz = img.GetSize();
	if(method == DOWNSCALE_MIPMAP) {
		Size sz = isz / Size(cx, cy);
		if(sz.cx >= a || sz.cy >= a) {
			img = DownScale(img, max(sz.cx, 1), max(sz.cy, 1));
			isz = img.GetSize();
		}
	}
	ImageBuffer ib(cx, cy);
	RGBA *t = ~ib;
	int ax = sGetA(a, isz.cx, cx, method);
	int ay = sGetA(a, isz.cy, cy, method);
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
			for(int yy = -ay + 1; yy <= ay; yy++) {
				double ky = kernel((yy - dy) * a / ay);
				const RGBA *l = img[minmax(sy + yy, 0, isz.cy - 1)];
				for(int xx = -ax + 1; xx <= ax; xx++) {
					const RGBA& s = l[minmax(sx + xx, 0, isz.cx - 1)];
					double weight = ky * kernel((xx - dx) * a / ax);
					red   += weight * s.r;
					green += weight * s.g;
					blue  += weight * s.b;
					alpha += weight * s.a;
					w += weight;
				}
    		}
			t->r = Saturate255((int)(red / w));
			t->g = Saturate255((int)(green / w));
			t->b = Saturate255((int)(blue / w));
			t->a = Saturate255((int)(alpha / w));
			t++;
		}
	}
	return ib;
}
#endif