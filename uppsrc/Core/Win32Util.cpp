#include "Core.h"

namespace Upp {

#ifdef PLATFORM_WIN32

static HINSTANCE app_instance;

HINSTANCE AppGetHandle()
{
	if(!app_instance)
		app_instance = GetModuleHandle(NULL);
	return app_instance;
}

void AppSetHandle(HINSTANCE dll_instance) { app_instance = dll_instance; }

bool IsWin2K()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 5;
}

bool IsWinXP()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion > 5 || of.dwMajorVersion == 5 && of.dwMinorVersion >= 1;
}

bool IsWinVista()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 6;
}

bool IsWin7()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 6 && of.dwMinorVersion >= 1;
}

dword GetWindowsBuildVersion()
{
	LONG(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
    DllFn(RtlGetVersion, "ntdll", "RtlGetVersion");

	if(!RtlGetVersion)
		return 0;

	OSVERSIONINFOEXW osInfo;
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	RtlGetVersion(&osInfo);
	return osInfo.dwBuildNumber;
}

bool IsWin11()
{
	static bool b = GetWindowsBuildVersion() > 22000;
	return b;
}


String AsString(const wchar_t *buffer) {
	if(!buffer)
		return Null;
	return AsString(buffer, (int)wcslen(buffer));
}

String AsString(const wchar_t *buffer, int count) { // Convert with code page...
	if(!buffer)
		return Null;
	StringBuffer temp(count);
	for(char *p = temp, *e = p + count; p < e;)
		*p++ = (char)*buffer++;
	return String(temp);
}

String AsString(const wchar_t *buffer, const wchar_t *end) {
	if(!buffer)
		return Null;
	return AsString(buffer, (int)(end - buffer));
}

String GetWinRegString(const char *value, const char *path, HKEY base_key, dword wow) {
	HKEY key = 0;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ|wow, &key) != ERROR_SUCCESS)
		return String::GetVoid();
	dword type, len;
	if(RegQueryValueEx(key, value, 0, &type, NULL, &len) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return String::GetVoid();
	}
	StringBuffer raw_len(len);
	if(RegQueryValueEx(key, value, 0, 0, (byte *)~raw_len, &len) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return String::GetVoid();
	}
	if(len > 0 && (type == REG_SZ || type == REG_EXPAND_SZ))
		len--;
	raw_len.SetLength(len);
	RegCloseKey(key);
	return String(raw_len);
}

int GetWinRegInt(const char *value, const char *path, HKEY base_key, dword wow) {
	HKEY key = 0;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ|wow, &key) != ERROR_SUCCESS)
		return Null;
	int data;
	dword type, length = sizeof(data);
	if(RegQueryValueEx(key, value, 0, &type, (byte *)&data, &length) != ERROR_SUCCESS)
		data = Null;
	RegCloseKey(key);
	return data;
}

bool SetWinRegString(const String& string, const char *value, const char *path, HKEY base_key, dword wow) {
	HKEY key = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|wow, NULL, &key, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(key, value, 0,	REG_SZ, string, string.GetLength() + 1) == ERROR_SUCCESS);
	RegCloseKey(key);
	return ok;
}

bool SetWinRegExpandString(const String& string, const char *value, const char *path, HKEY base_key, dword wow) {
	HKEY key = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|wow, NULL, &key, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(key, value, 0,	REG_EXPAND_SZ, string, string.GetLength() + 1) == ERROR_SUCCESS);
	RegCloseKey(key);
	return ok;
}

bool SetWinRegInt(int data, const char *value, const char *path, HKEY base_key, dword wow)
{
	HKEY key = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS|wow, NULL, &key, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(key, value, 0, REG_DWORD, (const byte *)&data, sizeof(data)) == ERROR_SUCCESS);
	RegCloseKey(key);
	return ok;
}

void DeleteWinReg(const String& key, HKEY base, dword wow) {
	HKEY hkey;
	if(RegOpenKeyEx(base, key, 0, KEY_READ|wow, &hkey) != ERROR_SUCCESS)
		return;
	Vector<String> subkeys;
	char temp[_MAX_PATH];
	dword len;
	for(dword dw = 0; len = sizeof(temp), RegEnumKeyEx(hkey, dw, temp, &len, 0, 0, 0, 0) == ERROR_SUCCESS; dw++)
		subkeys.Add(temp);
	RegCloseKey(hkey);
	while(!subkeys.IsEmpty())
		DeleteWinReg(key + '\\' + subkeys.Pop(), base);

	static LONG (WINAPI *RegDeleteKeyEx)(HKEY, LPCTSTR, REGSAM, DWORD);
	DllFn(RegDeleteKeyEx, "Advapi32.dll", "RegDeleteKeyExA");

	if(wow && RegDeleteKeyEx)
		RegDeleteKeyEx(base, key, wow, 0);
	else
		RegDeleteKey(base, key);
}

String GetSystemDirectory() {
	WCHAR temp[MAX_PATH];
	*temp = 0;
	::GetSystemDirectoryW(temp, sizeof(temp));
	return FromSystemCharsetW(temp);
}

String GetWindowsDirectory() {
	WCHAR temp[MAX_PATH];
	*temp = 0;
	GetWindowsDirectoryW(temp, sizeof(temp));
	return FromSystemCharsetW(temp);
}

void *GetDllFn(const char *dll, const char *fn)
{
	if(HMODULE hDLL = LoadLibrary(dll))
		return (void *)GetProcAddress(hDLL, fn);
	return NULL;
}

String GetModuleFileName(HINSTANCE instance) {
	WCHAR h[_MAX_PATH];
	GetModuleFileNameW(instance, h, _MAX_PATH);
	return FromSystemCharsetW(h);
}

#ifdef DEPRECATED
bool SyncObject::Wait(int ms)
{
	return WaitForSingleObject(handle, ms);
}

bool SyncObject::Wait()
{
	return Wait(INFINITE);
}

SyncObject::SyncObject()
{
	handle = NULL;
}

SyncObject::~SyncObject()
{
	if(handle) CloseHandle(handle);
}

Win32Event::Win32Event()
{
	handle = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void Win32Event::Set()
{
	SetEvent(handle);
}
#endif

#endif

}
