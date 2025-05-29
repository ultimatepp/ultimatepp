#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		Size sz = GetSize();
		w.DrawRect(sz, LtCyan());
	}
	
	void MainMenu(Bar& bar)
	{
		bar.Sub("File", [=](Bar& bar) {
			bar.Add("Exit", [=] { Break(); });
		});
	}
	
	MenuBar bar;
	
	MyApp() {
		CustomTitleBar();
		Add(bar.LeftPos(0, 300).TopPos(0, 30));
		bar.Set([=](Bar& bar) {
			MainMenu(bar);
		});
	};
};

GUI_APP_MAIN
{
	TopWindow win;
	win.Sizeable().Zoomable().OpenMain();
	MyApp().Sizeable().Zoomable().Run();
}
