#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	ArrayCtrl a;
	MarginFrame f;

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtBlue());
	}

	MyApp() {
		for(int i = 0; i < 50; i++)
			a.AddColumn("A", 100);
		a.HeaderObject().Absolute();
	
		for(int i = 0; i < 40; i++) {
			a.Add("This is very very long line...");
			a.Add("This is long line, but the next column is not empty", 12);
			a.Add("Short line");
		}
		Sizeable();
		Add(a.SizePos());
		
	//	a.Transparent();
		a.SetFrame(f);
		f.SetColor(LtGreen());
		f.SetMargins(Rect(20, 50, 20, 10));
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
