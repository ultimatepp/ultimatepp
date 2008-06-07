#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow w;
	w.Title("My application").MinimizeBox().Sizeable();
	w.SetRect(0, 0, 200, 300);
	w.Run();
}
