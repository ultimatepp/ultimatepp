#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win, win2;
	win.Title("xxx");
	win.Open();
	win2.OpenMain();
	win2.Run();
	win.Urgent();
	win2.Run();
}
