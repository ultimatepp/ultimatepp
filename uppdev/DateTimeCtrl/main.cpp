#include "DateTimeCtrl.h"

GUI_APP_MAIN
{
//	Ctrl::ShowRepaint(100);
	TopWindow win;
	DateTimeCtrl dtc;
	win.Add(dtc.TopPos(100, 20).LeftPos(100, 200));
	win.Run();
}

