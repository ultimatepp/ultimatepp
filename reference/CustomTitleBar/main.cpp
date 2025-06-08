#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	FrameTop<StaticRect> bararea; // we represent whole TitleBar area as frame
	ParentCtrl           barrect; // to do custom caption clipping
	MenuBar              menubar;
	LineEdit             editor;
	Label                title;
	
	void MainMenu(Bar& bar)
	{
		bar.Sub("File", [=](Bar& bar) {
			bar.Add("Exit", [=] { Break(); });
		});
	}

	void SetMenuBar() {
		menubar.Set([=](Bar& bar) {
			MainMenu(bar);
		});
	}

	bool IsCustomTitleBarDragArea(Point p) override
	{ // identifies which titlebar areas can be clicked for dragging the window
		p += GetScreenRect().TopLeft();
		return !menubar.GetScreenRect().Contains(p);
	}
	
	MyApp() {
		Sizeable().Zoomable();

		Icon(CtrlImg::new_doc());

		int h = menubar.GetStdHeight();
		CustomTitleBar(h); // h is suggested minimum height
		
		if(IsCustomTitleBar()) {
			menubar.Transparent();
			auto cm = GetCustomTitleBarMetrics();
			bararea.Height(cm.height);
			AddFrame(bararea);
			bararea << barrect.VSizePos().HSizePos(cm.lm, cm.rm);
			ImageBuffer m(1, 2);
			m[0][0] = Blend(SWhite(), SLtCyan());
			m[0][1] = Blend(SWhite(), SLtMagenta());
			bararea.Background(Image(m)); // simple gradient
			barrect << menubar;
			SetMenuBar(); // run it here to get GetWidth
			menubar.LeftPos(0, menubar.GetWidth()).TopPos((cm.height - h) / 2, h);
			barrect << title.HSizePos(menubar.GetWidth(), cm.rm).VSizePos();
			title.SetLabel("This is CustomTitleBar example");
			title.AlignCenter();
			title.AlignVCenter();
		}
		else { // if it is not supported
			AddFrame(menubar);
			SetMenuBar();
			Title("This is CustomTitleBar example - CustomTitleBar not active");
		}
		Add(editor.SizePos());
	};
};

GUI_APP_MAIN
{
	MyApp().Run();
}
