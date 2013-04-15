#include "bicubic.h"

#if 1

#define LDUMP(x) // DUMP(x)

static const int *sGetKernel(double (*kfn)(double x), int a, int shift)
{
	INTERLOCKED {
		static VectorMap<Tuple3<uintptr_t, int, int>, Buffer<int> > kache;
		Tuple3<uintptr_t, int, int> key = MakeTuple((uintptr_t)kfn, a, shift);
		Buffer<int> *k = kache.FindPtr(key);
		if(k)
			return *k;
		RTIMING("GenKernel");
		Buffer<int>& ktab = kache.GetAdd(key);
		ktab.Alloc(((2 * a) << shift) + 1);
		for(int i = 0; i < ((2 * a) << shift) + 1; i++)
			ktab[i] = int((1 << shift) * (*kfn)((double)i / (1 << shift) - a));
		return ktab;
	}
}

force_inline
int sGetk(const int *kernel, int x, int a, int shift)
{
	x += a << shift;
	ASSERT(x >= 0 && x < ((2 * a) << shift) + 1);
	return kernel[x];
}

static int sGeta(int a, int src, int tgt, int method, int& shift)
{
	if(method != DOWNSCALE_WIDE)
		return a;
	int n = min(max(src / tgt, 1) * a, 31);
	shift = 8 - n / 8;
	return n;
}

Image RescaleWithKernel2(const Image& img, int cx, int cy, double (*kfn)(double x), int a, int method)
{
	Size isz = img.GetSize();

	int shiftx, shifty;
	int ax = sGeta(a, isz.cx, cx, method, shiftx);
	int ay = sGeta(a, isz.cy, cy, method, shifty);

	int shift = min(shiftx, shifty);
	
	const int *kernel = sGetKernel(kfn, a, shift);

	Buffer<int> px(cx * 2 * ax * 2 * ay * 2);
	int *xd = px;
	for(int x = 0; x < cx; x++) {
		int sx = x * isz.cx / cx;
		int dx = ((x * isz.cx) << shift) / cx - (sx << shift);
		for(int yy = -ay + 1; yy <= ay; yy++) {
			for(int xx = -ax + 1; xx <= ax; xx++) {
				*xd++ = minmax(sx + xx, 0, isz.cx - 1);
				*xd++ = sGetk(kernel, ((xx << shift) - dx) * a / ax, a, shift);
			}
		}
	}

	ImageBuffer ib(cx, cy);
	RGBA *t = ~ib;
	for(int y = 0; y < cy; y++) {
		int sy = y * isz.cy / cy;
		int dy = ((y * isz.cy) << shift) / cy - (sy << shift);
		xd = ~px;
		Buffer<int> py(2 * ay * 2);
		int *yd = py;
		for(int yy = -ay + 1; yy <= ay; yy++) {
			*yd++ = sGetk(kernel, ((yy << shift) - dy) * a / ay, a, shift);
			*yd++ = minmax(sy + yy, 0, isz.cy - 1);
		}
		for(int x = 0; x < cx; x++) {
			int red   = 0;
			int green = 0;
			int blue  = 0;
			int alpha = 0;
			int w = 0;
			yd = py;
			for(int yy = 2 * ay; yy-- > 0;) {
				int ky = *yd++;
				const RGBA *l = img[*yd++];
				for(int xx = 2 * ax; xx-- > 0;) {
					const RGBA& s = l[*xd++];
					int weight = ky * *xd++;
					red   += weight * s.r;
					green += weight * s.g;
					blue  += weight * s.b;
					alpha += weight * s.a;
					w += weight;
				}
			}
			t->r = Saturate255(red / w);
			t->g = Saturate255(green / w);
			t->b = Saturate255(blue / w);
			t->a = Saturate255(alpha / w);
			t++;
		}
	}
	return ib;
}

#endif
