#include "ide.h"
//#include "Install.h"

#ifdef PLATFORM_WIN32

String GetShellFolder(const char *name, HKEY type)
{
	return GetWinRegString(name, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", type);
}

void DelKey(const char *dir, const char *key)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, key);
	RegCloseKey(hkey);
}

void RemoveWindowsItems()
{
	DeleteFile(AppendFileName(GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE), "Ultimate++ IDE.lnk"));
	DeleteFile(AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), "TheIde.lnk"));

	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Ultimate++", "DisplayName");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Ultimate++", "UninstallString");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", "Ultimate++");
}

void Uninstall()
{
	String path = GetExeFilePath();
	String bat = AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), "removepp.bat");
	String dir = GetFileFolder(path);
	if(!PromptYesNo("[*3 Do you wish to uninstall Ultimate`+`+ development system ?&&]"
	                "Uninstall will remove [* " + DeQtf(dir) + "] directory and all "
	                "registry and desktop items associated with Ultimate`+`+.")) return;
	DeleteFolderDeep(dir);
	RemoveWindowsItems();
	SaveFile(bat,
		":Repeat\r\n"
		"del \"" + path + "\"\r\n"
		"if exist \"" + path + "\" goto Repeat\r\n"
		"del \"" + GetExeDirFile("theide.log") + "\"\r\n"
		"del \"" + GetExeDirFile("dbghelp.dll") + "\"\r\n"
		"rmdir \"" + dir + "\"\r\n"
		"del \"" + bat + "\"\r\n"
	);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	char h[512];
	strcpy(h, bat);
	if(CreateProcess(NULL, h, NULL, NULL, FALSE,
	                  IDLE_PRIORITY_CLASS, NULL, GetShellFolder("Desktop", HKEY_CURRENT_USER),
	                  &si, &pi))
		Exclamation("Uninstall successful.");
	else
		Exclamation("Uninstall failed to remove some files...&" + DeQtf(GetLastErrorMessage()));
}

#define Ptr Ptr_
#define byte byte_
#define CY win32_CY_


#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY

bool CreateShellLink(const char *filepath, const char *linkpath, const char *desc, int icon)
{
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
	                        (PVOID *) &psl);
	if(SUCCEEDED(hres)) {
		psl->SetPath(filepath);
		psl->SetDescription(desc);
		if(icon >= 0)
			psl->SetIconLocation(filepath, icon);
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if (SUCCEEDED(hres)) {
			WCHAR szPath[_MAX_PATH] = { 0 };
			MultiByteToWideChar(CP_ACP, 0, linkpath, (int)strlen(linkpath), szPath, _MAX_PATH);
			hres = ppf->Save(szPath, TRUE);
			ppf->Release();
		}
	}
	psl->Release();
	CoUninitialize();
	return SUCCEEDED(hres);
}

bool InstallDesktopIcon(const char *exe, const char *lnk, const char *desc)
{
	return CreateShellLink(exe,
	                       AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), lnk),
	                       desc, -1);
}

bool InstallProgramGroup(const char *exe, const char *groupname, const char *lnk, int icon = -1)
{
	String dir = GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE);
	if(groupname) {
		dir = AppendFileName(dir, groupname);
		CreateDirectory(dir, NULL);
	}
	return CreateShellLink(exe, AppendFileName(dir, lnk), "", icon);
}

void InstallUninstall(const char *name, const char *dname, const char *cmdline)
{
	String path = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + String(name);
	SetWinRegString(dname, "DisplayName", path);
	SetWinRegString(cmdline, "UninstallString", path);
}

void InstallUninstall(const char *exe, const char *name)
{
	InstallUninstall(name, name, String(exe) + " -uninstall");
}

String Rdir(const char *s, const String& dir, const String& dir2)
{
	String r;
	while(*s) {
		if(*s == '#')
			r.Cat(dir);
		else
		if(*s == '@')
			r.Cat(dir2);
		else
			r.Cat(*s);
		s++;
	}
	return r;
}

bool CheckLicense()
{
	if(!FileExists((GetExeDirFile("license.chk"))))
		return true;
	ShowSplash();
	Ctrl::ProcessEvents();
	Sleep(2000);
	HideSplash();
	Ctrl::ProcessEvents();
	WithLicenseLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, "License agreement");
	d.license = GetTopic("ide/app/BSD$en-us").text;
	d.license.Margins(4);
	d.license.SetZoom(Zoom(18, 100));
	d.ActiveFocus(d.license);
	if(d.Run() != IDOK) {
		Uninstall();
		return false;
	}
	DeleteFile(GetExeDirFile("license.chk"));
	return true;
}

void BrowseField(EditField *f)
{
	FileSel fs;
	String s = ~*f;
	if(DirectoryExists(s))
		fs.ActiveDir(s);
	if(fs.ExecuteSelectDir("Select the directory for MyApps"))
		*f <<= ~fs;
}

bool Install()
{
	{
		WithInfoLayout<TopWindow> d;
		CtrlLayoutOKCancel(d, "Installation guide");
		d.info = GetTopic("ide/app/install$en-us").text;
		d.info.Margins(4);
		d.info.SetZoom(Zoom(18, 100));
		d.ActiveFocus(d.info);
		if(d.Run() != IDOK)
			return false;
		WithInstallLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, "Ultimate++ user-code setup");
		String h = GetExeFilePath().Mid(0, 3) + "MyApps";
		dlg.myapps <<= h;
		dlg.myapps.AddList(h);
		dlg.myapps.AddList(GetHomeDirFile("MyApps"));
		dlg.browse <<= callback1(BrowseField, &dlg.myapps);
		dlg.ActiveFocus(dlg.myapps);
		if(dlg.Run() != IDOK)
			return false;
		String ipp = GetExeDirFile("install.upp");
		String exe = GetExeFilePath();
		String dir = GetFileFolder(exe);
		String dir2 = ~dlg.myapps;
		RealizeDirectory(dir2);
		FileIn in(ipp);
		String out = GetExeDirFile("out");
		if(ToLower(exe).StartsWith(ToLower(GetProgramsFolder())) || ToLower(exe).Find("program files") >= 0) {
			// We do not want to put MyApps folder dir into Program Files
			out = GetHomeDirFile("upp.out");
		}
		while(!in.IsEof()) {
			Vector<String> ln = Split(in.GetLine(), '|');
			if(ln.GetCount() != 4)
				break;
			SaveFile(AppendFileName(dir, ln[0]),
				"UPP = " + AsCString(Rdir(ln[1], dir, dir2)) + ";\r\n"
				"COMMON = " + AsCString(Rdir(ln[2], dir, dir2)) + ";\r\n"
				"OUTPUT = " + AsCString(out /* Rdir(ln[3], dir, dir2) */) + ";\r\n"
			);
		}
		in.Close();
		if(dlg.icon)
			InstallDesktopIcon(exe, "TheIde.lnk", "Ultimate++ IDE");
		InstallProgramGroup(exe, NULL, "Ultimate++ IDE.lnk");
		InstallUninstall(exe, "Ultimate++");
		DeleteFile(ipp);
	}
	AutoSetup();
	PromptOK("Ultimate`+`+ setup was finished.&Press OK to launch TheIDE.&"
	         "[* WARNING:] Do not put important files into the install directory as they "
	         "would be [* deleted] during [* uninstall] or [* upgrade] process!");;
	return true;
}

#else

bool CopyFolder(const char *dst, const char *src, Progress *pi)
{
	if(strcmp(src, dst) == 0)
		return true;
	RealizeDirectory(dst);
	if(pi)
		pi->SetText(dst);
	FindFile ff(AppendFileName(src, "*"));
	while(ff) {
		if(pi && pi->StepCanceled())
			return false;
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFolder())
			if(!CopyFolder(d, s, pi))
				return false;
		if(ff.IsFile())
			SaveFile(d, LoadFile(s));
		ff.Next();
	}
	return true;
}

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
	fs->Set(path);
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
