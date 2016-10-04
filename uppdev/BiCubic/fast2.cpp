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

static int sGeta(int a, int src, int tgt, int& shift)
{
	int n = min(max(src / tgt, 1) * a, 31);
	shift = 8 - n / 8;
	return n;
}

Image RescaleFilter(const Image& img, Size sz, const Rect& sr,
                    double (*kfn)(double x), int a,
                    Gate2<int, int> progress)
{
	ASSERT(Rect(img.GetSize()).Contains(sr));
	Size isz = sr.GetSize();

	int shiftx, shifty;
	int ax = sGeta(a, isz.cx, sz.cx, shiftx);
	int ay = sGeta(a, isz.cy, sz.cy, shifty);

	int shift = min(shiftx, shifty);
	
	const int *kernel = sGetKernel(kfn, a, shift);

	Buffer<int> px(sz.cx * 2 * ax * 2 * ay * 2);
	int *xd = px;
	for(int x = 0; x < sz.cx; x++) {
		int sx = x * isz.cx / sz.cx;
		int dx = ((x * isz.cx) << shift) / sz.cx - (sx << shift);
		for(int yy = -ay + 1; yy <= ay; yy++) {
			for(int xx = -ax + 1; xx <= ax; xx++) {
				*xd++ = minmax(sx + xx, 0, isz.cx - 1) + sr.left;
				*xd++ = sGetk(kernel, ((xx << shift) - dx) * a / ax, a, shift);
			}
		}
	}

	ImageBuffer ib(sz);
	RGBA *t = ~ib;
	for(int y = 0; y < sz.cy; y++) {
		if(progress(y, sz.cy))
			break;
		int sy = y * isz.cy / sz.cy;
		int dy = ((y * isz.cy) << shift) / sz.cy - (sy << shift);
		xd = ~px;
		Buffer<int> py(2 * ay * 2);
		int *yd = py;
		for(int yy = -ay + 1; yy <= ay; yy++) {
			*yd++ = sGetk(kernel, ((yy << shift) - dy) * a / ay, a, shift);
			*yd++ = minmax(sy + yy, 0, isz.cy - 1) + sr.top;
		}
		for(int x = 0; x < sz.cx; x++) {
			int red   = 0;
			int green = 0;
			int blue  = 0;
			int alpha = 0;
			int w = 0;
			yd = py;
			int hasalpha = 0;
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
					hasalpha |= s.a - 255;
					w += weight;
				}
			}
			if(hasalpha) {
				t->a = alpha = Saturate255(alpha / w);
				t->r = clamp(red / w, 0, alpha);
				t->g = clamp(green / w, 0, alpha);
				t->b = clamp(blue / w, 0, alpha);
			}
			else {
				t->a = 255;
				t->r = Saturate255(red / w);
				t->g = Saturate255(green / w);
				t->b = Saturate255(blue / w);
			}
			t++;
		}
	}
	return ib;
}

Image RescaleFilter(const Image& img, Size sz,
                    double (*kfn)(double x), int a,
                    Gate2<int, int> progress)
{
	return RescaleFilter(img, sz, img.GetSize(), kfn, a, progress);
}

#endif
