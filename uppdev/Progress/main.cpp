#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Progress pi;
	for(;;) {
		if(pi.StepCanceled())
			break;
//		Sleep(rand() % 1000);
	}
//	Ctrl::SetXPStyle(false);
	TopWindow win;
	ProgressIndicator p1, p2;
	win.Add(p1.LeftPos(10, 100).TopPos(10, 16));
	win.Add(p2.LeftPos(10, 16).TopPos(40, 100));
	win.Open();
	p1.SetTotal(300);
	p1.SetColor(Red);
	while(win.IsOpen()) {
		p1.Set(GetTickCount() / 20 % 300, 300);
		p2.Set(GetTickCount() / 20 % 300, 0);
		Ctrl::ProcessEvents();
	}
}
