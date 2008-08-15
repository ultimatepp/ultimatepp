#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	TopWindow winA, winB;
	TopWindow win;

	win.OpenMain();
	winA.SetRect(100, 100, 100, 100);
	winA.Open(&win);
	winB.SetRect(100, 100, 100, 100);
	winB.Open(&win);
	EditField f;
	winA.Add(f.SizePos());
	win.Run();
}
