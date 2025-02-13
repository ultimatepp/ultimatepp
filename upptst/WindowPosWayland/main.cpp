#include <CtrlLib/CtrlLib.h>

using namespace Upp;

INITBLOCK {
	Ctrl::UseWayland();
}

struct MyApp : TopWindow {
	TimeCallback tm;
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(GetSize(), SWhite());
		int y = 0;
		String txt;
		#define DMP(x)  txt << #x << " " << x << "\n";
		DMP(Ctrl::IsWayland());
		DMP(GetScreenRect());
		DMP(GetVirtualScreenArea());
		DMP(GetVirtualWorkArea());
		DMP(GetWorkArea());
		DMP(GetMousePos());
		
		DrawSmartText(w, 0, 0, sz.cx, txt);
	}
	
	MyApp() {
		Sizeable().Zoomable();
		tm.Set(-200, [=] { Refresh(); });
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
