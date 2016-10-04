#include "CtrlLib/CtrlLib.h"

GUI_APP_MAIN
{
	TopWindow win;
	StaticRect r;
	RightTabs tabs;
	r.SetFrame(tabs);
	tabs.Add(CtrlImg::save(), "Test2: Save1");
	tabs.Add(CtrlImg::save(), "Test2: Save2");
	tabs.Add(CtrlImg::save(), "Test2: Save3");
	win.Add(r.SizePos());
	win.Sizeable().Zoomable();
	win.Run();
}
