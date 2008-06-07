#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct NonModalDialog : public TopWindow {
	Button b;

	void DoClose() {
		Close();
	}

	typedef NonModalDialog CLASSNAME;

	NonModalDialog() {
		SetRect(0, 0, 200, 50);
		Add(b.SetLabel("Close non-modal dialog").SizePos());
		b <<= THISBACK(DoClose);
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

	typedef MainWindow CLASSNAME;

	MainWindow() {
		SetRect(0, 0, 400, 100);
		Add(b.SetLabel("Open/close non-modal dialog").SizePos());
		b <<= THISBACK(DoOpen);
	}
};

GUI_APP_MAIN
{
	MainWindow win;
	win.Run();
}
