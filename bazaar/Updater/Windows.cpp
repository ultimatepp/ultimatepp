#include "Updater.h"

#ifdef PLATFORM_WIN32

#include <plugin/bmp/bmp.h>

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

String Updater::GetShellFolder(const char *name, HKEY type)
{
	return GetWinRegString(name, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", type);
}

void Updater::DelKey(const char *dir, const char *key)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, key);
	RegCloseKey(hkey);
}

bool Updater::CreateShellLink(const char *filepath, const char *linkpath, const char *desc, const char *iconPath)
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
		if(iconPath)
			psl->SetIconLocation(iconPath, 0);
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

HRESULT SetContosoAsDefaultForDotHTM()
{
    IApplicationAssociationRegistration* pAAR;

    HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration,
                                  NULL,
                                  CLSCTX_INPROC,
                                  __uuidof(IApplicationAssociationRegistration),
                                  (void**)&pAAR);
    if (SUCCEEDED(hr))
    {
        hr = pAAR->SetAppAsDefault(L"Contoso.WebBrowser.1.06",
                                   L".htm",
                                   AT_FILEEXTENSION);

        pAAR->Release();
    }

    return hr;
}

bool Updater::SetFileAssociations(String const &ext, Image const &icon, String const &exePath)
{
	return true;
}

// links application to OS shell
// (i.e., add icon, menu entry, mimetype....)
bool Updater::ShellLink(void)
{
	bool success = true;
	
	String exePath = GetProgramsFolder() + "/" + appName + "/" + appName + ".exe";
	String iconPath = GetProgramsFolder() + "/" + appName + "/" + appName + ".ico";
	String linkName = appName + ".lnk";

	// creates the needed icon in executable folder
	if(!IsNull(icon))
	{
		Vector<Image>img;
		img << icon;
		String imgs = WriteIcon(img);
		SaveFile(iconPath, imgs);
	}

	// installs desktop icon
	if(desktopIcon)
	{
		String desktopPath = GetShellFolder("Desktop", HKEY_CURRENT_USER);
		CreateShellLink(exePath, AppendFileName(desktopPath, linkName), comment, iconPath);
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
	CreateShellLink(exePath, AppendFileName(groupFolder, linkName), comment, iconPath);

	// installs uninstaller
	String uninstallRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName;
	SetWinRegString(appName, "DisplayName", uninstallRegPath);
	SetWinRegString("\"" + exePath + "\" --UNINSTALL", "UninstallString", uninstallRegPath);

	return success;
}

// unlinks application
bool Updater::ShellUnlink(void)
{
	bool success = true;
	
	String linkName = appName + ".lnk";
	String iconPath = GetProgramsFolder() + "/" + appName + "/" + appName + ".ico";

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
	String desktopLink = AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), linkName);
	if(FileExists(desktopLink))
		FileDelete(desktopLink);

	// deletes the icon
	if(FileExists(iconPath))
		FileDelete(iconPath);

	return success;
}

#endif
