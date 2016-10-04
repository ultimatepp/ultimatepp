#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	EditString test1, test2;
	TopWindow app;
	app.Add(test1.AlignRight().TopPos(0, Ctrl::STDSIZE).HSizePos());
	app.Add(test2.TopPos(Ctrl::STDSIZE, Ctrl::STDSIZE).HSizePos());
	app.Run();
}
