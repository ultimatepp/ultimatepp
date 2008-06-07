#ifndef _EyeCare_EyeCare_h_
#define _EyeCare_EyeCare_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <EyeCare/EyeCare.lay>
#include <CtrlCore/lay.h>

class EyeCare : public WithEyeCareLayout<TopWindow> {
	enum { LAUNCH, RELAX, WASH };
	int          state;
	int          ignore;
	TimeCallback relax, wash, dismiss;
	TrayIcon     trayicon;
	WithConfigurationLayout<TopWindow> config;


	void Perform(int newstate);
	void Relax();
	void Wash();
	void Configure();
	void Dismiss();

	void MainMenu(Bar& menu);
	void SettingsMenu(Bar& bar);
	void HelpMenu(Bar& bar);
	void TrayMenu(Bar& bar);

	void Ignore();
	void Launch();
	void Restart();
	void Sync();
	void About();
	void StartLaunch();
	void Exit();

public:
	void Serialize(Stream& s);
	void Do();

	typedef EyeCare CLASSNAME;

	EyeCare();
};

#endif
