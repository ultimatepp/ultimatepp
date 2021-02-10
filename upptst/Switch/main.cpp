#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	Switch sw;
	sw.Add("one");
	sw.Add("two");
	sw.Add("three");
	sw.DisableCase(1);
	win.Add(sw.SizePos());
	win.Run();
}
