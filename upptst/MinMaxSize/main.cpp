#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow
{
	DropList dl;
	Button   move;

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), LtCyan());
		DrawFrame(w, 0, 0, 500, 500, Black());
		DrawFrame(w, 0, 0, 400, 400, Black());
		DrawFrame(w, 0, 0, 600, 600, Black());
		w.DrawText(10, 10, AsString(GetScreenView()));
		w.DrawText(10, 50, AsString(GetScreenRect()));
		w.DrawText(10, 90, AsString(GetRect()));
		w.DrawText(10, 130, AsString(GetMousePos()));
	}
	
	void MouseMove(Point p, dword keyflags) override {
		Refresh();
	}

	void Layout() override {
		Refresh();
	}

	Size GetMinSize() const override {
		return Size(400, 400);
	}

	Size GetMaxSize() const override {
		return Size(600, 600);
	}

    typedef App CLASSNAME;

    App()
    {
        SetRect(100, 200, 500, 500);
		Sizeable().Zoomable();
		
		Add(dl.LeftPos(10, 101).BottomPos(0));
		for(int i = 0; i < 20; i++)
			dl.Add(i);
		
		Add(move.HSizePos(120, 0).BottomPosZ(0, 24));
		move.SetLabel("Move to 320, 420, 400");
		move << [=] {
			DLOG("================ MOVE");
			SetRect(320, 420, 400, 400);
		};
	}
};

struct App2 : App {
	Label title;
	
	App2() {
        SetRect(700, 200, 500, 500);
		Ctrl *tb = CustomTitleBar(SYellow(), GetStdFontCy() + DPI(4));
		if(tb)
			*tb << title.SizePos();
		
		title = "\1[g= [* custom [/_ titlebar";
	}
};


GUI_APP_MAIN
{
	App app, app_csd;
	App2 app2;

    app.Title("SSD");
	app.OpenMain();

#ifdef PLATFORM_POSIX
    app_csd.Title("CSD");
	app_csd.Force_csd_();
	app_csd.SetRect(1300, 200, 500, 500);
	app_csd.OpenMain();
#endif

	app2.OpenMain();
	
	app.Run();
}
