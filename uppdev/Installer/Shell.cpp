#include <Installer/install.h>

#include <winnls.h>
#include <winnetwk.h>

#define Ptr Ptr_
#include <wincon.h>
#include <shlobj.h>
#undef Ptr

bool CreateShellLink(const char *filepath, const char *linkpath, const char *desc, int icon)
{
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	hres = CoInitialize(NULL);
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
			MultiByteToWideChar(CP_ACP, 0, linkpath, strlen(linkpath), szPath, _MAX_PATH);
			hres = ppf->Save(szPath, TRUE);
			ppf->Release();
		}
	}
	psl->Release();
	CoUninitialize();
	return SUCCEEDED(hres);
}

String GetShellFolder(const char *name, HKEY type)
{
	return GetWinRegString(name,
	         "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		     type);
}

bool InstallDesktopIcon(const char *exe, const char *lnk, const char *desc)
{
	return CreateShellLink(exe,
	                       AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), lnk),
	                       desc, -1);
}

bool InstallProgramGroup(const char *exe, const char *groupname, const char *lnk, int icon)
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
