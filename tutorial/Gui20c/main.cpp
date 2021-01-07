#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button ok, cancel;
	EditDate date;

	MyAppWindow() {
		SetRect(0, 0, Zx(200), Zy(90));
		Add(date.LeftPosZ(10, 80).TopPosZ(10, 20));
		Add(ok.SetLabel("OK").LeftPosZ(10, 64).TopPosZ(40, 24));
		Add(cancel.SetLabel("Cancel").LeftPosZ(100, 64).TopPosZ(40, 24));

		ok.Ok() << Acceptor(IDOK);
		cancel.Cancel() << Rejector(IDCANCEL);
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	switch(app.Run()) {
	case IDOK:
		PromptOK(String().Cat() << "OK: " << ~app.date);
		break;
	case IDCANCEL:
		Exclamation("Canceled");
	}
}
