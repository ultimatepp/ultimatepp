#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow w;
	w.Title("My application").Zoomable().Sizeable();
	w.SetRect(0, 0, 200, 300);
	w.Run();
}
