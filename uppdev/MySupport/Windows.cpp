#include "support.h"

void DelKey(HKEY base_key, const char *dir, const char *key) {
	HKEY hkey;
	if(RegOpenKeyEx(base_key, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, key);
	RegCloseKey(hkey);
}

bool AssociateFileType(const String ext, const String desc, const String app, const String icnf) {
	
	SetWinRegString(GetFileTitle(app) + ext, "", ext, HKEY_CLASSES_ROOT);
	SetWinRegString(desc, "", GetFileTitle(app) + ext, HKEY_CLASSES_ROOT);
	SetWinRegString("\"" + app + "\" \"%1\"", "", GetFileTitle(app) + ext + String("\\Shell\\Open\\Command"), HKEY_CLASSES_ROOT);
	if(icnf.GetLength() > 0) SetWinRegString(icnf, "", GetFileTitle(app) + ext + String("\\DefaultIcon"), HKEY_CLASSES_ROOT);
	else SetWinRegString(app + ", 0", "", GetFileTitle(app) + ext + String("\\DefaultIcon"), HKEY_CLASSES_ROOT);
	
	String buff = GetExeFileTypeAssociation(ext);
	//buff = buff.Left(buff.Find(".exe")+5);
	if(buff == app) return true;
	else return false;
}

String GetExeFileTypeAssociation(const String ext) {

	String buff = GetWinRegString("", GetWinRegString("", ext, HKEY_CLASSES_ROOT) + String("\\Shell\\Open\\Command"), HKEY_CLASSES_ROOT);
	buff = buff.Left(buff.Find(".exe")+4);
	if(buff.Left(1) == "\"") return buff.Right(buff.GetCount() - 1);
	else return buff;
}
