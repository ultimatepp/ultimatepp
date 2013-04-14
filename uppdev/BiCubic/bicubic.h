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

Image DownScale(const Image& img, int nx, int ny);
