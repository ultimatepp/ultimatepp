#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	EditString es;
	TopWindow win;
	es <<= "Something to test";
	win.Add(es.HSizePos().TopPos(0, Ctrl::STDSIZE));
	win.Run();
}
