#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct NonModalDialog : public TopWindow {
	Button b;

	NonModalDialog() {
		SetRect(0, 0, Zx(200), Zy(50));
		Add(b.SetLabel("Close non-modal dialog").SizePos());
		b << [=] { Close(); };
	}
};

struct MainWindow : public TopWindow {
	NonModalDialog dlg;
	Button b;

	MainWindow() {
		SetRect(0, 0, Zx(400), Zy(100));
		Add(b.SetLabel("Open/close non-modal dialog").SizePos());
		b << [=] {
			if(dlg.IsOpen())
				dlg.Close();
			else
				dlg.Open(this);
		};
	}
};

GUI_APP_MAIN
{
	MainWindow win;
	win.Run();
}
