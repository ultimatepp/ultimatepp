#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	virtual void Paint(Draw& w) {
		ImageDraw iw(400, 400);
		iw.Alpha().DrawRect(0, 0, 400, 400, Black());
		iw.Alpha().DrawEllipse(0, 0, 200, 200, GrayColor(100));
		iw.DrawRect(0, 0, 400, 400, Black);
		iw.DrawEllipse(0, 0, 200, 200, LtCyan);
		
		Image a = Rescale(iw, 200, 200);

		w.DrawRect(GetSize(), White);
		w.DrawImage(30, 30, a, Rect(10, 10, 50, 50));
		w.DrawImage(500, 30, a, Blue);

		
		ImageBuffer ib(50, 50);
		for(int y = 0; y < 50; y++) {
			RGBA *l = ib[y];
			for(int x = 0; x < 50; x++) {
				if(y == 0 || y == 49 || x == 0 || x == 49)
					*l++ = Black();
				else {
					l->a = 2 * (x + y);
					l->r = 4 * x;
					l->g = 4 * y;
					l->b = 200;
					l++;
				}
			}
		}
		Premultiply(ib);
		Image b = ib;

		Over(a, Point(70, 70), b, b.GetSize());

		w.DrawImage(20, 500, a);
	}
	
};

GUI_APP_MAIN
{
	App().Run();
}

