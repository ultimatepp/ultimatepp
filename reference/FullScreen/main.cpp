#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	Button b;
	b.SetLabel("CLOSE");
	b <<= win.Breaker();
	win.Add(b.LeftPos(10, 100).TopPos(10, 30));
	win.FullScreen().Run();
}
