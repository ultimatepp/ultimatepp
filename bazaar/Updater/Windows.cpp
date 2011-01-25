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

#include <SysExec/SysExec.h>

NAMESPACE_UPP

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

// register the application for shell extensions handling
bool Updater::RegisterApplication(String const &appName, String const &appPath)
{
	SetWinRegString(appPath, NULL, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" + appName + ".exe", HKEY_LOCAL_MACHINE);
	return true;
}

// register the application for shell extensions handling
bool Updater::UnregisterApplication(String const &appName)
{
	DeleteWinReg("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" + appName + ".exe", HKEY_LOCAL_MACHINE);
	return true;
}

// register file associations
bool Updater::SetFileAssociation(String const &appName, String const &ext, Image const &icon)
{
	// strips eventually present "*." inside ext
	int dotPos = ext.Find('.');
	if(dotPos >= 0)
		dotPos++;
	else
		dotPos = 0;
	String ex = ext.Mid(dotPos);

	// get executable path
	String exePath = GetProgramsFolder() + "\\" + appName + "\\" + appName + ".exe";
	
	// get path for icon
	String iconPath = GetProgramsFolder() + "\\" + appName + "\\" + ex + ".ico";
	
	// if icon present, save it on file -- format is ext.ico
	if(!IsNull(icon))
	{
		Vector<Image>img;
		img << icon;
		String imgs = WriteIcon(img);
		SaveFile(iconPath, imgs);
	}
	
	// docs tells that association mechanics should be different between pre-Vista and post-Vista OS
	// The "normal" way works indeed for all of them, so we use it here;
	// I'll let this code here for reference, in case I shall change it later
	if(IsVistaOrLater() & false)
	{
	}
	// otherwise, old way
	else
	{
		SetWinRegString(ex + "Files", NULL, "." + ex, HKEY_CLASSES_ROOT);
		SetWinRegString(appName + " data files", NULL, ex + "Files", HKEY_CLASSES_ROOT);
		if(!IsNull(icon))
			SetWinRegString(iconPath, NULL, ex + "Files\\DefaultIcon", HKEY_CLASSES_ROOT);
		SetWinRegString("\"" + exePath + "\" \"%1\"", NULL, ex + "Files\\shell\\open\\command", HKEY_CLASSES_ROOT);
	}
	
	// update association icon cache
	if(IsXpOrLater())
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

	return true;
}

// remove file associations
bool Updater::RemoveFileAssociation(String const &ext)
{
	// strips eventually present "*." inside ext
	int dotPos = ext.Find('.');
	if(dotPos >= 0)
		dotPos++;
	else
		dotPos = 0;
	String ex = ext.Mid(dotPos);
	
	// no need to remove icon files, they get removed
	// with app program folder !

	// docs tells that association mechanics should be different between pre-Vista and post-Vista OS
	// The "normal" way works indeed for all of them, so we use it here;
	// I'll let this code here for reference, in case I shall change it later
	if(IsVistaOrLater() & false)
	{
	}
	// otherwise, old way
	else
	{
		DeleteWinReg("." + ex, HKEY_CLASSES_ROOT);
		DeleteWinReg(ex + "Files", HKEY_CLASSES_ROOT);
	}

	// update association icon cache
	if(IsXpOrLater())
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

	return true;
}

// links application to OS shell
// (i.e., add icon, menu entry, mimetype....)
bool Updater::ShellLink(void)
{
	bool success = true;
	
	String exePath = GetProgramsFolder() + "\\" + appName + "\\" + appName + ".exe";
	String iconPath = GetProgramsFolder() + "\\" + appName + "\\" + appName + ".ico";
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

	// registers application
	RegisterApplication(appName, exePath);
	
	// register file extensions for application
	for(int iExt = 0; iExt < extensions.GetCount(); iExt++)
		SetFileAssociation(appName, extensions[iExt], icon);

	return success;
}

// unlinks application
bool Updater::ShellUnlink(void)
{
	bool success = true;
	
	String linkName = appName + ".lnk";
	String iconPath = GetProgramsFolder() + "\\" + appName + "\\" + appName + ".ico";

	// unregister file extensions for application
	for(int iExt = 0; iExt < extensions.GetCount(); iExt++)
		RemoveFileAssociation(extensions[iExt]);

	// unregisters application
	UnregisterApplication(appName);

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

END_UPP_NAMESPACE

#endif
