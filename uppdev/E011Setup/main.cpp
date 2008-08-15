#include "main.h"
/*
#define TOPICFILE <E011Setup/app.tpp/all.i>
#include <Core/topic_group.h>
*/
SelectFolder::SelectFolder() {
	CtrlLayoutOKCancel(*this, "Installazione del Tool di mappatura dei Contalitri E011");
	Icon(setup16());
	browse <<= THISBACK(BrowsePath);
	info.SetQTF(GetTopic("E011Setup", "app", "firstLay$it-it"));
	info.SetZoom(ZoomP(info, 140));
	installPath = GetWinRegString("ProgramFilesDir", "Software\\Microsoft\\Windows\\CurrentVersion", HKEY_LOCAL_MACHINE);
	path = installPath + "\\E011";

	return;
};

bool SelectFolder::Perform() {
	int r = Run();
	installPath = ~path;
	if(r != IDOK) return false;
	else return true;
}

void SelectFolder::BrowsePath() {
	FileSel _path;
	_path.ActiveDir((const char*)~installPath);
	if(!_path.ExecuteSelectDir()) return;
	installPath = _path;
	path = _path;
	return;
}

#define VERSIONE 1
#define REVISIONE 1

GUI_APP_MAIN {
	SetLanguage(LNGFromText("IT-IT"));
	SetDefaultCharset(CHARSET_WIN1252);
	Draw::SetStdFont(ScreenSans(11));

	SelectFolder sf;
	if(!sf.Perform()) return;
	sf.Hide();

	RealizePath(installPath);
	if(Extract(GetExeDirFile("e011.zip"), installPath + "\\", true) == 1000001) {
		Exclamation("Errore. Pacchetto di installazione incompleto !");
		return;
	}
	InstallUninstall((const char*)~String(installPath + "\\" + FormatInt(VERSIONE) + "." + FormatInt(REVISIONE) + "\\e011.exe"), "E011", " --uninstall");

	WithInfoOptionLay<TopWindow> form1;
	CtrlLayoutOK(form1, "Tool E011 - Installazione Icona Desktop");
	form1.Icon(setup16());
	form1.info.SetQTF("[2 Desideri creare un'icona di avvio sul Desktop di Windows?]");
	form1.info.SetZoom(ZoomP(form1.info, 140));
	form1.option.SetLabel("Crea icona di avvio sul Descktop");
	form1.option <<= true;
	if(form1.Run() == IDOK) {
		if((bool)~form1.option == true) {
			InstallDesktopIcon((const char*)~String(installPath + "\\" + FormatInt(VERSIONE) + "." + FormatInt(REVISIONE) + "\\e011.exe"), "Contalitri E011.lnk", "Contalitri E011");
		}
	}
	form1.Hide();
	InstallProgramGroup("Contalitri E011", (const char*)~String(installPath + "\\" + FormatInt(VERSIONE) + "." + FormatInt(REVISIONE) + "\\e011.exe"), "", "Contalitri E011.lnk", "Contalitri E011", "", 0);
	InstallProgramGroup("Contalitri E011", (const char*)~String(installPath + "\\" + FormatInt(VERSIONE) + "." + FormatInt(REVISIONE) + "\\e011.exe"), "--uninstall", "Rimuovi E011.lnk", "Rimuovi E011", (const char*)~String(installPath + "\\" + FormatInt(VERSIONE) + "." + FormatInt(REVISIONE) + "\\trash_16.ico"), 0);
	PromptOK("Installazione completata con successo.");
	return;
}
