#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct TestRect : Ctrl {
	void Paint(Draw& w) override {
		Size sz = GetSize();
		return;
		w.DrawRect(sz, Yellow());
		DrawFatFrame(w, sz, LtGreen(), 8);
		DrawFatFrame(w, sz, Red(), 2);
		for(int i = 0; i < 100; i++)
			w.DrawText(i * 100, 2, AsString(i));
	}
};

struct MyApp : TopWindow {
	FrameTop<StaticRect> bararea; // we represent whole TitleBar area as frame
	ParentCtrl           barrect; // to do custom caption clipping
	MenuBar              menubar, menu2;
	LineEdit             editor;
	Label                title;
	Label                label;
	TestRect             rect;
	Button               button;
	bool                 zooming = true;
	WithDropChoice<EditString> ed;
	
	void MainMenu(Bar& bar)
	{
		bar.Sub("File", [=](Bar& bar) {
			bar.Add("Zooming", [=] {
				zooming = !zooming;
				Zoomable(zooming);
			});
			bar.Add("Yellow", [=] { CustomTitleBar(Yellow()); });
			bar.Add("Cyan", [=] { CustomTitleBar(Cyan()); });
			bar.Add("LtCyan", [=] { CustomTitleBar(LtCyan()); });
			bar.Add("Blue", [=] { CustomTitleBar(Blue()); });
			bar.Add("Gray", [=] { CustomTitleBar(Gray()); });
			bar.Separator();
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
	
	virtual void Layout() override
	{
		String s = IsMinimized() ? "Minimized" : IsMaximized() ? "Maximized" : "Overlapped";
		Title(s);
		DLOG("Layout " << s << ", rect: " << GetScreenRect() << ", mousepos: " << GetMousePos());
	}
	
	MyApp() {
		Sizeable().Zoomable();
		Icon(CtrlImg::new_doc());
			AddFrame(menubar);
			SetMenuBar();
			Title("This is CustomTitleBar example - CustomTitleBar not active");
		Add(editor.SizePos());

		Ctrl *tb = CustomTitleBar(LtCyan(), GetStdFontCy() + DPI(4));
		if(tb) {
//			*tb << label.SizePos();
			*tb << rect.SizePos();
			rect.SetFrame(BlackFrame());
			rect << label.SizePos();
			label.SetLabel("\1[g= This is test");
			button.SetLabel("Close");
			rect << button.RightPos(DPI(2), DPI(100)).VSizePos(2, 2);
			button << [=] {
				Break();
			};
			rect << ed.RightPos(DPI(110), DPI(100)).VSizePos(2, 2);
			for(int i = 0; i < 200; i++)
				ed.AddList(AsString(i));
		}

		String txt;
		for(int i = 0; i < 200; i++)
			txt << i << '\n';
		editor <<= txt;
	};
};

GUI_APP_MAIN
{
	MyApp().Run();
}
