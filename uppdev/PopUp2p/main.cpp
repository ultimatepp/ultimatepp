#include <CtrlLib/CtrlLib.h>

struct PopUp2 : StaticRect {
	PopUp2() { Color(SRed); }
};

struct PopUp1 : StaticRect
{
	PopUp2 p2;

	virtual void LeftDown(Point, dword) {
		p2.SetRect(100, 100, 100, 100);
		if(!p2.IsOpen())
			p2.PopUp(this);
	}

	PopUp1() { Color(SBlue); }
};

struct MyCtrl : TopWindow
{
    PopUp1 p1;

	virtual void LeftDown(Point, dword) {
		p1.SetRect(150, 150, 100, 100);
		if(!p1.IsOpen())
			p1.PopUp(this);
    }
};

GUI_APP_MAIN
{
	MyCtrl().Run();
}
