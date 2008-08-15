#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void DoLine(RGBA *line, int phase, int y, Size sz)
{
//	DLOG("DoLine " << (void *)line << ' ' << y << ' ' << phase);
	Point c = sz / 2;
	c = Point(int(sin((double)phase / 131) * c.x + c.x), int(sin((double)phase / 127) * c.y + c.y));
	int yy = (y - c.y) * (y - c.y);
	for(int x = 0; x < sz.cx; x++) {
		double d = (x - c.x) * (x - c.x) + yy;
		line[x] = GrayColor((int)(120 + 120 * sin(d / 1000 - (double)phase / 5)));
	}
//	DLOG("Done DoLine Phase " << phase << " y " << y);
}

GUI_APP_MAIN
{
	for(int phase = 0; phase < 1000000; phase++) {
		Size sz = Size(400, 400);
		ImageBuffer ib(sz);
		{
			CoWork co;
			for(int y = 0; y < sz.cy; y++)
				co & callback4(DoLine, ib[y], phase, y, sz);
		}
//		DLOG("Finished phase " << phase);
		Image m = ib;
	}
//	App().Run();
}
