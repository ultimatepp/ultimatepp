#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DropDate d1, d2;
	d1.Default(Date(1990, 1, 1));
	TopWindow win;
	win.Add(d1.HSizePos().TopPos(0, Ctrl::STDSIZE));
	win.Add(d2.HSizePos().TopPos(Ctrl::STDSIZE, Ctrl::STDSIZE));
	win.Run();
}
