#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	RGBA pixels[65536];

	virtual void Paint(Draw& w) {
		Rect r = w.GetPaintRect();
		w.DrawRect(r, LtGray());
		Point p = r.TopLeft();
		if(p.x < 256 && p.y < 256)
			SetSurface(w, r, pixels, Size(256, 256), p);
	}

	MyApp() {
		RGBA *t = pixels;
		for(int x = 0; x < 256; x++)
			for(int y = 0; y < 256; y++) {
				int d = (x - 128) * (x - 128) + (y - 128) * (y - 128);
				RGBA c = Black();
				if(d > 120 * 120)
					c = White();
				if(d < 120 * 120)
					c.r = 255 - min(255 * d / (120 * 120), 255);
				*t++ = c;
			}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
