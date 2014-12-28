#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DropList list;
	list.Add(1).Add(2);
	TopWindow win;
	win.Add(list.HSizePos().TopPos(0));
	list.Error("Cannot be NULL");
	win.Run();
}
