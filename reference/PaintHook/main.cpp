#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Ctrl::InstallPaintHook([](Ctrl *ctrl, Draw& w, const Rect&) {
		w.DrawRect(100, 100, 500, 200, Blue());
		w.DrawText(100, 120, "PAINT HOOK!", Arial(50), Yellow());
	});
	
	TopWindow win1, win2;
	win1.OpenMain();
	win2.Run();
}
