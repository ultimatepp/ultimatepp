#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	TimeCallback tm;
	
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), SColorPaper());
		if(IsClipboardAvailableImage())
			w.DrawText(0, 0, "IMAGE");
		if(IsClipboardAvailableText())
			w.DrawText(0, 50, "TEXT");
		if(IsAvailableFiles(Ctrl::Clipboard()))
			w.DrawText(0, 100, "FILES");
	}
	
	MyApp() {
		tm.Set(-200, [=] { Refresh(); });
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
