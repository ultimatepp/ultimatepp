#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Rect  rect;
	Color color;
	
	MyAppWindow() {
		Zoomable().SetRect(0, 0, 200, 200);
		
		RandomizeRect();
		SetTimeCallback(-2000, [=] { OnTimer(); });
	}
	
	~MyAppWindow() {
		KillTimeCallback();
	}
	
	void Paint(Draw& w) override {
		Size sz = GetSize();
		
		w.DrawRect(sz, White());
		w.DrawRect(rect, color);
	}
	
	void OnTimer() {
		RandomizeRect();
		Refresh();
	}
	
	void RandomizeRect() {
		Size sz = GetSize();
		
		int length = 50;
		int x = Random() % (sz.cx - length);
		int y = Random() % (sz.cy - length);
		
		rect = Rect(x, y, x + length, y + length);
		color = Color(Random() % 255, Random() % 255, Random() % 255);
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
