#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct TestCtrl : Ctrl {
	virtual void LeftDown(Point p, dword keyflags) {
		TopWindow *w = GetAscendant<TopWindow>();
		if(w)
			w->Title(AsString(p));
	}
};

GUI_APP_MAIN
{
	TopWindow win;
	TestCtrl h;
	win << h.SizePos();
	win.Run();
}
