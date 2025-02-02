class InfoCtrl : public FrameLR<Ctrl> {
public:
	virtual void Paint(Draw& w);
	virtual void FrameLayout(Rect& r);

private:
	struct Tab {
		PaintRect             info;
		int                   width;

		Tab() { width = 0; }
	};

	Array<Tab>   tab;
	PaintRect    temp;
	bool         right;
	String       defaulttext;
	TimeCallback temptime;

public:
	void Set(int tab, const PaintRect& info, int width);
	void Set(int tab, const Value& info, int width);
	void Set(const PaintRect& info);
	void Set(const Value& info);
	void Temporary(const PaintRect& info, int timeoout = 2000);
	void Temporary(const Value& info, int timeout = 2000);
	void EndTemporary();

	int  GetTabCount() const              { return tab.GetCount(); }
	int  GetTabOffset(int t) const;
	int  GetRealTabWidth(int tabi, int width) const;

	void operator=(const String& s)       { Set(s); }

	InfoCtrl& SetDefault(const String& d) { defaulttext = d; Refresh(); return *this; }
	InfoCtrl& Left(int w);
	InfoCtrl& Right(int w);
	InfoCtrl& LeftZ(int w)                { return Left(Zx(w)); }
	InfoCtrl& RightZ(int w)               { return Right(Zx(w)); }

	typedef InfoCtrl CLASSNAME;

	InfoCtrl();
};

class StatusBar : public InfoCtrl {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);
	virtual void Paint(Draw& w);

public:
	struct Style : public ChStyle<Style> {
		Value look;
	};

private:
	int      cy;
	SizeGrip grip;
	
	struct TopFrame : public CtrlFrame {
		virtual void FrameLayout(Rect& r);
		virtual void FramePaint(Draw& draw, const Rect& r);
		virtual void FrameAddSize(Size& sz);
		
		const Style *style;
	};
	
	TopFrame frame;

	void    SetText(const String& s)          { Set(s); }

public:
	void operator=(const String& s)           { Set(s); }

	operator Event<const String&>()       { return pteback(this, &StatusBar::SetText); }

	Event<const String&> operator~()      { return pteback(this, &StatusBar::SetText); }

	StatusBar&  Height(int _cy);
	StatusBar&  NoSizeGrip()                  { RemoveFrame(grip); return *this; }

	static const Style& StyleDefault();

	InfoCtrl& SetStyle(const Style& s)        { frame.style = &s; Refresh(); return *this; }

	StatusBar();
	~StatusBar();
};


Display& ProgressDisplay();

class ProgressInfo {
	InfoCtrl *info;
	String    text;
	int       tw;
	int       tabi;
	int       cx;
	int       total;
	int       pos;
	
	int       granularity;
	dword     set_time;

	void Refresh();
	void Reset();

public:
	ProgressInfo& Text(const String& s)         { text = s; Refresh(); return *this; }
	ProgressInfo& TextWidth(int cx)             { tw = cx; Refresh(); return *this; }
	ProgressInfo& Width(int _cx)                { cx = _cx; Refresh(); return *this;  }
	ProgressInfo& Placement(int _tabi)          { tabi = _tabi; Refresh(); return *this; }
	ProgressInfo& Info(InfoCtrl& _info)         { info = &_info; info->EndTemporary(); Refresh(); return *this; }
	ProgressInfo& Total(int _total)             { total = _total; Refresh(); return *this; }

	ProgressInfo& Set(int _pos, int _total);

	void          Set(int _pos)                 { Set(_pos, total); }
	int           Get() const                   { return pos; }
	int           GetTotal() const              { return total; }

	void operator=(int p)                       { Set(p); }
	void operator++()                           { Set(pos + 1, total); }
	operator int()                              { return pos; }

	ProgressInfo()                              { Reset(); }
	ProgressInfo(InfoCtrl& f)                   { Reset(); Info(f); }
	~ProgressInfo();
};
