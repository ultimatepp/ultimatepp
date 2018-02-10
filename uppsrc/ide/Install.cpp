#include "ide.h"
//#include "Install.h"

#ifndef PLATFORM_WIN32

bool CopyFolder(Progress& pi, const char *dst, const char *src)
{
	return CopyFolder(dst, src, &pi);
}

void ChkSupp(const char *s, String& dir)
{
	if(IsNull(dir) && FileExists(AppendFileName(s, "GCC.bm")))
		dir = s;
}

String DefaultInstallFolder()
{
	String DefaultFolder;
	String ExeTitle = ToUpper(GetExeTitle());
	for(int i = 0 ; i < ExeTitle.GetCount();i++) {
		if(ExeTitle[i] >= 'a' && ExeTitle[i] <= 'z')
			ExeTitle.Set(i, ExeTitle[i] + 'A'-'a');
	}
	if(ExeTitle.Find("SVN") >= 0)
		DefaultFolder = "upp-svn";
	else if(ExeTitle.Find("DEV") >= 0)
		DefaultFolder = "upp-dev";
	else if(ExeTitle.Find("BETA") >= 0)
		DefaultFolder = "upp-beta";
	else
		DefaultFolder = "upp";
	
	return DefaultFolder;
}

struct XInstallDlg : public WithXInstallLayout<TopWindow> {

private:

	FrameRight<Button> pathbrowse;

	void	FindInstFolder();

public:

	typedef XInstallDlg CLASSNAME;

	XInstallDlg();
};

void XInstallDlg::FindInstFolder() {
	FileSel *fs = &OutputFs();
	fs->Set(~path);
	if(! fs->ExecuteSelectDir("Select output directory ..."))
		return;
	path <<= ~(*fs);
}

XInstallDlg::XInstallDlg() {
	CtrlLayoutOKCancel(*this, "Ultimate++ user setup");
	pathbrowse <<= THISBACK(FindInstFolder);
	pathbrowse.SetMonoImage(CtrlImg::smallright()).NoWantFocus();
	uppsrc = true;
	reference = true;
	examples = true;
	tutorial = true;
	bazaar = true;
	path.AddFrame(pathbrowse);
	if(FileExists(ConfigFile("installpath")))
		path <<= LoadFile(ConfigFile("installpath"));
	else
		path <<= AppendFileName(FromSystemCharset(getenv("HOME")), DefaultInstallFolder());
}

bool Install()
{
	if(!(InstallWizard().Run()&(IDOK|IDCANCEL))) return false;

	String supp=UpdaterCfg().globalsrc;
	FindFile ff(ConfigFile("*.bm"));
	if(!ff) {
		ff.Search(AppendFileName(supp, "*.bm"));
		while(ff) {
			FileCopy(ff.GetPath(), ConfigFile(ff.GetName()));
			ff.Next();
		}
	}
	// 2008/06/01 -- add valgrind suppression file
	String ValgSupp = ConfigFile("valgrind.supp");
	if(IsNull(LoadFile(ValgSupp)))
		SaveFile(ValgSupp, LoadFile(AppendFileName(supp, "uppsrc/ide/valgrind.supp")));
	// 2008/06/01 -- END
	//PromptOK("Ultimate`+`+ user setup was finished.&Press OK to launch TheIDE.");
	return true;
}

//void Uninstall() {}

#endif
