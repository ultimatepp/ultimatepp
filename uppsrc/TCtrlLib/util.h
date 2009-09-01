NAMESPACE_UPP

#define ASSERT_INDEX(arrayctrl, index) ASSERT(arrayctrl.GetIndexCount() == index)

#define TOOL(x) \
	void        COMBINE(Tool, x)(Bar& bar); \
	void        COMBINE(On, x)(); \
	static void COMBINE(Help, x)();

#define TOOL_LOCAL(x) \
	void        COMBINE(Tool, x)(Bar& bar, bool local); \
	void        COMBINE(On, x)(); \
	static void COMBINE(Help, x)();

class Pump
{
public:
	struct Item
	{
		virtual ~Item() {}
		virtual void Run(bool write) = 0;
	};

	Pump()                                          {}

	void         Run(bool write)                    { for(int i = 0; i < items.GetCount(); i++) items[i].Run(write); }

	void         Read()                             { Run(false); }
	void         Write()                            { Run(true); }

	bool         IsEmpty() const                    { return items.IsEmpty(); }

	Pump&        Add(Item *item)                    { items.Add(item); return *this; }

	Pump&        operator << (One<Item> item)       { Add(-item); return *this; }
	Pump&        operator << (Item *item)           { Add(item); return *this; }
	void         operator << (bool write)           { Run(write); }

	void         Clear()                            { items.Clear(); }

public:
	Array<Item> items;
};

inline Pump WithPump() { return Pump(); }

template <class A, class B>
struct PumpItemSet : public Pump::Item
{
	PumpItemSet(A& a, B& b) : a(a), b(b) {}
	virtual void Run(bool write) { if(write) a = b; else b = a; }
	A& a; B& b;
};

template <class A, class B>
inline One<Pump::Item> PumpSet(A& a, B& b) { return new PumpItemSet<A, B>(a, b); }

template <class A, class B>
struct PumpItemData : public Pump::Item
{
	PumpItemData(A& a, B& b) : a(a), b(b) {}
	virtual void Run(bool write) { if(write) a = ~b; else b <<= a; }
	A& a; B& b;
};

template <class A, class B>
inline One<Pump::Item> PumpData(A& a, B& b) { return new PumpItemData<A, B>(a, b); }

template <class B>
struct PumpItemDataRef : public Pump::Item
{
	PumpItemDataRef(Ref ref, B& b) : ref(ref), b(b) {}
	virtual void Run(bool write) { if(write) ref.SetValue(~b); else b <<= Value(ref); }
	Ref ref; B& b;
};

template <class B>
inline One<Pump::Item> PumpDataRef(Ref ref, B& b) { return new PumpItemDataRef<B>(ref, b); }

template <class A, class B>
struct PumpItemEnumData : public Pump::Item
{
	PumpItemEnumData(A& a, B& b) : a(a), b(b) {}
	virtual void Run(bool write) { if(write) a = A(int(~b)); else b <<= (int)a; }
	A& a; B& b;
};

template <class A, class B>
inline One<Pump::Item> PumpEnumData(A& a, B& b) { return new PumpItemEnumData<A, B>(a, b); }

template <class A>
struct PumpItemArray : public Pump::Item
{
	PumpItemArray(A& a, ArrayCtrl& array, int row, int column) : a(a), array(array), row(row), column(column) {}
	virtual void Run(bool write)
	{
		if(write) { a = array.Get(row, column); }
		else      { array.Set(row, column, a); }
	}

	A&         a;
	ArrayCtrl& array;
	int        row;
	int        column;
};

template <class A, class B>
struct PumpItemScaledData : public Pump::Item
{
	PumpItemScaledData(A& a, B& b, double scale) : a(a), b(b), scale(scale) {}
	virtual void Run(bool write)
	{
		if(write) { double d = ~b; a   = (IsNull(d) ? d : d * scale); }
		else      { b <<= (IsNull(a) ? double(Null) : (double)a / scale); }
	}

	A& a;
	B& b;
	double scale;
};

template <class A, class B>
struct PumpItemIntScaledData : public Pump::Item
{
	PumpItemIntScaledData(A& a, B& b, double scale) : a(a), b(b), scale(scale) {}
	virtual void Run(bool write)
	{
		if(write) { double d = ~b; a   = (IsNull(d) ? (int)Null : fround(d * scale)); }
		else      { b <<= (IsNull(a) ? double(Null) : (double)a / scale); }
	}

	A& a;
	B& b;
	double scale;
};

template <class A, class B>
One<Pump::Item> PumpScaledData(A& a, B& b, double scale) { return new PumpItemScaledData<A, B>(a, b, scale); }

template <class A, class B>
One<Pump::Item> PumpAngleData(A& a, B& b) { return PumpScaledData<A, B>(a, b, M_PI / 180.0); }

template <class A, class B>
One<Pump::Item> PumpIntScaledData(A& a, B& b, double scale) { return new PumpItemIntScaledData<A, B>(a, b, scale); }

template <class A, class B>
One<Pump::Item> PumpIntAngleData(A& a, B& b) { return PumpIntScaledData<A, B>(a, b, M_PI / 180.0); }

template <class A>
inline One<Pump::Item> PumpArray(A& a, ArrayCtrl& array, int row, int column)
{ return new PumpItemArray<A>(a, array, row, column); }

template <class A>
inline One<Pump::Item> PumpArray(A& a, ArrayCtrl& array, int row, Id column)
{ return new PumpItemArray<A>(a, array, row, array.GetPos(column)); }

template <class A, class C>
struct PumpItemArrayCast : public Pump::Item
{
	PumpItemArrayCast(A& a, ArrayCtrl& array, int row, int column) : a(a), array(array), row(row), column(column) {}

	virtual void Run(bool write)
	{
		if(write) { a = A(C(array.Get(row, column))); }
		else      { array.Set(row, column, (C)a); }
	}

	A&         a;
	ArrayCtrl& array;
	int        row;
	int        column;
};

template <class C, class A>
inline One<Pump::Item> PumpArrayCast(A& a, ArrayCtrl& array, int row, int column)
{ return new PumpItemArrayCast<A, C>(a, array, row, column); }

template <class C, class A>
inline One<Pump::Item> PumpArrayCast(A& a, ArrayCtrl& array, int row, Id column)
{ return new PumpItemArrayCast<A, C>(a, array, row, array.GetPos(column)); }

struct PumpItemArrayRef : public Pump::Item
{
	PumpItemArrayRef(Ref ref, ArrayCtrl& array, int row, int column)
		: ref(ref), array(array), row(row), column(column) {}

	virtual void Run(bool write)
	{
		if(write) { ref.SetValue(array.Get(row, column)); }
		else      { array.Set(row, column, Value(ref)); }
	}

	Ref        ref;
	ArrayCtrl& array;
	int        row;
	int        column;
};

inline One<Pump::Item> PumpArrayRef(Ref ref, ArrayCtrl& array, int row, int column)
{ return new PumpItemArrayRef(ref, array, row, column); }

inline One<Pump::Item> PumpArrayRef(Ref ref, ArrayCtrl& array, int row, Id column)
{ return new PumpItemArrayRef(ref, array, row, array.GetPos(column)); }

bool   IsUnique(const ArrayCtrl& table, int column);
bool   IsUnique(const ArrayCtrl& table, Id column);
int    SwapRows(ArrayCtrl& table, int row1, int row2);
void   SetColumn(ArrayCtrl& table, int index, const Value& value);
void   SetColumn(ArrayCtrl& table, Id column, const Value& value);
Index<int> GetSelIndex(const ArrayCtrl& table);

BorderFrame& TopJoinFrame();
BorderFrame& BottomJoinFrame();

class TabPageCtrl : public StaticRect {
public:
	TabPageCtrl() { Transparent(); Color(Null); /*Blend(SColorFace(), SColorPaper()));*/ }
};

class DropListAll : public DropList
{
public:
	DropListAll() { DisplayAll(); }
};

class DropButton : public FrameRight<Button>
{
public:
	DropButton();
	virtual bool HotKey(dword key);

public:
	Gate1<dword> WhenHotKey;
};

class ChoiceList : public PopUpTable
{
public:
	typedef ChoiceList CLASSNAME;

	ChoiceList();

	ChoiceList& Attach(Ctrl& owner);
	void        Detach();

	ChoiceList& operator >> (Ctrl& owner) { return Attach(owner); }

	void        OnDrop();
	void        OnToggle();
	void        OnSelect();
	void        OnCancel();

	ChoiceList& DropWidth(int _wd)    { dropbutton.Width(_wd); return *this; }
	ChoiceList& DropShow(bool ds)     { dropbutton.Show(ds); return *this; }
	ChoiceList& DataItem(int _item)   { item = _item; return *this; }
	ChoiceList& DataItem(Id _col_id)  { item = GetPos(_col_id); return *this; }
	int         GetDataItem() const   { return item; }

	bool        OnKey(dword key);

public:
	Callback    WhenDrop;

protected:
	void        Initialize();

protected:
	DropButton  dropbutton;
	Ctrl       *owner;
	int         item;
};

class ExtendedButton : public FrameRight<Button>
{
public:
	ExtendedButton();
	virtual bool HotKey(dword key);
};

class ExtendedFileButton : public FrameRight<Button>
{
public:
	ExtendedFileButton();
	virtual bool HotKey(dword key);
};

class MenuButton : public ExtendedButton
{
public:
	MenuButton();

	Callback1<Bar&> WhenBar;

private:
	void            OnMenu();
};

//////////////////////////////////////////////////////////////////////
// DragDropCtrl::

class DragDropCtrl : public Ctrl
{
public:
	DragDropCtrl();
	virtual ~DragDropCtrl();

	// drag & drop interface
	virtual bool Push(Point pt, dword keyflags);
	virtual void Drag(Point pt, Point last, Point next, dword keyflags);
	virtual void Drop(Point pt, Point end, dword keyflags);
	virtual void Click(Point pt, dword keyflags);
	virtual void Cancel();

	virtual void DragRect(const Rect& last, const Rect& next, dword keyflags);
	virtual void DropRect(const Rect& rc, dword keyflags);

	void         DragShow(bool _show = true);
	void         DragHide()                    { DragShow(false); }
	void         DragStop(bool accept, dword keyflags);
	void         DragStop(bool accept = false) { DragStop(accept, last_key); }

	bool         IsDragging() const            { return state == ON || state == HIDDEN; }
	bool         IsPushed() const              { return state != OFF; }

	// control overrides
	virtual void LeftDown(Point pt, dword keyflags);
	virtual void LeftDouble(Point pt, dword keyflags);
	virtual void LeftUp(Point pt, dword keyflags);
	virtual void RightDown(Point pt, dword keyflags);
	virtual void RightDouble(Point pt, dword keyflags);
	virtual void RightUp(Point pt, dword keyflags);
	virtual void MouseMove(Point pt, dword keyflags);
	virtual bool Key(dword key, int repcnt);

private:
	Point        start, last;
	dword        last_key;
	char         state;
	enum { OFF, ON, POSSIBLE, HIDDEN };
};

class StdArrayOrder : public ArrayCtrl::Order
{
public:
	StdArrayOrder() {}
	StdArrayOrder(int i, bool d = false, dword l = 0)    { Add(i, d, l); }

	void         Clear()                                 { index.Clear(); desc.Clear(); lang.Clear(); }
	void         Add(int i, bool d = false, dword l = 0) { index.Add(i); desc.Add(d); lang.Add(l); }
	void         Set(int i, bool d = false, dword l = 0) { Clear(); Add(i, d, l); }

	virtual bool operator () (const Vector<Value>& row1, const Vector<Value>& row2) const;

private:
	Vector<int>   index;
	Vector<bool>  desc;
	Vector<dword> lang;
};

class ArrayPair : public Ctrl
{
public:
	typedef ArrayPair CLASSNAME;
	ArrayPair();

	ArrayPair&              RowName(const char *s)                  { left.RowName(s); right.RowName(s); return *this; }
	ArrayPair&              AppendLine(bool b = true)               { left.AppendLine(b); right.AppendLine(b); return *this; }
	ArrayPair&              NoAppendLine()                          { return AppendLine(false); }
	ArrayPair&              Inserting(bool b = true)                { left.Inserting(b); right.Inserting(b); return AppendLine(b); }
	ArrayPair&              NoInserting()                           { return Inserting(false); }
	ArrayPair&              Removing(bool b = true)                 { left.Removing(b); right.Removing(b); return *this; }
	ArrayPair&              NoRemoving()                            { return Removing(false); }
	ArrayPair&              Appending(bool b = true)                { left.Appending(b); right.Appending(b); return *this; }
	ArrayPair&              BeforeAfterInserting(int q = 1)         { left.BeforeAfterInserting(q); right.BeforeAfterInserting(q); return *this; }
	ArrayPair&              AfterBeforeInserting(int q = 2)         { left.AfterBeforeInserting(q); right.AfterBeforeInserting(q); return *this; }
	ArrayPair&              Duplicating(bool b = true)              { left.Duplicating(b); right.Duplicating(b); return *this; }
	ArrayPair&              NoInsertAppend(bool b = true)           { left.NoInsertAppend(b); right.NoInsertAppend(b); return *this; }
	ArrayPair&              Moving(bool b = true)                   { left.Moving(b); right.Moving(b); return *this; }
	ArrayPair&              NoDuplicating()                         { return Duplicating(false); }
	ArrayPair&              AskRemove(bool b = true)                { left.AskRemove(b); right.AskRemove(b); return *this; }
	ArrayPair&              NoAskRemove()                           { return AskRemove(false); }

	ArrayPair&              Header(bool b = true)                   { left.Header(b); right.Header(b); return *this; }
	ArrayPair&              NoHeader()                              { return Header(false); }
	ArrayPair&              Track(bool b = true)                    { left.Track(b); right.Track(b); return *this; }
	ArrayPair&              NoTrack()                               { return Track(false); }
	ArrayPair&              VertGrid(bool b = true)                 { left.VertGrid(b); right.VertGrid(b); return *this; }
	ArrayPair&              NoVertGrid()                            { return VertGrid(false); }
	ArrayPair&              HorzGrid(bool b = true)                 { left.HorzGrid(b); right.HorzGrid(b); return *this; }
	ArrayPair&              NoHorzGrid()                            { return HorzGrid(false); }
	ArrayPair&              Grid(bool b = true)                     { return VertGrid(b).HorzGrid(b); }
	ArrayPair&              NoGrid()                                { return Grid(false); }
	ArrayPair&              GridColor(Color c)                      { left.GridColor(c); right.GridColor(c); return *this; }
	ArrayPair&              NoCursor(bool b = true)                 { left.NoCursor(b); right.NoCursor(b); return *this; }
	ArrayPair&              MouseMoveCursor(bool b = true)          { left.MouseMoveCursor(b); right.MouseMoveCursor(b); return *this; }
	ArrayPair&              NoMouseMoveCursor()                     { return MouseMoveCursor(false); }
	ArrayPair&              AutoHideSb(bool b = true)               { left.AutoHideSb(b); right.AutoHideSb(); return *this; }
	ArrayPair&              NoAutoHideSb()                          { return AutoHideSb(false); }
	ArrayPair&              MultiSelect(bool b = true)              { left.MultiSelect(b); right.MultiSelect(b); return *this; }

	ArrayPair&              Vert(bool vert = true);
	ArrayPair&              Horz()                                  { return Vert(false); }
	bool                    IsVert() const                          { return is_vert; }

	void                    SetPos(int pos);
	void                    HalfPos()                               { SetPos(5000); }
	int                     GetPos() const                          { return pane_pos; }

	virtual void            Layout();

	virtual void            SetData(const Value& value);
	virtual Value           GetData() const;

	void                    SetKey(int i);
	void                    SetKey(Id id);
	int                     GetKey() const { return key_index; }

	void                    SetSort(int i, const ArrayCtrl::Order& order);
	void                    SetSort(Id id, const ArrayCtrl::Order& order) { SetSort(left.GetPos(id), order); }
	void                    SetSort(int i);
	void                    SetSort(Id id)                                { SetSort(left.GetPos(id)); }
	void                    NoSort()                                      { SetSort(-1, std_order); }

	void                    UpdateGUI();

	TOOL(Left)
		TOOL(Add)
		TOOL(AddAll)

	TOOL(Right)
		TOOL(Remove)
		TOOL(RemoveAll)

public:
	Callback                WhenAdd;
	ArrayCtrl               left, right;

private:
	void                    OnRightCursor();
	void                    SortLeft();
	void                    SortRight();

private:
	Button                  add, add_all, remove, remove_all;
	int                     key_index;
	int                     sort_index;
	int                     pane_pos;
	bool                    is_vert;
	const ArrayCtrl::Order *order;
	StdArrayOrder           std_order;
};

template <class T>
Vector<T> GetArrayColumn(const ArrayCtrl& array, int column)
{
	Vector<T> out;
	out.SetCount(array.GetCount());
	for(int i = 0; i < out.GetCount(); i++)
		out[i] = array.Get(i, column);
	return out;
}

template <class T>
Vector<T> GetKeyVector(const ArrayPair& pair)
{
	ASSERT(pair.GetKey() >= 0);
	return GetArrayColumn<T>(pair.right, pair.GetKey());
}

template <class C>
void SetKeyContainer(ArrayPair& pair, const C& container)
{
	typedef typename C::ConstIterator CI;
	int k = pair.GetKey();
	ASSERT(k >= 0);
	for(CI b = container.Begin(), e = container.End(); b != e; ++b)
		if(pair.left.FindSetCursor(*b, k))
			pair.OnAdd();
	if(pair.left.GetCount() > 0)
		pair.left.SetCursor(0);
	if(pair.right.GetCount() > 0)
		pair.right.SetCursor(0);
}

class ValueArrayCtrl : public ArrayCtrl
{
public:
	typedef ValueArrayCtrl CLASSNAME;
	ValueArrayCtrl();

	virtual void    SetData(const Value& data);
	virtual Value   GetData() const;
	virtual bool    Accept();

public:
	Callback        WhenSetData;
	Gate            WhenAccept;

private:
	void            OnArrayAction();
};

class CallbackDataCtrl : public Ctrl {
public:
	CallbackDataCtrl() { NoWantFocus(); }

	virtual void            SetData(const Value& v)    { WhenSetData(v); }
	virtual Value           GetData() const            { Value v; WhenGetData(v); return v; }
	virtual bool            Key(dword key, int repcnt) { return WhenKey(key) || Ctrl::Key(key, repcnt); }

public:
	Callback1<const Value&> WhenSetData;
	Callback1<Value&>       WhenGetData;
	Gate1<dword>            WhenKey;
};

//////////////////////////////////////////////////////////////////////
// Option3::

/*
class Option3 : public Pusher
{
public:
	Option3();

	operator       bool() const           { return state == 1; }
	void           operator = (bool b)    { state = b ? 1 : 0; }

	Option3&       NotNull(bool nn)       { not_null = nn; return *this; }
	Option3&       NoNotNull()            { return NotNull(false); }

	virtual void   Paint(Draw& draw);
	virtual Size   GetMinSize() const;
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const; // Null = indeterminate

protected:
	virtual void   RefreshPush();
	virtual void   RefreshFocus();
	virtual void   PerformAction();

protected:
	char           state; // 2 = indeterminate
	bool           not_null;
};
*/

#ifdef PLATFORM_WIN32
void           DropFilesRegister(HWND hwnd);
Point          DropFilesGetPos(WPARAM hdrop);
Vector<String> DropFilesGetList(WPARAM hdrop);
#endif

bool ShowErrorFocus(bool cond, const char *text, Ctrl& focus);

END_UPP_NAMESPACE
