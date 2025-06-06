enum {
	IDEXIT = 9999
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
	virtual void     SetMinSize(Size sz);
	virtual bool     IsCustomTitleBarDragArea(Point p);

public:
	struct TopStyle : ChStyle<TopStyle> {
		Value background;
	};

protected:
	enum {
		TIMEID_DEFSYNCTITLE = Ctrl::TIMEID_COUNT,
		TIMEID_COUNT,
	};

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
	
	int           exitcode = 0;

	void          PlaceFocus();
	void          ActiveFocus0(Ctrl& ctrl);
	Abreak       *FindAddAction(int ID);
	Abreak       *FindAction(int ID);

	Rect        overlapped;

	void        SyncSizeHints();
	void        SyncTitle();
	void        SyncCaption();

	void        SetupRect(Ctrl *owner);
	
	void        FixIcons();

	void        DefaultBreak();

	enum { MINIMIZED, MAXIMIZED, OVERLAPPED };

	WString     title;
	bool        minimizebox:1;
	bool        maximizebox:1;
	bool        noclosebox:1;
	bool        sizeable:1;
	bool        tool:1;
	bool        frameless:1;
	bool        urgent:1;
	bool        close_rejects:1;
	bool        custom_titlebar:1;
	byte        state;
	Image       icon, largeicon;

	int         custom_titlebar_cy = 0;
	int         active_titlebar_button = -1;
	bool        active_titlebar_active = false;

	const TopStyle *st;
	
	void        GuiPlatformConstruct();
	void        GuiPlatformDestruct();

#ifdef GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE
	#include GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE
#else
	GUIPLATFORM_TOPWINDOW_DECLS
#endif

	friend class Ctrl;

public:
	virtual     void ShutdownWindow();

	Event<>     WhenClose;

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

	void       Open(Ctrl *owner);
	void       Open();
	void       OpenMain();
	int        Run(bool appmodal = false);
	int        RunAppModal()                          { return Run(true); }
	int        Execute();
	bool       ExecuteOK()                            { return Execute() == IDOK;     }
	bool       ExecuteCancel()                        { return Execute() == IDCANCEL; }
	
	int        GetExitCode() const                    { return exitcode; }

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
	TopWindow& FullScreen(bool b = true);
	bool       IsFullScreen() const                   { return fullscreen; }
	TopWindow& FrameLess(bool b = true);
	bool       IsFrameLess() const                    { return frameless; }
	TopWindow& Urgent(bool b = true);
	bool       IsUrgent() const                       { return urgent; }
	TopWindow& NoAccessKeysDistribution()             { dokeys = false; return *this; }
	TopWindow& NoCloseBox(bool b = true)              { noclosebox = b; return *this; }
	TopWindow& CloseBoxRejects(bool b = true)         { close_rejects = b; return *this; }

	TopWindow& Icon(const Image& m);
	TopWindow& LargeIcon(const Image& m);
	TopWindow& Icon(const Image& smallicon, const Image& largeicon);
	
	TopWindow&  CustomTitleBar(int min_cy = 0);
	bool        IsCustomTitleBar() const;

	struct CustomTitleBarMetrics {
		int   lm;
		int   rm;
		int   height;
		Color background;
	};
	
	CustomTitleBarMetrics GetCustomTitleBarMetrics() const;

	static const TopStyle& StyleDefault();
	TopWindow&  SetStyle(const TopStyle& s);
	
	Image      GetIcon() const                        { return icon; }
	Image      GetLargeIcon() const                   { return largeicon; }

	void       SerializePlacement(Stream& s, bool reminimize = false);
	
	static void ShutdownWindows();

	typedef TopWindow CLASSNAME;

	TopWindow();
	~TopWindow();
};

void Maxisize(TopWindow& win, int screencxmax);

void ArrangeOKCancel(Ctrl& ok, Ctrl& cancel);

int  SwapOKCancel();
void SwapOKCancel_Write(int b);

void SetLayout_Size(Ctrl& ctrl, Size sz);
void SetLayout_Size(Ctrl& ctrl, int cx, int cy);

template <class L>
void InitLayout(Ctrl& ctrl, L& layout)
{
	InitLayout(ctrl, layout, layout, layout);
}

template <class T>
void CtrlLayout(T& ctrl) {
	InitLayout(ctrl, ctrl, ctrl, ctrl);
	Size sz = T::GetLayoutSize();
	SetLayout_Size(ctrl, sz);
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
