#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	MenuBar  bar;
	LineEdit editor;
	Label    title;
	FrameTop<Ctrl> test;

	void Layout() override {
		int ch = GetCustomTitleBarMetrics().height;
		int h = bar.GetHeight();
		int w = bar.GetWidth();
		bar.LeftPos(0, w).TopPos((ch - h) / 2, h);
		
		title.HSizePos(w, 0).VSizePos();
	}

	MyApp() {
		Icon(CtrlImg::select_all());

		Ctrl *tb = CustomTitleBar(Blend(SWhiteGray(), SLtMagenta(), 20));
		
		if(tb) {
			tb->Add(bar);
			tb->Add(title);
		}
		else
			AddFrame(bar);
		bar.Transparent();
		bar.Set([=](Bar& bar) {
			bar.Sub("File", [=](Bar& bar) {
				bar.Add("Yellow", [=] { CustomTitleBar(Yellow()); });
				bar.Add("Red", [=] { CustomTitleBar(LtRed()); });
				bar.Add("Exit", [=] { Break(); });
			});
		});

		title = "\1[g This is [* title";
		title.AlignCenter();

		Add(editor.SizePos());
	};
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Zoomable().Run();
}
