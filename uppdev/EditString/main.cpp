#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	EditString es;
	TopWindow win;
	String h;
	h << "Something ";
	h.Cat('\0');
	h << " to test";
	es <<= h;
	win.Add(es.HSizePos().TopPos(0, Ctrl::STDSIZE));
	win.Run();
}
