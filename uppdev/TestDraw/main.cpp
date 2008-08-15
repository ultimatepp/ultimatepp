#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void DoPaint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(sz, SWhite);
	w.DrawRect(200, 200, 1, 1, SBlack);
	for(int i = 0; i < 3600; i += 450)
		w.DrawText(200, 200, i, "Test of angle!", Arial(30));
}

class TestDraw : public TopWindow {
	virtual void Paint(Draw& w) {
		DoPaint(w);
	}

public:
	typedef TestDraw CLASSNAME;
	TestDraw();
};



TestDraw::TestDraw()
{
	Sizeable();
}

GUI_APP_MAIN
{
	Exclamation("Hello world!");
//	TestDraw().Run();
}
