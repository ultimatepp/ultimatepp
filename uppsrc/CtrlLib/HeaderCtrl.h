class HeaderCtrl : public Ctrl, public CtrlFrame {
public:
	virtual void  CancelMode();
	virtual void  Paint(Draw& draw);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  LeftDouble(Point p, dword keyflags);
	virtual void  LeftDrag(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  MouseLeave();
	virtual void  LeftUp(Point, dword);
	virtual void  RightDown(Point, dword);
	virtual void  Serialize(Stream& s);
	virtual void  Layout();

	virtual void  FrameAdd(Ctrl& parent);
	virtual void  FrameRemove();
	virtual void  FrameLayout(Rect& r);
	virtual void  FrameAddSize(Size& sz);

public:
	struct Style : ChStyle<Style> {
		Value look[4];
		int   gridadjustment;
		bool  pressoffset;
	};

	class Column : public LabelBase {
	protected:
		virtual void  LabelUpdate();

		String      tip;
		HeaderCtrl *header;
		double      ratio;
		int         min, max;
		int         margin;
		Color       paper;
		int         index;
		bool        visible;

		void        Paint(bool& first, Draw& w,
		                  int x, int y, int cx, int cy, bool disabled, bool push, bool hl);

		friend class HeaderCtrl;

	public:
		Event<>     WhenLeftClick;
		Event<>     WhenLeftDouble;
		Event<>     WhenAction;
		Event<Bar&> WhenBar;

		Column&  Min(int _min)                     { min = _min; return *this; }
		Column&  Max(int _max)                     { max = _max; return *this; }
		Column&  MinMax(int m, int n)              { return Min(m).Max(n); }
		Column&  Fixed(int f)                      { return MinMax(f, f); }
		Column&  Tip(const char *s)                { tip = s; return *this; }
		Column&  SetPaper(Color c)                 { paper = c; return *this; }
		Column&  SetRatio(double ratio);
		Column&  SetMargin(int m);

		void     Show(bool b = true);
		void     Hide()                            { Show(false); }

		int      GetMargin() const                 { return margin + 2; }
		Color    GetPaper() const                  { return paper; }
		int      GetIndex() const                  { return index; }
		bool     IsVisible() const                 { return visible; }
		double   GetRatio() const                  { return ratio; }

		Column();
	};

	friend class Column;

	enum { PROPORTIONAL, REDUCELAST, REDUCENEXT, SCROLL, FIXED };

protected:

	Array<Column> col;
	HScrollBar    sb;

	mutable int          oszcx;
	mutable Vector<Rect> tabrect;

	int   split, pushi, li, ti;
	bool  isdrag;
	Image dragtab;
	int   dragd, dragx;
	Rect  colRect;
	bool  push:1;
	bool  track:1;
	bool  invisible:1;
	bool  moving:1;
	byte  mode;
	int   light;
	int   height;
	const Style *style;
	bool  autohidesb;

	double Denominator() const;
	void   Reduce(int q, double& delta, double rs, int szcx, bool checkmin);
	int    GetNextTabWidth(int i);
	int    GetLastVisibleTab();
	void   WScroll();
	void   Scroll();
	void   ScrollVisibility();
	void   DoSbTotal();
	void   SbTotal();
	void   SetTabWidth0(int i, int cx);
	int    SumMin(int from);
	int    SumMax(int from);
	void   Distribute(const Vector<int>& sci, double delta);
	void   ReCompute();
	void   InvalidateDistribution()                              { oszcx = -1; }
	void   RefreshDistribution();
	Vector<int> GetVisibleCi(int from);
	
	friend class ArrayCtrl;

public:
	Event<>       WhenLayout;
	Event<>       WhenScroll;
	Event<>       WhenScrollVisibility;

	Rect          GetTabRect(int i);

	const Column& Tab(int i) const                        { return col[i]; }
	Column&       Tab(int i);

	Column&       Add(const char *text, double ratio = 0);
	Column&       Add();
	const Column& operator[](int i) const                 { return col[i]; }
	int           GetCount() const                        { return col.GetCount(); }
	void          Reset();

	void          ShowTab(int i, bool show = true);
	void          HideTab(int i)                          { ShowTab(i, false); }
	bool          IsTabVisible(int i)                     { return col[i].visible; }
	void          SetTabRatio(int i, double ratio);
	double        GetTabRatio(int i) const                { return InvZxf(col[i].ratio); }
	void          SetTabWidth(int i, int cx);
	int           GetTabWidth(int i);
	
	void          SwapTabs(int first, int second);
	void          MoveTab(int from, int to);
	int           GetTabIndex(int i) const                { return col[i].index; }
	int           FindIndex(int ndx) const;

	void          StartSplitDrag(int s);
	int           GetSplit(int x);

	int           GetScroll() const                       { return sb; }
	bool          IsScroll() const                        { return sb.IsShown(); }

	void          SetHeight(int cy);
	int           GetHeight() const;
	
	int           GetMode() const                         { return mode; }
	
	static const Style& StyleDefault();

	HeaderCtrl&   Invisible(bool inv);
	HeaderCtrl&   Track(bool _track = true)               { track = _track; sb.Track(track); return *this; }
	HeaderCtrl&   NoTrack()                               { return Track(false); }
	HeaderCtrl&   Proportional();
	HeaderCtrl&   ReduceNext();
	HeaderCtrl&   ReduceLast();
	HeaderCtrl&   Absolute();
	HeaderCtrl&   Fixed();
	HeaderCtrl&   SetStyle(const Style& s)                { style = &s; Refresh(); return *this; }
	HeaderCtrl&   Moving(bool b = true)                   { moving = b; return *this; }
	HeaderCtrl&   AutoHideSb(bool b = true)               { autohidesb = b; SbTotal(); return *this; }
	HeaderCtrl&   NoAutoHideSb()                          { return AutoHideSb(false); }
	HeaderCtrl&   HideSb(bool b = true)                   { sb.Show(!b); return *this; }

	HeaderCtrl&   SetScrollBarStyle(const ScrollBar::Style& s)   { sb.SetStyle(s); return *this; }

	static int GetStdHeight()                             { return Draw::GetStdFontCy() + 4; }

	typedef HeaderCtrl CLASSNAME;

	HeaderCtrl();
	virtual ~HeaderCtrl();
};
