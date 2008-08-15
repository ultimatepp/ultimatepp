$uvs: PENDING CONFLICT
#ifndef _TabBar_TabBar_h_
#define _TabBar_TabBar_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS TabBarImg
#define IMAGEFILE <TabBar/TabBar.iml>
#include <Draw/iml_header.h>

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

	bool		  HasBorder()				{ return border >= 0; }
	AlignedFrame& SetBorder(int _border)	{ border = _border; }

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
	enum
	{
		QT_MARGIN = 6,
		QT_SPACE = 10,
		QT_SBHEIGHT = 4,
		QT_SBSEPARATOR = 1,
		QT_FILEICON = 16,
		QT_SPACEICON = 5
	};
	struct Tab : Moveable<Tab>
	{
		Value data;
		String group;
		bool visible;
		int x, cx;
		int y, cy;
		int id;
		Size tsz;
		Tab() : visible(true), id(-1)
		{}
		int Right() const { return x + cx; }
		bool HasMouse(const Point& p) const;
		bool HasMouseCross(const Point& p, int h, int type) const;
	};

	struct Group : Moveable<Group>
	{
		Group()	{}
		String name;
		Value data;
		int active;
		int count;
		int first;
		int last;
	};
private:
	int id;
	TabScrollBar sc;
$uvs: REPOSITORY DELETE
	Scroller scroller;
	void Scroll();
$uvs: END REPOSITORY DELETE

	Vector<Group> groups;
	Vector<Tab> tabs;
	int highlight;
	int active;
	int target;
	int cross;
	bool crosses:1;
	bool isctrl:1;
	bool isdrag:1;
	bool grouping:1;
	bool autoscrollhide:1;
	bool nosel:1;
	bool nohl:1;
	bool inactiveshadow:1;
	int neverempty;
	Point mouse, oldp;
	int group;
	const Display *display;
	Image dragimg;

	static Style leftstyle, rightstyle, bottomstyle;
	const Style *style[4];

	void DrawTab(Draw &w, const Style &s, const Size &sz, int i, bool enable);
	int  TabPos(const String &g, bool &first, int i, int j, bool inactive);
	void SyncScrollBar(int total);
$uvs: REPOSITORY INSERT
	void Scroll();
$uvs: END REPOSITORY INSERT

	int  FindId(int id) const;
	int  GetNext(int n) const;
	int  GetPrev(int n) const;

	int GetWidth(int n);
	int GetWidth() const;
	int GetHeight() const				{ return TabBar::GetStyleHeight(*style[TOP]); }
	static int 	GetStyleHeight(const Style& s);

	int   GetNextId();
	int   GetScrollPos() 				{ return sc.GetPos(); }

	static Value AlignValue(int align, const Value &v, const Size &isz);
protected:
	virtual void Paint(Draw &w);
	virtual void LeftDown(Point p, dword keysflags);
	virtual void LeftUp(Point p, dword keysflags);
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

	void Repos();
	void MakeGroups();
	int  FindGroup(const String& g) const;
	void CloseAll();
	void DoGrouping(int n);
	void DoCloseGroup(int n);
	void NewGroup(const String &name, const Value &data = Value());

	// Sub-class display overide & helpers
	virtual void DrawTabData(Draw& w, Point p, const Size &sz, const Value& q, const Font &font,
		Color ink, dword style);
	virtual Size GetStdSize(Value &q);

	int		TextAngle()		{ return AlignedFrame::IsVert() ? (GetAlign() == LEFT ? 900 : 2700) : 0; }
	void 	TabCenterText(Point &p, const Size &sz, const Font &f)	{ TabCenter(p, sz, f.GetHeight()); }
	void 	TabCenter(Point &p, const Size &sz, const Size &hsz)	{ TabCenter(p, sz, IsVert() ? sz.cx : sz.cy); }
	void 	TabCenter(Point &p, const Size &sz, int h);

	int 	GetTargetTab(Point p);

	const Style &GetStyle() { return *style[GetAlign()]; }

	// Sub-class menu overrides
	virtual void ContextMenu(Bar& bar);
	virtual void GroupMenu(Bar &bar, int n);
public:
	typedef TabBar CLASSNAME;

	Callback WhenHighlight;
	Callback WhenCursor;
	Callback1<Value> WhenClose;
$uvs: REPOSITORY INSERT
	Callback WhenCloseAll;
$uvs: END REPOSITORY INSERT

	TabBar();

	TabBar& Add(const Value &data, bool make_active = false)						{ return Add(data, Null, make_active); }
	TabBar& Add(const Value &data, String group = Null, bool make_active = false);
	TabBar& Insert(int ix, const Value &data, bool make_active = false)	{ return Insert(ix, data, Null, make_active); }
	TabBar& Insert(int ix, const Value &data, String group = Null, bool make_active = false);
	void 	Close(int n);
	void 	Clear();

	TabBar& Crosses(bool b = true);
	TabBar& Grouping(bool b = true);
	TabBar& AutoScrollHide(bool b = true);
	TabBar& InactiveShadow(bool b = true);

	TabBar& SetDisplay(const Display &d) 	{ display = &d; Refresh(); }

	int 	Find(const Value &v) const;

	Value  	Get(int n) const				{ ASSERT(n >= 0 && n < tabs.GetCount()); return tabs[n].data;}
	void	Set(int n, const Value &data, const String &group = Null);
	virtual Value GetData() const			{ return HasCursor() ? Get(active) : Value(); }
	virtual void SetData(const Value &data) { int n = Find(data); if (n >= 0) SetCursor(n); }

	Value 	GetGroupData(const String &s) const 				{ return GetGroupData(FindGroup(s)); }
	void 	SetGroupData(const String &s, const Value &data) 	{ SetGroupData(FindGroup(s), data); }
	Value 	GetGroupData(int n) const 							{ ASSERT(n >= 0 && n < groups.GetCount()); return groups[n].data; }
	void 	SetGroupData(int n, const Value &data) 				{ ASSERT(n >= 0 && n < groups.GetCount()); groups[n].data = data;}

	String 	GetGroupName() const      	{ return group == 0 ? Null : groups[group].name; }
	int  	SetGroup(const String &s)   { group = max(0, FindGroup(s)); return group; }
	int  	SetGroup(int c)             { group = c; return group;     }
	int  	GetGroup() const            { return group;            	   }
	void 	SetGroupActive(int id)      { groups[group].active = id;   }
	int  	GetGroupActive() const      { return groups[group].active; }
	int  	GetFirst() const            { return groups[group].first;  }
	int  	GetLast() const             { return groups[group].last;   }
	bool 	IsGroupAll() const          { return group == 0;           }
	int    	GetCount() const 			{ return tabs.GetCount(); }
	int    	GetCursor() const 			{ return active; }
	int	   	GetHighlight() const 		{ return highlight; }
	void   	SetCursor(int n);
	void	SetTabGroup(int n, const String &group);

	bool   	HasCursor() const			{ return active >= 0; }
	bool   	HasHighlight() const		{ return highlight >= 0; }

	TabBar& NeverEmpty(int limit = 1)	{ neverempty = max(limit, 1); Refresh(); return *this; }
	TabBar& CanEmpty()					{ neverempty = 0; Refresh(); return *this; }

	TabBar& SetStyle(int align, const Style& s)  	{ ASSERT(align >= 0 && align < 4); style[align] = &s; Refresh(); return *this; }
	static const Style& StyleDefault();
	static const Style& StyleLeft();
	static const Style& StyleRight();
	static const Style& StyleBottom();
	static const Style& AlignStyle(int align, Style &s);
	static void ResetStyles();

	virtual void Serialize(Stream& s);
};


#endif

