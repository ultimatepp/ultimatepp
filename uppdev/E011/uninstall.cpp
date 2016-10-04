#include "e011.h"

#ifdef PLATFORM_WIN32

String GetShellFolder(const char *name, HKEY type) {
	return GetWinRegString(name, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", type);
}

void DelKey(const char *dir, const char *key) {
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, key);
	RegCloseKey(hkey);
}

void RemoveWindowsItems() {
	DeleteFolderDeep(AppendFileName(GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE), "\\Contalitri E011"));
	DeleteFile(AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), "Contalitri E011.lnk"));

	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\E011", "DisplayName");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\E011", "UninstallString");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", "E011");
}

void Uninstall() {
	SetLanguage(LNGFromText("IT-IT"));
	SetDefaultCharset(CHARSET_WIN1252);
	Draw::SetStdFont(ScreenSans(11));

	String path = GetExeFilePath();
	String bat = AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), "ERemove.bat");
	String dir = GetFileFolder(path);
	if(!PromptYesNo("[*3 Desideri veramente rimuovere il tool del Contalitri E011 dal computer ?&&]"
	                "Saranno rimossi tutti i file e le cartelle.")) return;
	DeleteFolderDeep(dir);
	RemoveWindowsItems();
	SaveFile(bat,
		":Repeat\r\n"
		"del \"" + path + "\"\r\n"
		"if exist \"" + path + "\" goto Repeat\r\n"
		"del \"" + GetExeDirFile("e011.log") + "\"\r\n"
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
	if(!CreateProcess(NULL, h, NULL, NULL, FALSE, IDLE_PRIORITY_CLASS, NULL, GetShellFolder("Desktop", HKEY_CURRENT_USER), &si, &pi))
		Exclamation("Rimozione incompleta ... Impossibile cancellare alcuni file.");
	else
		Exclamation("Rimozione terminata con successo.");
}

#else

void Uninstall() {}

#endif
