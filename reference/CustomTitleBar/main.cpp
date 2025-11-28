#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
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
		Ctrl *custom_bar = CustomTitleBar(h);
		
		if(custom_bar) {
			menubar.Transparent();
			auto cm = GetCustomTitleBarMetrics();
			*custom_bar << menubar;
			SetMenuBar(); // run it here to get GetWidth
			menubar.LeftPos(0, menubar.GetWidth()).TopPos((cm.height - h) / 2, h);
			*custom_bar << title.HSizePos(menubar.GetWidth(), cm.rm).VSizePos();
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
