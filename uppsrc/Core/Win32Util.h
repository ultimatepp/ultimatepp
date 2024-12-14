#ifdef PLATFORM_WIN32

#include <winreg.h>

#ifdef PLATFORM_WINCE
inline bool IsWinNT()    { return false; }
inline bool IsWinXP()    { return false; }
inline bool IsWin2K()    { return false; }
inline bool IsWinVista() { return false; }
inline bool IsWin7()     { return false; }
#else
inline bool IsWinNT() { return GetVersion() < 0x80000000; }
bool IsWin2K();
bool IsWinXP();
bool IsWinVista();
bool IsWin7();
bool IsWin11();
#endif

HINSTANCE AppGetHandle();
void      AppSetHandle(HINSTANCE dll_instance);

String AsString(const wchar_t *buffer);
String AsString(const wchar_t *buffer, int count);
String AsString(const wchar_t *buffer, const wchar_t *end);

String GetWinRegString(const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
int    GetWinRegInt(const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
bool   SetWinRegString(const String& string, const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
bool   SetWinRegExpandString(const String& string, const char *value, const char *path, HKEY base_key, dword wow = 0);
bool   SetWinRegInt(int data, const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
void   DeleteWinReg(const String& key, HKEY base = HKEY_LOCAL_MACHINE, dword wow = 0);

void  *GetDllFn(const char *dll, const char *fn);

template <class T>
void   DllFn(T& x, const char *dll, const char *fn)
{
	x = (T)GetDllFn(dll, fn);
}

bool Win32CreateProcess(const char *command, const char *envptr, STARTUPINFOW& si, PROCESS_INFORMATION& pi, const char *cd);

#ifndef PLATFORM_WINCE
String GetSystemDirectory();
String GetWindowsDirectory();
#endif
String GetModuleFileName(HINSTANCE instance = AppGetHandle());

#ifdef DEPRECATED
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

class Win32Event : public SyncObject {
public:
	void       Set();

	Win32Event();
};
#endif

#endif
