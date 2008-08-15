#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TrayIcon {
	virtual void LeftDouble() {
		Icon(CtrlImg::open());
		PromptOK("TrayIcon launched this prompt!");
		Icon(CtrlImg::save());
	}

	virtual void LeftDown() {
//		Info("", "xx");
		Info("TrayIcon", "You have clicked the TrayIcon!\n""TrayIcon is U++ reference example.");
//		Exclamation("!!!");
	}

	virtual void Menu(Bar& bar) {
		bar.Add("Info..", THISBACK(LeftDouble));
		bar.Separator();
		bar.Add("Exit", THISBACK(Break));
	}

	typedef App CLASSNAME;

	App() {
		Icon(CtrlImg::save());
		Tip("This is U++ TrayIcon");
	}
};

GUI_APP_MAIN
{
	App().Run();
}

/*
struct App : TrayIcon {
    void Info() {
//        Icon(Tray::Icon1());
        PromptOK("TrayIcon launched this prompt!");
//        Icon(Tray::Icon());
		Message(MESSAGE_WARNING, "A title", "A text");

    }

    void Menu(Bar& bar) {
        bar.Add("Info..", THISBACK(Info));
        bar.Separator();
        bar.Add("Exit", THISBACK(Break));
    }

    typedef App CLASSNAME;

    App() {
        WhenBar = THISBACK(Menu);
        WhenLeftDouble = THISBACK(Info);
        Icon(CtrlImg::copy());
        Tip("This is U++ tray icon");
    }
};
*/
