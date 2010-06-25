#ifndef _TabBar_TabBar_h_
#define _TabBar_TabBar_h_

#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TabBarImg
#define IMAGEFILE <TabBar/TabBar.iml>
#include <Draw/iml_header.h>

NAMESPACE_UPP

//#define TABBAR_DEBUG

struct AlignedFrame : FrameCtrl<Ctrl>
{
	int layout;
	int framesize;
	int border;
public:	
	enum
	{
		LEFT = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3
	};
	
	AlignedFrame() : border(0), framesize(0), layout(TOP) {}
	
	virtual void FrameLayout(Rect &r);
	virtual void FrameAddSize(Size& sz);
	virtual void FramePaint(Draw& w, const Rect& r);
	
	bool		  IsVert() const	{ return (layout & 1) == 0; }
	bool		  IsHorz() const	{ return layout & 1; }
	bool		  IsTL() const		{ return layout < 2; }
	bool		  IsBR() const		{ return layout >= 2; }
	
	AlignedFrame& SetAlign(int align) { layout = align; FrameSet(); RefreshParentLayout(); return *this; }
	AlignedFrame& SetLeft()		{ return SetAlign(LEFT); }
	AlignedFrame& SetTop()		{ return SetAlign(TOP); }
	AlignedFrame& SetRight()	{ return SetAlign(RIGHT); }
	AlignedFrame& SetBottom()	{ return SetAlign(BOTTOM); }	
	AlignedFrame& SetFrameSize(int sz, bool refresh = true);
		
	int 		  GetAlign() const		{ return layout; }
	int			  GetFrameSize() const 	{ return framesize; }
	int			  GetBorder() const		{ return border; }
protected:
	void Fix(Size& sz);
	void Fix(Point& p);
	Size Fixed(const Size& sz);
	Point Fixed(const Point& p);
	
	bool		  HasBorder()				{ return border >= 0; }
	AlignedFrame& SetBorder(int _border)	{ border = _border; return *this; }
	
	virtual	void  FrameSet()				{ }
};

class TabScrollBar : public AlignedFrame
{
	private:
		int total;
		double pos, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		virtual void UpdatePos(bool update = true);
		void RefreshScroll();
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
		void Clear();
		void Set(const TabScrollBar& t);
		bool IsScrollable() const;
		Callback WhenScroll;
};

class TabBar : public AlignedFrame
{
public:
	struct Style : public TabCtrl::Style { };
	
protected:
	enum {
		TB_MARGIN = 6,
		TB_SPACE = 10,
		TB_SBHEIGHT = 4,
		TB_SBSEPARATOR = 1,
		TB_ICON = 16,
		TB_SPACEICON = 5
	};	

	struct Tab : Moveable<Tab> {
		int id;
		
		Image  img;
		Value  key;
		Value  value;
		String group;
		
		String  stackid;
		int 	stack;

		bool visible;

		Point pos;
		Size  size;
		int	  stdcx;
		
		Point cross_pos;
		Size  cross_size;
		
		Point tab_pos;
		Size  tab_size;
		
		Tab() : visible(true), id(-1), stack(-1) { }
		bool HasMouse(const Point& p) const;
		bool HasMouseCross(const Point& p) const;
		bool HasIcon() const	{ return !img.IsEmpty(); }
		int  Right() const 		{ return pos.x + size.cx; } 
	};
	struct Group : Moveable<Group> {
		Group()	{}
		String name;
		int active;
		int count;
		int first;
		int last;
	};
	// Tab sorting structures
	struct TabSort {
		virtual bool operator()(const Tab &a, const Tab &b) const = 0;
	};	
	struct TabGroupSort : public TabSort {
		virtual bool operator()(const Tab &a, const Tab &b) const { return a.group < b.group; }
	};
protected:
	struct TabValueSort : public TabSort {
		virtual bool operator()(const Tab &a, const Tab &b) const { return (*vo)(a.value, b.value); }
		const ValueOrder *vo;
	};
	struct TabKeySort : public TabSort {
		virtual bool operator()(const Tab &a, const Tab &b) const { return (*vo)(a.key, b.key); }
		const ValueOrder *vo;
	};	
	
protected:
	TabScrollBar 	sc;
	
	Vector<Group> 	groups;
	Vector<Tab> 	tabs;
	Vector<int>		separators;
	int 			active;

private:
	int id;
	int highlight;
	int target;
	int cross;
	bool crosses:1;
	bool isctrl:1;
	bool isdrag:1;
	bool grouping:1;
	bool autoscrollhide:1;		
	bool nosel:1;
	bool nohl:1;
	bool inactivedisabled:1;
	bool stacking:1;
	bool stacksort:1;
	bool groupsort:1;
	bool groupseps:1;
	bool tabsort:1;
	bool allownullcursor:1;
	bool icons:1;
	int mintabcount;
	Point mouse, oldp;
	int group;
	const Display *display;
	Image dragtab;
	int stackcount;	
	Value group_sep_vert;
	Value group_sep_horz;

	TabSort *tabsorter;
	TabSort *groupsorter;
	TabSort *stacksorter;
	TabValueSort valuesorter_inst;
	TabKeySort 	 keysorter_inst;
	TabValueSort stacksorter_inst;

	static Style leftstyle, rightstyle, bottomstyle;
	const Style *style[4];
private:
	void PaintTab(Draw &w, const Style &s, const Size &sz, int i, bool enable, bool dragsample = false);
	
	int  TabPos(const String &g, bool &first, int i, int j, bool inactive);	
	void SyncScrollBar(bool synctotal = true);
	void Scroll();

	int  FindId(int id) const;
	int  GetNext(int n) const;
	int  GetPrev(int n) const;

	int GetWidth(int n);
	int GetExtraWidth();
	int GetWidth() const;
	int GetHeight(bool scrollbar = true) const;

	void DoStacking();
	void DoUnstacking();
	void InsertIntoStack(Tab &t, int ix);
	int  FindStackHead(int stackix) const;
	int  FindStackTail(int stackix) const;
	bool IsStackHead(int n) const;
	bool IsStackTail(int n) const;
	int SetStackHead(Tab &t);
	void CycleTabStack(int head, int n);
	int CycleTabStack(int n);
	
	static int GetStyleHeight(const Style& s);
	
	int   GetNextId();
	int   GetScrollPos() 				{ return sc.GetPos(); }		
	
	static Value AlignValue(int align, const Value &v, const Size &isz);
protected:	
	virtual void Paint(Draw &w);
	virtual void LeftDown(Point p, dword keysflags);
	virtual void LeftUp(Point p, dword keysflags);
	virtual void LeftDouble(Point p, dword keysflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void MiddleDown(Point p, dword keyflags);
	virtual void MiddleUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keysflags);
	virtual void MouseLeave();
	virtual void DragAndDrop(Point p, PasteClip& d);
	virtual void LeftDrag(Point p, dword keyflags);
	virtual void DragEnter();
	virtual void DragLeave();
	virtual void DragRepeat(Point p);
	virtual void CancelMode();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void FrameSet();
	virtual void FrameLayout(Rect& r);
	virtual void Layout();

	// Mouse handling/tab positioning
	bool ProcessMouse(int i, const Point& p);
	bool ProcessStackMouse(int i, const Point& p);
	void SetHighlight(int n);
	int  GetTargetTab(Point p);	
	void Repos();
	
	// Grouping
	void MakeGroups();
	int  FindGroup(const String& g) const;
	void CloseAll(int exception);	
	void DoGrouping(int n);
	void DoCloseGroup(int n);
	void NewGroup(const String &name);
	void DoTabSort(TabSort &sort);
	void SortTabs0();
	void SortStack(int stackix);
	void SortStack(int stackix, int head, int tail);
	
	// Insertion without repos/refresh - for batch actions
	int InsertKey0(int ix, const Value &key, const Value &value, Image icon = Null, String group = Null);
	
	// Sub-class Paint override
	virtual void PaintTab(Draw& w, const Rect &r, const Tab& tab, const Font &font, 
						Color ink, dword style);
	virtual void PaintStackedTab(Draw& w, const Rect &r, const Tab& tab, const Font &font, 
						Color ink, dword style);
	virtual void PaintSeparator(Draw &w, const Rect r);
	virtual Size GetStdSize(const Tab &t); 
	virtual Size GetStackedSize(const Tab &t);
	Size 		 GetStdSize(const Value &v); 
	
	// Paint helpers
	int		GetTextAngle()	{ return AlignedFrame::IsVert() ? (GetAlign() == LEFT ? 900 : 2700) : 0; }	
	Point	GetTextPosition(const Rect& r, int cy, int space) const;
	Point   GetImagePosition(const Rect& r, int cx, int cy, int space, int side) const;
	bool	PaintIcons() 	{ return icons; }
	
	const Style &GetStyle() { return *style[GetAlign()]; }
	
	// Sub-class menu overrides
	virtual void ContextMenu(Bar& bar);
	virtual void GroupMenu(Bar &bar, int n);
	// Sorting/Stacking overriddes
	virtual String 		GetStackId(const Tab &a)			{ return a.group; }
public:
	typedef TabBar CLASSNAME;

	Callback 					WhenHighlight;		// Executed on tab mouse-over
	Callback 					WhenLeftDouble;		// Executed on left-button double-click (clicked tab will be the active tab)
	Gate1<Value> 				CancelClose; 		// Return true to cancel action. Parameter: Key of closed tab
	Callback1<Value>			WhenClose; 			// Executed before tab closing. Parameter: Key of closed tab
	Gate	 					CancelCloseAll;		// Return true to cancel action;
	Callback 		 			WhenCloseAll;		// Executed before 'Close All' action
	Gate1<Vector<Value> >		CancelCloseSome;	// Return true to cancel action (executed with list of closing tabs)
	Callback1<Vector<Value> >	WhenCloseSome;		// Executed before any 'Close' action (with list of closing tabs)

	TabBar();

	TabBar& Add(const Value &value, Image icon = Null, String group = Null, bool make_active = false);
	TabBar& Insert(int ix, const Value &value, Image icon = Null, String group = Null, bool make_active = false);
	
	TabBar& AddKey(const Value &key, const Value &value, Image icon = Null, String group = Null, bool make_active = false);
	TabBar& InsertKey(int ix, const Value &key, const Value &value, Image icon = Null, String group = Null, bool make_active = false);
	
	void 	Close(int n);
	void 	CloseData(const Value &key);
	void 	Clear();

	TabBar& Crosses(bool b = true);
	TabBar& Stacking(bool b = true);
	TabBar& Grouping(bool b = true);
	TabBar& GroupSeparators(bool b = true);
	TabBar& AutoScrollHide(bool b = true);
	TabBar& InactiveDisabled(bool b = true);
	TabBar& AllowNullCursor(bool b = true);
	TabBar& Icons(bool v = true);

	TabBar& SortTabs(bool b = true);
	TabBar& SortTabsOnce();
	TabBar& SortTabsOnce(TabSort &sort);
	TabBar& SortTabs(TabSort &sort);

	TabBar& SortTabValues(ValueOrder &sort);
	TabBar& SortTabValuesOnce(ValueOrder &sort);
	TabBar& SortTabKeys(ValueOrder &sort);
	TabBar& SortTabKeysOnce(ValueOrder &sort);
	
	TabBar& SortGroups(bool b = true);
	TabBar& SortGroups(TabSort &sort);
	TabBar& SortStacks(bool b = true);
	TabBar& SortStacks(TabSort &sort);
	TabBar& SortStacks(ValueOrder &sort);

	bool	IsValueSort() const				{ return tabsort; }
	bool	IsGroupSort() const				{ return groupsort; }
	bool	IsStackSort() const				{ return stacksort; }
	
	bool	IsGrouping() const				{ return grouping; }
	bool	HasGroupSeparators() const		{ return separators; }
	bool	IsStacking() const				{ return stacking; }
	bool	IsShowInactive() const			{ return inactivedisabled; }
	
	TabBar& NeverEmpty()					{ MinTabCount(1); }
	TabBar& MinTabCount(int cnt)			{ mintabcount = max(cnt, 0); Refresh(); return *this; }
	
	TabBar& SetDisplay(const Display &d) 	{ display = &d; Refresh(); }
	TabBar& SetBorder(int border)           { AlignedFrame::SetBorder(border); return *this; }
	int 	FindKey(const Value &v) const;
	int 	FindValue(const Value &v) const;
	
	Value  	GetKey(int n) const				{ ASSERT(n >= 0 && n < tabs.GetCount()); return tabs[n].key;}
	Value  	GetValue(int n) const			{ ASSERT(n >= 0 && n < tabs.GetCount()); return tabs[n].value;}
	Value  	Get(const Value &key) const		{ return GetValue(FindKey(key)); }
	void	Set(int n, const Value &newkey, const Value &newvalue, Image icon = Null);
	void	Set(const Value	&key, const Value &newvalue, Image icon = Null);
	void	Set(int n, const Value &newvalue, Image icon = Null);
	void	SetIcon(int n, Image icon);
	void 	SetTabGroup(int n, const String &group);
	TabBar &SetGroupSeparators(Value horz, Value vert);	
	
	virtual Value 	GetData() const			{ return (HasCursor() && active < GetCount()) ? GetKey(active) : Value(); }
	virtual void 	SetData(const Value &key);
	
	String 	GetGroupName() const      	{ return (group == 0) ? Null : groups[group].name; }
	String 	GetGroupName(int i) const	{ return groups[i].name;	   }
	int  	SetGroup(const String &s)   { DoGrouping(max(0, FindGroup(s))); return group; }
	int  	SetGroup(int c)             { DoGrouping(c); return group;     }
	int  	GetGroup() const            { return group;            	   }
	int 	GetGroupCount() const		{ return groups.GetCount();	   }
	void 	SetGroupActive(int id)      { groups[group].active = id;   }
	int  	GetGroupActive() const      { return groups[group].active; }
	int  	GetFirst() const            { return groups[group].first;  }
	int  	GetLast() const             { return groups[group].last;   }
	bool 	IsGroupAll() const          { return group == 0;           }			
	
	int    	GetCursor() const 			{ return active; }
	bool   	HasCursor() const			{ return active >= 0; }
	int	   	GetHighlight() const 		{ return highlight; }
	bool   	HasHighlight() const		{ return highlight >= 0; }
	int    	GetCount() const 			{ return tabs.GetCount(); }

	void   	SetCursor(int n);
	void	KillCursor()				{ SetCursor(-1); Refresh(); }	

	Image 	GetDragSample();
	Image 	GetDragSample(int n);

	Vector<Value> GetKeys() const;
	Vector<Image> GetIcons() const;
	int			  GetScrollPos() const	{ return sc.GetPos(); }
	TabBar&		  CopySettings(const TabBar &src);

	TabBar& SetStyle(int align, const Style& s)  	{ ASSERT(align >= 0 && align < 4); style[align] = &s; Refresh(); return *this; }
	static const Style& StyleDefault();
	static const Style& StyleLeft();
	static const Style& StyleRight();
	static const Style& StyleBottom();
	static const Style& AlignStyle(int align, Style &s);	
	static void ResetStyles();	
};

#include "FileTabs.h"

END_UPP_NAMESPACE

#endif
