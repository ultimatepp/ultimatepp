#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DDUMP(sizeof(Ctrl::PackedLogPos));
	TopWindow win;
	win.SetRect(0, 0, 200, 200);
	Ctrl a;
	win << a;

	for(int pass = 0; pass < 2; pass++) {
		DLOG("=================");
		DDUMP(win.GetRect());
		DDUMP(a.LeftPos(10, 100).TopPos(10, 100).GetRect());
		DDUMP(a.GetScreenRect());
		DDUMP(a.RightPos(10, 100).BottomPos(10, 100).GetRect());
		DDUMP(a.GetScreenRect());
		DDUMP(a.VSizePos(10, 10).HSizePos(10, 10).GetRect());
		DDUMP(a.GetScreenRect());
		DDUMP(a.HCenterPos(100).VCenterPos(100).GetRect());
		DDUMP(a.GetScreenRect());
		win.SetRect(100000, 100000, 30000, 30000);
	}
	
#if 0
	{
		Ctrl x;
		x.LeftPos(10, 100).TopPos(10, 100);
		Ctrl::Logc p = x.GetPos().x;
		DDUMP(p.GetAlign());
		DDUMP(p.GetA());
		DDUMP(p.GetB());
	}
	
	PromptOK("Test");
	TopWindow win;
	win.SetRect(100000, 100000, 200, 200);
	DDUMP(win.GetRect());
	
	Rect r(1234567, 8, 9, 10);
	win.SetRectAttr(Ctrl::ATTR_RECT, r);
	DDUMP(win.GetRectAttr(Ctrl::ATTR_RECT));
#endif
}
