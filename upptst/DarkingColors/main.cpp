#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct ColorTest : TopWindow {
	ColorPusher color;
	
	void Paint(Draw& w) override {
		Size sz = GetSize();
		w.DrawRect(0, 0, sz.cx, sz.cy / 2, ~color);
		w.DrawRect(0, sz.cy / 2, sz.cx, sz.cy / 2 + 1, DarkTheme((Color)~color));
	}
	
	ColorTest() {
		Add(color.LeftPosZ(0, 300).TopPosZ(0, 300));
		color << [=] { Refresh(); Title(AsString(DarkTheme((Color)~color))); };
	}
};

GUI_APP_MAIN
{
#ifndef _DEBUG
	for(int i = 0; i < 10000000; i++) {
		RTIMING("MakeDark");
		DarkTheme(Color(30, 40, 5));
		DarkTheme(Color(30, 40, 6));
		DarkTheme(Color(30, 40, 7));
		DarkTheme(Color(30, 40, 8));
	}
	for(int i = 0; i < 10000000; i++) {
		RTIMING("DarkThemeCached");
		DarkThemeCached(Color(30, 40, 5));
		DarkThemeCached(Color(30, 40, 6));
		DarkThemeCached(Color(30, 40, 7));
		DarkThemeCached(Color(30, 40, 8));
		DarkThemeCached(Color(30, 40, 9));
	}
#endif
	ColorTest().Run();
}
