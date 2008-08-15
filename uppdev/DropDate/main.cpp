#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
//	WithDropChoice<EditString> d;
//	d.AddList("Test");
	DropDate d;
	TopWindow win;
	win.Add(d.SizePos());
	win.Run();
}
