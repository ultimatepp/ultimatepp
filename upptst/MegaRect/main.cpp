#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	RDUMP(sizeof(Ctrl));
	
	TopWindow win;
	win.SetRect(100, 100, 100, 100);
	DDUMP(win.GetRect());
	win.SetRect(100000, 100000, 100, 100);
	DDUMP(win.GetRect());
	win.SetRect(100000, 100000, 100000, 100000);
	DDUMP(win.GetRect());
	win.SetRect(-100000, -100000, -100000, -100000);
	DDUMP(win.GetRect());
}
