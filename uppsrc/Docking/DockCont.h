class DockWindow;

struct ImgButton : public Pusher {
	private:
		const Value *look;
		Image img;
	public:		
		ImgButton()										{ Transparent(true); }
		virtual void Paint(Draw& w);
		virtual void MouseEnter(Point p, dword kflags) 	{ Refresh(); Pusher::MouseEnter(p, kflags); }
		virtual void MouseLeave() 						{ Refresh(); Pusher::MouseLeave(); }

		ImgButton& 	SetImage(const Image& _img) 		{ img = _img; Refresh(); return *this; }
		ImgButton& 	SetLook(const Value *_look) 		{ look = _look; Refresh(); return *this; }
};

class DockCont : public ToolWin {
public:
	typedef DockCont CLASSNAME;

	virtual void RightDown(Point p, dword keyflags) 	{ WindowMenu(); }
	
	virtual void Layout();
	virtual void ChildRemoved(Ctrl *child);
	virtual void ChildAdded(Ctrl *child);
	virtual bool Key(dword key, int count);


/*	virtual void ChildGotFocus() 						{ handle.RefreshFocus(true); TopWindow::ChildGotFocus(); }
	virtual void ChildLostFocus() 						{ handle.RefreshFocus(HasFocusDeep()); TopWindow::ChildLostFocus(); }
	virtual void GotFocus() 							{ handle.RefreshFocus(true); }
	virtual void LostFocus() 							{ handle.RefreshFocus(HasFocusDeep()); } */
public:
	enum DockState {
		STATE_NONE = -1,
		STATE_FLOATING,
		STATE_DOCKED, 
		STATE_AUTOHIDE,
		STATE_TABBED
	};

private:
	class DockContMenu : public DockMenu {
	public:
		typedef DockContMenu CLASSNAME;
		DockCont *cont;
		
		DockContMenu(DockWindow *dockwindow = NULL) : DockMenu(dockwindow) { };
		
		void ContainerMenu(Bar& bar, DockCont *c, bool withgroups  = true);
	private:		
		virtual void MenuDock(int align, DockableCtrl *dc);
		virtual void MenuFloat(DockableCtrl *dc);
		virtual void MenuAutoHide(int align, DockableCtrl *dc);	
		virtual void MenuClose(DockableCtrl *dc);	
	};	
	friend class DockContMenu;
		
	class DockContFrame : public CtrlFrame {
		virtual void FrameLayout(Rect& r)                   { r.Deflate(2); }
		virtual void FramePaint(Draw& w, const Rect& r) {
			DrawFrame(w, r, SColorShadow());
			DrawFrame(w, r.Deflated(1), SColorPaper);
		}
		virtual void FrameAddSize(Size& sz) { sz += 4; }
	};		
		
	struct DockContHandle : public FrameCtrl<Ctrl> {
		Callback WhenContext;
		Callback WhenLeftDrag;
		Callback WhenLeftDouble;
		DockContHandle() 										{ dc = NULL; /*focus = false;*/ }
		DockableCtrl *dc;
//		bool focus;
		virtual void FrameLayout(Rect& r);
		virtual void FrameAddSize(Size& sz);
		
		virtual void Paint(Draw& w);
		virtual void LeftDrag(Point p, dword keyflags)	{ /*RefreshFocus(true);*/ WhenLeftDrag(); }
		virtual void LeftDouble(Point p, dword keyflags) { /*RefreshFocus(true);*/ WhenLeftDouble(); }
		virtual void RightDown(Point p, dword keyflags) { /*RefreshFocus(true);*/ WhenContext(); }
		
		//void	RefreshFocus(bool _focus);
		int		GetHandleSize(const DockableCtrl::Style& s) const;
	};
	
	DockState		dockstate;	
	DockTabBar 		tabbar;
	DockContHandle 	handle;
	ImgButton 		close, autohide, windowpos;	
	Size 			usersize;
	bool 			waitsync:1;	
	bool			animating:1;
	DockWindow *	base;
	const DockableCtrl::Style *style;

	// Callbacks
	// Tab callbacks
	void 	TabSelected();
	void	TabDragged(int ix);
	void	TabContext(int ix);
	void	TabClosed0(Value v);								
	void	TabClosed(Value v);								
	void	TabsClosed(ValueArray vv);
	// Menus/Buttons
	void 	Float();
	void 	Dock(int align);
	void 	AutoHide();
	void 	AutoHideAlign(int align);
	void	RestoreCurrent();

	void 	CloseAll();

	DockableCtrl *Get0(int ix) const;
	DockableCtrl *GetCurrent0() const							{ return Get0(tabbar.GetCursor()); }
	
	void 	AddRemoveButton(Ctrl& c, bool state);
	bool 	IsDockAllowed0(int align, const Value& v) const;
	void	SyncButtons(DockableCtrl& dc);
	Ctrl   *FindFirstChild() const;
	void	ChildTitleChanged(Ctrl *child, WString title, Image icon);

	void	State(DockWindow& dock, DockCont::DockState state);	

	static Ctrl            *CtrlCast(const Value& v)  	{ return IsDockCont(v) ? (Ctrl *)ContCast(v) : (Ctrl *)DockCast(v); }
	static DockCont        *ContCast(const Value& v)  	{ return ValueTo<DockCont *>(v); } 
	static DockableCtrl    *DockCast(const Value& v)  	{ return ValueTo<DockableCtrl *>(v); }
	static bool			    IsDockCont(const Value& v) 	{ return IsType<DockCont *>(v); }
	Ctrl                   *GetCtrl(int ix) const 		{ return CtrlCast(tabbar.GetKey(ix)); }
	static Value 			ValueCast(DockableCtrl *dc) { return RawToValue<DockableCtrl *>(dc); }
	static Value 			ValueCast(DockCont *dc)  	{ return RawToValue<DockCont *>(dc); }
public:
	void 			SetCursor(int ix)					{ tabbar.SetCursor(ix); TabSelected(); }	
	void 			SetCursor(Ctrl& c);
	int 			GetCursor()	const					{ return tabbar.GetCursor(); }
	DockableCtrl& 	Get(int ix) const					{ return *Get0(ix); }
	DockableCtrl& 	GetCurrent() const					{ return Get(tabbar.GetCursor()); }
	void 			AddFrom(DockCont& cont, int except = -1);
	int				GetCount() const					{ return tabbar.GetCount(); }
	void 			Clear();	

	void 			SortTabs(bool b);
	void 			SortTabs(ValueOrder &sorter);
	void 			SortTabsOnce();
	void 			SortTabsOnce(ValueOrder &sorter);

	virtual void 	MoveBegin();
	virtual void 	Moving();
	virtual void 	MoveEnd();		
	virtual void 	WindowMenu();	
		
	void 			Animate(Rect target, int ticks, int interval = 0);
		
	bool 			IsDocked() const			{ return dockstate == STATE_DOCKED; }
	int				GetDockAlign() const;		
	int				GetAutoHideAlign() const;		
	bool			IsFloating() const			{ return dockstate == STATE_FLOATING; }
	bool 			IsAutoHide() const			{ return dockstate == STATE_AUTOHIDE; }	
	bool			IsTabbed() const			{ return dockstate == STATE_TABBED; }
	bool 			IsDockedAny() const 		{ return dockstate != STATE_FLOATING && dockstate != STATE_NONE; }
	bool			IsHidden() const			{ return dockstate == STATE_NONE; }	
	DockState		GetDockState() const		{ return dockstate; }
	
	void			StateNotDocked(DockWindow *dock = NULL) 	{ if (dock) base = dock; dockstate = STATE_NONE; }
	void			StateDocked(DockWindow& dock)				{ State(dock, STATE_DOCKED); }
	void 			StateFloating(DockWindow& dock)				{ State(dock, STATE_FLOATING); Title(GetTitle()); }
	void			StateAutoHide(DockWindow& dock)				{ State(dock, STATE_AUTOHIDE); Hide(); }
	void			StateTabbed(DockWindow& dock)				{ State(dock, STATE_TABBED); Hide(); }	
	
	void			SetAllDockerPos();
	
	DockWindow     *GetDockWindow() const			{ return base; }
	void			SyncButtons()					{ if (GetCount()) SyncButtons(GetCurrent()); }
	void			SyncTabs(int align, bool text);				
	void 			Lock(bool lock);
	void 			SyncFrames();
	void 			SyncFrames(bool hidehandle);
	void			SignalStateChange();	
	void			SignalStateChange(int from, int to);	
	
	void 			Grouping(bool grouping)			{ tabbar.Grouping(grouping); GroupRefresh(); }
	void			GroupRefresh();	
	void			GetGroups(Vector<String>& groups);
	
	void			WindowButtons(bool menu, bool hide, bool close);

	void			Highlight();
	Image 			GetHighlightImage();
	
	virtual Size	GetMinSize() const;
	virtual Size	GetMaxSize() const;
	virtual Size	GetStdSize() const;	
	Size			GetUserSize() const				{ return usersize; }
	void 			SetUserSize(Size sz)			{ usersize = sz; }
	void			SyncUserSize(bool h, bool v);		

	WString 		GetTitle(bool force_count = false) const; 
	void			ChildTitleChanged(DockableCtrl &dc);
	void			ChildTitleChanged(DockCont &dc);
	
	bool			IsDockAllowed(int align, int dc_ix = -1) const;
		
	virtual void 	Serialize(Stream& s);
	
	DockCont();		
};
