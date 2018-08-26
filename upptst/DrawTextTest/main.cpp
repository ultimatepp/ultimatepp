#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, White());
	
	int angle = 0;
	
	int x = 0;
	int y = 0;
	
	while(angle < 360) {
		w.DrawText(x, y, AsString(angle));
		
		w.DrawRect(x + 49, y + 49, 3, 3, LtRed());
		w.DrawText(x + 50, y + 50, angle * 10, AsString(angle));
		
		angle += 10;
		x += 100;
		
		if(x + 100 > sz.cx) {
			x = 0;
			y += 100;
		}
	}
}

GUI_APP_MAIN
{
	MyApp app;
	app.Run();
}
