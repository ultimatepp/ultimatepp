#ifdef PLATFORM_WIN32

#include <winreg.h>

#ifdef PLATFORM_WINCE
inline bool IsWinNT() { return false; }
inline bool IsWinXP() { return false; }
inline bool IsWin2K() { return false; }
inline bool IsWinVista() { return false; }
#else
inline bool IsWinNT() { return GetVersion() < 0x80000000; }
bool IsWin2K();
bool IsWinXP();
bool IsWinVista();
#endif

HINSTANCE AppGetHandle();
void      AppSetHandle(HINSTANCE dll_instance);

String AsString(const wchar_t *buffer);
String AsString(const wchar_t *buffer, int count);
String AsString(const wchar_t *buffer, const wchar_t *end);

String GetWinRegString(const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
int    GetWinRegInt(const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
bool   SetWinRegString(const String& string, const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
bool   SetWinRegExpandString(const String& string, const char *value, const char *path, HKEY base_key);
bool   SetWinRegInt(int data, const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
void   DeleteWinReg(const String& key, HKEY base = HKEY_LOCAL_MACHINE);

#ifndef PLATFORM_WINCE
String GetSystemDirectory();
String GetWindowsDirectory();
#endif
String GetModuleFileName(HINSTANCE instance = AppGetHandle());

//deprecated
class SyncObject {
protected:
	HANDLE     handle;

public:
	bool       Wait(int time_ms);
	bool       Wait();

	HANDLE     GetHandle() const { return handle; }

	SyncObject();
	~SyncObject();
};

//deprecated
class Event : public SyncObject {
public:
	void       Set();

	Event();
};

#endif
