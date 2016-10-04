class UTLCtrl : public Ctrl {
public:
   // typedef UTLCtrl CLASSNAME;
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual void  LeftDown(Point p, dword flags);
	virtual void  LeftDouble(Point p, dword flags);
	virtual void  RightDown(Point p, dword flags);
	virtual void  MouseMove(Point p, dword keyflags);
    virtual Image CursorImage(Point p, dword keyflags);
	virtual void  MouseLeave();
	virtual void  LeftUp(Point, dword);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool  Key(dword key, int);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  ChildGotFocus();
	virtual void  ChildRemoved(Ctrl *);
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;

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
		Array<String>  subItemText;//sub item text description,by Ulti

		bool           canopen;
		bool           canselect;

		Node& SetImage(const Image& img)          { image = img; return *this; }
		Node& Set(const Value& v)                 { key = value = v;  return *this; }
		Node& Set(const Value& v, const Value& t) { key = v; value = t; return *this; }
		Node& SetDisplay(const Display& d)        { display = &d;  return *this; }
		Node& SetSize(Size sz)                    { size = sz; return *this; }
		Node& CanOpen(bool b = true)              { canopen = b; return *this; }
		Node& CanSelect(bool b)                   { canselect = b; return *this; }

		Node();
		Node(const Image& img, const Value& v);
		Node(const Image& img, const Value& v, const Value& t);
		Node(const Value& v);
		Node(const Value& v, const Value& t);
	};

private:
	struct Item : Node {
		union {
			int            parent;
			int            freelink;
		};

		bool           isopen;
		bool           sel;
		Vector<int>    child;
		int            linei;

		Size GetValueSize() const;
		Size GetSize() const;

		Item() { isopen = false; linei = -1; parent = -1; canselect = true; sel = false; }
	};
	struct Line : Moveable<Line> {
		int  level;
		int  itemi;
		int  ll;
	};

	Array<Item>    item;
	Vector<Line>   line;
	Mitor<int>     colWidth;//Column width array,by Ulti
	Array<String>  colTitle;//Column head text array,by Ulti
	int            colCnt;//Number of Columns,by Ulti
	int            rowHeight:20;//Height of a Row,by Ulti
	int            dragPtx;//dragging point,by Ulti
	int            dragCol;
	bool           isdrag;//is dragging or not,by Ulti
	int            cursor;
	int            freelist;
	int            levelcx;
	bool           nocursor;
	bool           dirty;
	bool           multiselect;
	bool           isselection;
	ScrollBars     sb;
	Scroller       scroller;
	Display        *display;


	void   Dirty(int id = 0);

	void   ReLine(int, int);
	void   SyncTree();
	void   RemoveSubtree(int id);
	void   Scroll();
	int    FindLine(int y) const;
	int    FindCol();
	void   RefreshLine(int i);
	void   RefreshItem(int id);
	void   SetCursorLineSync(int i);
	void   SetCursorLine(int i, bool scroll);
	void   SetCursor(int id, bool scroll);

	bool   Tab(int d);

	void   GatherOpened(int id, Vector<int>& o);
	void   UpdateSelect();
	void   ClearSelTree(int id);
	bool   UpdateSelTree(int id);
	void   ShiftSelect(int l1, int l2);
	bool   ShowDragArrow(Point p);
	void   SetSBSize();
    int    RMargin;
public:
	Callback1<int>  WhenOpen;
	Callback1<int>  WhenClose;
	Callback        WhenCursor;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;
	Callback        WhenSelection;

	void   SetRoot(const Node& n);
	void   SetRoot(const Image& img, Value v);
	void   SetRoot(const Image& img, Value v, Value t);

	int    Insert(int parent, int i, const Node& n);
	int    Insert(int parent, int i);
	int    Insert(int parent, int i, const Image& img, Value value, bool withopen = false);
	int    Insert(int parent, int i, const Image& img, Value key, Value value, bool withopen = false);

	int    Add(int parent, const Node& n);
	int    Add(int parent);
	int    Add(int parent, const Image& img, Value value, bool withopen = false);
	int    Add(int parent, const Image& img, Value key, Value value, bool withopen = false);

	void   Remove(int id);
	void   RemoveChildren(int id);

	int    GetChildCount(int id) const                         { return item[id].child.GetCount(); }
	int    GetChild(int id, int i) const                       { return item[id].child[i]; }
	int    GetParent(int id) const                             { return item[id].parent; }

	Value  Get(int id) const;
	Value  GetValue(int id) const;
	Value  operator[](int id) const                            { return Get(id); }
	void   Set(int id, Value value);
	void   Set(int id, Value key, Value value);

	int    GetLineCount();
	int    GetItemAtLine(int i);
	int    GetLineAtItem(int id);
    int    GetLineLevel(int i);
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
    int    GetLevel(int id);
	Point  GetScroll() const;
	void   ScrollTo(Point sc);

	Value  Get() const;
	Value  GetValue() const;

	int    Find(Value key);
	bool   FindSetCursor(Value key);


	void   Clear();

	bool         IsSelection() const            { return isselection; }
	void         ClearSelection();
	void         SelectOne(int id, bool sel);
	bool         IsSelected(int id) const       { return item[id].sel; }
    int    GetWidth();
    int    GetWidth(int col);
    int    GetActureWidth(int col);

    void   SetColCount(int cnt);
    void   SetColWidth(int col,int width);
    void   SetTitle(int col,String title);
    void   SetSubText(int id,int sub,String txt);
	void   Dump();

	UTLCtrl& NoCursor(bool b = true)    { nocursor = b; if(b) KillCursor(); return *this; }
	UTLCtrl& MultiSelect(bool b = true) { multiselect = true; return *this; }

	typedef UTLCtrl CLASSNAME;

	UTLCtrl();
	~UTLCtrl();
};
