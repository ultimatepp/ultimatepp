#include "App.h"
#include "setup.brc"

#define IMAGECLASS AppImg
#define IMAGEFILE <E011Setup/setup.iml>
#include <Draw/iml_source.h>

SelectFolder::SelectFolder() {
	CtrlLayoutOKCancel(*this, t_("Installazione del Tool di mappatura dei Contalitri E011"));
	Icon(AppImg::setup16());
	browse <<= THISBACK(BrowsePath);
	info.SetQTF(GetTopic("E011Setup", "app", "firstLay$it-it"));
	info.SetZoom(ZoomP(info, 140));
	installPath = GetWinRegString("ProgramFilesDir", "Software\\Microsoft\\Windows\\CurrentVersion", HKEY_LOCAL_MACHINE);
	path = installPath + "\\EPD\\Tool E011";
	
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

SelectFolderGroup::SelectFolderGroup() {
	CtrlLayoutOK(*this, "Tool E011 - Installazione Icone");
	Icon(AppImg::setup16());

	return;	
}

void SelectFolderGroup::Perform() {
	
	prgfolderinfo.SetQTF("[2 Selezionare il gruppo di programmi in cui installare le icone di avvio. ]");
	prgfolderinfo.SetZoom(ZoomP(prgfolderinfo, 140));

	prgfolderlist.AddColumn("", 10);
	prgfolderlist.AddColumn("", 90);
	FindFile ff(GetShellFolder("Programs", HKEY_CURRENT_USER) + "\\*.*");
	prgfolderlist.WhenAction = THISBACK(SelectFolder);
	int i = 0;
	while(ff) {
		String name = ff.GetName();
		if(ff.IsFolder()) {
			prgfolderlist.SetDisplay(i,0, Single<DisplayProgramFolderIcon>());			
			prgfolderlist.Set(i,1, name);
			prgfolderlist.SetLineCy(i, 18);
			i++;
		}
		ff.Next();
	}
	prgfolder = String("EPD\\E011-V15");
	
	desktopiconinfo.SetQTF("[2 Desideri creare un'icona di avvio sul Desktop di Windows?]");
	desktopiconinfo.SetZoom(ZoomP(desktopiconinfo, 140));
	option.SetLabel("Crea icona di avvio sul Descktop");
	option <<= true;
	if(Run() == IDOK) {
		if((bool)~option == true) {
			InstallDesktopIcon((const char*)~String(installPath + "\\" + FormatInt(APP_MAJORVER) + "." + FormatInt(APP_MINORVER) + "\\e011.exe"), "Tool E011.lnk", "Tool E011");
		}
	}
	Hide();

/*	dbDatabase db1;
	dbRecord rc1;

	db1.Open(installPath + "\\1.5\\e011.cfg");	
	if(prgfolder.GetLength() > 0) {		
		rc1("StartFolderPath") = String(prgfolder);		
		InstallProgramGroup(~String(prgfolder), (const char*)~String(installPath + "\\" + FormatInt(APP_MAJORVER) + "." + FormatInt(APP_MINORVER) + "\\e011.exe"), "", "E011 Tool.lnk", "E011 V20", "", 0);
		InstallProgramGroup(~String(prgfolder), (const char*)~String(installPath + "\\" + FormatInt(APP_MAJORVER) + "." + FormatInt(APP_MINORVER) + "\\e011.exe"), "--uninstall", "Rimuovi E011.lnk", "Rimuovi E011 V20", (const char*)~String(installPath + "\\" + FormatInt(APP_MAJORVER) + "." + FormatInt(APP_MINORVER) + "\\Support\\iclib.icl"), 1);
	}
	else {
		rc1("StartFolderPath") = "";		
	}
	db1.PutRecord("WindowsKeys", 1, rc1);
	db1.Close();
*/	
	return;
}

void SelectFolderGroup::SelectFolder() {

	prgfolder = String(prgfolderlist.Get(prgfolderlist.GetCursor(),1)) + String("\\EPD\\E011-V15");
}

GUI_APP_MAIN {
	SetLanguage(LNGFromText("IT-IT"));
	SetDefaultCharset(CHARSET_WIN1252);
	Draw::SetStdFont(Tahoma(11));
	
	SelectFolder sf;
	if(!sf.Perform()) return;
	sf.Hide();
	
	// Verifico se esiste e salvo vecchio file cfg.
	FileMove(~String(installPath + "\\1.5\\e011.cfg"), ~String(installPath + "\\1.5\\e011_cfg.old"));
	
	MyProgress p;
	int ts = 0;
	int pg = 0;
	for(int i = 0; i < root_all_count; i++) ts += root_all_length[i];
	for(int i = 0; i < help_all_count; i++) ts += help_all_length[i];
	for(int i = 0; i < support_all_count; i++) ts += support_all_length[i];
	for(int i = 0; i < mappature_all_count; i++) ts += mappature_all_length[i];
	
	p.SetMax(ts);
	p.SetLev(0);
	p.Title("Installazione in corso");
	p.AlignText(Ctrl::LEFT);
	RealizePath(installPath + "\\1.5\\");
	for(int i = 0; i < root_all_count; i++) {
	  p.SetText(root_all_files[i]);
		if(!SaveFile(installPath + "\\1.5\\" + root_all_files[i], BZ2Decompress(String(root_all[i], root_all_length[i]), p))) {
			Exclamation("Errore. Pacchetto di installazione incompleto !");
			return;
		}
		pg += root_all_length[i];
		p.SetLev(pg);
	}	
	RealizePath(installPath + "\\1.5\\help.tpp\\");
	for(int i = 0; i < help_all_count; i++) {
	  p.SetText(help_all_files[i]);
		if(!SaveFile(installPath + "\\1.5\\help.tpp\\" + help_all_files[i], BZ2Decompress(String(help_all[i], help_all_length[i]), p))) {
			Exclamation("Errore. Pacchetto di installazione incompleto !");
			return;
		}
		pg += help_all_length[i];
		p.SetLev(pg);
	}
	RealizePath(installPath + "\\1.5\\Support\\");
	for(int i = 0; i < support_all_count; i++) {
	  p.SetText(support_all_files[i]);
		if(!SaveFile(installPath + "\\1.5\\Support\\" + support_all_files[i], BZ2Decompress(String(support_all[i], support_all_length[i]), p))) {
			Exclamation("Errore. Pacchetto di installazione incompleto !");
			return;
		}
		pg += support_all_length[i];
		p.SetLev(pg);
	}
	RealizePath(installPath + "\\Mappature\\");
	for(int i = 0; i < mappature_all_count; i++) {
	  p.SetText(mappature_all_files[i]);
		if(!SaveFile(installPath + "\\Mappature\\" + mappature_all_files[i], BZ2Decompress(String(mappature_all[i], mappature_all_length[i]), p))) {
			Exclamation("Errore. Pacchetto di installazione incompleto !");
			return;
		}
		pg += mappature_all_length[i];
		p.SetLev(pg);
	}
	p.Close();
		
	InstallUninstall((const char*)~String(installPath + "\\" + FormatInt(APP_MAJORVER) + "." + FormatInt(APP_MINORVER) + "\\e011.exe"), "EPD - E011 V20", " --uninstall");

	PromptOK("Installazione completata con successo.");
	return;
}
