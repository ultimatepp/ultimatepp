#include "bicubic.h"

#if 1

#define LDUMP(x) // DUMP(x)

#define up(x)  ((x) << 8)
#define dn(x)  ((x + (1 << 7)) >> 8)
#define dn2(x)  ((x + (1 << 15)) >> 16)

static int sGeta(int a, int src, int tgt, int method)
{
	if(method != DOWNSCALE_WIDE)
		return a;
	return min(max(src / tgt, 1) * a, 7);
}

force_inline
int sGetk(const int *kernel, int x, int a)
{
	x += up(a);
	ASSERT(x >= 0 && x < up(2 * a) + 1);
	return kernel[x];
}

static const int *sGetKernel(double (*kfn)(double x), int a)
{
	INTERLOCKED {
		static VectorMap<Tuple2<uintptr_t, int>, Buffer<int> > kache;
		Tuple2<uintptr_t, int> key = MakeTuple((uintptr_t)kfn, a);
		Buffer<int> *k = kache.FindPtr(key);
		if(k)
			return *k;
		RTIMING("GenKernel");
		Buffer<int>& ktab = kache.GetAdd(key);
		ktab.Alloc(up(2 * a) + 1);
		for(int i = 0; i < up(2 * a) + 1; i++)
			ktab[i] = int(up(1) * (*kfn)((double)i / up(1) - a));
		return ktab;
	}
}

Image RescaleWithKernel(const Image& img, int cx, int cy, double (*kfn)(double x), int a, int method)
{
	// ToDo: Cache this!
	const int *kernel = sGetKernel(kfn, a);

	Size isz = img.GetSize();

	int ax = sGeta(a, isz.cx, cx, method);
	int ay = sGeta(a, isz.cy, cy, method);
	
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
			for(int yy = -ay + 1; yy <= ay; yy++) {
				int ky = sGetk(kernel, (up(yy) - dy) * a / ay, a);
				const RGBA *l = img[minmax(sy + yy, 0, isz.cy - 1)];
				for(int xx = -ax + 1; xx <= ax; xx++) {
					const RGBA& s = l[minmax(sx + xx, 0, isz.cx - 1)];
					int weight = ky * sGetk(kernel, (up(xx) - dx) * a / ax, a);
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
			t->r = Saturate255(red / w);
			t->g = Saturate255(green / w);
			t->b = Saturate255(blue / w);
			t->a = Saturate255(alpha / w);
			LDUMP((int)t->a);
			LDUMP(w);
			t++;
		}
	}
	return ib;
}

#endif
