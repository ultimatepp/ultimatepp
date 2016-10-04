#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	ArrayCtrl a;
	
	bool Order(int i1, int i2) {
		DDUMP(a.Get(i1, 0));
		DDUMP(a.Get(i2, 0));
		return a.Get(i1, 0) < a.Get(i2, 0);
	}
	
	void DoTip(Point p)
	{
		if(p.y & 1)
			a.Tip("Hello!");
		else
			a.Tip("");
	}
	
	typedef App CLASSNAME;

	App() {
		Add(a.SizePos());
		Sizeable().Zoomable();

		a.AddColumn("Test");
		for(int i = 0; i < 200; i++) {
			a.Add((int)Random(10000));
		}
	//	a.Sort(THISBACK(Order));
		a.Sort();
		
		a.WhenMouseMove = THISBACK(DoTip);
	}
};

GUI_APP_MAIN
{
	DUMP("Test");
	App().Run();
}
