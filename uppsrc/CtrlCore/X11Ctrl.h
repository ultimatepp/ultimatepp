	bool         ignoretakefocus:1;
protected:
	struct XWindow {
		Ptr<Ctrl>    ctrl;
		bool         exposed;
		Vector<Rect> invalid;
		Ptr<Ctrl>    owner;
		Ptr<Ctrl>    last_active;
		XIC          xic;
	};

private:
	static ArrayMap<Window, XWindow>& Xwindow();
	static int       Xbuttons;
	static int       Xbuttontime;
	static Point     Xbuttonpos;
	static Window    grabWindow, focusWindow;
	static Point     mousePos;
	static int       PopupGrab;
	static Ptr<Ctrl> popupWnd;
	static Index<String> sel_formats;
	static Ptr<Ctrl>     sel_ctrl;
	static void     ProcessEvent(XEvent *event);
	static void     TimerAndPaint();
	static void     ProcessEvent(XEvent& event);
	       void     Invalidate(XWindow& xw, const Rect& r);
	       void     DoPaint(const Vector<Rect>& invalid);
	       void     SetLastActive(XWindow *w, Ctrl *la);
	       XWindow *GetXWindow();
	static void     SyncMousePos();
	static void     ReleaseGrab();
	static Vector<Event<> > hotkey;
	static Vector<dword> modhot;
	static Vector<dword> keyhot;

	       void  StartPopupGrab();
	static void  EndPopupGrab();
	static void  SyncIMPosition();

	friend bool  GetMouseRight();
	friend bool  GetMouseLeft();
	friend bool  GetMouseMiddle();
	friend Point GetMousePos();
	friend void  sPanicMessageBox(const char *title, const char *text);

protected:
	       void   Create(Ctrl *owner, bool redirect, bool savebits);
	       void   SyncExpose();
	       void   TakeFocus();
	static Window GetXServerFocusWindow();
	       void   AddGlobalRepaint();
	static void   KillFocus(Window w);
	static void   FocusSync();

	       void DropEvent(XWindow& w, XEvent *event);
	static void DropStatusEvent(XEvent *event);
	static Index<String> drop_formats;
	static String  Unicode(const WString& w);
	static WString Unicode(const String& s);
	static bool   ClipHas(int type, const char *fmt);
	static String ClipGet(int type, const char *fmt);

	       XWindow *AddXWindow(Window &w);
	       void RemoveXWindow(Window &w);
	       XWindow *XWindowFromWindow(Window &w);

public:
	struct Xclipboard {
		Window win;

		VectorMap<int, ClipData> data;

		String Read(int fmt, int selection, int property);
		void   Write(int fmt, const ClipData& data);
		bool   IsAvailable(int fmt, const char *type);

		void   Clear()                     { data.Clear(); }
		void   Request(XSelectionRequestEvent *se);

		Xclipboard();
		~Xclipboard();
	};

	static Xclipboard& xclipboard();

	static int  Xeventtime;

	static XIM  xim;

	void DnD(Window src, bool paste);

	virtual void    EventProc(XWindow& w, XEvent *event);
	virtual bool    HookProc(XEvent *event);
	Window  GetWindow() const         { return utop ? utop->window : None; }
	static int    GetCaretBlinkTime()               { return 500; }
	static  Ctrl   *CtrlFromWindow(Window w);
	static bool    TrapX11Errors();
	static void    UntrapX11Errors(bool b);

	Window GetParentWindow(void) const;
	Ctrl *GetParentWindowCtrl(void) const;
	Rect GetRectInParentWindow(void) const;

	static void SyncNativeWindows(void);
public:
	static void InitX11(const char *display);
	static void ExitX11();
	static void GuiFlush()                              { XFlush(Xdisplay); }
