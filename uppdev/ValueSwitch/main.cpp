#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	Switch h;
	h.Add("One", "One");
	h.Add("Two", "Two");
	h.Add("Three", "Three");
	h.EnableValue("One", false);
	h.EnableValue("Two", false);
	win.Add(h.SizePos());
	win.Run();
	h.EnableValue("Two", true);
	win.Run();
}

