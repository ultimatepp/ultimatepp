#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		Size sz = GetSize();
//		w.DrawRect(sz, WhiteGray()); return;

		auto h = GetCustomTitleBarMetrics().height;
		Color c2 = SLtCyan();
		for(int i = 0; i < h; i++)
			w.DrawRect(0, i, sz.cx, 1, Blend(WhiteGray(), c2, i * 255 / h));
	}
	
	void Layout() override {
		auto m = GetCustomTitleBarMetrics();
		int h = bar.GetHeight();
		bar.LeftPos(m.lm, bar.GetWidth()).TopPos((m.height - h) / 2, h);
	}
	
	void MainMenu(Bar& bar)
	{
		bar.Sub("File", [=](Bar& bar) {
			bar.Add("Exit", [=] { Break(); });
		});
	}
	
	MenuBar bar;
	LineEdit editor;
	
	MyApp() {
		Icon(CtrlImg::select_all());

		CustomTitleBar(200);
		
		Add(bar);
		bar.Transparent();
		bar.Set([=](Bar& bar) {
			MainMenu(bar);
		});

		DDUMP(bar.GetWidth());
		DDUMP(bar.GetHeight());
		
		DDUMP(GetCustomTitleBarMetrics().height);

		Add(editor.VSizePos(GetCustomTitleBarMetrics().height).HSizePos());
	};
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Zoomable().Run();
}
