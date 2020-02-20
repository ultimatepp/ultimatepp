#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	double phase = 0;
	
	TimeCallback tm;
	
	void PaintCircle(Draw& w, int sc) {
		sc /= 2;
		int pp = int(phase * sc);
		w.DrawEllipse(int(sc - pp), int(sc - pp), int(2 * pp), int(2 * pp), SBlue());
	}
	
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, SWhite());
		PaintCircle(w, min(sz.cx, sz.cy));
	}
	
	MyApp() {
		tm.Set(-10, [=] {
			phase = msecs() % 2000 / 2000.0;
			for(int pass = 0; pass < 2; pass++) {
				int sc = pass ? 32 : 16;
				ImageDraw iw(sc, sc);
				iw.DrawRect(0, 0, sc, sc, White());
				PaintCircle(iw, sc);
				if(pass)
					LargeIcon(iw);
				else
					Icon(iw);
			}
			Refresh();
		});
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
