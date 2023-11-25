#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		String text;
		text << "Behold: "
		     << WString(0x1F970, 1).ToString()
		     << WString(0x1F603, 1).ToString()
		     << WString(0x1F923, 1).ToString()
		     << WString(0x1F970, 1).ToString()
		     << " is not that nice?";
		;
		w.DrawText(50, 200, text, StdFont());
		w.DrawText(50, 250, text, StdFont().Italic());
		w.DrawText(50, 700, 300, text, StdFont());

		w.DrawText(500, 200, text, StdFont(DPI(20)));
		w.DrawText(500, 250, text, StdFont(DPI(20)).Italic());
		w.DrawText(500, 700, 300, text, StdFont(DPI(20)));
	}
};

GUI_APP_MAIN {
	MyApp().Sizeable().Zoomable().Run();
}