class Splitter : public Ctrl {
public:
	virtual void   Layout();
	virtual void   Paint(Draw& draw);
	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftDown(Point p, dword keyflags);
	virtual void   LeftUp(Point p, dword keyflags);
	virtual Image  CursorImage(Point p, dword keyflags);
	virtual void   Serialize(Stream& s);

public:
	struct Style : ChStyle<Style> {
		Value  vert[2], horz[2];
		int    width;
		bool   dots;
	};

protected: // Because of docking... (will be private)
	Vector<int> pos;
	Vector<int> mins;
	Vector<int> minpx;
	int         style;
	int         mouseindex;
	bool        vert;
	int         inset;
	const Style *chstyle;

	int         FindIndex(Point client) const;
	int         GetMins(int i) const;
	int         GetBarWidth() const;
	void        SyncMin();

	static void PaintDots(Draw& w, const Rect& r, bool vert);
	
	friend class SplitterFrame;

public:
	Event<>   WhenSplitFinish;

	void      Set(Ctrl& l, Ctrl& r);

	Splitter& SetPos(int newpos, int index = 0);
	int       GetPos(int index = 0) const          { return index < pos.GetCount() ? pos[index] : 10000; }

	int       GetCount() const                     { return GetChildCount(); }

	int       PosToClient(int pos) const;
	int       ClientToPos(Point client) const;

	void      Zoom(int i);
	void      NoZoom()                             { Zoom(-1); }
	int       GetZoom() const                      { return style; }

	void      SetMin(int i, int w)                 { mins.At(i, 0) = w; SyncMin(); }
	void      SetMinPixels(int i, int w)           { minpx.At(i, 0) = w; SyncMin(); }
	
	int       GetSplitWidth() const;

	void      Add(Ctrl& pane);
	Splitter& operator<<(Ctrl& pane)               { Add(pane); return *this; }
	void      Insert(int pos, Ctrl& pane);
	void      Remove(Ctrl& pane);
	void      Swap(Ctrl& pane, Ctrl& newpane);

	static const Style& StyleDefault();

	Splitter& Vert(Ctrl& top, Ctrl& bottom);
	Splitter& Horz(Ctrl& left, Ctrl& right);
	Splitter& Vert()                               { vert = true; Layout(); return *this; }
	Splitter& Horz()                               { vert = false; Layout(); return *this; }
	bool      IsHorz() const                       { return !vert; }
	bool      IsVert() const                       { return vert; }
	Splitter& SetStyle(const Style& s);

	void      Clear();
	void      Reset();

	Splitter();
	virtual ~Splitter();
};

class SplitterFrame : public CtrlFrame, private Ctrl {
public:
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();
	virtual void FrameAddSize(Size& sz);
	virtual void FrameLayout(Rect& r);

	virtual void Paint(Draw& draw);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);

private:
	Point ref;
	Size  parentsize;
	int   type, minsize, sizemin;
	int   size, size0;
	const Splitter::Style *style;

	int   BoundSize();

public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	void Serialize(Stream& s);

	SplitterFrame& Set(Ctrl& c, int size, int type);
	SplitterFrame& Left(Ctrl& c, int size)    { return Set(c, size, LEFT); }
	SplitterFrame& Top(Ctrl& c, int size)     { return Set(c, size, TOP); }
	SplitterFrame& Right(Ctrl& c, int size)   { return Set(c, size, RIGHT); }
	SplitterFrame& Bottom(Ctrl& c, int size)  { return Set(c, size, BOTTOM); }

	SplitterFrame& MinSize(int sz)            { minsize = sz; return *this; }
	SplitterFrame& SizeMin(int sz)            { sizemin = sz; return *this; }
	SplitterFrame& SetStyle(const Splitter::Style& s);

	int  GetType() const                      { return type; }
	int  GetSize() const                      { return size; }
	void SetSize(int sz)                      { size = sz; RefreshParentLayout(); }

	void Show(bool show = true)				  { Ctrl::Show(show); }
	void Hide()								  { Ctrl::Hide(); }
	bool IsShown()							  { return Ctrl::IsShown(); }

	SplitterFrame();
};
