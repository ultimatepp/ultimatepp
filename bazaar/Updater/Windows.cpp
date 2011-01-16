#include "Updater.h"

#ifdef PLATFORM_WIN32


#define Ptr Ptr_
#define byte byte_
#define CY win32_CY_


#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>
#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ò

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

bool CreateShellLink(const char *filepath, const char *linkpath, const char *desc, int icon)
{
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl);
	if(SUCCEEDED(hres)) 
	{
		psl->SetPath(filepath);
		psl->SetDescription(desc);
		if(icon >= 0)
			psl->SetIconLocation(filepath, icon);
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if (SUCCEEDED(hres))
		{
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

// links application to OS shell
// (i.e., add icon, menu entry, mimetype....)
bool Updater::ShellLink(void)
{
	bool success = true;
	
	String exePath = GetProgramsFolder() + "/" + appName + "/" + appName + ".exe";
	String linkName = appName + ".lnk";

	// installs desktop icon
	if(desktopIcon)
	{
		String desktopPath = GetShellFolder("Desktop", HKEY_USERS);
		CreateShellLink(exePath, AppendFileName(desktopPath, linkName), comment, -1);
	}

	// installs program group
	// we could use "cathegory" to create a menu group... but I don't
	// know how internationalize it. On ubuntu is automatic.
	// maybe we could add it later
	String groupName = ""; // that could be GetLNGString(cathegory) ????
	String groupFolder = GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE);
	if(groupName != "")
	{
		groupFolder = AppendFileName(groupFolder, groupName);
		CreateDirectory(groupFolder, NULL);
	}
	CreateShellLink(exePath, AppendFileName(groupFolder, linkName), comment, -1);

	// installs uninstaller
	String uninstallRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName;
	SetWinRegString(appName, "DisplayName", uninstallRegPath);
	SetWinRegString(appName + " --UNINSTALL", "UninstallString", uninstallRegPath);

	return success;
}

// unlinks application
bool Updater::ShellUnlink(void)
{
	bool success = true;
	
	String linkName = appName + ".lnk";

	// remove uninstaller
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName, "DisplayName");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName, "UninstallString");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", appName);
	
	// remove program group
	String groupName = ""; // that could be GetLNGString(cathegory) ????
	String groupFolder = GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE);
	if(groupName != "")
	{
		groupFolder = AppendFileName(groupFolder, groupName);
		if(DirectoryExists(groupFolder))
			DeleteFolderDeep(groupFolder);
	}
	else
	{
		String linkPath = AppendFileName(groupFolder, linkName);
		if(FileExists(linkPath))
			FileDelete(linkPath);
	}
	
	// remove desktop icon
	String desktopLink = AppendFileName(GetShellFolder("Desktop", HKEY_USERS), linkName);
	if(FileExists(desktopLink))
		FileDelete(desktopLink);

	return success;
}

#endif
