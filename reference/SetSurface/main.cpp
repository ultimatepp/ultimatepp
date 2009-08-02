#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	RGBA pixels[65536];

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtGray());
		SetSurface(w, 0, 0, 256, 256, pixels);
		SetSurface(w, RectC(300, 128, 128, 128), pixels, Size(256, 256), Point(0, 0));
		SetSurface(w, RectC(300, 0, 128, 64), pixels, Size(256, 256), Point(128, 128));
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
