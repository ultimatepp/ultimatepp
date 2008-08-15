#include <CtrlLib/CtrlLib.h>

struct OverPaintFrame : CtrlFrame {
	bool x;

	virtual int  OverPaint() const { return 4; }
	virtual void FrameLayout(Rect& r) {}
	virtual void FrameAddSize(Size& sz) {}
	virtual void FramePaint(Draw& w, const Rect& r) {
		DrawFrame(w, r.Inflated(3), x ? Blue : Red);
	}
};

struct OverPaintCtrl : Ctrl {
	OverPaintFrame frame;

//	virtual int  OverPaint() const { return 4; }
	virtual void LeftDown(Point p, dword keyflags) { frame.x = !frame.x; RefreshFrame(); }
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, Cyan);
	}

	OverPaintCtrl() {
		SetFrame(frame);
	}
};

GUI_APP_MAIN
{
	TopWindow win;
	OverPaintCtrl ctrl;
	ctrl.SetRect(20, 20, 20, 20);
	win.Add(ctrl);
	win.Run();
	DUMP(sizeof(Ctrl));
}
