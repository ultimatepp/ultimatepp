class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();

protected:
	HWND   hwnd;
};


Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

#ifdef PLATFORM_WINCE

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow) \
{ \
	UPP::Ctrl::InitWin32(hInstance); \
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log..."); \
	UPP::DeleteUsrLog(); \
	UPP::Ctrl::ExitWin32(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

#else

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	UPP::Ctrl::InitWin32(hInstance); \
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log..."); \
	UPP::DeleteUsrLog(); \
	UPP::Ctrl::ExitWin32(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

#define DLL_APP_MAIN \
void _DllMainAppInit(); \
\
BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpReserved) \
{ \
	if(fdwReason == DLL_PROCESS_ATTACH) { \
		Ctrl::InitWin32(AppGetHandle()); \
		AppInitEnvironment__(); \
		_DllMainAppInit(); \
	} \
	else \
	if(fdwReason == DLL_PROCESS_DETACH) { \
		Ctrl::ExitWin32(); \
	} \
	return true; \
} \
\
void _DllMainAppInit()

#endif

#ifndef PLATFORM_WINCE

class DHCtrl : public Ctrl {
public:
	virtual void    State(int reason);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();

private:
	void OpenHWND();
	void SyncHWND();
	
protected:
	void CloseHWND();
	HWND   hwnd;

public:
	HWND    GetHWND()                              { return hwnd; }
//	void    Refresh()                              { InvalidateRect(GetHWND(), NULL, false); }

	DHCtrl();
	~DHCtrl();
};

#endif

