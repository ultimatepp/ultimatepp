#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TrayIcon {
	void LeftDouble() {
		PromptOK("TrayIcon launched this prompt!");
	}

	void Message() {
		Info("Title", "This is a baloon message!");
	}

	void Menu(Bar& bar) {
		bar.Add("Info..", THISBACK(LeftDouble));
		bar.Add("Message..", THISBACK(Message));
		bar.Separator();
		bar.Add("Exit", THISBACK(Break));
	}

	typedef App CLASSNAME;

	App() {
//		WhenBar = THISBACK(Menu);
//		WhenLeftDouble = THISBACK(Info);
		Icon(CtrlImg::File());
		Tip("This is U++ tray icon");
	}
};

GUI_APP_MAIN
{
	App().Run();
}
