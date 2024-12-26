#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	ColorPusher p, p1;
	p.DarkContent();
	win.Add(p.LeftPos(100, 100).TopPos(100, 50));
	win.Add(p1.LeftPos(200, 100).TopPos(100, 50));
	win.Run();
}
