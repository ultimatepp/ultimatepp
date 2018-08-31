#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	Font monaco;
	monaco.Height(20).FaceName("Monaco");

	Size sz = GetSize();
	w.DrawRect(sz, White());
	
	int angle = 0;
	
	int x = 0;
	int y = 0;
	
	Font fnt = Arial();
	fnt = monaco().Italic();
	while(angle < 360) {
		w.DrawText(x, y, AsString(angle));
		
		w.DrawRect(x + 49, y + 49, 3, 3, LtRed());
		w.DrawText(x + 50, y + 50, angle * 10, AsString(angle), fnt);
		
		angle += 10;
		x += 100;
		
		if(x + 100 > sz.cx) {
			x = 0;
			y += 100;
		}
	}
	
	for(int i = 0; i < 4; i++) {
		w.DrawText(0, y, "Hello World", monaco);
		w.DrawText(200, y, "Hello World", monaco().Bold());
		w.DrawText(400, y, "Hello World", monaco().Italic());
		w.DrawText(600, y, "Hello World", monaco().Bold().Italic());
		w.DrawText(800, y, "Hello World", monaco().Underline());
		if(i == 1)
			monaco = Arial(20);
		if(i == 2)
			monaco = Roman(20);
		y += 30;
	}
}

GUI_APP_MAIN
{
	MyApp app;
	app.Run();
}
