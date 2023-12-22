#include "Examples.h"

void ImageFilters(Painter& sw)
{
	int nx = 0;
	int ny = 0;
	auto Do = [&](const char *name, int filter) {
		Size isz = TestImg::test().GetSize();
		int x = nx * 3 * isz.cx;
		int y = ny * 3 * isz.cy;
		sw.DrawText(x + 10, y, name, Arial(28));
		sw.Rectangle(x + 10, y + 30, 7 * isz.cx / 3, 7 * isz.cy / 3)
		  .ImageFilter(filter)
		  .Fill(TestImg::test(), x + 10, y + 30, x + 10 + 7 * isz.cx / 3, y + 30, FILL_PAD);
		nx++;
		if(nx > 2) {
			nx = 0;
			ny++;
		}
	};

	Do("Nearest", FILTER_NEAREST);
	Do("Bilinear", FILTER_BILINEAR);
	Do("B-spline", FILTER_BSPLINE);
	Do("Costella", FILTER_COSTELLA);
	Do("Bicubic Mitchell", FILTER_BICUBIC_MITCHELL);
	Do("Bicubic Catmull Rom", FILTER_BICUBIC_CATMULLROM);
	Do("Lanczos3", FILTER_LANCZOS3);
}

INITBLOCK {
	RegisterExample("Image filters", ImageFilters);
}
