#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	TopWindow  win;
	StaticRect r;
	r.Color(Blend(SColorShadow, SColorFace, 180));
	win.Add(r.HSizePos(10, 10).VSizePos(10, 10));
	r.SetFrame(RightSeparatorFrame());
	win.SetRect(0, 0, 100, 40);
	win.Run();
}
