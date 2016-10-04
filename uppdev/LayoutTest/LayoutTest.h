#ifndef _LayoutTest_LayoutTest_h
#define _LayoutTest_LayoutTest_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include "TitleFrame.h"

#define LAYOUTFILE <LayoutTest/LayoutTest.lay>
#include <CtrlCore/lay.h>

class PopupWindow : public Ctrl {
public:
	typedef PopupWindow CLASSNAME;

	Callback WhenMoveStart;
	Callback WhenMove;
	Callback WhenMoveEnd;
	Callback WhenClose;

	PopupWindow();
	virtual void 	ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	virtual Image 	FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual void 	MouseMove(Point p, dword keyflags);
	virtual void 	LeftUp(Point p, dword keyflags)	{ EndMoveResize(); }
	virtual void 	RightUp(Point p, dword keyflags)		{ EndMoveResize(); }
	virtual Image 	CursorImage(Point p, dword keyflags);

	virtual void	ChildGotFocus()					{ titlebar.SetFocusLook(true); }
	virtual void	ChildLostFocus()				{ titlebar.SetFocusLook(false); }

	virtual void Paint(Draw &w) 					{ w.DrawRect(GetSize(), SColorFace); }

	const Image &	GetIcon()						{ return titlebar.GetImage(); } // This really should be in TopWindow
	PopupWindow &	Icon(const Image& m)			{ titlebar.SetImage(m); return *this; }
	PopupWindow &	Icon(const Image& smallicon, const Image& largeicon) { titlebar.SetImage(smallicon); return *this; }
	PopupWindow &	Title(const char *_title)		{ titlebar.SetTitle(_title); return *this; }
	PopupWindow &	Title(const WString& _title)	{ titlebar.SetTitle(_title); return *this; }

	void Open(Ctrl *owner = NULL);

	void SetMinSize(Size sz)						{ ASSERT(!sz.IsNullInstance()); minsz = sz; }
	virtual Size GetMinSize() const					{ return minsz; }
private:
	TitleFrame titlebar;
	bool moving:1;
	Rect rstart;
	int sizedir;
	Point mstart;
	Size minsz;

	void EndMoveResize();
};

class LayoutTest : public WithLayoutTestLayout<TopWindow> {
public:
	typedef LayoutTest CLASSNAME;
	WithTestLayout<PopupWindow> p;
	WithTestLayout<PopupWindow> c;

	LayoutTest();

	void OnB1() {
		CtrlLayout(p, "Popup");
		p.ok <<= callback(&p, &Ctrl::Close);
		p.cancel <<= callback(&p, &Ctrl::Close);
		p.SetRect(Rect(0, 0, 200, 200));
		p.Open(this);
	}
};

#endif
