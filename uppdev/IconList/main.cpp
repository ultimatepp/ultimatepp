#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	IconList list;
	for(int i = 0; i < 500; i++)
		list.Add(CtrlImg::copy());
	win.Sizeable().Zoomable().Add(list.SizePos());
	win.Run();
}
