#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
	}

	Image CursorImage(Point p, dword keyflags) override {
		if(p.x < GetSize().cx / 2)
			return Image::Hand();
		else
			return CtrlImg::HelpCursor0();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
