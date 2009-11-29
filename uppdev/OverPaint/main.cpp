#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct OverCtrl : public Ctrl, public CtrlFrame {
	virtual void FrameAddSize(Size& sz) {}
	virtual void FrameLayout(Rect& r) {}
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.left - 10, r.top - 10, r.GetWidth() + 20, r.GetHeight() + 20, Blue());
		w.DrawRect(r, Red());
	}

	virtual int OverPaint() const { return 10; }
	
	OverCtrl() {
		SetFrame(*this);
	}
};

GUI_APP_MAIN
{
	OverCtrl ctrl;
	TopWindow win;
	
	win.Add(ctrl.LeftPos(40, 40).TopPos(40, 10));
	
	win.Run();
}

