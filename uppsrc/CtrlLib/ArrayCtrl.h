template <class T>
void DefaultCtrlFactoryFn(One<Ctrl>& ctrl)
{
	ctrl.Create<T>();
}

template <class T>
Callback1<One<Ctrl>&> DefaultCtrlFactory()
{
	return callback(DefaultCtrlFactoryFn<T>);
}

Color SColorEvenRow();

class ArrayCtrl : public Ctrl {
public:
	virtual void  CancelMode();
	virtual bool  Accept();
	virtual void  Reject();
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual bool  Key(dword key, int);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  LeftDrag(Point p, dword);
	virtual void  LeftUp(Point p, dword flags);
	virtual void  RightDown(Point p, dword);
	virtual void  MouseMove(Point p, dword);
	virtual void  MouseLeave();
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword);
	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  ChildGotFocus();
	virtual void  ChildLostFocus();
	virtual void  Serialize(Stream& s);

public:
	struct IdInfo {
		Value              insertval;
		ValueGen          *insertgen;
		Function<Value ()> inserts;
		int              (*accel)(int);

		IdInfo& InsertValue(const Value& v)     { insertval = v; return *this; }
		IdInfo& InsertValue(ValueGen& g)        { insertgen = &g; return *this; }
		IdInfo& Inserts(Function<Value ()> gen) { inserts = gen; return *this; }
		IdInfo& Accel(int (*filter)(int))       { accel = filter; return *this; }
		IdInfo& Accel()                         { return Accel(CharFilterDefaultToUpperAscii); }

		bool    HasInsertValue() const          { return insertgen || inserts || !IsNull(insertval); }
		Value   GetInsertValue()                { return insertgen ? insertgen->Get() : inserts ? inserts() : insertval; }

		IdInfo()                                { insertgen = NULL; accel = NULL; }
	};

	class Column : FormatConvert {
		ArrayCtrl            *arrayctrl;
		int                   index;
		Vector<int>           pos;
		const Convert        *convert;
		Function<Value(const Value&)> convertby;
		Ptr<Ctrl>             edit;
		const Display        *display;
		Event<int, One<Ctrl>&> factory;
		Event<One<Ctrl>&>     factory1;
		int                 (*accel)(int);
		int                   margin;
		bool                  cached;
		bool                  clickedit;
		mutable Any           cache;
		const ValueOrder     *order;
		Function<int (const Value& a, const Value& b)> cmp;
		Gate<int, int>        line_order;


		void   InvalidateCache(int i);
		void   InsertCache(int i, int n);
		void   RemoveCache(int i);
		void   ClearCache();
		void   Sorts();
		
		typedef Column CLASSNAME;

		friend class ArrayCtrl;

	public:
		Column& Add(int _pos)                      { pos.Add(_pos); return *this; }
		Column& Add(const Id& id)                  { pos.Add(-arrayctrl->AsNdx(id)); return *this; }
		Column& AddIndex(const Id& id)             { arrayctrl->AddIndex(id); return Add(id); }
		Column& AddIndex()                         { Add(arrayctrl->GetIndexCount()); arrayctrl->AddIndex(); return *this; }

		Column& SetConvert(const Convert& c);
		Column& ConvertBy(Function<Value(const Value&)> cv);
		Column& SetFormat(const char *fmt);
		Column& SetDisplay(const Display& d);
		Column& NoEdit();
		Column& Edit(Ctrl& e);
		template <class T>
		Column& Ctrls()                            { return Ctrls(DefaultCtrlFactory<T>()); }
		Column& WithLined(Event<int, One<Ctrl>&> factory);
		Column& With(Event<One<Ctrl>&> factory);
		Column& InsertValue(const Value& v);
		Column& InsertValue(ValueGen& g);
		Column& Inserts(Function<Value ()> gen);
		Column& NoClickEdit()                      { clickedit = false; return *this; }
		Column& Cache();
		Column& Accel(int (*filter)(int))          { accel = filter; return *this; }
		Column& Accel()                            { return Accel(CharFilterDefaultToUpperAscii); }

		Column& Sorting();
		Column& Sorting(const ValueOrder& o);
		Column& SortingLined(Gate<int, int> line_order);
		Column& SortingBy(Function<int (const Value& a, const Value& b)> cmp);
		Column& SortDefault(bool desc = false);

		Column& Margin(int m)                      { margin = m; return *this; }

		HeaderCtrl::Column& HeaderTab();
		const HeaderCtrl::Column& HeaderTab() const;
		Column& Tip(const char *tip)               { HeaderTab().Tip(tip); return *this; }

		Column();

// deprecated (due to overloading issues)
		Column& Ctrls(Callback1<One<Ctrl>&> factory);
		Column& Ctrls(void (*factory)(One<Ctrl>&)) { return Ctrls(Event<int, One<Ctrl>&>([=](int, One<Ctrl>& h) { factory(h); })); }
		Column& Ctrls(Event<int, One<Ctrl>&> factory);
		Column& Ctrls(void (*factory)(int, One<Ctrl>&)) { return Ctrls(Event<int, One<Ctrl>&>([=](int a, One<Ctrl>& b){ factory(a, b); })); }
		Column& Sorting(Gate<int, int> order) { return SortingLined(order); }
		Column& Sorting(int (*c)(const Value& a, const Value& b)) { return SortingBy(c); }
	};

	struct Order {
		virtual bool operator()(const Vector<Value>& row1, const Vector<Value>& row2) const = 0;
		virtual ~Order() {}
	};

private:
	struct ItemOrder;
	struct RowOrder;
	struct SortPredicate;

	struct Control : Moveable<Control> {
		int   pos;
		Ctrl *ctrl;
	};

	struct CellCtrl : ParentCtrl {
		virtual void LeftDown(Point, dword);

		bool       owned;
		bool       value;
		Ctrl      *ctrl;
	};

	struct CellInfo : Moveable<CellInfo> {
		BitAndPtr ptr;

		void           Free();
		void           Set(Ctrl *ctrl, bool owned, bool value);
		bool           IsCtrl() const             { return ptr.GetBit(); }
		CellCtrl&      GetCtrl() const            { ASSERT(IsCtrl()); return *(CellCtrl *)ptr.GetPtr(); }

		void           Set(const Display& d)      { ASSERT(!IsCtrl()); ptr.Set0((void *)&d); }
		bool           IsDisplay() const          { return !ptr.GetBit() && ptr.GetPtr(); }
		const Display& GetDisplay() const         { ASSERT(IsDisplay()); return *(const Display *)ptr.GetPtr(); }

		CellInfo(CellInfo&& s);
		CellInfo() {}
		~CellInfo();
	};

	friend class Column;
	friend struct SortPredicate;

	struct Ln : Moveable<Ln> {
		int y;
		int cy;
		Ln()               { cy = Null; }
	};

	struct Line : Moveable<Line> {
		bool          select:1;
		bool          enabled:1;
		bool          visible:1;
		bool          heading:1;
		Color         paper;
		Vector<Value> line;

		Line() { select = false; enabled = true; visible = true; heading = false; paper = Null; }
	};
	
	static int StdValueCompare(const Value& a, const Value& b) { return Upp::StdValueCompare(a, b); }


	Vector<Line>               array;
	HeaderCtrl                 header;
	ScrollBar                  sb;
	Scroller                   scroller;
	Array<Column>              column;
	Vector<Control>            control;
	ArrayMap<Id, IdInfo>       idx;
	Vector<Ln>                 ln;
	Vector< Vector<CellInfo> > cellinfo;
	Vector<bool>               modify;
	Vector<int>                column_width, column_pos;
	DisplayPopup               info;
	const Order               *columnsortsecondary;
	int                        min_visible_line, max_visible_line;
	int                        ctrl_low, ctrl_high;
	int                        sorting_from;
	Index<String>              id_ndx;
	MarginFrame                scrollbox;

	int   keypos;
	int   cursor;
	int   anchor;
	int   linecy;
	int   virtualcount;
	Point clickpos;
	int   dropline, dropcol;
	int   sortcolumn;
	bool  sortcolumndescending;
	bool  columnsortfindkey;
	int   acceptingrow;

	mutable int   selectcount;

	String row_name;
	Color  gridcolor;
	Color  evenpaper, evenink, oddpaper, oddink;

	bool  horzgrid:1;
	bool  vertgrid:1;
	bool  nocursor:1;
	bool  mousemove:1;
	bool  editmode:1;
	bool  insertmode:1;

	bool  inserting:1;
	bool  appending:1;
	bool  appendline:1;
	bool  noinsertappend:1;
	bool  autoappend:1;
	bool  removing:1;
	bool  moving:1;
	bool  askremove:1;
	bool  duplicating:1;
	bool  multiselect:1;
	bool  hasctrls:1;
	bool  headerctrls:1;
	bool  popupex:1;
	bool  nobg:1;
	bool  focussetcursor:1;
	bool  allsorting:1;
	bool  spanwidecells:1;
	bool  accept_edits:1;

	mutable bool  selectiondirty:1;

	unsigned  bains:2;
	
	bool  isdrag:1;
	bool  selclick:1;

	Image cursor_override;

	int    Pos(int np) const;

	void   InsertCache(int i);
	void   RemoveCache(int i);

	void   SetSb();
	void   MinMaxLine();
	void   SyncColumnsPos();
	void   HeaderLayout();
	void   HeaderScroll();
	void   Scroll();
	int    FindEnabled(int i, int dir);
	void   Page(int q);

	void   DoPoint(Point p, bool dosel = true);
	void   DoClick(Point p, dword flags);
	int    GetClickColumn(int ii, Point p);
	void   ClickColumn(Point p);
	void   ClickSel(dword flags);

	Point           FindCellCtrl(Ctrl *c);
	Ctrl           *SyncLineCtrls(int i, Ctrl *p = NULL);
	void            SyncPageCtrls();
	void            SyncCtrls(int i0 = 0);
	bool            IsCtrl(int i, int j) const;
	const CellCtrl& GetCellCtrl(int i, int j) const;
	CellCtrl&       GetCellCtrl(int i, int j);
	void            SetCtrlValue(int i, int ii, const Value& v);

	void   PlaceEdits();
	void   EndEdit();
	void   ColEditSetData(int col);
	void   CtrlSetData(int col);
	Value  Get0(int i, int ii) const;
	void   Set0(int i, int ii, const Value& v);
	void   AfterSet(int i, bool sync_ctrls = true);

	void   Reline(int i, int y);
	void   Remove0(int i);
	void   DisableCtrls();
	void   SetCtrls();
	void   DoRemovem();
	bool   KillCursor0();

	const Display& GetCellInfo(int i, int j, bool f0, Value& v, Color& fg, Color& bg, dword& st);
	Ctrl&  SetCtrl(int i, int j, Ctrl *newctrl, bool owned, bool value);
	Size   DoPaint(Draw& w, bool sample);
	void   SpanWideCell(const Display& d, const Value& q, int cm, int& cw, Rect& r, int i, int& j);

	bool   TestKey(int i, int key);

	bool   SetCursor0(int i, bool dosel = true);

	void   SyncSelection() const;
	void   KeyMultiSelect(int aanchor, dword key);

	void   HeaderScrollVisibility();

	void   RefreshSel();
	bool   DnDInsert(int line, int py, PasteClip& d, int q);
	void   DnD(int line, int col);
	enum { DND_INSERTLINE = -1, DND_LINE = -2 };

	bool   ColumnSortPred(int i1, int i2, int column, const ValueOrder *o);
	bool   OrderPred(int i1, int i2, const ArrayCtrl::Order *o);
	bool   DescendingPred(int i1, int i2, const Gate<int, int> *pred);
	void   SyncInfo();
	void   SortA();
	void   SortB(const Vector<int>& o);

	void   SelectOne(int i, bool sel = true, bool raise = true);
	
	int    AsNdx(const String& id)              { return id_ndx.FindAdd(id); }

	using Ctrl::IsModified;

	// These are listed here as private because name has changed to SetMap/AddMap
	void       Set(int i, const ValueMap& m);
	void       Add(const ValueMap& m);
	
	bool       IsLineVisible0(int i) const { return i < 0 ? false : i < array.GetCount() ? array[i].visible : true; }

public: // temporary (TRC 06/07/28) // will be removed!
	Ctrl&  SetCtrl(int i, int j, Ctrl *newctrl) { return SetCtrl(i, j, newctrl, true, true); }

protected:
	virtual bool UpdateRow();
	virtual void RejectRow();

	void   ClearModify();

public:
	Event<>           WhenSel; // the most usual ArrayCtrl event

	Event<>           WhenLeftDouble;
	Event<Point>      WhenMouseMove;
	Event<>           WhenEnterKey;
	Event<>           WhenLeftClick;
	Event<Bar&>       WhenBar;
	Gate<>            WhenAcceptRow;
	Event<>           WhenUpdateRow;
	Event<>           WhenArrayAction;
	Event<>           WhenStartEdit;
	Event<>           WhenAcceptEdit;
	Event<>           WhenCtrlsAction;
	Event<>           WhenScroll;
	Event<>           WhenHeaderLayout;
	Event<>           WhenColumnSorted;

	Event<int, bool&> WhenLineEnabled;
	Event<int, bool&> WhenLineVisible;

	Event<>                     WhenDrag;
	Event<int, int, PasteClip&> WhenDropCell;
	Event<int, PasteClip&>      WhenDropInsert;
	Event<int, PasteClip&>      WhenDropLine;
	Event<PasteClip&>           WhenDrop;

	//Deprecated - use WhenSel
	Event<>           WhenEnterRow;
	Event<>           WhenKillCursor;
	Event<>           WhenCursor;
	Event<>           WhenSelection;

	IdInfo&    IndexInfo(int ii);
	IdInfo&    IndexInfo(const Id& id);
	IdInfo&    AddIndex(const Id& id);
	IdInfo&    AddIndex();
	int        GetIndexCount() const        { return idx.GetCount(); }
	Id         GetId(int ii) const          { return idx.GetKey(ii); }
	int        GetPos(const Id& id) const   { return idx.Find(id); }
	IdInfo&    SetId(int ii, const Id& id);
	IdInfo&    AddKey(const Id& id)         { ASSERT(idx.GetCount() == 0); return AddIndex(id); }
	IdInfo&    AddKey()                     { ASSERT(idx.GetCount() == 0); return AddIndex(); }
	Id         GetKeyId() const             { return idx.GetKey(0); }

	Column&    AddColumn(const char *text = NULL, int w = 0);
	Column&    AddColumn(const Id& id, const char *text, int w = 0);
	Column&    AddColumnAt(int ii, const char *text, int w = 0);
	Column&    AddColumnAt(const Id& id, const char *text, int w = 0);
	Column&    AddRowNumColumn(const char *text = NULL, int w = 0);

	int                       GetColumnCount() const   { return column.GetCount(); }
	int                       FindColumnWithPos(int pos) const;
	int                       FindColumnWithId(const Id& id) const;
	Vector<int>               FindColumnsWithPos(int pos) const;
	Vector<int>               FindColumnsWithId(const Id& id) const;
	Column&                   ColumnAt(int i)          { return column[i]; }
	Column&                   ColumnAt(const Id& id)   { return column[FindColumnWithId(id)]; }
	HeaderCtrl::Column&       HeaderTab(int i)         { return header.Tab(i); }
	HeaderCtrl::Column&       HeaderTab(const Id& id)  { return header.Tab(FindColumnWithId(id)); }
	const Column&             ColumnAt(int i) const    { return column[i]; }
	const Column&             ColumnAt(const Id& id) const   { return column[FindColumnWithId(id)]; }
	const HeaderCtrl::Column& HeaderTab(int i) const   { return header.Tab(i); }
	const HeaderCtrl::Column& HeaderTab(const Id& id) const   { return header.Tab(FindColumnWithId(id)); }

	const HeaderCtrl&         HeaderObject() const     { return header; }
	HeaderCtrl&               HeaderObject()           { return header; }

	void       SerializeHeader(Stream& s)    { header.Serialize(s); } // deprecated
	void       SerializeSettings(Stream& s);

	IdInfo&    AddCtrl(Ctrl& ctrl);
	IdInfo&    AddCtrl(const Id& id, Ctrl& ctrl);
	IdInfo&    AddIdCtrl(Ctrl& ctrl)         { return AddCtrl(ctrl.GetLayoutId(), ctrl); }
	void       AddCtrlAt(int ii, Ctrl& ctrl);
	void       AddCtrlAt(const Id& id, Ctrl& ctrl);
	void       AddRowNumCtrl(Ctrl& ctrl);

	void       SetCount(int c);
	void       SetVirtualCount(int c);
	int        GetCount() const;
	void       Clear();
	void       Shrink();
	Value      Get(int i, int ii) const;
	Value      Get(int i, const Id& id) const;
	void       Set(int i, int ii, const Value& v);
	void       Set(int i, const Id& id, const Value& v);

	Value      Get(int ii) const;
	Value      Get(const Id& id) const;
	Value      GetOriginal(int ii) const;
	Value      GetOriginal(const Id& id) const;
	bool       IsModified(int ii) const;
	bool       IsModified(const Id& id) const;
	Value      GetKey() const;
	Value      GetOriginalKey() const;
	void       Set(int ii, const Value& v);
	void       Set(const Id& id, const Value& v);

	Value      GetColumn(int row, int col) const;
	Value      GetConvertedColumn(int row, int col) const;

	int        GetSelectCount() const;
	bool       IsSelection() const                              { return GetSelectCount(); }
	void       Select(int i, bool sel = true)                   { SelectOne(i, sel); }
	void       Select(int i, int count, bool sel = true);
	bool       IsSelected(int i) const                          { return i < array.GetCount() && array[i].select; }
	void       ClearSelection(bool raise = true);
	bool       IsSel(int i) const;
	Vector<int> GetSelKeys() const;

	void       EnableLine(int i, bool e);
	void       DisableLine(int i)                               { EnableLine(i, false); }
	bool       IsLineEnabled(int i) const;
	bool       IsLineDisabled(int i) const                      { return !IsLineEnabled(i); }

	void       ShowLine(int i, bool e);
	void       HideLine(int i)                                  { ShowLine(i, false); }
	bool       IsLineVisible(int i) const;

	Vector<Value> ReadRow(int i) const; // deprecated name
	Vector<Value> GetLine(int i) const                          { return ReadRow(i); }

	void       Set(int i, const Vector<Value>& v);
	void       Set(int i, Vector<Value>&& v);
	void       Set(int i, const VectorMap<String, Value>& m);

	void       Add();

#define  E__Add(I)      void Add(__List##I(E__Value));
	__Expand(E__Add)
#undef   E__Add

	void       Add(const Vector<Value>& v)                      { Set(array.GetCount(), v); }
	void       Add(Vector<Value>&& v)                           { Set(array.GetCount(), pick(v)); }
	void       Add(const Nuller& null)                          { Add((Value)Null); }
	void       Add(const VectorMap<String, Value>& m);
	template <typename... Args>
	void       Add(const Args& ...args)                         { Add(gather<Vector<Value>>(args...)); }

	void       SetMap(int i, const ValueMap& m);
	void       AddMap(const ValueMap& m);
	ValueMap   GetMap(int i) const;

	void       SetArray(int i, const ValueArray& va);
	void       AddArray(const ValueArray& va);
	ValueArray GetArray(int i) const;

	void       AddSeparator();
	void       AddHeading(const Value& v);
	bool       IsHeading(int i) const                           { return i < array.GetCount() && array[i].heading; }

	void       Insert(int i);
	void       Insert(int i, int count);
	void       Insert(int i, const Vector<Value>& v);
	void       Insert(int i, const Vector< Vector<Value> >& v);

	void       Remove(int i);

	void       RemoveSelection();

	Image      GetDragSample();

	void       InsertDrop(int line, const Vector<Vector<Value>>& data, PasteClip& d, bool self);
	void       InsertDrop(int line, const Vector<Value>& data, PasteClip& d, bool self);
	void       InsertDrop(int line, const Value& data, PasteClip& d, bool self);
	void       InsertDrop(int line, const ArrayCtrl& src, PasteClip& d);
	void       InsertDrop(int line, PasteClip& d);

	bool       IsCursor() const                        { return cursor >= 0; }
	bool       SetCursor(int i);
	bool       KillCursor();
	void       CancelCursor();
	int        GetCursor() const                       { return cursor; }
	void       GoBegin();
	void       GoEnd();
	int        GetCursorSc() const;
	void       ScCursor(int a);
	void       CenterCursor();
	void       ScrollInto(int line);
	void       ScrollIntoCursor();
	void       SetCursorEditFocus();
	int        GetScroll() const;
	void       ScrollTo(int sc);
	void       ShowAppendLine();
	bool       AcceptRow(bool endedit = false);

	void       Move(int d);
	void       SwapUp();
	void       SwapDown();

	int        Find(const Value& v, int ii = 0, int from = 0) const;
	int        Find(const Value& v, const Id& id, int from = 0) const;

	bool       FindSetCursor(const Value& val, int ii = 0, int from = 0);
	bool       FindSetCursor(const Value& val, const Id& id, int from = 0);

	void       ReArrange(const Vector<int>& order);

	void       Sort(int from, int count, Gate<int, int> order);
	void       Sort(Gate<int, int> order);
	void       Sort(const ArrayCtrl::Order& order);
	void       Sort(int from, int count, const ArrayCtrl::Order& order);
	void       Sort(int (*compare)(const Vector<Value>& v1, const Vector<Value>& v2));
	void       Sort(int from, int count,
	                int (*compare)(const Vector<Value>& v1, const Vector<Value>& v2));
	void       Sort(int ii, int (*compare)(const Value& v1, const Value& v2)
	                = StdValueCompare);
	void       Sort(const Id& id, int (*compare)(const Value& v1, const Value& v2)
	                = StdValueCompare);
	void       Sort()                                  { Sort(0); }

	void       ColumnSort(int column, Gate<int, int> order);
	void       ColumnSort(int column, const ValueOrder& order);
	void       ColumnSort(int column, int (*compare)(const Value& a, const Value& b) = StdValueCompare);

	void       SetSortColumn(int ii, bool descending = false);
	void       ToggleSortColumn(int ii);
	void       DoColumnSort();
	int        GetSortColumn() const                   { return sortcolumn; }
	bool       IsSortDescending() const                { return sortcolumndescending; }

	bool       IsInsert() const                        { return insertmode; }

	void            SetDisplay(int i, int col, const Display& d);
	void            SetRowDisplay(int i, const Display& d);
	void            SetColumnDisplay(int j, const Display& d);
	const Display&  GetDisplay(int row, int col);
	const Display&  GetDisplay(int col);

	void       RefreshRow(int i);

	void       SetCtrl(int i, int col, Ctrl& ctrl, bool value = true);
	Ctrl      *GetCtrl(int i, int col);
	template <class T>
	T&         CreateCtrl(int i, int col, bool value = true) { T *c = new T; SetCtrl(i, col, c, true, value); SyncLineCtrls(i); return *c; }

	ArrayCtrl& SetLineCy(int cy);
	ArrayCtrl& SetEditLineCy()                         { return SetLineCy(EditField::GetStdHeight() + DPI(4)); }
	void       SetLineCy(int i, int cy);
	int        GetLineCy() const                       { return linecy; }
	int        GetLineY(int i) const;
	int        GetLineCy(int i) const;
	int        GetTotalCy() const;
	int        GetLineAt(int y) const;
	
	void       SetLineColor(int i, Color c);

	Rect       GetCellRect(int i, int col) const;
	Rect       GetCellRectM(int i, int col) const;
	Rect       GetScreenCellRect(int i, int col) const;
	Rect       GetScreenCellRectM(int i, int col) const;

	Point      GetClickPos() const                     { return clickpos; }
	int        GetClickColumn() const                  { return clickpos.x; }
	int        GetClickRow() const                     { return clickpos.y; }

	bool       StartEdit(int d = 0);
	int        GetEditColumn() const;
	bool       IsEdit() const                          { return editmode; }

	void       DoEdit();
	void       DoInsert(int cursor);
	void       DoInsertBefore();
	void       DoInsertAfter();
	void       DoAppend();
	bool       DoRemove();
	void       DoDuplicate();
	void       DoSelectAll();
	void       StdBar(Bar& menu);

	bool       IsEditing() const;
	bool       AcceptEnter();

	void       ClearCache();
	void       InvalidateCache(int i);

	void       ScrollUp()                              { sb.PrevLine(); }
	void       ScrollDown()                            { sb.NextLine(); }
	void       ScrollPageUp()                          { sb.PrevPage(); }
	void       ScrollPageDown()                        { sb.NextPage(); }
	void       ScrollEnd()                             { sb.End(); }
	void       ScrollBegin()                           { sb.Begin(); }

	String     AsText(String (*format)(const Value&), bool sel = false,
	                  const char *tab = "\t", const char *row = "\r\n",
	                  const char *hdrtab = "\t", const char *hdrrow = "\r\n") const;
	void       SetClipboard(bool sel = false, bool hdr = true) const;
	String     AsQtf(bool sel = false, bool hdr = true);
	String     AsCsv(bool sel = false, int sep = ';', bool hdr = true);

	String     RowFormat(const char *s);

	ArrayCtrl& RowName(const char *s)                  { row_name = s; return *this; }
	ArrayCtrl& AppendLine(bool b = true)               { appendline = b; return *this; }
	ArrayCtrl& NoAppendLine()                          { return AppendLine(false); }
	bool       IsAppendLine() const                    { return appendline; }
	ArrayCtrl& Inserting(bool b = true)                { inserting = b; return AppendLine(b); }
	ArrayCtrl& NoInserting()                           { return Inserting(false); }
	bool       IsInserting() const                     { return inserting; }
	ArrayCtrl& Removing(bool b = true)                 { removing = b; return *this; }
	ArrayCtrl& NoRemoving()                            { return Removing(false); }
	bool       IsRemoving() const                      { return removing; }
	ArrayCtrl& Appending(bool b = true)                { appending = b; return *this; }
	bool       IsAppending() const                     { return appending || autoappend; }
	ArrayCtrl& AutoAppending(bool b = true)            { autoappend = b; return *this; }
	bool       IsAutoAppending() const                 { return autoappend; }
	ArrayCtrl& BeforeAfterInserting(int q = 1)         { bains = 1; return *this; }
	ArrayCtrl& AfterBeforeInserting(int q = 2)         { bains = 2; return *this; }
	ArrayCtrl& Duplicating(bool b = true)              { duplicating = b; return *this; }
	ArrayCtrl& NoInsertAppend(bool b = true)           { noinsertappend = b; return *this; }
	bool       IsDuplicating() const                   { return duplicating; }
	ArrayCtrl& Moving(bool b = true)                   { moving = b; return *this; }
	bool       IsMoving() const                        { return moving; }
	ArrayCtrl& NoDuplicating()                         { return Duplicating(false); }
	ArrayCtrl& AskRemove(bool b = true)                { askremove = b; return *this; }
	ArrayCtrl& NoAskRemove()                           { return AskRemove(false); }
	bool       IsAskRemove() const                     { return askremove; }

	ArrayCtrl& Header(bool b = true)                   { header.Invisible(!b); return *this; }
	ArrayCtrl& NoHeader()                              { return Header(false); }
	ArrayCtrl& Track(bool b = true)                    { header.Track(b); return *this; }
	ArrayCtrl& NoTrack()                               { return Track(false); }
	ArrayCtrl& VertGrid(bool b = true)                 { vertgrid = b; Refresh(); return *this; }
	ArrayCtrl& NoVertGrid()                            { return VertGrid(false); }
	ArrayCtrl& HorzGrid(bool b = true)                 { horzgrid = b; Refresh(); return *this; }
	ArrayCtrl& NoHorzGrid()                            { return HorzGrid(false); }
	ArrayCtrl& Grid(bool b = true)                     { return VertGrid(b).HorzGrid(b); }
	ArrayCtrl& NoGrid()                                { return Grid(false); }
	ArrayCtrl& GridColor(Color c)                      { gridcolor = c; return *this; }
	ArrayCtrl& EvenRowColor(Color paper = SColorEvenRow(), Color ink = SColorText);
	ArrayCtrl& OddRowColor(Color paper = SLtYellow(), Color ink = SColorText);
	ArrayCtrl& NoCursor(bool b = true)                 { nocursor = b; return *this; }
	ArrayCtrl& MouseMoveCursor(bool b = true)          { mousemove = b; return *this; }
	ArrayCtrl& NoMouseMoveCursor()                     { return MouseMoveCursor(false); }
	ArrayCtrl& AutoHideSb(bool b = true)               { sb.AutoHide(b); return *this; }
	ArrayCtrl& NoAutoHideSb()                          { return AutoHideSb(false); }
	ArrayCtrl& HideSb(bool b = true)                   { sb.Show(!b); return *this; }
	ArrayCtrl& AutoHideHorzSb(bool b = true)           { header.AutoHideSb(b); return *this; }
	ArrayCtrl& NoAutoHideHorzSb()                      { return AutoHideHorzSb(false); }
	ArrayCtrl& HideHorzSb(bool b = true)               { header.HideSb(b); return *this; }
	
	ArrayCtrl& MultiSelect(bool b = true)              { multiselect = b; return *this; }
	bool       IsMultiSelect() const                   { return multiselect; }
	ArrayCtrl& NoBackground(bool b = true)             { nobg = b; Transparent(); Refresh(); return *this; }
	ArrayCtrl& PopUpEx(bool b = true)                  { popupex = b; SyncInfo(); return *this; }
	ArrayCtrl& NoPopUpEx()                             { return PopUpEx(false); }
	ArrayCtrl& NoFocusSetCursor()                      { focussetcursor = false; return *this; }
	ArrayCtrl& MovingHeader(bool b)                    { header.Moving(b); return *this; }
	ArrayCtrl& NoMovingHeader()                        { return MovingHeader(false); }
	ArrayCtrl& ColumnSortFindKey(bool b = true)        { columnsortfindkey = b; return *this; }
	ArrayCtrl& AllSorting();
	ArrayCtrl& ColumnSortSecondary(const Order& order) { columnsortsecondary = &order; return *this; }
	ArrayCtrl& NoColumnSortSecondary()                 { columnsortsecondary = NULL; return *this; }
	ArrayCtrl& SortingFrom(int from)                   { sorting_from = from; return *this; }

	ArrayCtrl& ColumnWidths(const char *s);
	String     GetColumnWidths();

	ArrayCtrl& SetScrollBarStyle(const ScrollBar::Style& s)   { sb.SetStyle(s); header.SetScrollBarStyle(s); return *this; }
	ArrayCtrl& SetHeaderCtrlStyle(const HeaderCtrl::Style& s) { header.SetStyle(s); return *this; }

	ArrayCtrl& CursorOverride(const Image& arrow)             { cursor_override = arrow; return *this; }
	ArrayCtrl& NoCursorOverride()                             { return CursorOverride(Null); }
	
	ArrayCtrl& SpanWideCells(bool b = true)                   { spanwidecells = b; Refresh(); return *this; }
	
	ArrayCtrl& AcceptEdits(bool b = true)                     { accept_edits = b; return *this; }

	void Reset();

	typedef ArrayCtrl CLASSNAME;

	ArrayCtrl();
	virtual ~ArrayCtrl();
};

class ArrayOption : public Display, public Pte<ArrayOption> {
public:
	typedef ArrayOption CLASSNAME;
	ArrayOption();
	virtual ~ArrayOption();

	virtual void       Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;

	void               Connect(ArrayCtrl& ac, int ii = 0);
	void               Connect(ArrayCtrl& ac, const Id& id)        { Connect(ac, ac.GetPos(id)); }

	void               Disconnect();

	ArrayCtrl::Column& AddColumn(ArrayCtrl& ac, const char *text = NULL, int w = 0);
	ArrayCtrl::Column& AddColumn(ArrayCtrl& ac, const Id& id, const char *text, int w = 0);

	ArrayOption&       TrueFalse(Value _t, Value _f)               { t = _t; f = _f; return *this; }
	Value              GetFalse() const                            { return f; }
	Value              GetTrue() const                             { return t; }
	ArrayOption&       ThreeState(bool b = true)                   { threestate = b; return *this; }
	ArrayOption&       NoThreeState()                              { return ThreeState(false); }
	bool               IsThreeState() const                        { return threestate; }

	ArrayOption&       HSwitch(bool hs = true)                     { hswitch = hs; return *this; }
	ArrayOption&       NoHSwitch()                                 { return HSwitch(false); }
	bool               IsHSwitch() const                           { return hswitch; }

	ArrayOption&       VSwitch(bool vs = true)                     { vswitch = vs; return *this; }
	ArrayOption&       NoVSwitch()                                 { return VSwitch(false); }
	bool               IsVSwitch() const                           { return vswitch; }

	ArrayOption&       Frame(bool frm = true)                      { frame = frm; return *this; }
	ArrayOption&       NoFrame()                                   { return Frame(false); }
	bool               IsFrame() const                             { return frame; }

	void               Click();

public:
	Event<>            WhenAction;

	Event<>            operator <<= (Event<>  cb)                  { return WhenAction = cb; }

private:
	Vector<int>        index;
	ArrayCtrl         *array;
	Value              t, f;
	bool               hswitch, vswitch;
	bool               threestate;
	bool               frame;
};
