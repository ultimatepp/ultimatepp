$uvs: PENDING CONFLICT
#ifndef _Docking_Docking_h_
#define _Docking_Docking_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <Docking/DockableCtrl.h>
#include <Docking/DockCont.h>
#include <Docking/DockPane.h>
#include <Docking/DockMenu.h>

class DockWindow : public TopWindow
{
public:
	typedef DockWindow CLASSNAME;

	enum {
		DOCK_NONE	= -1,
		DOCK_LEFT 	= SplitterFrame::LEFT,
		DOCK_TOP 	= SplitterFrame::TOP,
		DOCK_RIGHT 	= SplitterFrame::RIGHT,
		DOCK_BOTTOM = SplitterFrame::BOTTOM,
	};

	virtual void State(int reason);
protected:
	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_ANIMATE,
		   TIMEID_ANIMATE_DELAY,
           TIMEID_COUNT };
	// DnD interface
	struct HighlightCtrl : public DockableCtrl
	{
$uvs: REPOSITORY INSERT
		HighlightCtrl() 		{ Transparent(false); }
		void ClearHighlight() 	{ img.Clear(); }
		void SetHighlight(const Value &hl, bool _isnested, dword _nestedkey, Image bg = Image());
		virtual void Paint(Draw &w);
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
		HighlightCtrl() { Transparent(false); }
		virtual void Paint(Draw &w);
		const Value *highlight;
		dword nestedkey;
		bool isnested;
$uvs: END REPOSITORY DELETE
		int oldframesize;
		Rect bounds;
$uvs: REPOSITORY INSERT
	private:
		Image img;
		dword nestedkey;
		bool isnested;
		const Value *highlight;
	};

protected:
	// Highlight control
	virtual HighlightCtrl &	GetHighlightCtrl()							{ return hlc; }
	void 			StartHighlight(DockCont *dcptr);
	void 			Highlight(int align, DockCont &cont, DockCont *target);
	void 			StopHighlight(bool do_animatehl);
	// Animation
	void			FloatAnimate(DockCont &dc, Rect target);
	// Called by containers to signal drag-drop events
	virtual void 	ContainerDragStart(DockCont &dc);
	virtual void 	ContainerDragMove(DockCont &dc);
	virtual void 	ContainerDragEnd(DockCont &dc);
	// Container docking/undocking
	void 			DockContainer(int align, DockCont &c, int pos = -1);
	void 			DockAsTab(DockCont &target, DockableCtrl &dc);
	void 			DockContainerAsTab(DockCont &target, DockCont &c, bool do_nested);
	void 			FloatContainer(DockCont &c, Point p = Null);
	void 			FloatFromTab(DockCont &c, DockableCtrl &tab)		{ FloatFromTab(c, *CreateContainer(tab)); }
	void 			FloatFromTab(DockCont &c, DockCont &tab);
	void 			AutoHideContainer(int align, DockCont &c);
	void 			CloseContainer(DockCont &c);
	DockCont *		TabifyGroup(String group);
	void 			Undock(DockCont &c);
	void 			Unfloat(DockCont &c);
	void 			Detach(DockCont &c)									{ Undock(c); Unfloat(c); }
	// For finding drag-drop targets and computing boundary rect
	DockCont *		GetMouseDockTarget();
	DockCont *		FindDockTarget(DockCont &dc, int &al);
	int 			FindDocker(DockableCtrl *dc);
	Rect 			GetAlignBounds(int al, Rect r, bool center, bool allow_lr = true, bool allow_tb = true);
	int 			GetPointAlign(const Point p, Rect r, bool center, bool allow_lr = true, bool allow_tb = true);
	int 			GetQuad(Point p, Rect r);
	// Helpers
	bool 			IsTL(int align) const								{ return align < 2; } //ie (align == DOCK_LEFT || align == DOCK_TOP)
	bool 			IsTB(int align) const								{ return align & 1; } //ie (align == DOCK_TOP || align == DOCK_BOTTOM)
	int  			GetDockAlign(const Ctrl &c) const;
	int	 			GetDockAlign(const Point &p) const;
	bool			IsFrameAnimating(int align) const		{ return frameanim[align].inc; }
	bool			IsPaneAnimating(int align) const		{ return dockpane[align].IsAnimating(); }
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	};
	virtual void ContainerDragStart(DockCont &dc);
	virtual void ContainerDragMove(DockCont &dc);
	virtual void ContainerDragEnd(DockCont &dc);

	virtual HighlightCtrl &GetHighlightCtrl()				{ return hlc; }
	void 	StartHighlight(DockCont *dcptr);
	void 	Highlight(int align, DockCont &cont, DockCont *target);
	void 	StopHighlight(bool do_animate);

	int  	GetDockAlign(const Ctrl &c) const;
	int	 	GetDockAlign(const Point &p) const;

	// Container docking/undocking
	void 	DockContainer(int align, DockCont &c, int pos = -1, int except = -1);
	void 	DockAsTab(DockCont &target, DockableCtrl &dc);
	void 	DockContainerAsTab(DockCont &target, DockCont &c, bool do_nested);

	void 	FloatContainer(DockCont &c, int except = -1, Point p = Null);
	void 	FloatFromTab(DockCont &c, DockableCtrl &tab)	{ FloatFromTab(c, *CreateContainer(tab)); }
	void 	FloatFromTab(DockCont &c, DockCont &tab);

	void 	AutoHideContainer(int align, DockCont &c, int except);
	void 	CloseContainer(DockCont &c);

	DockCont *TabifyGroup(String group);

	void 	Undock(DockCont &c);
	void 	Unfloat(DockCont &c);
	void 	Detach(DockCont &c)								{ Undock(c); Unfloat(c); }

	// Helpers
	bool 		IsTL(int align) const						{ return align < 2; } //ie (align == DOCK_LEFT || align == DOCK_TOP)
	bool 		IsTB(int align) const						{ return align & 1; } //ie (align == DOCK_TOP || align == DOCK_BOTTOM)
	DockCont *	GetMouseDockTarget();
	DockCont *	FindDockTarget(DockCont &dc, int &al);
	int 		FindDocker(DockableCtrl *dc);

	Rect 		GetAlignBounds(int al, Rect r, bool center);
	int 		GetPointAlign(const Point p, Rect r, bool center, bool allow_lr = true, bool allow_tb = true);
	int 		GetQuad(Point p, Rect r);
$uvs: END REPOSITORY DELETE

	friend class DockCont;

private:
$uvs: REPOSITORY DELETE
	Array<DockCont> 			conts;
	Vector<DockableCtrl *> 		dockers;
	ArrayMap<String, String> 	layouts;
	Array<DockableCtrl>			ctrls;
$uvs: END REPOSITORY DELETE
	bool init;
	bool tabbing;
	bool autohide;
$uvs: REPOSITORY INSERT
	bool animatehl;
	bool animatewnd;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	bool animate;
$uvs: END REPOSITORY DELETE
	bool nestedtabs;
	bool grouping;
	bool menubtn;
	bool closebtn;
	bool hidebtn;
	bool dockable[4];
$uvs: REPOSITORY INSERT
	dword nesttoggle;

	Array<DockCont> 			conts;
	Vector<DockableCtrl *> 		dockers;
	ArrayMap<String, String> 	layouts;
	Array<DockableCtrl>			ctrls;

	HighlightCtrl 				hlc;
	DockPane					dockpane[4];
	SplitterFrame				dockframe[4];
	AutoHideBar					hideframe[4];
	DockMenu					menu;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	HighlightCtrl 	hlc;
	DockPane		dockpane[4];
	SplitterFrame	dockframe[4];
	AutoHideBar		hideframe[4];
	DockMenu		menu;
$uvs: END REPOSITORY DELETE

	struct FrameAnim {
		int target;
		int inc;
	};
$uvs: REPOSITORY INSERT
	FrameAnim 					frameanim[4];
	int 						animdelay;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	int animdelay;
	FrameAnim frameanim[4];

$uvs: END REPOSITORY DELETE
public:
	void 		DockLayout();

	virtual void DockInit() { }

$uvs: REPOSITORY INSERT
	DockableCtrl &	Dockable(Ctrl &ctrl, WString title);
	DockableCtrl &	Dockable(Ctrl &ctrl, const char *title = 0)			{ return Dockable(ctrl, (WString)title); }

	void 			DockLeft(DockableCtrl &dc, int pos = -1)				{ Dock(DOCK_LEFT, dc, pos); }
	void 			DockTop(DockableCtrl &dc, int pos = -1)					{ Dock(DOCK_TOP, dc, pos); }
	void 			DockRight(DockableCtrl &dc, int pos = -1)				{ Dock(DOCK_RIGHT, dc, pos); }
	void 			DockBottom(DockableCtrl &dc, int pos = -1)				{ Dock(DOCK_BOTTOM, dc, pos); }
	void 			Dock(int align, DockableCtrl &dc, int pos = -1);
	void 			Tabify(DockableCtrl &target, DockableCtrl &dc);

	void 			Float(DockableCtrl &dc, Point p = Null);
	void 			Float(DockableCtrl &dc, const char *title, Point p = Null);

	void 			AutoHide(DockableCtrl &dc);
	void 			AutoHide(int align, DockableCtrl &dc);

	void 			Close(DockableCtrl &dc);

	void 			DockGroup(int align, String group, int pos = -1);
	void 			FloatGroup(String group);
	void 			AutoHideGroup(int align, String group);
	void 			TabDockGroup(int align, String group, int pos = -1);
	void 			TabFloatGroup(String group);
	void			CloseGroup(String group);

	bool 			IsDockVisible(int align) const	{ ASSERT(align >= 0 && align <= 4); return dockpane[align].IsVisible(); }
	void 			SetFrameSize(int align, int size);

	DockWindow &	AnimateDelay(int ms)			{ animdelay = max(ms, 0); return *this; }
	DockWindow &	Animate(bool highlight = true, bool window = true, int ticks = 10, int interval = 20);
	DockWindow &	NoAnimate()						{ return Animate(false); }
	bool			IsAnimated() const				{ return animatehl || animatewnd; }
	bool			IsAnimatedWindows() const		{ return animatehl; }
	bool			IsAnimatedHighlight() const		{ return animatewnd; }

	DockWindow &	Tabbing(bool _tabbing = true) 	{ tabbing = _tabbing; return *this; }
	DockWindow &	NoTabbing()						{ return Tabbing(false); }
	bool			IsTabbing() const				{ return tabbing; }

	DockWindow &	NestedTabs(bool _nestedtabs = true)	{ nestedtabs = _nestedtabs; return *this; }
	DockWindow &	NoNestedTabs()						{ return NestedTabs(false); }
	bool			IsNestedTabs() const				{ return nestedtabs; }

	DockWindow &	Grouping(bool grouping = true);
	DockWindow &	NoGrouping()						{ return Grouping(false); }
	bool			IsGrouping() const					{ return grouping; }

	DockWindow &	AllowDockAll();
	DockWindow &	AllowDockNone();
	DockWindow &	AllowDockLeft(bool v = true)	{ dockable[DOCK_LEFT] = v; return *this; }
	DockWindow &	AllowDockTop(bool v = true)		{ dockable[DOCK_TOP] = v; return *this; }
	DockWindow &	AllowDockRight(bool v = true)	{ dockable[DOCK_RIGHT] = v; return *this; }
	DockWindow &	AllowDockBottom(bool v = true)	{ dockable[DOCK_BOTTOM] = v; return *this; }
	DockWindow &	AllowDock(int a, bool v = true)	{ ASSERT(a >= 0 && a < 4); dockable[a] = v; return *this; }
	bool			IsDockAllowed(int align)		{ ASSERT(align >= 0 && align < 4); return dockable[align]; }
	bool			IsDockAllowed(int align, DockableCtrl &dc);

	DockWindow &	AutoHide(bool v = true);
	bool			IsAutoHide()					{ return autohide; }

	dword			NestedToggleKey()				{ return nesttoggle; }
	dword			SetNestedToggleKey(dword key) 	{ nesttoggle = key; }

	DockWindow &	WindowButtons(bool menu, bool hide, bool close);
	bool			HasMenuButtons() const			{ return menubtn; }
	bool			HasHideButtons() const			{ return hidebtn; }
	bool			HasCloseButtons() const			{ return closebtn; }

	DockableCtrl &	Register(DockableCtrl &dc);
	void		  	Deregister(DockableCtrl &dc);
	const Vector<DockableCtrl *> &GetDockableCtrls() const { return dockers; }

	void			DockManager();
	void			DockWindowMenu(Bar &bar);

	void			SerializeWindow(Stream &s);
	void 			SerializeLayout(Stream &s, bool withsavedlayouts = true);

	int 			SaveLayout(String name);
	void 			LoadLayout(int ix);
	void 			LoadLayout(String name)			{ LoadLayout(layouts.Find(name)); }
	void			DeleteLayout(String name)		{ layouts.RemoveKey(name); }
	String 			GetLayoutName(int ix) const 	{ return layouts.GetKey(ix); }
	int  			LayoutCount() const				{ return layouts.GetCount(); }
	const ArrayMap<String, String> & GetLayouts() const { return layouts; }

	void 			DisableFloating()				{ EnableFloating(false); }
	void 			EnableFloating(bool enable = true);

	void 			SetHighlightStyle(DockableCtrl::Style &s)	{ GetHighlightCtrl().SetStyle(s); }
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	DockableCtrl &Dockable(Ctrl &ctrl, WString title);
	DockableCtrl &Dockable(Ctrl &ctrl, const char *title = 0)			{ return Dockable(ctrl, (WString)title); }

	void 		DockLeft(DockableCtrl &dc, int pos = -1)				{ Dock(DOCK_LEFT, dc, pos); }
	void 		DockTop(DockableCtrl &dc, int pos = -1)					{ Dock(DOCK_TOP, dc, pos); }
	void 		DockRight(DockableCtrl &dc, int pos = -1)				{ Dock(DOCK_RIGHT, dc, pos); }
	void 		DockBottom(DockableCtrl &dc, int pos = -1)				{ Dock(DOCK_BOTTOM, dc, pos); }
	void 		Dock(int align, DockableCtrl &dc, int pos = -1);
	void 		DockAsTab(DockableCtrl &target, DockableCtrl &dc);

	void 		Float(DockableCtrl &dc, Point p = Null);
	void 		Float(DockableCtrl &dc, const char *title, Point p = Null);

	void 		AutoHide(DockableCtrl &dc);
	void 		AutoHide(int align, DockableCtrl &dc);

	void 		Close(DockableCtrl &dc);

	void 		DockGroup(int align, String group, int pos = -1);
	void 		FloatGroup(String group);
	void 		AutoHideGroup(int align, String group);
	void 		TabDockGroup(int align, String group, int pos = -1);
	void 		TabFloatGroup(String group);
	void		CloseGroup(String group);

	bool 		IsDockVisible(int align) const	{ ASSERT(align >= 0 && align <= 4); return dockpane[align].IsVisible(); }
	void 		SetFrameSize(int align, int size);

	DockWindow &AnimateDelay(int ms)			{ animdelay = max(ms, 0); }
	DockWindow &Animate(bool animate = true, int ticks = 10, int interval = 20);
	DockWindow &NoAnimate()						{ return Animate(false); }
	bool		IsAnimated() const				{ return animate; }

	DockWindow &Tabbing(bool _tabbing = true) 	{ tabbing = _tabbing; return *this; }
	DockWindow &NoTabbing()						{ return Tabbing(false); }
	bool		IsTabbing() const				{ return tabbing; }

	DockWindow &NestedTabs(bool _nestedtabs = true)	{ nestedtabs = _nestedtabs; return *this; }
	DockWindow &NoNestedTabs()						{ return NestedTabs(false); }
	bool		IsNestedTabs() const				{ return nestedtabs; }

	DockWindow &Grouping(bool grouping = true);
	DockWindow &NoGrouping()						{ return Grouping(false); }
	bool		IsGrouping() const					{ return grouping; }

	DockWindow &AllowDockAll();
	DockWindow &AllowDockNone();
	DockWindow &AllowDockLeft(bool v = true)	{ dockable[DOCK_LEFT] = v; return *this; }
	DockWindow &AllowDockTop(bool v = true)		{ dockable[DOCK_TOP] = v; return *this; }
	DockWindow &AllowDockRight(bool v = true)	{ dockable[DOCK_RIGHT] = v; return *this; }
	DockWindow &AllowDockBottom(bool v = true)	{ dockable[DOCK_BOTTOM] = v; return *this; }
	DockWindow &AllowDock(int a, bool v = true)	{ ASSERT(a >= 0 && a < 4); dockable[a] = v; return *this; }
	bool		IsDockAllowed(int align)		{ ASSERT(align >= 0 && align < 4); return dockable[align]; }
	bool		IsDockAllowed(int align, DockableCtrl &dc);

	DockWindow &AutoHide(bool v = true);
	bool		IsAutoHide()					{ return autohide; }

	dword		NestedToggleKey()				{ return K_CTRL | K_SHIFT; }

	DockWindow &WindowButtons(bool menu, bool hide, bool close);
	bool		HasMenuButtons() const			{ return menubtn; }
	bool		HasHideButtons() const			{ return hidebtn; }
	bool		HasCloseButtons() const			{ return closebtn; }

	DockableCtrl &	Register(DockableCtrl &dc);
	void			Deregister(DockableCtrl &dc);
	const Vector<DockableCtrl *> &GetDockableCtrls() const { return dockers; }

	void		DockManager();
	void		DockWindowMenu(Bar &bar);

	void		SerializeWindow(Stream &s);
	void 		SerializeLayout(Stream &s, bool withsavedlayouts = true);
	int 		SaveLayout(String name);
	void 		LoadLayout(int ix);
	void 		LoadLayout(String name)			{ LoadLayout(layouts.Find(name)); }
	void		DeleteLayout(String name)		{ layouts.RemoveKey(name); }
	String 		GetLayoutName(int ix) const 	{ return layouts.GetKey(ix); }
	int  		LayoutCount() const				{ return layouts.GetCount(); }
	const ArrayMap<String, String> & GetLayouts() const { return layouts; }

	void DisableFloating()						{ EnableFloating(false); }
	void EnableFloating(bool enable = true);

	void SetHighlightStyle(DockableCtrl::Style &s)	{ GetHighlightCtrl().SetStyle(s); }
$uvs: END REPOSITORY DELETE

	DockWindow();

private:
	// Container management
$uvs: REPOSITORY INSERT
	DockCont *		GetContainer(DockableCtrl &dc)		{ return dynamic_cast<DockCont *>(dc.GetParent()); }
	DockCont *  	CreateContainer();
	DockCont *		CreateContainer(DockableCtrl &dc);
	void			DestroyContainer(DockCont &c);
	DockCont * 		GetReleasedContainer(DockableCtrl &dc);

	void 			SyncContainer(DockCont &c);
	void 			SyncAll();

	// Helpers
	void 			Dock0(int align, Ctrl &c, int pos, bool do_animatehl = false, bool ishighlight = false);
	void 			Undock0(Ctrl &c, bool do_animatehl = false, int fsz = -1, bool ishighlight = false);

	void			StartFrameAnimate(int align, int targetsize);
	void			FrameAnimateTick();

	Size			CtrlBestSize(const Ctrl &c, bool restrict = true) const;

	void			ClearLayout();
};

$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	DockCont *	GetContainer(DockableCtrl &dc)		{ return dynamic_cast<DockCont *>(dc.GetParent()); }
	DockCont *  CreateContainer();
	DockCont *	CreateContainer(DockableCtrl &dc);
	void		DestroyContainer(DockCont &c);
	DockCont * 	GetReleasedContainer(DockableCtrl &dc);

	void 		SyncContainer(DockCont &c);
	void 		SyncAll();

	// Helpers
	void 		Dock0(int align, Ctrl &c, int pos, bool do_animate = false);
	void 		Undock0(Ctrl &c, bool do_animate = false, int fsz = -1);

	void		StartFrameAnimate(int align, int targetsize);
	void		FrameAnimateTick();
	bool		IsFrameAnimating(int align) const		{ return frameanim[align].inc; }

	Size		CtrlBestSize(const Ctrl &c, bool restrict = true) const;
};
/*
$uvs: END REPOSITORY DELETE
class PopUpDockWindow : public DockWindow
{
public:
	struct Style : ChStyle<Style> {
		Value inner[5];
		Value outer[4];
		Value highlight;
		int innersize;
		int outersize;
	};
	static const Style& StyleDefault();

protected:
	virtual void ContainerDragStart(DockCont &dc);
	virtual void ContainerDragMove(DockCont &dc);
	virtual void ContainerDragEnd(DockCont &dc);

private:
	struct PopUpButton : public Ctrl
	{
		const Value *icon;
		const Value *hlight;
		virtual void Paint(Draw &w)	{ Rect r = GetSize(); ChPaint(w, r, *icon); if (hlight) ChPaint(w, r, *hlight); }
		PopUpButton() { hlight = NULL; }
	};

	const Style * 	style;
	DockCont *		last_target;
	PopUpButton *	last_popup;
$uvs: REPOSITORY DELETE
	Point 			last_cp;
$uvs: END REPOSITORY DELETE
	PopUpButton 	inner[5];
	PopUpButton 	outer[4];

	int 	PopUpHighlight(PopUpButton *pb, int cnt);
	void 	ShowOuterPopUps(DockCont &dc);
	void 	ShowInnerPopUps(DockCont &dc, DockCont *target);
	void 	ShowPopUp(PopUpButton &pb, const Rect &r);
	void 	HidePopUps(bool _inner, bool _outer);
public:
	PopUpDockWindow &	SetStyle(const Style &s);
	PopUpDockWindow();
};
$uvs: REPOSITORY INSERT

$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
*/
$uvs: END REPOSITORY DELETE

#define LAYOUTFILE <Docking/Docking.lay>
#include <CtrlCore/lay.h>

class DockConfigDlg : public WithDockConfigLayout<TopWindow>
{
public:
	typedef DockConfigDlg CLASSNAME;

	DockConfigDlg(DockWindow &dockwindow);
private:
	DockWindow  &					dock;
	const Vector<DockableCtrl *> &	dockers;
	VectorMap<String, int> 			groups;
	DockMenu 						menu;
	String 							backup;
	int 							all;
	DockableCtrl *					highlight;

	void RefreshTree(bool dogroups = false);

	void OnTreeContext(Bar &bar);
	void OnSaveLayout();
	void OnLoadLayout();
	void OnDeleteLayout();
	void OnTreeCursor();
	void OnListCursor();
	void OnNewGroup();
	void OnDeleteGroup();
	void OnOK();
	void OnCancel();

	void OnTreeDrag();
	void OnTreeDrop(int parent, int ii, PasteClip& d);

	void	Highlight(DockableCtrl *dc);
	void 	StopHighlight();

$uvs: REPOSITORY INSERT
	void 	DeleteGroup(int id);

$uvs: END REPOSITORY INSERT
	String  DockerString(DockableCtrl *dc) const;
	String  PositionString(DockableCtrl *dc) const;
	int 	ListIndex() const;
	void 	DupePrompt(String type);
};

#endif

