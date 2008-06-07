#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Frame1 : CtrlFrame {
	virtual void FrameLayout(Rect& r) {
		r.left += 20;
	}
	virtual void FrameAddSize(Size& sz) {
		sz.cx += 20;
	}
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.left, r.top, 20, r.Height(), SLtGreen);
	}
};

struct Frame2 : Frame1 {
	Button x;

	virtual void FrameLayout(Rect& r) {
		x.SetFrameRect(r.left, r.top, 16, 16);
		r.left += 16;
	}
	virtual void FrameAddSize(Size& sz) {
		sz.cx += 16;
	}
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.left, r.top + 16, 20, r.Height() - 16, SLtMagenta);
	}
	virtual void FrameAdd(Ctrl& parent) {
		parent.Add(x);
	}
	virtual void FrameRemove() {
		x.Remove();
	}

	Frame2() {
		x.LeftPos(0, 16).TopPos(0, 16).NoWantFocus();
	}
};

struct CtrlSelfFrame : Ctrl, CtrlFrame
{
    virtual void FrameLayout(Rect &r) {
        r.left += 20;
        r.top += 20;
    }
    virtual void FrameAddSize(Size &sz) {
        sz += Size(20, 20);
    }
    virtual void FramePaint(Draw &w, const Rect &r) {
        w.DrawRect(r.left, r.top, r.Width(), 20, SGreen);
        w.DrawRect(r.left, r.top + 20, 20, r.Height() - 20, SBlue);
    }
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SYellow);
	}
	CtrlSelfFrame() {
		SetFrame(*this);
		AddFrame(InsetFrame());
	}
};

GUI_APP_MAIN
{
	TopWindow  win;
	StaticRect ctrl;
	ctrl.Color(SLtCyan);
	win.Add(ctrl.SizePos());
	win.SetRect(0, 0, 100, 100);
	win.Run();

	ctrl.SetFrame(InsetFrame());
	win.Run();

	ctrl.AddFrame(BlackFrame());
	win.Run();

	Frame1 frame1;
	ctrl.InsertFrame(0, frame1);
	win.Run();

	Frame2 frame2;
	ctrl.AddFrame(frame2);
	win.Run();

	ctrl.Remove();
	CtrlSelfFrame csf;
	win.Add(csf.SizePos());
	win.Run();
}
