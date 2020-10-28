class ColumnList : public Ctrl, private CtrlFrame {
public:
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual Image CursorImage(Point p, dword);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftUp(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  RightDown(Point p, dword);
	virtual void  LeftDrag(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword);
	virtual void  MouseLeave();
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool  Key(dword key, int count);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  CancelMode();
	virtual void  DragEnter();
	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();
	virtual Value GetData() const;
	virtual void  SetData(const Value& key);

private:
	virtual void  FrameLayout(Rect& r);
	virtual void  FrameAddSize(Size& sz);
	virtual void  FramePaint(Draw& draw, const Rect& r);

private:
	int        ncl;
	int        cx, cy;
	int        cursor, anchor;
	int        dx;
	int        ci;
	int        mpos;
	ScrollBar  sb;
	Scroller   scroller;
	CtrlFrame *frame;
	int        dropitem;
	bool       insert;
	bool       clickkill;
	bool       nobg;
	bool       popupex;
	bool       selclick;
	int        mode;

	DisplayPopup info;

	const Display *display;

	struct Item {
		bool           sel;
		bool           canselect;
		Value          key;
		Value          value;
		const Display *display;
	};

	Array<Item> item;
	int         selcount;
	bool        multi;

	struct  ItemOrder;
	friend struct ItemOrder;

	void    SetSb();
	void    Scroll();
	int     GetDragColumn(int x) const;
	int     RoundedCy();
	void    Page(bool down);
	void    PointDown(Point p);
	void    DoClick(Point p, dword flags);
	void    ShiftSelect();
	void    RefreshItem(int i, int ex = 0);
	void    RefreshCursor()                    { RefreshItem(cursor); }
	void    GetItemStyle(int i, Color& ink, Color& paper, dword& style);
	dword   PaintItem(Draw& w, int i, const Rect& r);
	void    SyncInfo();
	void    SetCursor0(int c, bool sel);
	void    UpdateSelect();
	void    RefreshSel();
	void    DoLeftDown(Point p, dword);
	dword   SwapKey(dword key);
	void    PaintRows(Draw &w, Size &sz);
	int     GetSbPos(const Size &sz) const;
	void    ScrollInto(int pos);

	bool    DnDInsert(int i, int py, PasteClip& d, int q);
	void    DnD(int _drop, bool _insert);
	
	friend class FileList;

public:
	enum {
		MODE_LIST,
		MODE_COLUMN,
		MODE_ROWS
	};

	Event<>      WhenLeftClick;
	Event<Point> WhenLeftClickPos;
	Event<>      WhenLeftDouble;
	Event<Bar&>  WhenBar;
	Event<>      WhenSel;

	Event<>                  WhenDrag;
	Event<int, PasteClip&>   WhenDropItem;
	Event<int, PasteClip&>   WhenDropInsert;
	Event<PasteClip&>        WhenDrop;

	// deprecated - use WhenSel
	Event<>          WhenSelection;
	Event<>          WhenEnterItem;
	Event<>          WhenKillCursor;

	int     GetColumnItems() const;
	int     GetColumnCx(int i = 0) const;
	int     GetPageItems() const;

	int     GetItem(Point p);
	Rect    GetItemRect(int i) const;

	int     GetCursor() const                          { return cursor; }
	void    SetCursor(int c);
	void    KillCursor();
	bool    IsCursor() const                           { return cursor >= 0; }

	int     GetSbPos() const                           { return GetSbPos(GetSize()); }
	void    SetSbPos(int y);
	
	int     GetScroll() const                          { return sb; }
	void    ScrollTo(int a)                            { sb.Set(a); }

	void    SetFrame(CtrlFrame& frame);

	void         Clear();
	void         Add(const Value& val, bool canselect = true);
	void         Add(const Value& val, const Display& display, bool canselect = true);
	void         Add(const Value& key, const Value& val, bool canselect = true);
	void         Add(const Value& key, const Value& val, const Display& display, bool canselect = true);

	int          GetCount() const                     { return item.GetCount(); }
	const Value& Get(int i) const                     { return item[i].key; }
	const Value& GetValue(int i) const                { return item[i].value; }
	const Value& operator[](int i) const              { return item[i].key; }

	void         Set(int ii, const Value& key, const Value& val, bool canselect = true);
	void         Set(int ii, const Value& key, const Value& val, const Display& display, bool canselect = true);
	void         Set(int ii, const Value& val, bool canselect = true);
	void         Set(int ii, const Value& val, const Display& display, bool canselect = true);

	void         Set(const Value& key, const Value& val, const Display& display, bool canselect = true);
	void         Set(const Value& key, const Value& val, bool canselect = true);

	void         Insert(int ii, const Value& val, bool canselect = true);
	void         Insert(int ii, const Value& val, const Display& display, bool canselect = true);
	void         Insert(int ii, const Value& key, const Value& val, bool canselect = true);
	void         Insert(int ii, const Value& key, const Value& val, const Display& display, bool canselect = true);
	void         Remove(int ii);
	void         Remove(const Value & key)			  { int ii = Find(key); if (ii >= 0) Remove(ii); }

	void         RemoveSelection();

	int          GetSelectCount() const               { return selcount; }
	bool         IsSelection() const                  { return selcount > 0; }
	void         ClearSelection();
	void         SelectOne(int i, bool sel);
	bool         IsSelected(int i) const;
	bool         IsSel(int i) const;

	int          Find(const Value& key) const;

	void         Sort(const ValueOrder& order);

	Image        GetDragSample();

	void         InsertDrop(int ii, const Vector<Value>& data, PasteClip& d, bool self);
	void         InsertDrop(int ii, const Vector<Value>& keys, const Vector<Value>& data, PasteClip& d, bool self);
	void         InsertDrop(int ii, const ColumnList& src, PasteClip& d);
	void         InsertDrop(int ii, PasteClip& d);

	void         SerializeSettings(Stream& s);

	ColumnList&  Mode(int m);
	ColumnList&  ListMode()                           { return Mode(MODE_LIST); }
	ColumnList&  RowMode()                            { return Mode(MODE_ROWS); }
	ColumnList&  ColumnMode()                         { return Mode(MODE_COLUMN); }
	ColumnList&  Columns(int _n)                      { ncl = _n; Refresh(); return *this; }
	int          GetColumns() const                   { return ncl; }
	ColumnList&  ItemHeight(int _cy)                  { cy = _cy; RefreshLayout(); SetSb(); Refresh(); return *this; }
	int          GetItemHeight() const                { return cy; }
	ColumnList&  ItemWidth(int _cx)                   { cx = _cx; RefreshLayout(); SetSb(); Refresh(); return *this; }
	int          GetItemWidth() const                 { return cx; }
	ColumnList&  RoundSize(bool b = true);
	ColumnList&  NoRoundSize()                        { return RoundSize(false); }
	ColumnList&  ClickKill(bool b = true)             { clickkill = b; return *this; }
	ColumnList&  NoClickKill()                        { return ClickKill(false); }
	ColumnList&  SetDisplay(const Display& d)         { display = &d; return *this; }
	ColumnList&  NoBackground(bool b = true)          { nobg = b; Transparent(); Refresh(); return *this; }
	ColumnList&  Multi(bool b = true)                 { multi = b; return *this; }
	bool         IsMulti() const                      { return multi; }
	ColumnList&  MultiSelect(bool b = true)           { multi = b; return *this; }
	bool         IsMultiSelect() const                { return multi; }
	ColumnList&  PopUpEx(bool b = true)               { popupex = b; return *this; }
	ColumnList&  NoPopUpEx()                          { return PopUpEx(false); }
	ColumnList&  AutoHideSb(bool b = true)            { sb.AutoHide(b); return *this; }
	ColumnList&  NoAutoHideSb()                       { return AutoHideSb(false); }

	ColumnList&  SetScrollBarStyle(const ScrollBar::Style& s) { sb.SetStyle(s); return *this; }

	typedef ColumnList CLASSNAME;

	ColumnList();
	virtual ~ColumnList();
};
