#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	MenuBar  bar;
	LineEdit editor;
	Label    title;

	void Layout() override {
		if(IsCustomTitleBar()) {
			int ch = GetCustomTitleBarMetrics().height;
			int h = bar.GetHeight();
			int w = bar.GetWidth();
			bar.LeftPos(0, w).TopPos((ch - h) / 2, h);
			
			title.HSizePos(w, 0).VSizePos();
		}
	}

	MyApp() {
		Sizeable().Zoomable();

		Icon(CtrlImg::select_all());

		Ctrl *tb = CustomTitleBar(Blend(SWhiteGray(), SLtMagenta(), 50), 50);
		
		if(tb) {
			tb->Add(bar);
			tb->Add(title);
			bar.Transparent();
		}
		else
			AddFrame(bar);
		bar.Set([=](Bar& bar) {
			bar.Sub("File", [=](Bar& bar) {
				bar.Add("Yellow", [=] { CustomTitleBar(Yellow(), 20); });
				bar.Add("Red", [=] { CustomTitleBar(LtRed(), 20); });
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
