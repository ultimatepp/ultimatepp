#include <CtrlLib/CtrlLib.h>

using namespace Upp;

INITBLOCK {
	Ctrl::UseWayland();
}

struct MyApp : TopWindow {
	TimeCallback tm;
	void Paint(Draw& w) override {
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
	
	void LeftDown(Point p, dword keyflags) override
	{
		new MyApp;
	}

	void Close() override {
		delete this;
	}

	MyApp() {
		Sizeable().Zoomable();
		tm.Set(-200, [=] { Refresh(); });
		OpenMain();
	}
};

GUI_APP_MAIN
{
	new MyApp;
	Ctrl::EventLoop();
}
