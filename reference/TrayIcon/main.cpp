#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS Tray
#define IMAGEFILE  <TrayIcon/tray.iml>
#include <Draw/iml.h>

struct App : TrayIcon {
	virtual void LeftDouble() {
		Icon(Tray::Icon1());
		PromptOK("TrayIcon launched this prompt!");
		Icon(Tray::Icon());
	}
	
	virtual void LeftDown() {
		Info("TrayIcon", "You have clicked the TrayIcon!\n""TrayIcon is U++ reference example.");
	}

	virtual void Menu(Bar& bar) {
		bar.Add("Info..", THISFN(LeftDouble));
		bar.Separator();
		bar.Add("Exit", [=] { Break(); });
	}

	typedef App CLASSNAME;

	App() {
		Icon(Tray::Icon());
		Tip("This is U++ TrayIcon");
	}
};
 
GUI_APP_MAIN
{
	App().Run();
}
