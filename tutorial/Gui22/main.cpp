#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct NonModalDialog : public TopWindow {
	Button b;

	void DoClose() {
		Close();
	}

	NonModalDialog() {
		SetRect(0, 0, 200, 50);
		Add(b.SetLabel("Close non-modal dialog").SizePos());
		b << [=] { DoClose(); };
	}
};

struct MainWindow : public TopWindow {
	NonModalDialog dlg;
	Button b;

	void DoOpen() {
		if(dlg.IsOpen())
			dlg.Close();
		else
			dlg.Open(this);
	}

	MainWindow() {
		SetRect(0, 0, 400, 100);
		Add(b.SetLabel("Open/close non-modal dialog").SizePos());
		b << [=] { DoOpen(); };
	}
};

GUI_APP_MAIN
{
	MainWindow win;
	win.Run();
}
