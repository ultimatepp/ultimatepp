#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

enum {
	DOWNSCALE_SIMPLE,
	DOWNSCALE_MIPMAP,
	DOWNSCALE_WIDE,
};

Image RescaleWithKernelE(const Image& _img, int cx, int cy, double (*kernel)(double x), int a, int method = DOWNSCALE_WIDE);
Image RescaleWithKernel(const Image& _img, int cx, int cy, double (*kernel)(double x), int a, int method = DOWNSCALE_WIDE);

Image RescaleFilter(const Image& img, Size sz, double (*kfn)(double x), int a, Gate2<int, int> progress = false);
Image RescaleFilter(const Image& img, Size sz, const Rect& sr, double (*kfn)(double x), int a, Gate2<int, int> progress = false);

Image DownScale(const Image& img, int nx, int ny);
