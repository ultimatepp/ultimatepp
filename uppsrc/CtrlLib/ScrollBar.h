
class ScrollBar : public FrameCtrl<Ctrl> {
	static int counter;
	static int maxcount;
public:
	struct SectionInfo;
	
	virtual Size GetStdSize() const;
	virtual void Paint(Draw& draw);
	virtual void LeftDown(Point p, dword);
	virtual void MouseMove(Point p, dword);
	virtual void MouseEnter(Point p, dword);
	virtual void MouseLeave();
	virtual void LeftUp(Point p, dword);
	virtual void LeftRepeat(Point p, dword);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void CancelMode();
	virtual void Layout();

	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);

public:
	struct Style : ChStyle<Style> {
		Color bgcolor;
		int barsize, arrowsize, thumbmin, overthumb, thumbwidth;
		bool through;
		Value vupper[4], vthumb[4], vlower[4];
		Value hupper[4], hthumb[4], hlower[4];
		Button::Style up, down, left, right;
		Button::Style up2, down2, left2, right2;
		bool          isup2, isdown2, isleft2, isright2;
	};

private:
	int     thumbpos;
	int     thumbsize;
	int     delta;
	int     pagepos;
	int     pagesize;
	int     totalsize;
	int16     linesize;
	int16     minthumb;

	int8    push;
	int8    light;
	bool    horz:1;
	bool    jump:1;
	bool    track:1;
	bool    autohide:1;
	bool    autodisable:1;
	bool    is_active:1;
	bool	disabled:1;

	const Style *style;

	void    GetSectionInfo(SectionInfo& info)const;
	
	void    Bounds();
	bool    SetThumb(int _thumbpos, int _thumbsize);
	void    Drag(Point p);

	void    Position();
	void    Uset(int a);

	int     ScrollBarSize() const           { return style->barsize; }
	

public:
	Event<>  WhenScroll;
	Event<>  WhenVisibility;
	Event<>  WhenLeftClick;

	bool    IsHorz() const                  { return horz; }
	bool    IsVert() const                  { return !horz; }

	void    Set(int pagepos, int pagesize, int totalsize);

	bool    Set(int pagepos);
	void    SetPage(int pagesize);
	void    SetTotal(int totalsize);
	
	bool    IsActive() const                { return is_active; }

	bool    ScrollInto(int pos, int linesize);
	bool    ScrollInto(int pos)             { return ScrollInto(pos, linesize); }

	bool    VertKey(dword key, bool homeend = true);
	bool    HorzKey(dword key);

	void    PrevLine();
	void    NextLine();
	void    PrevPage();
	void    NextPage();
	void    Begin();
	void    End();

	void    Wheel(int zdelta, int lines);
	void    Wheel(int zdelta);

	Size    GetViewSize() const;
	Size    GetReducedViewSize() const;

	int     Get() const                     { return pagepos; }
	int     GetPage() const                 { return pagesize; }
	int     GetTotal() const                { return totalsize; }
	int     GetLine() const                 { return linesize; }

	static const Style& StyleDefault();

	ScrollBar& Horz(bool b = true)          { horz = b; Refresh(); RefreshLayout(); return *this; }
	ScrollBar& Vert()                       { return Horz(false); }

	ScrollBar& SetLine(int _line)           { linesize = _line; return *this; }

	ScrollBar& Track(bool b = true)         { track = b; return *this; }
	ScrollBar& NoTrack()                    { return Track(false); }
	ScrollBar& Jump(bool b = true)          { jump = b; return *this; }
	ScrollBar& NoJump(bool b = true)        { return Jump(false); }
	ScrollBar& AutoHide(bool b = true);
	ScrollBar& NoAutoHide()                 { return AutoHide(false); }
	bool       IsAutoHide() const           { return autohide; }
	ScrollBar& AutoDisable(bool b = true);
	ScrollBar& NoAutoDisable()              { return AutoDisable(false); }
	ScrollBar& MinThumb(int sz)             { minthumb = sz; return *this; }
	ScrollBar& SetStyle(const Style& s);

	operator int() const                    { return pagepos; }
	int operator=(int pagepos)              { Set(pagepos); return pagepos; }

	ScrollBar();
	virtual ~ScrollBar();

	friend struct SectionInfo;
	
	struct SectionInfo{
		
		inline SectionInfo(const class ScrollBar* sb);
		
		int Width(int section)const{
			ASSERT(section>=0 && section<=6);
			return End(section)-Start(section);
		}
		int Start(int section)const{
			ASSERT(section>=0 && section<=7);
			return starts[section];
		}
		int End(int section)const{
			ASSERT(section>=0 && section<=6);
			return starts[section+1];
		}
		String ToString()const{
			String s;
			s << '[' << Width(0);
			for(int i = 1; i < 7; ++i)
			{
				s << ',' << Width(i);
			}
			s << ']';
			return s;
		}
		int WhichSection(int p)const;
		
		int WhichSection(Point p)const
		{
			return !GetAll().Contains(p) ? -1 : WhichSection(horz ? p.x : p.y);
		}
		int GetHV(int x, int y)const{ return horz ? x : y; }
		int& HV(int& x, int& y)const{ return horz ? x : y; }

		Rect GetAll()const;
		Rect Slider()const;
		Rect GetPartRect(int i)const;
		int  GetSliderSize()const { return End(4)-Start(2); }
		
		bool IsHorz()const{ return horz; }

		int starts[8];
		int wh; // width of a vertical scrollbar or height of a horz one.
		bool horz;
	};
};

inline ScrollBar::SectionInfo::SectionInfo(const ScrollBar* sb)
{
	ASSERT(sb != nullptr);
	sb->GetSectionInfo(*this);
}

inline int ScrollBarSize()                  { return ScrollBar::StyleDefault().barsize; }//!!

typedef ScrollBar VScrollBar;

// should avoid to use.
// introduced a full set of vtable for insignicant productivity gain.
class HScrollBar : public ScrollBar {
public:
	int operator=(int pagepos)              { Set(pagepos); return pagepos; }

	HScrollBar() { Horz(); }
};

class SizeGrip : public FrameRight<Ctrl> {
public:
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword);
	virtual Image CursorImage(Point p, dword);

public:
	SizeGrip();
	virtual ~SizeGrip();
};

class ScrollBars : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAdd(Ctrl& ctrl);
	virtual void FrameRemove();

protected:
	Ctrl      *box;
	ParentCtrl the_box;
	
	StaticRect box_bg;
	int        box_type;
	SizeGrip   grip;

	void    Scroll();

public:
	ScrollBar x;
	ScrollBar y;

	Event<>    WhenScroll;
	Event<>    WhenLeftClick;

	void    Set(Point pos, Size page, Size total);
	bool    Set(Point pos);
	bool    Set(int x, int y);
	void    SetPage(Size page);
	void    SetPage(int cx, int cy);
	void    SetTotal(Size total);
	void    SetTotal(int cx, int cy);

	void    SetX(int pos, int page, int total)       { x.Set(pos, page, total); }
	bool    SetX(int _x)                             { return x.Set(_x); }
	void    SetPageX(int cx)                         { x.SetPage(cx); }
	void    SetTotalX(int cx)                        { x.SetTotal(cx); }

	void    SetY(int pos, int page, int total)       { y.Set(pos, page, total); }
	bool    SetY(int _y)                             { return y.Set(_y); }
	void    SetPageY(int cy)                         { y.SetPage(cy); }
	void    SetTotalY(int cy)                        { y.SetTotal(cy); }

	bool    ScrollInto(Point pos, Size linesize);
	bool    ScrollInto(const Rect& r)                { return ScrollInto(r.TopLeft(), r.Size()); }
	bool    ScrollInto(Point pos);

	bool    ScrollIntoX(int pos, int linesize)       { return x.ScrollInto(pos, linesize); }
	bool    ScrollIntoX(int pos)                     { return x.ScrollInto(pos); }
	bool    ScrollIntoY(int pos, int linesize)       { return y.ScrollInto(pos, linesize); }
	bool    ScrollIntoY(int pos)                     { return y.ScrollInto(pos); }

	bool    Key(dword key);

	void    LineUp()                                 { y.PrevLine(); }
	void    LineDown()                               { y.NextLine(); }
	void    PageUp()                                 { y.PrevPage(); }
	void    PageDown()                               { y.NextPage(); }
	void    VertBegin()                              { y.Begin(); }
	void    VertEnd()                                { y.End(); }

	void    LineLeft()                               { x.PrevLine(); }
	void    LineRight()                              { x.NextLine(); }
	void    PageLeft()                               { x.PrevPage(); }
	void    PageRight()                              { x.NextPage(); }
	void    HorzBegin()                              { x.Begin(); }
	void    HorzEnd()                                { x.End(); }

	void    WheelX(int zdelta)                       { x.Wheel(zdelta); }
	void    WheelY(int zdelta)                       { y.Wheel(zdelta); }

	Size    GetViewSize() const;
	Size    GetReducedViewSize() const;

	Point   Get() const                              { return Point(x, y); }
	int     GetX() const                             { return x; }
	int     GetY() const                             { return y; }
	Size    GetPage() const                          { return Size(x.GetPage(), y.GetPage()); }
	Size    GetTotal() const                         { return Size(x.GetTotal(), y.GetTotal()); }
	Size    GetLine() const                          { return Size(x.GetLine(), y.GetLine()); }

	void    ShowX(bool show)                         { x.Show(show); }
	void    HideX()                                  { ShowX(false); }
	void    ShowY(bool show)                         { y.Show(show); }
	void    HideY()                                  { ShowY(false); }
	void    Show(bool show = true)                   { x.Show(show); y.Show(show); }
	void    Hide()                                   { Show(false); }

	ScrollBars& SetLine(int linex, int liney);
	ScrollBars& SetLine(Size line)                   { return SetLine(line.cx, line.cy); }
	ScrollBars& SetLine(int line)                    { return SetLine(line, line); }

	ScrollBars& Track(bool b = true);
	ScrollBars& NoTrack()                            { return Track(false); }
	ScrollBars& Jump(bool b = true);
	ScrollBars& NoJump(bool b = true)                { return Jump(false); }
	ScrollBars& AutoHide(bool b = true);
	ScrollBars& NoAutoHide()                         { return AutoHide(false); }
	ScrollBars& AutoDisable(bool b = true);
	ScrollBars& NoAutoDisable()                      { return AutoDisable(false); }

	ScrollBars& NormalBox();
	ScrollBars& NoBox();
	ScrollBars& FixedBox();

	ScrollBars& Box(Ctrl& box);
	ScrollBars& WithSizeGrip();

	ScrollBars& SetStyle(const ScrollBar::Style& s)  { x.SetStyle(s); y.SetStyle(s); return *this; }

	operator Point() const                           { return Get(); }
	Point operator=(Point p)                         { Set(p); return p; }

	ScrollBars();
	virtual ~ScrollBars();
};

class Scroller {
	Point psb;

public:
	void Scroll(Ctrl& p, const Rect& rc, Point newpos, Size cellsize = Size(1, 1));
	void Scroll(Ctrl& p, const Rect& rc, int newpos, int linesize = 1);
	void Scroll(Ctrl& p, Point newpos);
	void Scroll(Ctrl& p, int newposy);

	void Set(Point pos)          { psb = pos; }
	void Set(int pos)            { psb = Point(0, pos); }
	void Clear()                 { psb = Null; }

	Scroller()                   { psb = Null; }
};
