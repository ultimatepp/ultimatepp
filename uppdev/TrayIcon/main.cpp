#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS Tray
#define IMAGEFILE  <TrayIcon/tray.iml>
#include <Draw/iml.h>

struct App : TrayIcon {
	virtual void LeftDouble() {
		Icon(Tray::Icon1());
		PromptOK("Hezky česky říkej žížala!");
		Icon(Tray::Icon());
	}
	
	virtual void LeftDown() {
		Info("TrayIcon", "Hezky česky říkej žížala\n""TrayIcon is U++ reference example.");
	}

	virtual void Menu(Bar& bar) {
		bar.Add("Info..", THISBACK(LeftDouble));
		bar.Separator();
		bar.Add("Exit", THISBACK(Break));
	}

	typedef App CLASSNAME;

	App() {
		Icon(Tray::Icon());
		Tip("Hezky česky říkej žížala");
	}
};

GUI_APP_MAIN
{
	SetLanguage(LNG_('C','S','C','Z'));
	App().Run();
}
