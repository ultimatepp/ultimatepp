#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ConvertOpeningHours cv;
	DDUMP(cv.Format(cv.Scan("11:00-22:00 12:00-18:00 23:00-23:30")));
	EditField ef;
	ef.SetConvert(cv);
	TopWindow win;
	win.Add(ef.TopPos(0, Ctrl::STDSIZE).HSizePos());
	win.Run();
}
