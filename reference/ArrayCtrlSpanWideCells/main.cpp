#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.AddColumn("A", 100);
	a.AddColumn("B", 100);
	a.HeaderObject().Absolute();
	a.SpanWideCells();
	
	a.Add("This is very very long line...");
	a.Add("This is long line, but the next column is not empty", 12);
	a.Add("Short line");
	
	TopWindow win;
	win.Sizeable();
	win.Add(a.SizePos());
	win.Run();
}
