#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	DropTime tm;
	tm.Min(GetSysTime() - 10000).Max(GetSysTime() + 10000);
	win.Add(tm.HSizePos().TopPos(10, Ctrl::STDSIZE));
	win.Run();
}
