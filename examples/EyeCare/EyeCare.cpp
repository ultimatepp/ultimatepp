#include "EyeCare.h"

#define IMAGECLASS EyeCareImg
#define  IMAGEFILE <EyeCare/EyeCare.iml>
#include <Draw/iml.h>

#define TOPICFILE <EyeCare/app.tpp/all.i>
#include <Core/topic_group.h>

#ifdef _DEBUG
#define MINUTE_M 1000 // to reduce waiting when debugging....
#else
#define MINUTE_M 60 * 1000
#endif

void EyeCare::About()
{
	dismiss.Kill();
	ignore++;
	WithAboutDialogLayout<TopWindow> dlg;
	dlg.text = GetTopic("topic://EyeCare/app/about$en-us").text;
	CtrlLayoutOK(dlg, "About");
	dlg.CenterScreen();
	dlg.Run();
	ignore--;
	Restart();
}

void EyeCare::Restart()
{
	relax.KillSet(-(int)~config.relax * MINUTE_M, THISBACK(Relax));
	wash.KillSet(-(int)~config.wash * MINUTE_M, THISBACK(Wash));
}

void EyeCare::Perform(int newstate)
{
	if(newstate < state)
		return;
	state = newstate;
	String txt1[] = {
		t_("Welcome to Eye Care!"),
		t_("Eye relaxing time..."),
		t_("Eye washing time...")
	};
	String txt2[] = {
        t_("This program will remind you at regular intervals\nto take care of your eyes and relax/wash them."),
        t_("Now move your eyes away, from the monitor\nand view some distant object for 15 seconds."),
        t_("Now move to the wash room, and gently wash \nyour eyes with water to prevent dryness.")
	};
	dismiss.KillSet(15000, THISBACK(Dismiss));
	text1 = txt1[state];
	text2 = txt2[state];
	if(!IsOpen())
		OpenMain();
	Show();
	SetForeground();
}

void EyeCare::Launch()
{
	Perform(LAUNCH);
}

void EyeCare::Dismiss()
{
	state = -1;
	Hide();
}

void EyeCare::Relax()
{
	if(ignore) return;
	Perform(RELAX);
}

void EyeCare::Wash()
{
	if(ignore) return;
	Perform(WASH);
}

void EyeCare::Sync()
{
	TopMost(config.topmost);
	trayicon.Icon(ignore ? EyeCareImg::off() : EyeCareImg::icon());
	if(config.hideicon)
		ignore = false;
	trayicon.Show(!config.hideicon);
	Restart();
}

void EyeCare::Configure()
{
	dismiss.Kill();
	relax.Kill();
	wash.Kill();
	config.Execute();
	Sync();
}

void EyeCare::Ignore()
{
	ignore = !ignore;
	Sync();
}

void EyeCare::SettingsMenu(Bar& bar)
{
	bar.Add(t_("Configuration"), THISBACK(Configure))
	   .Help(t_("Change settings"));
	bar.Add(t_("Exit"), THISBACK(Exit))
	   .Help(t_("Exit the application"));
}

void EyeCare::HelpMenu(Bar& bar)
{
	bar.Add(t_("About"), THISBACK(About))
	   .Help(t_("About EyeCare"));
}

void EyeCare::MainMenu(Bar& bar)
{
	bar.Add(t_("Settings"), THISBACK(SettingsMenu));
	bar.Add(t_("Help"), THISBACK(HelpMenu));
}

void EyeCare::TrayMenu(Bar& bar)
{
	bar.Add(t_("Show EyeCare"), THISBACK(Launch));
	bar.Add(t_("Ignore eyes"), THISBACK(Ignore))
	   .Check(ignore);
	bar.Add(t_("About.."), THISBACK(About));
	bar.Add(t_("Exit"), THISBACK(Exit));
}

void EyeCare::Serialize(Stream& s)
{
	int version = 1;
	s / version;
	s % config.relax % config.wash % config.hideicon % config.topmost;
	s % ignore;
	SerializePlacement(s);
}

const char regpath[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const char regname[] = "EyeCareUpp";

void EyeCare::Exit()
{
	Break();
	trayicon.Break();
#ifdef PLATFORM_WIN32
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, regpath, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS) {
		RegDeleteValue(hkey, regname);
		RegCloseKey(hkey);
	}
#endif
}

void EyeCare::Do()
{
	LoadFromFile(*this);
	Sync();
	Launch();
	Restart();
#ifdef PLATFORM_WIN32
	HKEY key;
	if(RegCreateKey(HKEY_CURRENT_USER, regpath, &key) == ERROR_SUCCESS) {
		RegSetValueEx(key, regname, 0, REG_SZ, GetExeFilePath(), GetExeFilePath().GetLength() + 1);
		RegCloseKey(key);
	}
#endif
	trayicon.Run();
	StoreToFile(*this);
}

EyeCare::EyeCare()
{
	CtrlLayoutOKCancel(config, t_("Configuration"));
	CtrlLayout(*this, t_("EyeCare"));
	menu.Set(THISBACK(MainMenu));
	image.SetImage(EyeCareImg::eye());
	CenterScreen();
	hide <<= THISBACK(Hide);
	config.relax <<= 5;
	config.wash <<= 55;
	Icon(EyeCareImg::icon());
	state = -1;

	trayicon.WhenBar = THISBACK(TrayMenu);
	trayicon.WhenLeftDouble = THISBACK(Launch);
	trayicon.Icon(EyeCareImg::icon());
	trayicon.Tip(t_("EyeCare"));
}

GUI_APP_MAIN
{
#ifdef PLATFORM_WIN32
	static const char unique_name[] = "EyeCare_$_$_U++";
	if(::FindWindow(NULL, unique_name)) {
		Exclamation(t_("EyeCare is already running."));
		return;
	}
	TopWindow singlechk;
	singlechk.SetRect(-1, -1, 1, 1);
	singlechk.Hide();
	singlechk.Title(unique_name);
	singlechk.Open();
#endif

	EyeCare().Do();
}
