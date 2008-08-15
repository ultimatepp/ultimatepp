#include "main.h"

bool InstallDesktopIcon(const char *exe, const char *lnk, const char *desc) {
	return CreateShellLink(exe, "", AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), lnk), desc, "", 0);
}

bool InstallProgramGroup(const char *groupname, const char *exe, const char *args, const char *lnk, const char *desc, const char *fileicon, int icon) {
	String dir = GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE);
	if(groupname) {
		dir = AppendFileName(dir, groupname);
		RealizeDirectory(dir);
	}
	return CreateShellLink(exe, args, AppendFileName(dir, lnk), desc, fileicon, icon);
}

void InstallUninstall(const char *name, const char *dname, const char *cmdline){
	String path = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + String(dname);
	SetWinRegString(dname, "DisplayName", path);
	SetWinRegString((const char*)~String(name) + String(cmdline), "UninstallString", path);
}

String GetShellFolder(const char *name, HKEY type) {
	return GetWinRegString(name, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", type);
}

#include <winnls.h>
#include <winnetwk.h>

#define Ptr Ptr_
#include <wincon.h>
#include <shlobj.h>
#undef Ptr

bool CreateShellLink(const char *filepath, const char *args, const char *linkpath, const char *desc, const char *fileicon, int icon) {
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl);
	if(SUCCEEDED(hres)) {
		psl->SetPath(filepath);
		psl->SetArguments(args);
		psl->SetDescription(desc);
		if(icon >= 0) {
			if(strlen(fileicon)) psl->SetIconLocation(fileicon, icon);
			else psl->SetIconLocation(filepath, icon);
		}
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if (SUCCEEDED(hres)) {
			WCHAR szPath[_MAX_PATH] = { 0 };
			MultiByteToWideChar(CP_ACP, 0, linkpath, strlen(linkpath), szPath, _MAX_PATH);
			hres = ppf->Save(szPath, TRUE);
			ppf->Release();
		}
	}
	psl->Release();
	CoUninitialize();
	return SUCCEEDED(hres);
}

void DelKey(const char *dir, const char *key) {
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, key);
	RegCloseKey(hkey);
}
