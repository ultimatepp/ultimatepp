//$ class Ctrl {
private:
	bool         activex:1;
	bool         isdhctrl:1;

	static void WndDestroyCaret();
	void WndCreateCaret0(const Rect& cr);
	void WndCreateCaret(const Rect& cr);

	static  bool PeekMsg(MSG& msg);

	static LRESULT CALLBACK UtilityProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void RenderFormat(int format);
	static void RenderAllFormats();
	static void DestroyClipboard();

public:
	static Event     ExitLoopEvent;
	static void      EndSession()              { endSession = true; }
	static bool      IsEndSession()            { return endSession; }
	static HINSTANCE hInstance;

protected:
	static bool      endSession;
	static HCURSOR   hCursor;

	static VectorMap< HWND, Ptr<Ctrl> >& Windows();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static Event  OverwatchEndSession;
	static HWND   OverwatchHWND;
	static HANDLE OverwatchThread;

	static LRESULT CALLBACK OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI Win32OverwatchThread(LPVOID);

	static Rect GetScreenClient(HWND hwnd);
	struct CreateBox;
	void  Create0(CreateBox *cr);
	void  Create(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow);
	Image DoMouse(int e, Point p, int zd = 0);
	static void sProcessMSG(MSG& msg);

	static  Vector<Callback> hotkey;

	friend void sSetCursor(Ctrl *ctrl, const Image& m);
	
public:
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();
	virtual void    PreDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	HWND  GetHWND() const              { return parent ? NULL : top ? top->hwnd : NULL; }
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
//$ };
