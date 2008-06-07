#ifndef __INCLUDE_CTRL_TREECTRL_H__
#define __INCLUDE_CTRL_TREECTRL_H__

NAMESPACE_UPP

class OldTreeEdit;
class OldTreeCursor;
class OldTreeItem;
class OldTreeCtrl;

struct SimpleTreeItem
{
	friend class OldTreeItem;
	friend class OldTreeCtrl;

	SimpleTreeItem();

	enum TYPE { TEXT, PICT, CTRL };
	enum HITTEST
	{
		HNIL    = 0x00,
		HBUTTON = 0x01,
		HLEAF   = 0x02,
		HICON   = 0x04,
		HTEXT   = 0x08,
		HPICT   = 0x10,
		HCTRL   = 0x20,
		HOTHER  = 0x40,
		HALL    = HBUTTON | HLEAF | HICON | HTEXT | HPICT | HCTRL | HOTHER,
	};

protected:
	OldTreeCtrl *owner;
	OldTreeItem *parent;
	int       indent;
	Size      item_size;
	Size      tree_size; // size including children - set by tree control
	int       item_class; // user set item class
	unsigned  type        : 4; // TYPE
	bool      up_to_date  : 1;
	bool      is_static   : 1;
	bool      open        : 1;
	bool      enabled     : 1;
	bool      layout      : 1;
	bool      branch      : 1;
	bool      leaf        : 1;
	bool      selected    : 1;
	bool      auto_size   : 1;
	bool      auto_indent : 1;
	union
	{
		const Display *picture; // PICT
		Ctrl          *ctrl;    // CTRL
	};
};

class OldTreeItem : public SimpleTreeItem, Moveable<OldTreeItem>, DeepCopyOption<OldTreeItem>
{
	friend class OldTreeCtrl;
	friend class OldTreeCursor;
	friend class OldTreeEdit;

public:
	typedef Callback1<OldTreeItem&> Proc;
	typedef Array<OldTreeItem>      List;

	OldTreeItem();
	OldTreeItem(const OldTreeItem&, int); // deep copy

	TYPE          GetType() const              { return (TYPE)type; }

	void          SetCursor();

	OldTreeItem&     Open(bool _open = true);
	OldTreeItem&     Close()                      { return Open(false); }
	bool          IsOpenItem() const           { return open; }

	OldTreeItem&     Static(bool _static = true)  { is_static = _static; return *this; }
	OldTreeItem&     Dynamic()                    { return Static(false); }
	bool          IsStatic() const             { return is_static; }

	OldTreeItem&     UpToDate(bool _utd = true)   { up_to_date = _utd; return *this; }
	OldTreeItem&     Reopen()                     { return UpToDate(false); }
	bool          IsUpToDate() const           { return up_to_date; }
	bool          IsDirty() const              { return !up_to_date; }

	OldTreeItem&     Branch(bool _branch = true)  { branch = _branch; return *this; }
	OldTreeItem&     NoBranch()                   { return Branch(false); }
	bool          IsBranch() const             { return branch; }

	OldTreeItem&     Leaf(bool _leaf = true)      { leaf = _leaf; return *this; }
	OldTreeItem&     NoLeaf()                     { return Leaf(false); }
	bool          IsLeaf() const               { return leaf; }

	OldTreeItem&     Select(bool _sel = true); // accepts NULL this
	OldTreeItem&     Unselect()                   { return Select(false); }
	bool          IsSelected() const           { return selected; }

	OldTreeItem&     SelectTree(bool _sel = true); // accepts NULL this
	OldTreeItem&     UnselectTree()               { return SelectTree(false); }

	void          Touch(); // accepts NULL this
	void          TouchLayout(); // accepts NULL this
	void          ScrollInto(); // accepts NULL this

	OldTreeItem&     SetSize(Size sz)             { item_size = sz; auto_size = false; return *this; }
	OldTreeItem&     AutoSize()                   { if(this && !auto_size) { auto_size = true; TouchLayout(); } return *this; }
	int           GetWidth() const             { return item_size.cx; }
	int           GetHeight() const            { return item_size.cy; }
	Size          GetItemSize() const          { return item_size; }
	Size          GetTreeSize() const          { return tree_size; }
	Rect          GetRect(int hittest = HALL) const;

	OldTreeItem&     Indent(int _indent)          { indent = _indent; auto_indent = false; return *this; }
	OldTreeItem&     AutoIndent()                 { if(!auto_indent) { auto_indent = true; TouchLayout(); } return *this; }
	int           GetIndent() const            { return indent; }

	OldTreeItem&     EnableItem(bool _en = true)  { enabled = _en; return *this; }
	OldTreeItem&     DisableItem()                { return EnableItem(false); }
	bool          IsEnabledItem() const        { return enabled; }

	OldTreeItem&     SetImage(Image _image)       { image = _image; return *this; }
	OldTreeItem&     Text(String _text)           { text = _text; return *this; }
	OldTreeItem&     Class(int _class)            { item_class = _class; return *this; }

	Image         GetImage() const             { ASSERT(type == TEXT); return image; }
	String        GetText() const              { return text; }
	int           GetClass() const             { return this ? item_class : int(Null); }

	OldTreeItem&     Text(String t, Image i)      { return Text(t).SetImage(i); }

	void          SetData(Value val)           { value = val; }
	Value         GetData() const              { return value; }

	Value         operator ~ () const          { return value; }
	Value         operator <<= (Value v)       { return value = v; }

	String        GetTextOrValue() const;

	OldTreeItem&     SetDisplay(const Display& d) { type = PICT; picture = &d; return *this; }
	const Display& GetDisplay() const          { ASSERT(type == PICT); return *picture; }

	OldTreeItem&     SetControl(Ctrl& _ctrl)      { type = CTRL; ctrl = &_ctrl; return *this; }
	Ctrl&         GetControl() const           { ASSERT(type == CTRL); return *ctrl; }

	OldTreeItem&     Subtree(Proc _subtree)       { subtree = _subtree; up_to_date = false; return *this; }
	bool          IsSubtree() const;

	OldTreeItem&     SetParent(OldTreeItem *_parent) { parent = _parent; return *this; }
	OldTreeItem     *GetParent() const            { return parent; }

	OldTreeItem&     SetOwner(OldTreeCtrl *_owner)   { owner = _owner; return *this; }
	OldTreeCtrl     *GetOwner() const             { return owner; }

	bool          IsEmpty() const              { return children.IsEmpty(); }
	int           GetCount() const             { return children.GetCount(); }
	bool          IsOpenFull() const;

	int           GetCount(int clss) const;
	int           GetCountDeep() const;

	const OldTreeItem *FindValue(Value value) const;
	OldTreeItem     *FindValue(Value value);

	OldTreeItem&     Clear();
	OldTreeItem&     SetCount(int n);

	OldTreeItem&     Add();
	OldTreeItem&     Insert(int pos);
	void          Remove(int pos, int count = 1);
	OldTreeItem&     Top()                        { return children.Top(); }

	OldTreeItem&     Add(OldTreeItem *item);
	OldTreeItem&     Insert(int pos, OldTreeItem *item);
	OldTreeItem     *Detach(int pos);

	const OldTreeItem& operator [] (int i) const  { return children[i]; }
	OldTreeItem&     operator [] (int i)          { return children[i]; }

	const List&   GetChildren() const          { return children; }
	List&         GetChildren()                { return children; }

	int           GetIndex(const OldTreeItem *i) const { return i ? children.GetIndex(*i) : -1; }

	OldTreeItem&     Set(Proc proc);

	void          LayoutItem();
	void          PaintItem(Draw& draw, Point pos) const;
	HITTEST       FindItem(Point pt, OldTreeItem *& item) const;
	int           GetOpenCount() const;
	Rect          GetItemRect(int hittest = HALL) const;
	Rect          GetClientRect(int hittest = HALL) const;
	Rect          GetTreeRect() const;
	Rect          GetClientTreeRect() const;

	Bar::Item&    BarOpenClose(Bar& bar);
	Bar::Item&    BarOpenSubtree(Bar& bar);
	Bar::Item&    BarCloseSubtree(Bar& bar);

	void          OnOpenClose();
	void          OnOpenSubtree();
	void          OnCloseSubtree();

protected:
	void          OnOpenSubtreeRaw();

protected:
	String        text;
	Value         value;
	Image         image; // TEXT
	Proc          subtree;
	List          children;
};

class OldTreeCursor;

int   TreeCursor_Compare(const OldTreeCursor& a, const OldTreeCursor& b);

class OldTreeCursor : CompareRelOps<const OldTreeCursor&, TreeCursor_Compare,
                   MoveableAndDeepCopyOption<OldTreeCursor> >
{
	friend class OldTreeEdit;

public:
	enum FLAGS
	{
		RIGHT        = 0x01, // step into open items (Down, Up)
		RIGHT_CLOSED = 0x02, // step into closed items (Down, Up)
		LEFT         = 0x04, // step out of items
		HIERARCHY    = LEFT | RIGHT,
		ALL          = HIERARCHY | RIGHT_CLOSED,
	};

	OldTreeCursor();
	OldTreeCursor(const OldTreeItem& item);
	OldTreeCursor(const OldTreeCursor& another, int);

	OldTreeCursor         Copy() const         { return OldTreeCursor(*this, 0); }

	void               Clear()              { root = NULL; cursor.Clear(); }

	OldTreeCursor&        Adjust(); // ensures validity of cursor after removing items from table
	OldTreeCursor&        AdjustAfter(const OldTreeCursor& c, int delta);

	OldTreeCursor&        WeakAdjust();
	OldTreeCursor&        WeakAdjustAfter(const OldTreeCursor& c, int delta);

	bool               IsSubtreeOf(const OldTreeCursor& top) const;

	OldTreeItem          *Get() const;
	OldTreeItem          *GetParent() const;
	OldTreeCtrl          *GetRoot() const      { return root; }
	int                Top() const          { return cursor.Top(); }

	bool               IsRoot() const       { return root; }

	const OldTreeCursor&  Select(bool select = true, bool subtree = false) const;

	const OldTreeCursor&  Open() const;
	const OldTreeCursor&  Touch() const        { Get() -> Touch(); return *this; }
	const OldTreeCursor&  TouchLayout() const  { Get() -> TouchLayout(); return *this; }
	const OldTreeCursor&  AutoSize() const     { Get() -> AutoSize(); return *this; }
	const OldTreeCursor&  ScrollInto() const   { Get() -> ScrollInto(); return *this; }

	operator OldTreeItem *() const             { return Get(); }

	OldTreeItem          *operator ~ () const  { return Get(); }
	OldTreeItem&          operator * () const  { OldTreeItem *p = Get(); ASSERT(p); return *p; }
	OldTreeItem          *operator -> () const { OldTreeItem *p = Get(); ASSERT(p); return p; }

	OldTreeCursor&        BeginAll();
	OldTreeCursor&        EndAll();

	bool               IsBeginAll() const;
	bool               IsEndAll() const;

	bool               IsBegin() const;
	bool               IsEnd() const        { return Get() == NULL; }

	OldTreeCursor&        Find(const OldTreeItem& item);
	OldTreeItem          *Find(int clss, int flags = HIERARCHY);

	bool               Up(int flags = 0);   // go to previous item
	bool               Down(int flags = 0); // go to next item
	OldTreeCursor&        Right();             // go to 1st child
	OldTreeCursor&        Left();              // go to parent
	OldTreeCursor&        Begin();             // 1st item in current child array
	OldTreeCursor&        End();               // past last item in current child array

	const Vector<int>& GetIndex() const { return cursor; }
	Vector<int>&       GetIndex()       { return cursor; }

	static int         Compare(const OldTreeCursor& a, const OldTreeCursor& b);

	String             ToString() const;

protected:
	OldTreeCursor&        AddAfter(const OldTreeCursor& c, int delta);

protected:
	OldTreeCtrl          *root;
	Vector<int>        cursor;
};

class OldTreeEdit
{
public:
	enum OP
	{
		INS, UPD, DEL,
		OP_MASK = 0x3,
		KIDS = 0x4,
		DEEP = 0x8, // doesn't pick out the item
	};
	OldTreeEdit(int op) : op(op) {}
	OldTreeEdit(const OldTreeEdit& e, int) : op(e.op), item(e.item, 0), cursor(e.cursor, 0) {}

	static OldTreeEdit   Insert()                    { return OldTreeEdit(INS); }
	static OldTreeEdit   InsertChildren()            { return OldTreeEdit(INS | KIDS); }

	static OldTreeEdit   Update()                    { return OldTreeEdit(UPD); }
	static OldTreeEdit   UpdateChildren()            { return OldTreeEdit(UPD | KIDS); }

	static OldTreeEdit   Delete()                    { return OldTreeEdit(DEL); }
	static OldTreeEdit   DeleteChildren()            { return OldTreeEdit(DEL | KIDS); }

	OldTreeEdit&         PickItem(OldTreeItem& _item)   { item = _item; return *this; }
	OldTreeEdit&         Item(const OldTreeItem& _item) { item <<= _item; return *this; }
	const OldTreeItem&   GetItem() const             { return item; }
	OldTreeItem&         GetItem()                   { return item; }

	OldTreeEdit&         PickCursor(pick_ OldTreeCursor& _c)  { cursor = _c; return *this; }
	OldTreeEdit&         Cursor(const OldTreeCursor& _c){ cursor <<= _c; return *this; }
	const OldTreeCursor& GetCursor() const           { return cursor; }
	OldTreeCursor&       GetCursor()                 { return cursor; }

	int               GetOpCode() const           { return op; }
	OP                GetOp() const               { return (OP)(op & OP_MASK); }
	bool              IsInsert() const            { return (op & OP_MASK) == INS; }
	bool              IsUpdate() const            { return (op & OP_MASK) == UPD; }
	bool              IsDelete() const            { return (op & OP_MASK) == DEL; }
	bool              IsChildren() const          { return op & KIDS; }

	OldTreeEdit&         Perform(); // performs only
	OldTreeEdit&         Adjust(OldTreeCursor& pos);
	OldTreeEdit&         Adjust(const Vector<OldTreeCursor *>& list);

protected:
	int               op;
	OldTreeItem          item; // item for insert/update; received from delete
	OldTreeCursor        cursor;
};

class OldTreeCtrl : public Ctrl/* WithWheelScroll<Ctrl> */, public OldTreeItem
{
	friend class OldTreeItem;

public:
	typedef OldTreeCtrl CLASSNAME;

	OldTreeCtrl();
	virtual ~OldTreeCtrl();

	enum SELECTION_OP
	{
		KEEP_SEL, SET_SEL, XOR_SEL, DRAG_SEL,
	};

	virtual Value     GetData() const;
	virtual void      SetData(const Value& value);

	Value             operator ~ () const                 { return GetData(); }
	Value             operator <<= (Value v)              { SetData(v); return v; }
	Callback          operator <<= (Callback c)           { return WhenAction = c; }

	const OldTreeCursor& GetCursor() const                   { return cursor; }
	bool              IsCursor() const                    { return cursor; }
	OldTreeItem         *GetCursorItem() const               { return cursor.Get(); }
	OldTreeCtrl&         PickCursor(pick_ OldTreeCursor& cursor, int selection_op = SET_SEL);
	OldTreeCtrl&         SetCursor(const OldTreeCursor& cursor) { return PickCursor(cursor.Copy()); }
	OldTreeCtrl&         SetCursor(const OldTreeItem *item);

	int               GetHitTest() const                  { return hittest; }

	OldTreeCtrl&         Font(class Font _font)           { font = _font; TouchLayout(); return *this; }
	UPP::Font            GetFont() const                     { return font; }

	OldTreeItem&         Add()                               { return OldTreeItem::Add(); }

	OldTreeCtrl&         Offset(Size _offset)                { offset = _offset; Layout(); return *this; }
	Size              GetOffset() const                   { return offset; }

	OldTreeCtrl&         OpenMenu(bool _o)                   { open_menu = _o; return *this; }
	bool              IsOpenMenu() const                  { return open_menu; }

	OldTreeCtrl&         MultiSelect(bool _ms = true)        { multiselect = _ms; return *this; }
	OldTreeCtrl&         NoMultiSelect()                     { return MultiSelect(false); }
	bool              IsMultiSelect() const               { return multiselect; }

	bool              IsDragAction() const                { return drag_state == 1; }

	OldTreeCtrl&         HideTop();

	HITTEST           Find(Point pt, const OldTreeItem *& item) const;
	HITTEST           Find(Point pt, OldTreeItem *& item);
	OldTreeCursor        Find(int y) const;

	void              ScrollInto(Rect rc)                 { scroll.ScrollInto(rc); }
	Point             GetCursorSc() const;
	void              ScCursor(Point pt);

	virtual void      Layout();
	virtual void      Paint(Draw& draw);

	virtual void      LeftDown(Point pt, dword keyflags);
	virtual void      LeftUp(Point pt, dword keyflags);
	virtual void      LeftDouble(Point pt, dword keyflags);
	virtual void      RightDown(Point pt, dword keyflags);
	virtual void      MouseMove(Point pt, dword keyflags);
	virtual void      MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image     CursorImage(Point pt, dword keyflags);
	virtual bool      Key(dword key, int repcnt);

	virtual void      GotFocus();
	virtual void      LostFocus();

	void              OnOpenClose();
	void              OnPush();
	void              OnRight();
	void              StdBar(Bar& bar);
	bool              OnKey(dword key);

public:
	Callback          WhenOpenClose;
	Callback          WhenPush;   // item, hittest, keyflags
	Callback          WhenLeftDouble;
	Callback          WhenRight;
	Callback1<Bar&>   WhenBar;
	Gate1<dword>      WhenKey;
	Callback          WhenSetCursor;

	Callback3<Point, dword, char> WhenDrag; // 0 = begin, 1 = animate, 2 = cancel, 3 = commit
	Callback1<Image&> WhenDragImage;

protected:
	void              OnScroll();

protected:
	bool              updating;       // currently updating
	bool              open_menu;      // open/close menuitem
	bool              hide_top;       // ignore top level item
	bool              multiselect;    // allow concurrent selection of multiple tree items
	UPP::Font            font;
	Size              offset;
	Point             old_scroll;
	int               rowht;          // average row height
	ScrollBars        scroll;
	OldTreeCursor        cursor;
	OldTreeCursor        anchor;
	int               hittest;
	Point             drag_start;
	char              drag_state; // 0 = off, 1 = on, 2 = possible
};

END_UPP_NAMESPACE

#endif//__INCLUDE_CTRL_TREECTRL_H__
