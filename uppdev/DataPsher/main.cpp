#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	Draw::SetStdFont(Tahoma(11));
	SetDefaultCharset(CHARSET_WIN1250);
	SetLanguage(LNG_CZECH);
	DataPusher p;
	p <<= "ššššš";
	TopWindow win;
	win.Add(p.SizePos());
	win.Run();
}
