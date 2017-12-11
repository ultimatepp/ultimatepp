class TabCtrl : public Ctrl {
public:
	virtual bool  Accept();
	virtual void  Paint(Draw& draw);
	virtual void  CancelMode();
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MouseLeave();
	virtual bool  Key(dword key, int count);
	virtual bool  HotKey(dword key);
	virtual void  Layout();
	virtual Rect  GetOpaqueRect() const;
	virtual Value GetData() const;
	virtual void  SetData(const Value& data);

public:
	class Item {
		TabCtrl  *owner;

		int       x;
		Point     pictpos;
		Point     textpos;
		int       cx;

		String    text;
		PaintRect pict;
		Ptr<Ctrl> ctrl;
		bool      enabled;
		Ptr<Ctrl> slave;
		dword     key;

		friend class TabCtrl;

		void Layout(int x, int y, int cy);
		void Paint(Draw& w, int state);
		int  Right() const { return x + cx; }

	public:
		Item&          Text(const String& _text);
		Item&          Picture(PaintRect d);
		Item&          SetImage(const UPP::Image& _im)  { return Picture(PaintRect(ImageDisplay(), _im)); }
		Item&          SetCtrl(Ctrl *_ctrl);
		Item&          SetCtrl(Ctrl& c)                 { return SetCtrl(&c); }
		Item&          Slave(Ctrl *_slave);
		Item&          Key(dword _key)                  { key = _key; return *this; }

		Item&          Enable(bool _en = true);
		Item&          Disable()                        { return Enable(false); }
		bool           IsEnabled() const                { return enabled; }
		Ctrl          *GetSlave()                       { return slave; }
		const Ctrl    *GetSlave() const                 { return slave; }
		Ctrl          *GetCtrl()                        { return ctrl; }
		const Ctrl    *GetCtrl() const                  { return ctrl; }
		String         GetText() const                  { return text; }
		PaintRect      GetPicture() const               { return pict; }

		Item();

	//deprecated:
		Item&          Control(Ctrl *c)                 { return SetCtrl(c); }
		Item&          Image(const UPP::Image& m)       { return SetImage(m); }
	};

	struct Style : ChStyle<Style> {
		int tabheight, margin, extendleft;
		Rect sel, edge;
		Value normal[4], first[4], last[4], both[4], body;
		Color text_color[4];
		Font font;
	};

private:
	struct Tabs : public Ctrl {
		virtual void Paint(Draw& w);
	};

	Array<Item> tab;
	int         x0;
	int         hot;
	int         sel;
	Tabs        tabs;
	Button      left, right;
	ParentCtrl  pane;
	bool        accept_current, no_accept;

	const Style *style;

	static Image Fade(int i);

	void       PaintTabs(Draw& w);
	void       ScrollInto(int i);
	void       Left();
	void       Right();
	void       SyncHot();
	void       SyncTabs();
	int        TabsRight();
	void       Go(int d);
	int        FindInsert(Ctrl& slave);

public:
	Event<>  WhenSet;

	TabCtrl::Item& Add();
	TabCtrl::Item& Add(const char *text);
	TabCtrl::Item& Add(const Image& m, const char *text);
	TabCtrl::Item& Add(Ctrl& slave, const char *text);
	TabCtrl::Item& Add(Ctrl& slave, const Image& m, const char *text);

	TabCtrl::Item& Insert(int i);
	TabCtrl::Item& Insert(int i, const char *text);
	TabCtrl::Item& Insert(int i, const Image& m, const char *text);
	TabCtrl::Item& Insert(int i, Ctrl& slave, const char *text);
	TabCtrl::Item& Insert(int i, Ctrl& slave, const Image& m, const char *text);

	void  Remove(int i);

	int   GetTab(Point p) const;

	int   GetCount() const                       { return tab.GetCount(); }
	Item& GetItem(int i)                         { return tab[i]; }
	const Item& GetItem(int i) const             { return tab[i]; }

	void Set(int i);
	int  Get() const                             { return sel; }

	int  Find(const Ctrl& slave) const;
	void Set(Ctrl& slave);
	bool IsAt(Ctrl& slave)                       { return Get() == Find(slave); }

	TabCtrl::Item& Insert(Ctrl& before_slave);
	TabCtrl::Item& Insert(Ctrl& before_slave, const char *text);
	TabCtrl::Item& Insert(Ctrl& before_slave, const Image& m, const char *text);
	TabCtrl::Item& Insert(Ctrl& before_slave, Ctrl& slave, const char *text);
	TabCtrl::Item& Insert(Ctrl& before_slave, Ctrl& slave, const Image& m, const char *text);

	void  Remove(Ctrl& slave);

	void GoNext()                                { Go(1); }
	void GoPrev()                                { Go(-1); }

	Size     ComputeSize(Size pane);
	Size     ComputeSize();
	void     Add(Ctrl& c)                        { pane.Add(c.SizePos()); }
//	TabCtrl& operator<<(Ctrl& c)                 { Add(c); return *this; } // ambiguos with operator<<(lambda)

	static const Style& StyleDefault();

	TabCtrl& NoAccept(bool ac = true)            { no_accept = ac; return *this; }
	TabCtrl& AcceptCurrent(bool ac = true)       { accept_current = ac; return *this; }
	TabCtrl& AcceptAll()                         { return AcceptCurrent(false); }
	TabCtrl& SetStyle(const Style& s)            { style = &s; Refresh(); return *this; }

	void Reset();

	typedef TabCtrl CLASSNAME;

	TabCtrl();
};

class TabDlg : public TopWindow {
	void    Rearrange();

	bool    binit;
	Size    sz;

	void           PlaceButton(Button& b, int& r);
	TabCtrl::Item& Add0(Ctrl& tab, const char *text);
	TabDlg&        AButton(Button& b);

public:
	TabCtrl  tabctrl;
	Button   ok;
	Button   cancel;
	Button   exit;
	Button   apply;

	template <class T>
	TabCtrl::Item& Add(T& tab, const char *text)                   { CtrlLayout(tab); return Add0(tab, text); }
	template <class T>
	TabCtrl::Item& Add(T& tab, const Image& img, const char *name) { return Add(tab, name).Image(img); }
	template <class T>
	TabDlg&  operator()(T& tab, const char *text)                  { Add(tab, text); return *this; }
	template <class T>
	TabDlg&  operator()(T& tab, const Image& img, const char *txt) { Add(tab, img, txt); return *this; }

	TabDlg&  OK()                                                  { return AButton(ok); }
	TabDlg&  Cancel()                                              { return AButton(cancel); }
	TabDlg&  OKCancel()                                            { return OK().Cancel(); }
	TabDlg&  Exit()                                                { return AButton(exit); }
	TabDlg&  Apply()                                               { return AButton(apply); }

	TabDlg();
};
