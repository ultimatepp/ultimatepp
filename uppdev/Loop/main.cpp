#include <CtrlLib/CtrlLib.h>

struct MyApp : TopWindow {
	Rect rect;
	int  hline, vline;

	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SColorPaper());
		DrawFrame(w, rect, Black());
		w.DrawRect(0, hline, sz.cx, 1, SRed);
		w.DrawRect(vline, 0, 1, sz.cy, SBlue);
	}

	virtual void LeftDown(Point p, dword keyflags) {
		RectTracker tr(*this);
		Size sz = GetSize();
		if(keyflags & K_ALT) {
			tr.Dashed().Animation();
			rect = tr.Track(rect, ALIGN_CENTER, ALIGN_CENTER);
		}
		else
		if(keyflags & K_SHIFT) {
			tr.Solid();
			hline = tr.TrackHorzLine(0, 0, sz.cx, hline);
		}
		else
		if(keyflags & K_CTRL) {
			tr.Solid();
			vline = tr.TrackVertLine(0, 0, sz.cy, vline);
		}
		else {
			tr.Dashed().Animation();
			rect = tr.Track(rect, ALIGN_RIGHT, ALIGN_BOTTOM);
		}
		Refresh();
	}

	MyApp() {
		rect = RectC(100, 100, 100, 100);
		hline = 150;
		vline = 150;
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
