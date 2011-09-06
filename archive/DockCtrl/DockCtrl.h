//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ
// Version:	 0.52 (DEV-0803b.2)
//==============================================================================================

#ifndef DOCKCTRL_H
#define DOCKCTRL_H

#include <CtrlLib/CtrlLib.h>
#include <Draw/Draw.h>

using namespace UPP;

#define LAYOUTFILE <DockCtrl/DockCtrl.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS DockCtrlImgs
#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#include <Draw/iml_header.h>

#define TFILE <DockCtrl/DockCtrl.t>
#include <Core/t.h>

#include <CtrlLib/Ch.h>

class PaneSplitter;
class TabInterface;
class AutoHideBar;
class PaneFrame;
class DockBase;
class DockCtrl;
class DockWindow;
class TabWindow;
class DockableCtrl;

//----------------------------------------------------------------------------------------------
// DockCtrl: Chameleon Support.
//
struct DockCtrlChStyle : Moveable<DockCtrlChStyle>
{
	struct Style : ChStyle<Style>
	{
		Font 	font;
		int 	bartitlesize;
		int		barheight;
		Rect	barmargins;
		Value	barbackground[1];
		Value	barshut[3];
		Value	barhide[3];
		Value	barmenu[3];
		int		tabheight;
		int		tabmargin;
		int		tabextendleft;
		Rect	tabsel;
		Rect	tabedge;
		Value	tabnormal[4];
		Value	tabfirst[4];
		Value	tablast[4];
		Value	tabboth[4];
		Value	hidebutton[4];	
		Image	stdhighlight;
		Image	tabhighlight;
	};
	const Style* style;
	static const Style& StyleDefault();
	static const Style& StyleClassic();
	static const Style& StyleEnhanced();
};

static void DockCtrlImgsLook(Value* look, int i, int n)
{
	while(n--)
		*look++ = DockCtrlImgs::Get(i++);
}

static Image StandardHighlight(Color inside, Color border)
{
	Size sz(5, 5);
	ImageBuffer ib(sz);
	RGBA *q = ~ib;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			*(q++) = (i == 4 || j == 4 || !i || !j) ? border : inside;
	ib.SetHotSpot(Point(1, 1));
	ib.Set2ndSpot(Point(3, 3));
	return ib;
}

static Image AlphaHighlight(const Image &img, int alpha)
{
	ImageDraw draw(img.GetSize());
	draw.Alpha().DrawRect(img.GetSize(), Color(alpha, alpha, alpha));
	draw.DrawImage(0, 0, img);
	// MingW (5.1.3) fix - Damn, I hate that ever-complaining compiler.
	Image dimg = draw;
	// Is there a better way to set hotspots than this?
	ImageBuffer ib(dimg);
	ib.SetHotSpot(Point(1, 1));
	ib.Set2ndSpot(Point(3, 3));
	return ib;
}

//----------------------------------------------------------------------------------------------

struct DockCtrlCustomFrame : FrameCtrl<Ctrl>
{
public:
	enum 	{ LAYOUT_LEFT, LAYOUT_TOP, LAYOUT_RIGHT, LAYOUT_BOTTOM };
	enum 	{ TAB_MARGIN = 6, TAB_SPACE = 20, TAB_SBHEIGHT = 4, TAB_SBSEPARATOR = 1, TAB_FILEICON = 16, TAB_SPACEICON = 5 };
	enum 	{ BAR_MARGIN = 6, BAR_SPACE = 20, BAR_SBHEIGHT = 4, BAR_SBSEPARATOR = 1, BAR_FILEICON = 16, BAR_SPACEICON = 5 };
		
	void	SetLayout(int l)	{ layout = l; RefreshLayout();			}
	void	Fix(Size& sz)		{ if(!horizontal) Swap(sz.cx, sz.cy); 	}
	void	Fix(Point& p)		{ if(!horizontal) Swap(p.x, p.y);		}
	void	SetSize(int sz)		{ size = sz;							}
	bool	IsHorz()			{ return layout == LAYOUT_TOP || layout == LAYOUT_BOTTOM; }
	bool	IsVert()			{ return !IsHorz(); }
	
	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);

	DockCtrlCustomFrame& 			SetStyle(const DockCtrlChStyle::Style& s);
	const DockCtrlChStyle::Style*  	GetStyle();	

	int		layout;
	int 	size;
	bool	horizontal;
	const 	DockCtrlChStyle::Style* style;
	DockCtrlCustomFrame() : layout(LAYOUT_TOP), size(0), horizontal(true) {}	

};

class TabInterface : public DockCtrlCustomFrame
{
private:


	class TabScrollBar : public DockCtrlCustomFrame
	{
	private:
		int total;
		double pos, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		void UpdatePos(bool update = true);
		bool ready;
		Size sz;
	
	public:
		TabScrollBar();

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		virtual void Layout();

		int  GetPos() const;
		void SetPos(int p, bool dontscale = false);
		void AddPos(int p, bool dontscale = false);
		int  GetTotal() const;
		void AddTotal(int t);
		void SetTotal(int t);
		void GoEnd();
		void GoBegin();
		void Set(const TabScrollBar& t);
		TabScrollBar& SetLayout(int l);
		Callback WhenScroll;
	};


	
protected:
	void OnScrollBar() { Refresh(); };
	int  GetPos() { return scrollbar.GetPos(); }
	Point mouse, oldp;
	TabScrollBar scrollbar; 
	
public:
	struct Tab : Moveable<Tab>
	{
		DockableCtrl* dock;
		bool 	active, visible, highlighted;
		int		id;
		int 	x, y, cx, cy;
		Size 	textsize;
		String	name;
		Image	icon;
		int		Right()	 { return x + cx; };
		int		Bottom() { return y + cy; };
		bool	IsActive() { return active; }
		bool	IsVisible()	{ return visible; }
		bool	IsHighligted()	{ return highlighted; }
		bool	HasMouse(const Point& p)	{ return visible && Rect(x, y, x + cx, y + cy).Contains(p);}
		bool	HasMouseButton(const Point& p, int w, int h) 
		{
			if(!visible) return false;
			Size isz = DockCtrlImgs::DClose().GetSize();
			int iy = (h - isz.cy) / 2;
			int ix = x + cx - isz.cx - w - TAB_MARGIN;
			return p.x >= ix && p.x < ix + isz.cx && p.y >= iy && p.y < iy + isz.cy;
		}
		Tab() : id(-1), active(false), visible(false), highlighted(false) { dock = NULL; }
		~Tab() { if(dock) dock = NULL; }
	};
	
protected:
	Vector<Tab> tabs;
	int id;
	int active, highlight, visible;
	int fileicon, tabbutton;
	bool hastabbutton, hasfileicon, hasscrollbar, isdraggable;
	int target, source;
	bool isctrl;
	bool isdragged;
	String	internalname; // not to confuse with different types of ctrls (tabs X autohidebars)
	
public:
	const TabCtrl::Style *style;
	TabInterface& SetStyle(const TabCtrl::Style& s)  { style = &s; Refresh(); return *this; }
	static const TabCtrl::Style& StyleDefault();

public:
	void Add(DockableCtrl& dock, bool activate = false);
	void Add(String name, bool activate = false);
	void Remove(DockableCtrl& dock);
	void Remove(int n);
	void Close(int n);
	void CloseAll();
	
	void SetActiveTab(int n);
	void SetActiveTab(DockableCtrl& dock);
	void SetActiveTabTitle(String name, Image icon);
	int  GetActiveTab();
	int  Find(int itemid);
	int	 Find(DockableCtrl& dock);
	int  GetNext(int n);
	int  GetPrev(int n);
	virtual void ReposTabs();
	virtual void DrawTab(Draw& w, Size &sz, int i);
	void DrawLabel(Draw& w, int x, int y, Tab& t, bool isactive);	

	Vector<Tab>& GetTabs()	{return tabs; } 
	
	int GetWidth(int n);
	int GetWidth();
	int GetHeight();
	
	int GetNextId() { return ++id; }
	
	TabInterface& HasButtons(bool b) 		{ hastabbutton = b; ReposTabs(); if(IsOpen() && IsChild()) Refresh(); return *this; }
	TabInterface& HasIcons(bool b)	 		{ hasfileicon  = b;	ReposTabs(); if(IsOpen() && IsChild()) Refresh(); return *this; }
	TabInterface& HasScrollBar(bool b);	
	TabInterface& Draggable(bool b = true)	{ isdraggable = b; return *this; }
		
	TabInterface& SetLayout(int l);
		
protected:
	virtual void Paint(Draw& d);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void LeftDrag(Point p, dword keyflags);
	virtual void MiddleDrag(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void RightUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void MouseLeave();
	virtual void DragAndDrop(Point p, PasteClip& d);

	virtual void DragEnter();
	virtual void DragLeave();
	virtual void DragRepeat(Point p);
	virtual void CancelMode();

	int GetTargetTab(Point p);
	int	GetSourceTab(Point p);

public:
	Callback2<int, DockableCtrl&> WhenContext;
	Callback2<int, DockableCtrl&> WhenClose;
	Callback2<int, DockableCtrl&> WhenDrag;
	Callback2<int, DockableCtrl&> WhenActive;
	Callback WhenHighlight;
	Callback WhenSelect;

public:
	typedef TabInterface CLASSNAME;
	
	TabInterface();
	~TabInterface();
};

//----------------------------------------------------------------------------------------------

class PaneSplitter : public Splitter
{
public:
	typedef PaneSplitter CLASSNAME;
	
	enum {PANEANIMATION, TABANIMATION };
	
	void 			AddChildDock(DockableCtrl& dock, int position);
	void 			AddChildDock(DockableCtrl& dock);
	void 			RemoveChildDock(DockableCtrl& dock);
	void 			RemoveChildDock(int position);
	void 			RemoveChilds();
	void 			ReposChilds();
	DockableCtrl* 	GetChildAt(int position);
	int  			GetChildCount();
	bool 			HasChild();					

	void 			StartAnimation(DockableCtrl& dock, int position);
	void 			StartAnimation(int position);
	void 			StopAnimation();
	inline bool 	IsAnimating() const				{ return animating; }
	void 			Animate(int position = 0);
	void 			SetAnimationType(int type)		{ animationctrl.Type(animationtype = type); }
	Rect 			AnimationArea() 				{ return animationctrl.GetScreenRect();}
	Ctrl* 			GetAnimationCtrl() 				{ return (Ctrl*) &animationctrl; }		

	virtual void 	Layout();

	
private:
	Array<int> 		positions;
	void 			AnimateCallback();

public:
	class AnimationCtrl : public Ctrl
	{
	public:
		typedef 	AnimationCtrl CLASSNAME;
		void 		SetAnimImage(Image& img)		{ image = img;  Refresh(); }
		Image& 		GetAnimImage()					{ return image;  }
		void 		ClearAnimImage()				{ image.Clear(); }
		void 		AnimateVert();
		void 		AnimateHorz();
		void 		Paint(Draw& d);
		void 		Type(int type)					{ ctrltype = type; }
	public:
		AnimationCtrl()  { /*SetFrame(FieldFrame()); */ctrltype = PaneSplitter::PANEANIMATION; }

	private:
		Image 		image;
		int 		ctrltype;
	};
	AnimationCtrl animationctrl;
private:
	bool animating;
	int  animationtype;
	int pos, x, y;

public:
	PaneSplitter();
	~PaneSplitter();
};

//----------------------------------------------------------------------------------------------

class DockableCtrl : public TopWindow
{
public:
	typedef DockableCtrl CLASSNAME;
	
	enum { TYPE_DOCKWINDOW, TYPE_TABWINDOW, TYPE_DOCKBAR };										// Available DockableCtrl types.
	enum { DOCK_LEFT, DOCK_TOP, DOCK_RIGHT, DOCK_BOTTOM, DOCK_TABBED, DOCK_NONE };				// Possible alignments.
	enum { STATE_SHUT, STATE_SHOW, STATE_HIDE, STATE_AUTO, STATE_TABBED			};				// Possible states.
	
	inline bool		IsDockWindow()				{ return docktype == TYPE_DOCKWINDOW; 	}		
	inline bool		IsTabWindow()				{ return docktype == TYPE_TABWINDOW;	}		
	inline bool		IsDockBar()					{ return docktype == TYPE_DOCKBAR;		}		

	DockableCtrl& 	DockTop()					{ dockalignment = DOCK_TOP;		return *this;}
	DockableCtrl& 	DockLeft()					{ dockalignment = DOCK_LEFT;	return *this;}			
	DockableCtrl& 	DockRight()					{ dockalignment = DOCK_RIGHT;	return *this;}
	DockableCtrl& 	DockBottom()				{ dockalignment = DOCK_BOTTOM;	return *this;}
	DockableCtrl&	DockFloat()					{ dockalignment	= DOCK_NONE; dockstate= STATE_SHOW; return *this; }

	DockableCtrl& 	StateShut()					{ dockstate =  STATE_SHUT;		return *this;}
	DockableCtrl& 	StateShow()					{ dockstate =  STATE_SHOW;		return *this;}
	DockableCtrl& 	StateHide()					{ dockstate =  STATE_HIDE;		return *this;}
	DockableCtrl& 	StateAuto()					{ dockstate =  STATE_AUTO;		return *this;}
	DockableCtrl& 	StateTabbed()				{ dockstate =  STATE_TABBED;	return *this;}

	DockableCtrl& 	Posit(int position)			{ dockposition = position;		return *this;}

	DockableCtrl& 	DockingStyle(int alignment, int state, int position = 0);
	
	inline int 		Alignment() const			{ return dockalignment; }
	inline int 		State()		const			{ return dockstate;		}
	inline int 		Position()	const			{ return dockposition;	}
	
	inline bool 	IsDocked()					{ return !IsShut() && !IsTabbed() && !IsFloating();					}
	inline bool		IsHidden()					{ return IsDocked() && dockstate == STATE_HIDE;						}
	inline bool 	IsAutoHidden()				{ return IsDocked() && dockstate == STATE_AUTO;						}
	inline bool 	IsShut()					{ return dockalignment == DOCK_NONE && dockstate == STATE_SHUT; 	}
	inline bool 	IsFloating()				{ return dockalignment == DOCK_NONE && dockstate != STATE_SHUT; 	}
	inline bool 	IsTabbed()					{ return dockstate == STATE_TABBED;	}

	int 			GetType()					{ return docktype;	}							// Returns the type of the derived dockablectrl class.
	DockBase&		GetBase()					{ ASSERT(dockbase); return *dockbase;	}		// Returns the owner (base) framework.
 	String 			GetLabel() 	const			{ return  dockname; }							// Returns the name of the dockablectrl.
	Image  			GetIcon () 	const 			{ return  dockicon; }  							// Returns the icon associated with the dockablectrl.
	Size			GetSizeHint()				{ return docksize;	}							// Returns the absolute (ctrl + frames) size of the dockablectrl.

	DockableCtrl& 	SetLabel(String  title);									
	DockableCtrl& 	SetIcon(Image icon);

	Callback1<Bar&>	WhenContextMenu;
	
protected:
	DockableCtrl&	SetType(int type)			{ docktype = type;	return *this;}	
	DockableCtrl&	SetBase(DockBase* c)		{ dockbase = c;		return *this;}
	DockableCtrl&	SetSizeHint(Size sz)		{ docksize = sz;	return *this;}
	DockableCtrl&	SetSize(Size sz);
	void			ReSize();

private:
	int				dockalignment;
	int				dockstate;
	int				dockposition;
	int				docktype;
	Size			docksize;
	DockBase*		dockbase;
	Image			dockicon;
	String			dockname;
	MenuBar			dockmenu;
	int				dockdroptarget;
	int				dockdropstate;

protected:
	bool 			haschildbar;
	bool			hasdragbar;
	bool			isdragged;
	bool			isdraggable;

	friend class 	DockBase;
	friend class 	PaneFrame;
	friend class 	TabWindow;
	friend class	AutoHideBar;

public:
	const DockCtrlChStyle::Style* style;					

public:
	virtual void 	Dock(int alignment, int state, int position, bool check = true) = 0;
	virtual void 	FloatEx(Rect r)	= 0;
	virtual void 	Float() 		= 0;
	virtual void 	Show() 			= 0;
	virtual void 	Hide() 			= 0;
	virtual void 	AutoHide() 		= 0;
	virtual void 	Shut() 			= 0;
	virtual void 	Menu() 			= 0;
	virtual void 	Settings();
	
	virtual void 	Serialize(Stream& s) = 0;	

public:
	virtual Rect 	GetCtrlRect() = 0;	
	virtual Size 	GetCtrlSize() = 0;

public:
	void 			SetDropTarget(int target = DOCK_NONE, int state = STATE_SHUT)	{ dockdroptarget = target, dockdropstate = state; }
	int				GetDropTarget	()												{ return dockdroptarget;	}
	int				GetDropState	()												{ return dockdropstate;		}
	bool			HasDropTarget	()												{ return (dockdroptarget != DOCK_NONE);}
	bool			IsDropTargetAvailable(Rect r, Point p)							{ return r.Contains(p);		}
	
	void			ForbidDragAndDrop()												{ isdraggable = false;	};
	void			PermitDragAndDrop()												{ isdraggable = true;	};

protected:
	void 			StartWindowDrag();

#if defined(PLATFORM_WIN32)
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#elif defined(PLATFORM_X11)
	virtual void EventProc(XWindow& w, XEvent *event);
#endif
			
protected:
	virtual	void 	OpenWindow(Rect& r);
	virtual void	OpenWindow(int x, int y, int cx, int cy);
	virtual void 	ShutWindow();
	virtual void 	ContextMenu();
	virtual	void 	OnShutWindow();
	virtual void 	Layout();
	virtual void	Paint(Draw& d)												{ d.DrawRect(GetSize(), SColorFace()); }	
public:
	virtual void 	ShowDragBar()												{ hasdragbar = true; 	}
	virtual void 	HideDragBar()												{ hasdragbar = false;	}
	virtual bool 	HasDragBar()												{ return hasdragbar;	}
	virtual bool 	HasChildBar()												{ return haschildbar;	} 

public:
	DockableCtrl& 	AddChildBar(CtrlFrame& bar)			{ AddFrame(bar);  haschildbar = true; return *this; }
	DockableCtrl& 	Add(Ctrl& ctrl)						{ this << ctrl; return *this; }
	DockableCtrl& 	operator<<(Ctrl& ctrl)				{ Add(ctrl);	return *this; }

	DockableCtrl();
	~DockableCtrl();	
};

//----------------------------------------------------------------------------------------------

class DockWindow : public DockableCtrl
{
public:
	typedef DockWindow CLASSNAME;
	
	DockWindow&		AddMenuBar(MenuBar& bar);
	DockWindow&		AddToolBar(ToolBar& bar);
	DockWindow&		AddStatusBar(StatusBar& bar);
	
	inline bool 	HasMenuBar	()		{ return hasmenubar; 	}
	inline bool 	HasToolBar	()		{ return hastoolbar; 	}
	inline bool 	HasStatusBar()		{ return hasstatusbar;	}

	virtual Rect 	GetCtrlRect();	
	virtual Size 	GetCtrlSize();
	
	TabWindow*		GetOwnerTab()		{ return ownertab; }
	AutoHideBar*	GetOwnerBar()		{ return ownerbar; }
	
	DockWindow&		SetOwnerTab(TabWindow* tabwindow)	{ ownertab = tabwindow; 	return *this; }
	DockWindow&		SetOwnerBar(AutoHideBar* hidebar)	{ ownerbar = hidebar;		return *this; }

	virtual void 	Serialize(Stream& s);
	
public:
	virtual void 	Dock(int alignment, int state, int position, bool sizecheck = true);
	virtual void 	FloatEx(Rect r);
	virtual void 	Float();
	virtual void 	Show();
	virtual void 	Hide();
	virtual void 	AutoHide();
	virtual void 	Shut();
	virtual void 	Menu();
	
public:
	virtual void	ShowDragBar();
	virtual void	HideDragBar();
	virtual	DockWindow&	ShowDragBarButtons(bool b = true);
		
protected:
	virtual void 	OnMenuButton	();
	virtual void	OnShutButton	();
	virtual void	OnAutoHideButton();
	virtual void 	ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	virtual void 	MouseMove(Point p, dword keyflags);
	virtual void 	Paint(Draw& d);	

public:
	virtual void	DockWindowMenu(Bar& bar);
	virtual void	DockWindowDockMenu(Bar& bar);
	
private:
	class DragBar : public DockCtrlCustomFrame
	{
	public:
		typedef DragBar CLASSNAME;
		void		ShowButtons(bool b = true)		{ show 	= b;	if(IsVisible()) Refresh(); }
		void		SetColor (const Color& c)		{ color = c; 	}
		void		SetOwner (DockWindow* w)		{ owner = w;	}
		DockWindow&	GetOwner ()						{ ASSERT(owner); return *owner;	}
		int 		GetWidth ();
		int 		GetHeight();
	
		virtual		void Paint(Draw& d);
		virtual		void FrameAddSize(Size& sz);
		
	private:
		DockWindow* owner;
		Color		color;
		bool		show;		

	public:
		DragBar();
	};

	struct DragBarButton : public Button
	{
	public:
		void  		Paint(Draw& d);
		Ctrl& 		SetImage(const Image& normal, const Image& highlighted, const Image& pushed);
	private:
		const 		DockCtrlChStyle::Style* style;
		Image 		nimage;
		Image 		himage;
		Image 		pimage;
	};
	
private:
	bool 			hasmenubar;
	bool 			hastoolbar;
	bool 			hasstatusbar;
	bool			hasbarbuttons;
	TabWindow*		ownertab;
	AutoHideBar*	ownerbar;
	MenuBar* 		menubar;
	ToolBar* 		toolbar;
	StatusBar*		statusbar;
	DragBar			dragbar;
	DragBarButton	menubutton;
	DragBarButton	autohidebutton;
	DragBarButton	shutbutton;
	friend class 	TabWindow; 

public:
	DockWindow& SetStyle(const DockCtrlChStyle::Style& s);
	const DockCtrlChStyle::Style*  GetStyle();

public:
	DockWindow();
	~DockWindow();
};


//----------------------------------------------------------------------------------------------

class TabWindow : public DockWindow
{
public:
	typedef TabWindow CLASSNAME;
	
	void 			Attach(DockableCtrl& ctrl, int makeactive = false);
	void 			Detach(DockableCtrl& ctrl);
	void 			DetachAll();

	TabWindow&		SetGroup(String name)			{ groupname = name; groupwindow = true; return *this;}
	void 			SetActive(int index);
	void 			SetActive(DockableCtrl& ctrl);
	DockableCtrl* 	GetActiveCtrl();
	int  			GetActiveTab();
	int	 			ChildCount();	
	void 			StartTabAnimation(); 
	void 			StopTabAnimation();	

	bool 			HasCtrlInRange();
	bool 			HasTabbedDock();
	bool			IsGroupWindow()					{ return groupwindow;			}
	bool 			IsTabAnimating()				{ return animating;		 		}
	bool 			IsDestroyed()					{ return destroyed;				}	

	TabInterface& 	GetTabs()						{ return tabs;		}
	DockableCtrl*	GetChildAt(int position);
	TabWindow*		GetBaseTab();
	int				GetNestLevel()					{ return nestlevel; }

	void			SetNestLevel(int n);
	TabWindow&		SetLayout(int n) 				{ if(tabs.IsChild()) RemoveFrame(tabs); AddFrame(tabs.SetLayout(n)); return *this; }

	virtual	void	Serialize(Stream& s);

	virtual void 	FloatEx(Rect r)					{ DockWindow::FloatEx(r); 	RefreshTabWindowLabel(*this); }
	virtual void 	Float()							{ DockWindow::Float(); 		RefreshTabWindowLabel(*this); }
		
protected:
	void 			OnMenuButton();
	void 			OnShutButton();
	void 			OnAutoHideButton();
	virtual void 	OnShutWindow();
		
	virtual void 	OnActiveTab();
	virtual void 	OnTabClose(int id, DockableCtrl& ctrl);
	virtual void 	OnTabDrag(int id, DockableCtrl& ctrl);
	virtual void 	OnTabActive(int id, DockableCtrl& ctrl);
	virtual void 	OnTabContextMenu(int id, DockableCtrl& ctrl);	

	virtual void 	Paint(Draw& d);
	
private:
	void 			AttachNormal(DockableCtrl& ctrl, int makeactive = false);
	void 			AttachNested(DockableCtrl& ctrl, int makeactive = false);
	void 			Attach0(DockableCtrl& ctrl, int makeactive = false);
		
	void 			RefreshTabWindowLabel(DockableCtrl& ctrl);
	bool 			RemoveTabWindow();
	void			ReposChilds();
		
private:
	int 			position;
	int 			previous;
	int				nestlevel;
	int				childcount;
	bool 			destroyed;
	bool			animating;
	bool			groupwindow;
	String			groupname;
	Image			animimage;
	TabInterface 	tabs;
	DockableCtrl* 	activectrl;

public:
	TabWindow();
	~TabWindow();
};

//----------------------------------------------------------------------------------------------

class AutoHideBar : public TabInterface
{
public:
	typedef AutoHideBar CLASSNAME;

	AutoHideBar& 	Attach(DockableCtrl& ctrl);
	void 			Detach(DockableCtrl& ctrl);

	void 			ShowBar();
	void 			HideBar();

	int	 			GetSize()			{ return DockCtrlCustomFrame::size; }
	void 			SetSize(int sz) 	{ DockCtrlCustomFrame::size = sz; RefreshParentLayout(); }

	DockableCtrl* 	GetChild(int position);
	int	  			GetCount();

protected:

	virtual void	FrameRemove();
	virtual void	FrameAdd(Ctrl& parent);
	virtual void 	MouseMove(Point p, dword keysflags);
	virtual void 	MouseLeave();
	virtual void	MouseEnter(Point p, dword keyflags);

	void 			ShowWindow();
	void 			HideWindow();
	
	void 			OnHighlight();
	void 			OnClose(int id, DockableCtrl& ctrl);
	
	void 			AdjustSize(Rect &r, const Size &sz);

private:	
	struct PopupWindow : public Ctrl
	{
		Callback 		WhenEnter;
		Callback 		WhenLeave;
		virtual void 	ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags); 
		virtual void 	Paint(Draw& d) { Ctrl::Paint(d); }
		PopupWindow() { BackPaint();  }
	};

	PopupWindow 	popup;
	DockableCtrl* 	ctrl;
	int				childcount;
		
public:
	const	DockCtrlChStyle::Style* style;
	AutoHideBar& SetStyle(const DockCtrlChStyle::Style& s)	{ style = &s; Refresh(); return *this; }
	const DockCtrlChStyle::Style*  GetStyle()				{ return style ? style : &DockCtrlChStyle::StyleDefault(); 	}

public:
	AutoHideBar();
    ~AutoHideBar(); 
};

//----------------------------------------------------------------------------------------------
class PaneFrame : public CtrlFrame, private Ctrl
{

public:
	typedef PaneFrame CLASSNAME;

	PaneFrame& 			Attach(DockableCtrl& ctrl);
	PaneFrame& 			Detach(DockableCtrl& ctrl);

	void  				ShowFrame();
 	void  				ShowFrame(Size sz);
 	void  				HideFrame();
 	void  				LockFrame();
 	void  				UnlockFrame();

	bool  				HasChild(DockableCtrl& ctrl);
	DockableCtrl* 		GetChild(int position);
	int	  				GetCount();
	Vector<TabWindow*>& GetTabWindows();

	TabWindow*			AddTabWindow();
	void				AddtoTabWindow(DockableCtrl& ctrl);
	void				RemovefromTabWindow(DockableCtrl& ctrl);
	void 				RemoveTabWindows();

	void				RefreshTabWindowList();
			
private:
	PaneFrame& 			AttachAsDock(DockableCtrl& ctrl);
	PaneFrame& 			AttachAsTab(DockableCtrl& ctrl);
	PaneFrame& 			AttachAsAuto(DockableCtrl& ctrl);

	TabWindow*			AddTabWindow(DockableCtrl& ctrl);
	void				RemoveTabWindow(TabWindow* tabwindow);
	void				RemoveTabWindow(int position);
	void				RemovefromTabWindow(int position);
	TabWindow*			FindTabWindow(int position);
	TabWindow*			FindTabWindow(DockableCtrl& ctrl);
	
	inline TabWindow* 	GetActiveTabWindow();
	void				SetActiveTabWindow(TabWindow* tabwindow);
	bool				HasTabWindow(TabWindow* tabwindow);
			

private:
	PaneSplitter 		dockcontainer;
	AutoHideBar	 		hidebar;
	TabWindow*	 		activetabwindow;
	Vector<TabWindow*> 	tabwindows;
		
public:
	virtual void 		FrameAdd(Ctrl& parent);
	virtual void 		FrameRemove();
	virtual void 		FrameAddSize(Size& sz);
	virtual void 		FrameLayout(Rect& r);

	virtual void 		Paint(Draw& draw);
	virtual void 		LeftDown(Point p, dword keyflags);
	virtual void 		MouseMove(Point p, dword keyflags);
	virtual void 		LeftUp(Point p, dword keyflags);
	virtual Image 		CursorImage(Point p, dword keyflags);

private:
	Point 				ref;
	Size  				parentsize;
	int   				type, minsize, sizemin, maxsize, stdsize;
	int   				size, size0;
	int	  				dndpos;
	bool  				created;
	bool  				locked;
	int   				BoundSize();
	DockBase* 			base;
	int	  				ncount;
	
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	PaneFrame& 			SetLayout(DockBase* basectrl, int alignment, int _maxsize);
	
	PaneFrame& 			Set(Ctrl& c, int size, int type);
	PaneFrame& 			Left(Ctrl& c, int size)    		{ return Set(c, size, LEFT); }
	PaneFrame& 			Top(Ctrl& c, int size)    	 	{ return Set(c, size, TOP); }
	PaneFrame& 			Right(Ctrl& c, int size)  	 	{ return Set(c, size, RIGHT); }
	PaneFrame& 			Bottom(Ctrl& c, int size)  		{ return Set(c, size, BOTTOM); }

	PaneFrame& 			MinSize(int sz)        		    { minsize = sz; return *this; }
	PaneFrame& 			MaxSize(int sz)					{ maxsize = sz; return *this; }
	PaneFrame& 			SizeMin(int sz)         		{ sizemin = sz; return *this; }

	PaneFrame& 			Vert()							{ dockcontainer.Vert(); return *this; 	}
	PaneFrame& 			Horz()							{ dockcontainer.Horz(); return *this; 	}
	PaneFrame& 			RefreshPaneFrame()				{ RefreshParentLayout(); dockcontainer.Layout(); return *this; }
	
	PaneFrame& 			SetBase(DockBase* ctrlbase)		{ base = ctrlbase;	return *this;	}
	DockBase&  			GetBase()						{ ASSERT(base); return *base; 		}
	
	int  				GetType() const                	{ return type; 		}	
	int  				GetSize() const                	{ return size; 		}
	int					GetPaneMaxSize() const			{ return maxsize; 	}	
	void 				SetSize(int sz)                	{ size = sz; RefreshParentLayout(); }

	Rect 				GetScreenView()					{ return Ctrl::GetScreenView(); 	}
	Rect 				GetScreenRect()					{ return Ctrl::GetScreenRect();		}	
	
	void 				Enable()						{ Ctrl::Enable(); 	}
	void 				Disable()						{ Ctrl::Disable();	}
	void 				Show(bool show = true)		  	{ Ctrl::Show(show); }
	void 				Hide()						  	{ Ctrl::Hide(); 	}
	
	bool 				IsShown()					  	{ return Ctrl::IsShown(); 	}
	bool 				IsEnabled()						{ return Ctrl::IsEnabled();	}
	bool 				IsCreated()						{ return created;			}
	bool 				IsLocked()						{ return locked;			}
	         
public:
	PaneFrame();
	~PaneFrame();
	
//===============================================
// DockPane: Drag-n-Drop related methods.
//===============================================

public:
	bool				HasCtrlInRange(DockableCtrl&  ctrl, Point p);
	
	void				StartPaneFrameAnimation(DockableCtrl& ctrl, int position);
	void				StartTabWindowAnimation(DockableCtrl& ctrl, int position);
	void				StopPaneFrameAnimation();
	void				StopTabWindowAnimation();
		    	                    
private:
	bool				CalculateCtrlRange(DockableCtrl& t, Ctrl& c, Point p);
	bool				CalculateEmptyRange(Ctrl& c, Point p);

public:		    	               
	void				DnDSourceinRange()				{ _dndsource = true;  }
	void				DnDSourceoutofRange()			{ _dndsource = false; }
	bool				DnDHasSource()	const			{ return _dndsource;  }
		
	void				DnDAnimate(int position = dnd_none);

	void				DoDropAnimation(int command = ANIMATE_ALL, int position = dnd_none, DockableCtrl* dock = NULL);	
	bool				DnDIsAnimating() const			{ return _dndanim;	  }
	
	void				DnDAnimStart()					{ _dndanim = true; 	  }
	void 				DnDAnimStop()					{ _dndanim = false;	  }
		
public:
	enum	{ dnd_first = 1, dnd_last = 2, dnd_tab = 128, dnd_none = -1 };
	enum	{ TAB_ANIMATION_START, TAB_ANIMATION_STOP, PANE_ANIMATION_START, PANE_ANIMATION_STOP, ANIMATE_ALL};
		
private:
	bool 				_dndsource;
	bool 				_dndanim;
	int  				_dndposition;
	int  				_dndtype;
};

//----------------------------------------------------------------------------------------------

class DockBase : public TopWindow
{
public:
	typedef DockBase CLASSNAME;

	Ctrl& 				Dock(DockableCtrl& ctrl);
	DockableCtrl&		Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2);
	DockableCtrl&		Tabify(String groupname, DockableCtrl& ctrl1, DockableCtrl& ctrl2);
	DockableCtrl&		Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2, DockableCtrl& ctrl3, DockableCtrl& ctrl4);
	DockableCtrl&		Tabify(String groupname, DockableCtrl& ctrl1, DockableCtrl& ctrl2, DockableCtrl& ctrl3, DockableCtrl& ctrl4);

	void				TabifyGroupAndDock(String groupname, int alignment);
	void				TabifyGroupAndAutoHide(String groupname, int alignment);
	void				TabifyGroupAndFloat(String groupname);

	void				AllowAll();
	void				AllowLeft();								
	void				AllowTop();							
	void				AllowRight();						
	void				AllowBottom();								
	void				AllowLeftRight();							
	void				AllowTopBottom();						
	void				AllowLeftTop();					
	void				AllowRightBottom();						
	void				AllowLeftBottom();						
	void				AllowTopRight();					

	void				AllowNestedTabbing(bool b = true);
		
	void				ShowLeftPane(bool b = true);
	void				ShowTopPane(bool b = true);
	void				ShowRightPane(bool b = true);
	void				ShowBottomPane(bool b = true);
	
	Ctrl& 				GetBase()									{ return (Ctrl&) *this;			}
	PaneFrame& 			GetPaneFrame(int alignment)					{ return pane[alignment]; 		}
	AutoHideBar& 		GetHideBar(int alignment)					{ return hide[alignment];		}
	
	void				ControlPanel();				
	void 				RefreshWidgetLayout();
	void 				ResetWidgetLayout();

	void				SetKey(dword key)							{ hotkey = key; }
	dword				GetKey()									{ return hotkey;}

	void				AddWidgetLayout(String name, DockableCtrl& ctrl, int alignment, int state, int position = 0);
	void				SetWidgetLayout(String name);
	void				SetWidgetLayout(int index);

	virtual bool		Key(dword key, int count);
	virtual	void		Serialize(Stream& s);

protected:
	void				InitFrameWork();
	void				InitDefaultLayout();
	void				InitFrameWorkSettings();
	
	void				FrameWorkReady(bool b = true)				{ isready =  b;		}
	bool				IsFrameWorkReady()							{ return isready; 	}
	     
private:
	void 				Attach(DockableCtrl& ctrl);
	void 				Attach(DockableCtrl* ctrl);
	void 				Detach(DockableCtrl& ctrl);
	void 				Detach(DockableCtrl* ctrl);
	void 				DoDragAndDrop(DockableCtrl& ctrl, Point p, Size sz);

private:
	struct CtrlRecord : Moveable<CtrlRecord>
	{
		DockableCtrl* 	ctrl;
		int				id;
		CtrlRecord()	{ ctrl = NULL; id = 0; }
	};
	
	void				AddCtrlRecord(DockableCtrl& ctrl);
	void				RemoveCtrlRecord(DockableCtrl& ctrl);
	void				DeleteCtrlRecords();
	CtrlRecord*			GetCtrlRecord(DockableCtrl& ctrl);
	CtrlRecord*			GetCtrlRecordFromIndex(int index);
	CtrlRecord*			GetCtrlRecordFromGroup(String group);
	CtrlRecord*			GetCtrlRecordFromId(int id);
	Vector<CtrlRecord*>& GetCtrlRecords()							{ return ctrls; };

	DockableCtrl*		GetDockedWindowFromIndex(int index);
	TabWindow*			GetTabWindowFromIndex(int index);
	int					GetTabWindowCount();
	int					GetDockedWindowCount();

//	TabWindow*			AddTabWindow();
//	TabWindow*			AddTabWindow(DockableCtrl ctrl);
	void				RemoveTabWindows();
	void				RefreshTabWindowList();
	Vector<TabWindow*>&	GetTabWindows();
	
	void				SerializeLayout(Stream& s, bool deflay = false);

protected:
	void				NewWidgetLayout(String name);
	void				DelWidgetLayout(String name);
	int					AddWidgetLayout(String name);

	void				NewWidgetGroup(String name, bool predefined = false);
	bool				AddWidgettoGroup(String name, DockableCtrl& ctrl);
	bool				AddWidgettoGroup(String name, int id);
	bool				DeleteWidgetfromGroup(String name, DockableCtrl& ctrl);
	bool				DeleteWidgetfromGroup(String name, int id);	
	int					AddWidgetGroup(String name);
	void				DeleteWidgetGroup(String name);
	Vector<int>&		FindWidgetGroup(String name);
	bool				FindWidgetinGroup(String name, int id);

private:
	void				GroupDrop(int parent, int ii, PasteClip& d);	
	void				GroupDrag();
	void				GroupSelect();
	void				GroupMenu(Bar& bar);
	void				GroupDockMenu(Bar& bar, int command);
	void				GroupTabifyMenu(Bar& bar, int command);
	void				GroupMenuAction(String name, int command, int alignment);

	void				RefreshGroups();
	
	DockableCtrl&		TabifyGroup(String name);
			
	void				SetSkin(int index);

private:
	int					skinindex;
	int					layoutindex;
	bool				tabsicons;
	bool				tabsclose;
	bool				tabsnested;
	bool				tabsautoalign;
	bool				autohideicons;
	bool				autohideclose;
	bool				isready;
	bool				IsSideAllowed(int i)						{ return AllowedSides[i]; }
	bool				AllowedSides[4];
		
private:
	void 				OnPanelAction();
	void 				OnSelectSkin ();
	void				OnSelectLayout();
	void				OnAddNewLayout();
	void				OnDeleteLayout();
	void				OnAddNewGroup();
	void				OnDeleteGroup();
	void 				RefreshPanel();
	void				CleanUp();
			
private:
	Size 				panesize;
	PaneFrame 			pane[4];
	AutoHideBar			hide[4];
	dword				hotkey;
	String				predefinedlayout;
	String				predefinedgroup;
	String				activegroup;

	WithControlPanelLayout<TopWindow> 	controlpanel;
	WithWidgetListLayout<ParentCtrl>	listtab;
	WithWidgetGroupsLayout<ParentCtrl>	grouptab;
	
	TreeCtrl							grouptree;
	TabCtrl								paneltab;
		
	Vector<TabWindow*>					tabwindows;
	Array<ImageCtrl*> 					panelicons;
	ArrayMap<String, String>			layouts;
	ArrayMap<String, Vector<int> >		groups;
	Vector<CtrlRecord*> 				ctrls;
		
	friend class 		DockableCtrl;
	friend class 		DockWindow;
	friend class 		TabWindow;
	friend class 		PaneFrame;

public:
	const DockCtrlChStyle::Style* 	style;
	DockBase& 						SetStyle(const DockCtrlChStyle::Style& s);
	const DockCtrlChStyle::Style*  	GetStyle()	{ return style ? style : &DockCtrlChStyle::StyleDefault(); 	}

public:
	DockBase& WidgetGroup(String name);
	DockBase& operator%(DockableCtrl& ctrl);
	DockBase& WidgetLayout(String name);
	DockBase& operator/(DockableCtrl& ctrl);

public:
	DockBase();
	~DockBase();
};

class DockCtrl : public DockBase
{
public:
	typedef DockCtrl CLASSNAME;
	virtual void InitDockCtrl() 		{}	
	virtual void InitCustomLayouts()	{}
	void	State(int reason);
	void	Serialize(Stream& s)		{ DockBase::Serialize(s); }
private:
	bool ctrlinit;
	
public:
	DockCtrl();
	~DockCtrl();
};

#endif
