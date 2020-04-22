#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		
		for(Color c : { Black(), Blue(), Red(), Yellow() }) {
			w.DrawRect(10, 10, 200, 100, c);
			w.DrawRect(30, 30, 50, 50, InvertColor());
	
			w.DrawRect(130, 30, 50, 50, InvertColor());
			w.DrawRect(150, 30, 50, 50, InvertColor());
			w.Offset(0, 120);
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
