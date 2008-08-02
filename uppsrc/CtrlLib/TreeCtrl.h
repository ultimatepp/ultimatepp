class TreeCtrl : public Ctrl {
public:
	virtual void  CancelMode();
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual void  LeftDown(Point p, dword flags);
	virtual void  LeftUp(Point p, dword flags);
	virtual void  LeftDouble(Point p, dword flags);
	virtual void  LeftDrag(Point p, dword keyflags);
	virtual void  RightDown(Point p, dword flags);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void  MouseMove(Point, dword);
	virtual bool  Key(dword key, int);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  ChildGotFocus();
	virtual void  ChildRemoved(Ctrl *);
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;
	virtual void  DragEnter();
	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();

public:
	class Node
	{
		void           Init() { display = NULL; size = Null; margin = 2; canopen = false; canselect = true; }

	public:
		Image          image;
		int            margin;
		Value          key;
		Value          value;
		const Display *display;
		Size           size;
		Ptr<Ctrl>      ctrl;
		bool           canopen;
		bool           canselect;

		Node& SetImage(const Image& img)          { image = img; return *this; }
		Node& Set(const Value& v)                 { key = value = v; return *this; }
		Node& Set(const Value& v, const Value& t) { key = v; value = t; return *this; }
		Node& SetDisplay(const Display& d)        { display = &d; return *this; }
		Node& SetSize(Size sz)                    { size = sz; return *this; }
		Node& SetCtrl(Ctrl& _ctrl)                { ctrl = &_ctrl; return *this; }
		Node& CanOpen(bool b = true)              { canopen = b; return *this; }
		Node& CanSelect(bool b)                   { canselect = b; return *this; }

		Node();
		Node(const Image& img, const Value& v);
		Node(const Image& img, const Value& v, const Value& t);
		Node(const Value& v);
		Node(const Value& v, const Value& t);
		Node(Ctrl& ctrl);
		Node(const Image& img, Ctrl& ctrl, int cx = 0, int cy = 0);
	};

private:
	struct Item : Node {
		union {
			int            parent;
			int            freelink;
		};

		bool           free;
		bool           isopen;
		bool           sel;
		Vector<int>    child;
		int            linei;

		Size GetValueSize() const;
		Size GetCtrlSize() const;
		Size GetSize() const;

		Item() { isopen = false; linei = -1; parent = -1; canselect = true; sel = false; free = false; }
	};
	struct Line : Moveable<Line> {
		int  level;
		int  itemi;
		int  ll;
		int  y;
	};

	Array<Item>  item;
	Vector<Line> line;
	int          cursor;
	int          freelist;
	int          levelcx;
	int          anchor;
	int          selectcount;
	bool         nocursor;
	bool         noroot;
	bool         dirty;
	bool         hasctrls;
	bool         multiselect;
	bool         nobg;
	bool         popupex;
	bool         chldlck;
	bool         mousemove;
	bool         accel;

	bool         selclick;
	int          dropitem, dropinsert;
	Point        repoint;
	int          retime;
	Size         treesize;

	ScrollBars   sb;
	Scroller     scroller;
	Display     *display;

	DisplayPopup info;

	struct LineLess {
		bool   operator()(int y, const Line& l) const            { return y < l.y; }
	};

	struct SortOrder;

	const Display *GetStyle(int i, Color& fg, Color& bg, dword& st);
	void   Dirty(int id = 0);
	void   ReLine(int, int, Size&);
	void   RemoveSubtree(int id);
	void   Scroll();
	int    FindLine(int y) const;
	void   RefreshLine(int i, int ex = 0);
	void   RefreshItem(int id, int ex = 0);
	void   SetCursorLineSync(int i);
	void   MoveCursorLine(int c, int incr);
	void   SetCursorLine(int i, bool sc, bool sel, bool cb);
	void   SetCursor(int id, bool sc, bool sel, bool cb);
	void   RemoveCtrls(int itemi);
	void   SyncCtrls(bool add, Ctrl *restorefocus);
	bool   Tab(int d);
	void   Sort0(int id, const ValueOrder& order, bool byvalue);
	void   SortDeep0(int id, const ValueOrder& order, bool byvalue);
	void   GatherOpened(int id, Vector<int>& o);
	void   SelClear(int id);
	void   UpdateSelect();
	void   ShiftSelect(int l1, int l2);
	void   SelectOne0(int id, bool sel, bool cb = false);
	void   DnD(int itemid, int insert);
	bool   DnDInserti(int ii, PasteClip& d, bool bottom);
	bool   DnDInsert(int ii, int py, int q, PasteClip& d);
	void   RefreshSel();
	void   GatherSel(int id, Vector<int>& sel) const;
	void   DoClick(Point p, dword flags, bool down);
	void   SyncInfo();

protected:
	virtual void SetOption(int id);
	void         SyncTree();

public:
	Callback1<int>  WhenOpen;
	Callback1<int>  WhenClose;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;
	Callback        WhenSel;

	Callback                        WhenDrag;
	Callback2<int, PasteClip&>      WhenDropItem;
	Callback3<int, int, PasteClip&> WhenDropInsert;
	Callback1<PasteClip&>           WhenDrop;

	// deprecated - use WhenSel
	Callback        WhenCursor;
	Callback        WhenSelection;

	void   SetRoot(const Node& n);
	void   SetRoot(const Image& img, Value v);
	void   SetRoot(const Image& img, Value v, Value t);
	void   SetRoot(const Image& img, Ctrl& ctrl, int cx = 0, int cy = 0);
	int    Insert(int parentid, int i, const Node& n);
	int    Insert(int parentid, int i);
	int    Insert(int parentid, int i, const Image& img, Value value, bool withopen = false);
	int    Insert(int parentid, int i, const Image& img, Value key, Value value, bool withopen = false);
	int    Insert(int parentid, int i, const Image& img, Ctrl& c, int cx = 0, int cy = 0, bool wo = false);
	int    Add(int parentid, const Node& n);
	int    Add(int parentid);
	int    Add(int parentid, const Image& img, Value value, bool withopen = false);
	int    Add(int parentid, const Image& img, Value key, Value value, bool withopen = false);
	int    Add(int parentid, const Image& img, Ctrl& ctrl, int cx = 0, int cy = 0, bool withopen = false);
	void   Remove(int id);
	void   RemoveChildren(int id);

	void   Swap(int id1, int id2);
	void   SwapChildren(int parentid, int i1, int i2);

	int    GetChildCount(int id) const                         { return item[id].child.GetCount(); }
	int    GetChild(int id, int i) const                       { return item[id].child[i]; }
	int    GetChildIndex(int id, int childid) const;
	int    GetParent(int id) const                             { return item[id].parent; }

	Value  Get(int id) const;
	Value  GetValue(int id) const;
	Value  operator[](int id) const                            { return Get(id); }
	void   Set(int id, Value value);
	void   Set(int id, Value key, Value value);

	int    GetLineCount();
	int    GetItemAtLine(int i);
	int    GetLineAtItem(int id);

	Node   GetNode(int id) const                               { return item[id]; }
	void   SetNode(int id, const Node& n);

	bool   IsOpen(int id) const;
	void   Open(int id, bool open = true);
	void   Close(int id)                                       { Open(id, false); }

	void   OpenDeep(int id, bool open = true);
	void   CloseDeep(int id)                                   { OpenDeep(id, false); }

	void   MakeVisible(int id);

	void   SetCursorLine(int i);
	int    GetCursorLine() const                               { return cursor; }
	void   KillCursor();
	void   SetCursor(int id);
	int    GetCursor() const;
	bool   IsCursor() const                                    { return GetCursor() >= 0; }

	Point  GetScroll() const;
	void   ScrollTo(Point sc);

	Value  Get() const;
	Value  GetValue() const;

	int    Find(Value key);
	bool   FindSetCursor(Value key);

	void   Sort(int id, const ValueOrder& order, bool byvalue = false);
	void   SortDeep(int id, const ValueOrder& order, bool byvalue = false);

	void   Sort(int id, int (*compare)(const Value& v1, const Value& v2) = StdValueCompare,
	            bool byvalue = false);
	void   SortDeep(int id, int (*compare)(const Value& v1, const Value& v2) = StdValueCompare,
	                bool byvalue = false);

	void   SortByValue(int id, const ValueOrder& order);
	void   SortDeepByValue(int id, const ValueOrder& order);

	void   SortByValue(int id, int (*compare)(const Value& v1, const Value& v2) = StdValueCompare);
	void   SortDeepByValue(int id, int (*compare)(const Value& v1, const Value& v2) = StdValueCompare);

	void   Clear();

	void         ClearSelection();
	void         SelectOne(int id, bool sel = true);
	bool         IsSelected(int id) const       { return item[id].sel; }
	bool         IsSel(int id) const;
	int          GetSelectCount() const         { return selectcount; }
	bool         IsSelDeep(int id) const;
	Vector<int>  GetSel() const;
	void         Remove(const Vector<int>& id);
	void         RemoveSelection()              { Remove(GetSel()); }

	void         AdjustAction(int parent, PasteClip& x);
	void         InsertDrop(int parent, int ii, const TreeCtrl& src, PasteClip& x);
	void         InsertDrop(int parent, int ii, PasteClip& d);
	Image        GetDragSample();

	void         StdLeftDouble();

	void         ScrollIntoLine(int i);
	void         CenterLine(int i);

	void         ScrollIntoCursor();
	void         CenterCursor();

	Size         GetTreeSize() const         { return treesize; }

	void   Dump();

	TreeCtrl& NoCursor(bool b = true)        { nocursor = b; if(b) KillCursor(); return *this; }
	TreeCtrl& NoRoot(bool b = true)          { noroot = b; Dirty(); Refresh(); return *this; }
	TreeCtrl& LevelCx(int cx)                { levelcx = cx; Dirty(); return *this; }
	TreeCtrl& MultiSelect(bool b = true)     { multiselect = b; return *this; }
	TreeCtrl& NoBackground(bool b = true)    { nobg = b; Transparent(); Refresh(); return *this; }
	TreeCtrl& PopUpEx(bool b = true)         { popupex = b; return *this; }
	TreeCtrl& NoPopUpEx()                    { return PopUpEx(false); }
	TreeCtrl& MouseMoveCursor(bool m = true) { mousemove = m; return *this; }
	TreeCtrl& Accel(bool a = true)           { accel = a; return *this; }

	TreeCtrl& SetScrollBarStyle(const ScrollBar::Style& s) { sb.SetStyle(s); return *this; }

	typedef TreeCtrl CLASSNAME;

	TreeCtrl();
	~TreeCtrl();
};

int Copy(TreeCtrl& dst, int dstid, int i, const TreeCtrl& src, int srcid);

class OptionTree : public TreeCtrl {
	Vector<Option *> option;
	Array<Option>    aux;
	bool             manualmode;

	void SetOption(int i);
	void SetChildren(int id, bool b);

public:
	Callback WhenOption;

	void SetRoot(const Image& img, Option& option, const char *text = NULL);
	void SetRoot(Option& option, const char *text = NULL);
	void SetRoot(const Image& img, const char *text);
	void SetRoot(const char *text);
	int  Insert(int parent, int i, const Image& img, Option& option, const char *text = NULL);
	int  Insert(int parent, int i, Option& option, const char *text = NULL);
	int  Insert(int parent, int i, const Image& img, const char *text);
	int  Insert(int parent, int i, const char *text);
	int  Add(int parent, const Image& img, Option& option, const char *text = NULL);
	int  Add(int parent, Option& option, const char *text = NULL);
	int  Add(int parent, const Image& img, const char *text);
	int  Add(int parent, const char *text);
	
	void   SetLabel(int id, const char *text);
	String GetLabel(int id) const                   { return option[id]->GetLabel(); }

	int  Get(int id) const                          { return option[id]->Get(); }

	void Clear();

	typedef OptionTree CLASSNAME;

	OptionTree& ManualMode() 						{ manualmode = true; return *this; }

	OptionTree();
	~OptionTree();
};

class PopUpTree : public TreeCtrl {
public:
	typedef PopUpTree CLASSNAME;
	PopUpTree();
	virtual ~PopUpTree();

	void         PopUp(Ctrl *owner, int x, int top, int bottom, int width);
	void         PopUp(Ctrl *owner, int width);
	void         PopUp(Ctrl *owner);

	Callback     WhenCancel;
	Callback     WhenSelect;

	PopUpTree&   MaxHeight(int maxheight_)          { maxheight = maxheight_; return *this; }
	int          GetMaxHeight() const               { return maxheight; }

public:
	virtual void Deactivate();
	virtual void LeftUp(Point p, dword keyflags);
	virtual bool Key(dword key, int);
	virtual void CancelMode();

protected:
	void         DoClose();
	void         OpenClose(int i);

protected:
	int          maxheight;
	Point        showpos;
	int          showwidth;
	bool         up;
	bool         open;
};
