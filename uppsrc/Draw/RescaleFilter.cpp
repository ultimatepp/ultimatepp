#include "Draw.h"

namespace Upp {

#define LDUMP(x) // DUMP(x)

static double sNearest(double x)
{
	return (double)(x >= -0.5 && x <= 0.5);
}


static double sBiCubic_(double x, double B, double C)
{
	x = fabs(x);
	double x2 = x * x;
	double x3 = x * x * x;
	return
		1 / 6.0 * (x < 1 ? (12 - 9*B - 6*C) * x3 + (-18 + 12*B + 6*C) * x2 + (6 - 2*B) :
		           x < 2 ? (-B - 6*C) * x3 + (6*B + 30*C) * x2 + (-12*B - 48*C) *x + (8*B + 24*C) :
	               0);
}

static double sBspline(double x)
{
	return sBiCubic_(x, 1, 0);
}

static double sMitchell(double x)
{
	return sBiCubic_(x, 1 / 3.0, 1 / 3.0);
}

static double sCatmullRom(double x)
{
	return sBiCubic_(x, 0, 1 / 2);
}

static double sLinear(double x)
{
	x = fabs(x);
	if(x > 1)
		return 0;
	return 1 - fabs(x);
}

static double sLanczos(double x, int a)
{
	x = fabs(x);
	if(x < 1e-200)
		return 1;
	if(x >= a)
		return 0;
	return a * sin(M_PI * x) * sin(M_PI * x / a) / (M_PI * M_PI * x * x);
}

static double sLanczos2(double x)
{
	return sLanczos(x, 2);
}

static double sLanczos3(double x)
{
	return sLanczos(x, 3);
}

static double sLanczos4(double x)
{
	return sLanczos(x, 4);
}

static double sLanczos5(double x)
{
	return sLanczos(x, 5);
}

static double sCostella(double x)
{
	x = fabs(x);
	return x < 0.5 ? 0.75 - x * x :
	       x < 1.5 ? 0.5 * (x - 1.5) * (x - 1.5) :
	       0;
}

Tuple2<double (*)(double), int> GetImageFilterFunction(int filter)
{
	static Tuple2<double (*)(double), int> tab[] = {
		{ sNearest, 1 },
		{ sLinear, 1 },
		{ sBspline, 2 },
		{ sCostella, 2 },
		{ sMitchell, 2 },
		{ sCatmullRom, 2 },
		{ sLanczos2, 2 },
		{ sLanczos3, 3 },
		{ sLanczos4, 4 },
		{ sLanczos5, 5 },
	};
	ASSERT(filter >= FILTER_NEAREST && filter <= FILTER_LANCZOS5);
	return tab[clamp(filter, 0, __countof(tab))];
}

void ImageFilterKernel::Init(double (*kfn)(double), int a, int src_sz, int tgt_sz)
{
	this->a = a;
	n = min(max(src_sz / tgt_sz, 1) * a, 31);
	shift = 8 - n / 8;
	mul = 1 << shift;
	kernel_size = (2 * n) << shift;
	ashift = a << shift;
	INTERLOCKED {
		static VectorMap<Tuple3<uintptr_t, int, int>, Buffer<int> > kache;
		Tuple3<uintptr_t, int, int> key = MakeTuple((uintptr_t)kfn, a, shift);
		Buffer<int> *k = kache.FindPtr(key);
		if(k)
			kernel = *k;
		else {
			Buffer<int>& ktab = kache.GetAdd(key);
			ktab.Alloc(((2 * a) << shift) + 1);
			for(int i = 0; i < ((2 * a) << shift) + 1; i++)
				ktab[i] = int((1 << shift) * (*kfn)((double)i / (1 << shift) - a));
			kernel = ktab;
		}
	}
}

ImageFilterKernel::ImageFilterKernel(double (*kfn)(double), int a, int src_sz, int tgt_sz)
{
	Init(kfn, a, src_sz, tgt_sz);
}

void ImageFilterKernel::Init(int filter, int src_sz, int tgt_sz)
{
	auto t = GetImageFilterFunction(filter);
	Init(t.a, t.b, src_sz, tgt_sz);
}

Image RescaleFilter(const Image& img, Size sz, const Rect& sr,
                    double (*kfn)(double x), int a,
                    Gate<int, int> progress, bool co)
{
	ASSERT(Rect(img.GetSize()).Contains(sr));
	Size isz = sr.GetSize();
	
	if(isz.cx <= 0 || isz.cy <= 0 || sz.cx <= 0 || sz.cy <= 0)
		return Image();

	ImageFilterKernel kx(kfn, a, isz.cx, sz.cx);
	ImageFilterKernel ky(kfn, a, isz.cy, sz.cy);
	
	Buffer<int> px(sz.cx * 2 * kx.n * 2 * ky.n * 2);
	int *xd = px;
	Size cr = (Size(1 << kx.shift, 1 << ky.shift) -
	           Size(isz.cx << kx.shift, isz.cy << ky.shift) / sz) >> 1;
	for(int x = 0; x < sz.cx; x++) {
		int dx = ((x * isz.cx) << kx.shift) / sz.cx - cr.cx;
		int sx = dx >> kx.shift;
		dx -= sx << kx.shift;
		if(dx < 0)
			dx = 0;
		for(int yy = -ky.n + 1; yy <= ky.n; yy++)
			for(int xx = -kx.n + 1; xx <= kx.n; xx++) {
				*xd++ = clamp(sx + xx, 0, isz.cx - 1) + sr.left;
				*xd++ = kx.Get(xx, dx);
			}
	}

	ImageBuffer ib(sz);
	std::atomic<int> yy(0);
	CoDo(co, [&] {
		Buffer<int> py(2 * ky.n * 2);
		for(int y = yy++; y < sz.cy; y = yy++) {
			if(progress(y, sz.cy))
				break;
			int dy = ((y * isz.cy) << ky.shift) / sz.cy - cr.cy;
			int sy = dy >> ky.shift;
			dy -= sy << ky.shift;
			if(dy < 0)
				dy = 0;
			int *xd = px;
			int *yd = py;
			for(int yy = -ky.n + 1; yy <= ky.n; yy++) {
				*yd++ = ky.Get(yy, dy);
				*yd++ = clamp(sy + yy, 0, isz.cy - 1) + sr.top;
			}
			RGBA *t = ib[y];
	#ifdef CPU_SIMD
			for(int x = 0; x < sz.cx; x++) {
				f32x4 rgbaf = 0;
				f32x4 w = 0;
				yd = py;
				for(int yy = 2 * ky.n; yy-- > 0;) {
					int ky = *yd++;
					const RGBA *l = img[*yd++];
					for(int xx = 2 * kx.n; xx-- > 0;) {
						f32x4 s = LoadRGBAF(&l[*xd++]);
						f32x4 weight = f32all(float(ky * *xd++));
						rgbaf += weight * s;
						w += weight;
					}
				}
				StoreRGBAF(t++, ClampRGBAF(rgbaf / w));
			}
	#else
			for(int x = 0; x < sz.cx; x++) {
				int red   = 0;
				int green = 0;
				int blue  = 0;
				int alpha = 0;
				int w = 0;
				yd = py;
				int hasalpha = 0;
				for(int yy = 2 * ky.n; yy-- > 0;) {
					int ky = *yd++;
					const RGBA *l = img[*yd++];
					for(int xx = 2 * kx.n; xx-- > 0;) {
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
				if(w)
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
				else
					t->a = t->r = t->g = t->b = 0;
				t++;
			}
	#endif
		}
	});
	return Image(ib);
}

Image RescaleFilter(const Image& img, Size sz,
                    double (*kfn)(double x), int a,
                    Gate<int, int> progress, bool co)
{
	return RescaleFilter(img, sz, img.GetSize(), kfn, a, progress, co);
}

Image RescaleFilter(const Image& img, int cx, int cy,
                    double (*kfn)(double x), int a,
                    Gate<int, int> progress, bool co)
{
	return RescaleFilter(img, Size(cx, cy), img.GetSize(), kfn, a, progress, co);
}

Image RescaleFilter(const Image& img, Size sz, const Rect& sr, int filter, Gate<int, int> progress, bool co)
{
	if(IsNull(filter))
		return Rescale(img, sz, sr);
	auto t = GetImageFilterFunction(filter);
	return RescaleFilter(img, sz, sr, t.a, t.b, progress, co);
}

Image RescaleFilter(const Image& img, Size sz, int filter, Gate<int, int> progress)
{
	return RescaleFilter(img, sz, img.GetSize(), filter, progress);
}

Image RescaleFilter(const Image& img, int cx, int cy, int filter, Gate<int, int> progress)
{
	return RescaleFilter(img, Size(cx, cy), filter, progress);
}

Image CoRescaleFilter(const Image& img, Size sz, const Rect& sr, int filter, Gate<int, int> progress)
{
	return RescaleFilter(img, sz, sr, filter, progress, true);
}

Image CoRescaleFilter(const Image& img, Size sz, int filter, Gate<int, int> progress)
{
	return CoRescaleFilter(img, sz, img.GetSize(), filter, progress);
}

Image CoRescaleFilter(const Image& img, int cx, int cy, int filter, Gate<int, int> progress)
{
	return CoRescaleFilter(img, Size(cx, cy), filter, progress);
}

// Obsolete functions

Image RescaleBicubic(const Image& img, Size sz, const Rect& sr, Gate<int, int> progress)
{
	return RescaleFilter(img, sz, sr, FILTER_BICUBIC_MITCHELL, progress);
}

Image RescaleBicubic(const Image& img, int cx, int cy, Gate<int, int> progress)
{
	return RescaleBicubic(img, Size(cx, cy), img.GetSize(), progress);
}

Image RescaleBicubic(const Image& img, Size sz, Gate<int, int> progress)
{
	return RescaleBicubic(img, sz, img.GetSize(), progress);
}

}
