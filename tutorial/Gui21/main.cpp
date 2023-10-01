#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyCtrl : public Ctrl {
	int count = 0;

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		w.DrawText(2, 2, AsString(count));
	}

	void LeftDown(Point, dword) override {
		count++;
		Refresh();
	}
};

#define LAYOUTFILE <Gui21/Gui21.lay>
#include <CtrlCore/lay.h>

struct Gui21 : public WithGui21Layout<TopWindow> {
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
