private:
	bool         activex:1;
	bool         isdhctrl:1;

	static  bool GetMsg(MSG& msg);

	static  bool DumpMessage(Ctrl *w, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK UtilityProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void RenderFormat(int format);
	static void RenderAllFormats();
	static void DestroyClipboard();
	static void DoCancelPreedit();

	void UpdateDHCtrls();
	
	void UseImmersiveDarkModeForWindowBorder();

public:
	static Win32Event ExitLoopEvent;
	static bool       endsession;
	static void       EndSession();
	static HINSTANCE  hInstance;

protected:
	static HCURSOR   hCursor;
	
	static Point CurrentMousePos;
	
	friend Point GetMousePos();

	static VectorMap< HWND, Ptr<Ctrl> >& Windows();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static Win32Event OverwatchEndSession;
	static HWND       OverwatchHWND;
	static HANDLE     OverwatchThread;

	static LRESULT CALLBACK OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI Win32OverwatchThread(LPVOID);

	static Rect GetScreenClient(HWND hwnd);
	struct CreateBox;
	void  Create0(CreateBox *cr);
	void  Create(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow);
	Image DoMouse(int e, Point p, int zd = 0);

	void   PaintWinBarBackground(SystemDraw& w, const Rect& clip);
	void   PaintWinBar(SystemDraw& w, const Rect& clip);
	int    GetActiveTitleBarButton();
	static void   SyncCustomTitleBars();
	static int    GetWin32TitleBarHeight(const TopWindow *win);
	static int    GetWin32TitleBarButtonWidth();
	static Rect   GetTitleBarRect(const TopWindow *win);
	static bool   IsMaximized(HWND hwnd);

	static void sProcessMSG(MSG& msg);

	static  Vector<Event<> > hotkey;

	friend void sSetCursor(Ctrl *ctrl, const Image& m);
	
public:
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();
	virtual void    PreDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool    PreprocessMessage(MSG& msg);

	HWND  GetHWND() const                      { return GetParent() ? NULL : GetTop() ? GetTop()->hwnd : NULL; }
	HWND  GetOwnerHWND() const;

	static Ctrl  *CtrlFromHWND(HWND hwnd);

	Ctrl&   ActiveX(bool ax = true)            { activex = ax; return *this; }
	Ctrl&   NoActiveX()                        { return ActiveX(false); }
	bool    IsActiveX() const                  { return activex; }

	void   PopUpHWND(HWND hwnd, bool savebits = true, bool activate = true, bool dropshadow = false,
	                 bool topmost = false);

	static void InitWin32(HINSTANCE hinst);
	static void ExitWin32();
	static void GuiFlush()                              { ::GdiFlush(); }
