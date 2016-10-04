#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TestDraw : public TopWindow {
	Button ok;
	DocEdit edit;
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, SWhite);
		w.DrawRect(200, 200, 1, 1, SBlack);
		for(int i = 0; i < 3600; i += 450)
			w.DrawText(200, 200, i, "Test of angle!", Arial(30));
		w.DrawLine(0, 300, 1000, 300, PEN_DOT);
		w.DrawLine(0, 305, 1000, 305, PEN_DASH);
	}

public:
	typedef TestDraw CLASSNAME;
	TestDraw();
};



TestDraw::TestDraw()
{
	Sizeable();
	Add(ok.LeftPos(10, 100).TopPos(10, 20));
	ok.SetLabel("TEST");
	Add(edit.LeftPos(100, 300).TopPos(10, 100));
}

GUI_APP_MAIN
{
	DUMP(GetStdFont());
	FontInfo fi = GetStdFont().Info();
	DUMP(fi.GetHeight());
	DUMP(fi['X']);
	TestDraw().Run();
}
