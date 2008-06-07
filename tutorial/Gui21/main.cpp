#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyCtrl : public Ctrl {
	int count;

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawText(2, 2, AsString(count));
	}

	virtual void LeftDown(Point, dword) {
		count++;
		Refresh();
	}

	MyCtrl() { count = 0; }
};

#define LAYOUTFILE <Gui21/Gui21.lay>
#include <CtrlCore/lay.h>

struct Gui21 : public WithGui21Layout<TopWindow> {
	typedef Gui21 CLASSNAME;
	Gui21();
};

Gui21::Gui21()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	Gui21().Run();
}
