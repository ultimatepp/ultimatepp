#ifdef PLATFORM_X11

enum {
	IDOK = 1,
	IDCANCEL = 2,
	IDABORT = 3,
	IDRETRY = 4,
	IDIGNORE = 5,
	IDYES = 6,
	IDNO = 7,
	IDCLOSE = 8,
	IDHELP = 9,
};

#endif

enum {
	IDEXIT = IDYES
};

class TopWindow : public Ctrl {
public:
	virtual Size     GetMinSize() const;
	virtual Size     GetStdSize() const;
	virtual void     Activate();
	virtual void     Deactivate();
	virtual bool     Accept();
	virtual void     Reject();
	virtual void     Paint(Draw& w);
	virtual bool     IsShowEnabled() const;
	virtual bool     Key(dword key, int count);
	virtual void     Close();
	virtual String   GetDesc() const;
	virtual void     ChildGotFocus();

#ifdef PLATFORM_WIN32
public:
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	dword       style;
	dword       exstyle;
	HICON       ico, lico;

	void        DeleteIco0();
	void        DeleteIco();
#endif

protected:
	enum {
		TIMEID_DEFSYNCTITLE = Ctrl::TIMEID_COUNT,
		TIMEID_COUNT,
	};

#ifdef PLATFORM_X11
	virtual void    EventProc(XWindow& w, XEvent *event);
private:
	XSizeHints *size_hints;
	XWMHints   *wm_hints;
	XClassHint *class_hint;
	Size        xminsize, xmaxsize;
	bool        topmost;
#endif

	static  Rect      windowFrameMargin;

private:
	struct Abreak : Pte<Abreak> {
		int         ID;
		TopWindow *dlg;

		void    Accept()      { dlg->AcceptBreak(ID); }
		void    Reject()      { dlg->RejectBreak(ID); }
		void    Break()       { dlg->Break(ID); }
	};

	Array<Abreak> action;
	Ptr<Ctrl>     activefocus;
	String        backup;
	PaintRect     background;
	Size          minsize;
	bool          dokeys;
	bool          fullscreen;

	byte          center:2;

	void          PlaceFocus();
	void          ActiveFocus0(Ctrl& ctrl);
	Abreak       *FindAddAction(int ID);
	Abreak       *FindAction(int ID);

#ifdef PLATFORM_WIN32
	void          CenterRect(HWND owner, int center);
#endif

#ifdef PLATFORM_X11
	void          CenterRect(Ctrl *owner);
	void          DefSyncTitle();
	void          EndIgnoreTakeFocus();
#endif

	Rect        overlapped;

	void        SyncTitle0();
	void        SyncSizeHints();
	void        SyncTitle();
	void        SyncCaption0();
	void        SyncCaption();

	void        SetupRect();
	
	void        FixIcons();

	enum { MINIMIZED, MAXIMIZED, OVERLAPPED };

	WString     title;
	bool        minimizebox:1;
	bool        maximizebox:1;
	bool        noclosebox:1;
	bool        sizeable:1;
	bool        tool:1;
	bool        frameless:1;
	byte        state;
	Image       icon, largeicon;
#ifdef PLATFORM_X11
	Image       invert;
	WString     title2;
#endif

public:
	Callback    WhenClose;

	void        Backup();
	void        Restore();

	void        Break(int ID = IDEXIT);
	bool        AcceptBreak(int ID);
	void        RejectBreak(int ID);

	void        WorkAreaTrim();

	Callback    Breaker(int ID = IDEXIT);
	Callback    Acceptor(int ID);
	Callback    Rejector(int ID);

	TopWindow&  Breaker(Ctrl& m, int ID = -1);
	TopWindow&  Acceptor(Ctrl& m, int ID);
	TopWindow&  Rejector(Ctrl& m, int ID);

	TopWindow&  NoCenter()                          { center = 0; return *this; }
	TopWindow&  CenterOwner()                       { center = 1; return *this; }
	TopWindow&  CenterScreen()                      { center = 2; return *this; }

	void        SetMinSize(Size sz)                 { minsize = sz; }

#ifdef PLATFORM_WIN32
	void       Open(HWND ownerhwnd);
	TopWindow& Style(dword _style);
	dword      GetStyle() const                       { return style; }
	TopWindow& ExStyle(dword _exstyle);
	dword      GetExStyle() const                     { return exstyle; }
#endif

	void       Open(Ctrl *owner);
	void       Open();
	void       OpenMain();
	int        Run(bool appmodal = false);
	int        RunAppModal()                          { return Run(true); }
	int        Execute();
	bool       ExecuteOK()                            { return Execute() == IDOK;     }
	bool       ExecuteCancel()                        { return Execute() == IDCANCEL; }

	void       Minimize(bool effect = false);
	void       Maximize(bool effect = false);
	void       Overlap(bool effect = false);

	bool       IsMaximized() const                    { return state == MAXIMIZED; }
	bool       IsMinimized() const                    { return state == MINIMIZED; }
	bool       IsOverlapped() const                   { return state == OVERLAPPED; }

	TopWindow& ActiveFocus(Ctrl& ctrl)                { ActiveFocus0(ctrl); return *this; }

	TopWindow& Title(const WString& _title);
	TopWindow& Title(const char *s);
	const WString& GetTitle() const                   { return title; }
	TopWindow& Sizeable(bool b = true);
	TopWindow& NoSizeable()                           { return Sizeable(false); }
	bool       IsSizeable() const                     { return sizeable; }
	TopWindow& MinimizeBox(bool b = true);
	TopWindow& MaximizeBox(bool b = true);
	TopWindow& Zoomable(bool b = true)                { MinimizeBox(b); return MaximizeBox(b); }
	TopWindow& NoZoomable()                           { return Zoomable(false); }
	bool       IsZoomable() const                     { return maximizebox; }
	TopWindow& Background(const PaintRect& prect);
	const PaintRect& GetBackground() const            { return background; }
	TopWindow& ToolWindow(bool b = true);
	TopWindow& NoToolWindow()                         { return ToolWindow(false); }
	bool       IsToolWindow() const                   { return tool; }
	TopWindow& TopMost(bool b = true, bool stay_top = true);
	TopWindow& NoTopMost()                            { return TopMost(false); }
	bool       IsTopMost() const;
	TopWindow& FullScreen(bool b = true)              { fullscreen = b; return *this; }
	bool       IsFullScreen() const                   { return fullscreen; }
	TopWindow& FrameLess(bool b = true)               { frameless = b; return *this; }
	bool       IsFrameLess() const                    { return frameless; }
	TopWindow& NoAccessKeysDistribution()             { dokeys = false; return *this; }
	TopWindow& NoCloseBox(bool b = true)              { noclosebox = b; return *this; }

	TopWindow& Icon(const Image& m);
	TopWindow& LargeIcon(const Image& m);
	TopWindow& Icon(const Image& smallicon, const Image& largeicon);
	
	Image      GetIcon() const                        { return icon; }
	Image      GetLargeIcon() const                   { return largeicon; }

	void       SerializePlacement(Stream& s, bool reminimize = false);

	typedef TopWindow CLASSNAME;

	TopWindow();
	~TopWindow();
};

void Maxisize(TopWindow& win, int screencxmax);

void ArrangeOKCancel(Ctrl& ok, Ctrl& cancel);

int  SwapOKCancel();
void SwapOKCancel_Write(int b);

template <class L>
void InitLayout(Ctrl& ctrl, L& layout)
{
	InitLayout(ctrl, layout, layout, layout);
}

template <class T>
void CtrlLayout(T& ctrl) {
	InitLayout(ctrl, ctrl, ctrl, ctrl);
	Size sz = ctrl.AddFrameSize(T::GetLayoutSize());
	ctrl.SetMinSize(sz);
	ctrl.SetRect(sz);
}

template <class T>
void CtrlLayout(T& ctrl, const char *title) {
	CtrlLayout(ctrl);
	ctrl.Title(title);
}

template <class T>
void CtrlLayoutOK(T& ctrl, const char *title) {
	CtrlLayout(ctrl, title);
	ctrl.Acceptor(ctrl.ok, IDOK);
	ctrl.ok.Ok();
}

template <class T>
void CtrlLayoutCancel(T& ctrl, const char *title) {
	CtrlLayout(ctrl, title);
	ctrl.Rejector(ctrl.cancel, IDCANCEL);
	ctrl.cancel.Cancel();
}

template <class T>
void CtrlLayoutOKCancel(T& ctrl, const char *title) {
	CtrlLayoutOK(ctrl, title);
	ctrl.Rejector(ctrl.cancel, IDCANCEL);
	ctrl.cancel.Cancel();
	ArrangeOKCancel(ctrl.ok, ctrl.cancel);
}

template <class T>
void CtrlLayoutExit(T& ctrl, const char *title) {
	CtrlLayout(ctrl, title);
	ctrl.Acceptor(ctrl.exit, IDEXIT);
	ctrl.exit.Exit();
}
