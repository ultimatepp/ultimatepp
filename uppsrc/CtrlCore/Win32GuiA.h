class TopFrameDraw : public SystemDraw {
public:
	TopFrameDraw(Ctrl *ctrl, const Rect& r);
	~TopFrameDraw();

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
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	UPP::Ctrl::InitWin32(hInstance); \
	try { \
		GuiMainFn_(); \
	} \
	UPP::Ctrl::ExitWin32(); \
	UPP::Ctrl::ShutdownThreads(); \
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
	UPP::AppInitEnvironment__(); \
	GUI_APP_MAIN_HOOK \
	UPP::Ctrl::InitWin32(hInstance); \
	UPP::AppExecute__(GuiMainFn_); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::Ctrl::ExitWin32(); \
	UPP::Ctrl::ShutdownThreads(); \
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
		AppInitEnvironment__(); \
		Ctrl::InitWin32(AppGetHandle()); \
		UPP::AppExecute__(_DllMainAppInit); \
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
	virtual bool    PreprocessMessage(MSG& msg);

private:
	void OpenHWND();
	void SyncHWND();
	
	void RemoveActive();

	static Vector<DHCtrl *> all_active;

	static bool PreprocessMessageAll(MSG& msg);
	
	Rect current_pos = Null;
	int  current_visible = Null;

	friend class Ctrl;

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

