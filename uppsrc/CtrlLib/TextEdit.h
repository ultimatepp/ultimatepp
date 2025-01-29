class TextCtrl : public Ctrl, protected TextArrayOps {
public:
	virtual void  SetData(const Value& v);
	virtual Value GetData() const;
	virtual void  CancelMode();
	virtual String GetSelectionData(const String& fmt) const;
	virtual void  MiddleDown(Point p, dword flags);

public:
	struct UndoRec {
		int    serial;
		int    pos;
		int    size;
		String data;
		bool   typing;
		
		void   SetText(const String& text) { data = FastCompress(text); }
		String GetText() const             { return FastDecompress(data); }
	};

	struct UndoData {
		int              undoserial;
		BiArray<UndoRec> undo;
		BiArray<UndoRec> redo;

		void Clear()                     { undo.Clear(); redo.Clear(); }
	};

	struct EditPos : Moveable<EditPos> {
		int   sby;
		int64 cursor;

		void Serialize(Stream& s);
		void Clear()                      { sby = 0; cursor = 0; }
		EditPos()                         { Clear(); }
	};

	enum {
		INK_NORMAL,
		INK_DISABLED,
		INK_SELECTED,
		PAPER_NORMAL,
		PAPER_READONLY,
		PAPER_SELECTED,
		WHITESPACE,
		WARN_WHITESPACE,
		COLOR_COUNT,
	};

protected:
	virtual int64   GetTotal() const                  { return total; }
	virtual int     GetCharAt(int64 i) const          { return GetChar(i); }

	virtual void    DirtyFrom(int line);
	virtual void    SelectionChanged();
	virtual void    ClearLines();
	virtual void    InsertLines(int line, int count);
	virtual void    RemoveLines(int line, int count);
	virtual void    PreInsert(int pos, const WString& text);
	virtual void    PostInsert(int pos, const WString& text);
	virtual void    PreRemove(int pos, int size);
	virtual void    PostRemove(int pos, int size);
	virtual void    SetSb();
	virtual void    PlaceCaret(int64 newcursor, bool sel = false);
	virtual void    InvalidateLine(int i);
	virtual int     RemoveRectSelection();
	virtual WString CopyRectSelection();
	virtual int     PasteRectSelection(const WString& s);
	virtual String  GetPasteText();

	struct Ln : Moveable<Ln> {
		int    len;
		String text;

		Ln()                             { len = 0; }
	};

	Vector<Ln>       lin;
	int64            total;

	int64            cpos;
	int              cline;
	int64            cursor, anchor;
	int              undoserial;
	bool             rectsel;
	bool             incundoserial;
	bool             column_typing = false; // group undos for column typing
	int              undosteps;
	BiArray<UndoRec> undo;
	BiArray<UndoRec> redo;
	int              dirty;
	int              undo_op;
	byte             charset;
	bool             truncated;

	bool             selclick;
	Point            dropcaret;
	bool             isdrag;

	Color            color[COLOR_COUNT];

	bool             processtab, processenter;
	bool             nobg;
	int              max_total;
	int              max_line_len;
	
	mutable Stream  *view;
	struct ViewCache {
		int        blk;
		Vector<Ln> line;
	};
	mutable ViewCache view_cache[2];
	mutable int viewlines;
	
	Vector<int64>     offset256;
	Vector<int>       total256;
	int               view_loading_lock;
	int64             view_loading_pos;
	bool              view_all;

	void   IncDirty();
	void   DecDirty();
	int    Insert0(int pos, const WString& txt);
	void   Remove0(int pos, int size);
	int    InsertU(int pos, const WString& txt, bool typing = false);
	void   RemoveU(int pos, int size);
	void   Undodo();
	int    Insert(int pos, const WString& txt, bool typing);
	void   DoPaste() { Paste(); }
	void   DoRemoveSelection() { RemoveSelection(); }
	void   RefreshLines(int l1, int l2);
	static bool   IsUnicodeCharset(byte charset);

	int    Load0(Stream& in, byte charset, bool view);
	int    LoadLines(Vector<Ln>& ls, int n, int64& total, Stream& in, byte charset,
	                 int max_line_len, int max_total, bool& truncated,
	                 int *view_line_count = NULL) const;
	void   ViewLoading();

	void   SetLine(int i, const String& txt, int len) { lin[i].text = txt; lin[i].len = len; }
	void   SetLine(int i, const WString& w)           { SetLine(i, ToUtf8(w), w.GetCount()); }
	void   LineRemove(int i, int n)                   { lin.Remove(i, n); }
	void   LineInsert(int i, int n)                   { lin.InsertN(i, n); }
	const Ln& GetLn(int i) const;

	int    GetLinePos32(int& pos) const;
	bool   GetSelection32(int& l, int& h) const;
	int    GetPos32(int line, int column = 0) const   { return (int)GetPos64(line, column); }
	int    GetLength32() const;
	int    GetCursor32() const;

public:
	virtual void   RefreshLine(int i);

	Event<Bar&>  WhenBar;
	Event<>      WhenState;
	Event<>      WhenSel;
	Event<int64> WhenViewMapping;

	void   CachePos(int64 pos);
	void   CacheLinePos(int linei);

	enum CS { CHARSET_UTF8_BOM = 250, CHARSET_UTF16_LE, CHARSET_UTF16_BE, CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM };
	enum LE { LE_DEFAULT, LE_CRLF, LE_LF };

	int    Load(Stream& s, byte charset = CHARSET_DEFAULT)    { return Load0(s, charset, false); }
	bool   IsTruncated() const                                { return truncated; }
	void   Save(Stream& s, byte charset = CHARSET_DEFAULT, int line_endings = LE_DEFAULT) const;

	int    GetInvalidCharPos(byte charset = CHARSET_DEFAULT) const;
	bool   CheckCharset(byte charset = CHARSET_DEFAULT) const { return GetInvalidCharPos(charset) < 0; }

	int    LimitSize(int64 size) const                        { return int(view ? min((int64)max_total, size) : size); }

	int    GetLinePos(int& pos) const                         { return GetLinePos32(pos); }
	int    GetPos(int line, int column = 0) const             { return GetPos32(line, column); }

	void    Set(const WString& s);
	void    Set(const String& s, byte charset = CHARSET_DEFAULT);
	String  Get(byte charset = CHARSET_DEFAULT) const;
	String  Get(int64 pos, int size, byte charset = CHARSET_DEFAULT) const;
	WString GetW(int64 pos, int size) const;
	WString GetW() const                      { return GetW(0, LimitSize(GetLength64())); }

	void   ClearDirty();
	bool   IsDirty() const                    { return dirty; }

	void   Clear();

	int    GetLine(int64 pos) const           { return GetLinePos64(pos); }

	const String& GetUtf8Line(int i) const;
	WString       GetWLine(int i) const       { return ToUtf32(GetUtf8Line(i)); }
	String        GetEncodedLine(int i, byte charset = CHARSET_DEFAULT) const;
	int           GetLineLength(int i) const;

	int     GetLength() const                 { return GetLength32(); }
	int     GetLineCount() const              { return view ? viewlines : lin.GetCount(); }
	int     GetChar(int64 pos) const;
	int     GetChar() const                   { return cursor < GetLength64() ? GetChar(cursor) : 0; }
	int     operator[](int64 pos) const       { return GetChar(pos); }

	int     GetCursor() const                 { return GetCursor32(); }
	int     GetCursorLine() const             { return GetLine(GetCursor64()); }

	void    SetSelection(int64 anchor = 0, int64 cursor = INT_MAX);
	bool    IsSelection() const               { return IsAnySelection() && !rectsel; }
	bool    IsRectSelection() const           { return IsAnySelection() && rectsel; }
	bool    IsAnySelection() const            { return anchor >= 0 && anchor != cursor; }
	bool    GetSelection(int& l, int& h) const{ return GetSelection32(l, h); }
	bool    GetSelection(int64& l, int64& h) const;
	String  GetSelection(byte charset = CHARSET_DEFAULT) const;
	WString GetSelectionW() const;
	void    ClearSelection();
	bool    RemoveSelection();
	void    SetCursor(int64 cursor)              { PlaceCaret(cursor); }
	int     Paste(const WString& text);

	int     Insert(int pos, const WString& txt)  { return Insert(pos, txt, false); }
	int     Insert(int pos, const String& txt, byte charset = CHARSET_DEFAULT);
	int     Insert(int pos, const char *txt)     { return Insert(pos, WString(txt)); }
	void    Remove(int pos, int size);

	int     Append(const WString& txt)           { return Insert(GetLength(), txt); }
	int     Append(const String& txt, byte charset = CHARSET_DEFAULT) { return Insert(GetLength(), txt, charset); }
	int     Append(const char *txt)              { return Append(WString(txt)); }

	void      NextUndo();
	void      Undo();
	void      Redo();
	bool      IsUndo() const                  { return undo.GetCount(); }
	bool      IsRedo() const                  { return redo.GetCount(); }
	void      ClearUndo()                     { undo.Clear(); redo.Clear(); }
	void      ClearRedo()                     { redo.Clear(); }
	bool      IsUndoOp() const                { return undo_op; }
	UndoData  PickUndoData();
	void      SetPickUndoData(UndoData&& data);

	void      Cut();
	void      Copy();
	void      Paste();
	void      SelectAll();

	void      StdBar(Bar& menu);

	void      SetCharset(byte cs)              { charset = ResolveCharset(cs); }
	byte      GetCharset() const               { return charset; }

	void      SetColor(int i, Color c)         { color[i] = c; Refresh(); }
	Color     GetColor(int i) const            { return color[i]; }

	int       View(Stream& s, byte charset = CHARSET_DEFAULT)    { return Load0(s, charset, true); }
	void      WaitView(int line = INT_MAX, bool progress = false);
	void      LockViewMapping()                                  { view_loading_lock++; }
	void      UnlockViewMapping();
	void      SerializeViewMap(Stream& s);
	bool      IsView() const                                     { return view; }
	int64     GetViewSize() const                                { return view ? view->GetSize() : 0; }

	int       GetLinePos64(int64& pos) const;
	int64     GetPos64(int line, int column = 0) const;
	int64     GetLength64() const                                { return total; }
	int64     GetCursor64() const                                { return cursor; }

	TextCtrl& UndoSteps(int n)                 { undosteps = n; Undodo(); return *this; }
	int       GetUndoSteps() const             { return undosteps; }
	TextCtrl& ProcessTab(bool b = true)        { processtab = b; return *this; }
	TextCtrl& NoProcessTab()                   { return ProcessTab(false); }
	TextCtrl& ProcessEnter(bool b = true)      { processenter = b; return *this; }
	TextCtrl& NoProcessEnter()                 { return ProcessEnter(false); }
	TextCtrl& NoBackground(bool b = true)      { nobg = b; Transparent(); Refresh(); return *this; }
	TextCtrl& MaxLength(int len, int linelen)  { max_total = len; max_line_len = linelen; return *this; }
	bool      IsNoBackground() const           { return nobg; }
	bool      IsProcessTab() const             { return processtab; }
	bool      IsProcessEnter() const           { return processenter; }

	typedef TextCtrl CLASSNAME;

	TextCtrl();
	virtual ~TextCtrl();
};

class LineEdit : public TextCtrl {
public:
	virtual bool   Key(dword key, int count);
	virtual void   Paint(Draw& w);
	virtual void   LeftDown(Point p, dword flags);
	virtual void   RightDown(Point p, dword flags);
	virtual void   LeftRepeat(Point p, dword keyflags);
	virtual void   LeftDouble(Point p, dword keyflags);
	virtual void   LeftTriple(Point p, dword keyflags);
	virtual void   LeftUp(Point p, dword flags);
	virtual void   LeftDrag(Point p, dword flags);
	virtual void   MouseMove(Point p, dword flags);
	virtual void   MouseWheel(Point, int zdelta, dword);
	virtual void   HorzMouseWheel(Point, int zdelta, dword);
	virtual Image  CursorImage(Point, dword);
	virtual void   DragAndDrop(Point p, PasteClip& d);
	virtual void   DragRepeat(Point p);
	virtual void   DragLeave();
	virtual void   Layout();
	virtual void   RefreshLine(int i);
	virtual Font   GetPreeditFont();
	virtual Rect   GetCaret() const;

protected:
	virtual void    SetSb();
	virtual void    PlaceCaret(int64 newcursor, bool sel = false);
	virtual int     RemoveRectSelection();
	virtual WString CopyRectSelection();
	virtual int     PasteRectSelection(const WString& s);

public:
	enum Flags {
		SHIFT_L = 1,
		SHIFT_R = 2,
		SPELLERROR = 4,
		NOENDFILL = 8,
	};

	struct Highlight : Moveable<Highlight> {
		Color paper;
		Color ink;
		Font  font;
		word  flags;
		wchar chr;

		bool operator==(const Highlight& h) const
		     { return paper == h.paper && ink == h.ink && font == h.font; }
		
		Highlight() { flags = 0; }
	};

protected:
	virtual void  HighlightLine(int line, Vector<Highlight>& h, int64 pos);
	virtual void  NewScrollPos();

	ScrollBars       sb;
	int              gcolumn;
	int64            mpos;


	Font             font;
	Size             fsz;
	CharFilter       filter;
	int              tabsize;
	int              bordercolumn;
	Color            bordercolor;
	Color            hline, vline;
	int              vlinex;
	Scroller         scroller;
	Point            caretpos;
	int              caretlines = 1;
	bool             nohbar;
	bool             showtabs;
	bool             cutline;
	bool             overwrite;
	bool             showspaces;
	bool             block_caret;
	bool             showlines;
	bool             showreadonly;
	bool             warnwhitespace;
	bool             dorectsel; // TODO: Refactor this ugly hack!

	void   Paint0(Draw& w);

	void   AlignChar();

	void   MovePage(int dir, bool sel);

	void   PlaceCaret0();
	int    PlaceCaretNoG(int64 newcursor, bool sel = false);

	void   Scroll();
	void   SetHBar();
	Rect   DropCaret();
	void   RefreshDropCaret();
	void   DoPasteColumn() { PasteColumn(); }
	void   SyncFont();
	bool   IsDoubleChar(int ch) const;
	void   RectSelectionChar(int c);
	void   RectSelectionText(const WString& text);
	void   RectSelectionBackspace();
	void   RectSelectionDelete();
	void   RectSelectionLeftRight(int dir, bool homeend);
	int    RectSelectionOp(Event<int, Rect, int64, int64, WString&> op, Event<Rect&> changesel = Null);

	struct RefreshDraw;
	friend class TextCtrl;

	int    GetMousePos32(Point p) const       { return (int)GetMousePos(p); }

public:
	Event<> WhenScroll;
	Event<> WhenLeftUp;
	
	Size   GetFontSize() const;
	int64  GetGPos(int ln, int cl) const;
	int64  GetMousePos(Point p) const;
	Point  GetColumnLine(int64 pos) const;
	int64  GetColumnLinePos(Point pos) const  { return GetGPos(pos.y, pos.x); }
	Point  GetIndexLine(int64 pos) const;
	int64  GetIndexLinePos(Point pos) const;

	void   SetRectSelection(int64 l, int64 h);
	void   SetRectSelection(const Rect& rect);
	Rect   GetRectSelection() const;
	bool   GetRectSelection(const Rect& rect, int line, int64& l, int64& h);

	void   ScrollUp()                         { sb.LineUp(); }
	void   ScrollDown()                       { sb.LineDown(); }
	void   ScrollPageUp()                     { sb.PageUp(); }
	void   ScrollPageDown()                   { sb.PageDown(); }
	void   ScrollBegin()                      { sb.VertBegin(); }
	void   ScrollEnd()                        { sb.VertEnd(); }

	Rect   GetLineScreenRect(int line) const;

	void   TopCursor(int lines = 0);
	void   CenterCursor();

	void   MoveUpDown(int n, bool sel = false);

	void   MoveLeft(bool sel = false);
	void   MoveRight(bool sel = false);
	void   MoveUp(bool sel = false);
	void   MoveDown(bool sel = false);
	void   MovePageUp(bool sel = false);
	void   MovePageDown(bool sel = false);
	void   MoveHome(bool sel = false);
	void   MoveEnd(bool sel = false);
	void   MoveTextBegin(bool sel = false);
	void   MoveTextEnd(bool sel = false);

	bool   InsertChar(dword key, int count = 1, bool canoverwrite = false);
	void   DeleteChar();
	void   Backspace();
	void   DeleteLine();
	void   CutLine();

	void   PasteColumn(const WString& text);
	void   PasteColumn();
	
	void   Sort();

	Point   GetScrollPos() const              { return sb; }
	Size    GetPageSize()                     { return sb.GetPage(); }
	void    SetScrollPos(Point p)             { sb.Set(p); }

	EditPos GetEditPos() const;
	void    SetEditPos(const EditPos& pos);
	void    SetEditPosSb(const LineEdit::EditPos& pos);
	void    SetEditPosSbOnly(const LineEdit::EditPos& pos);

	void    ScrollIntoCursor();

	Point   GetCaretPoint() const             { return caretpos; }

	void    Clear();

	void    OverWriteMode(bool o = true)      { overwrite = o; PlaceCaret(cursor, false); }
	bool    IsOverWriteMode() const           { return overwrite; }

	void    RefreshChars(bool (*predicate)(int c));

	LineEdit& TabSize(int n);
	int       GetTabSize() const              { return tabsize; }
	LineEdit& BorderColumn(int col, Color c = SColorFace());
	int       GetBorderColumn() const         { return bordercolumn; }
	LineEdit& SetFont(Font f);
	Font      GetFont() const                 { return font; }
	LineEdit& NoHorzScrollbar(bool b = true)  { nohbar = b; ScrollIntoCursor(); return *this; }
	bool      IsNoHorzScrollbar() const       { return nohbar; }
	LineEdit& ShowTabs(bool st = true)        { showtabs = st; Refresh(); return *this; }
	bool      IsShowTabs() const              { return showtabs; }
	LineEdit& ShowSpaces(bool ss = true)      { showspaces = ss; Refresh(); return *this; }
	bool      IsShowSpaces() const            { return showspaces; }
	LineEdit& ShowLineEndings(bool sl = true) { showlines = sl; Refresh(); return *this; }
	bool      IsShowLineEndings() const       { return showlines; }
	LineEdit& WarnWhiteSpace(bool b = true)   { warnwhitespace = b; Refresh(); return *this; }
	bool      IsWantWhiteSpace() const        { return warnwhitespace; }
	LineEdit& WithCutLine(bool b)             { cutline = b; return *this; }
	LineEdit& NoCutLine()                     { return WithCutLine(false); }
	bool      IsWithCutLine() const           { return cutline; }
	LineEdit& SetFilter(int (*f)(int c))      { filter = f; return *this; }
	LineEdit& ShowReadOnly(bool b)            { showreadonly = b; Refresh(); return *this; }
	LineEdit& NoShowReadOnly()                { return ShowReadOnly(false); }
	bool      IsShowReadOnly()                { return showreadonly; }
	LineEdit& ShowCurrentLine(Color color)    { hline = color; Refresh(); return *this; }
	LineEdit& ShowCurrentColumn(Color color)  { vline = color; Refresh(); return *this; }
	LineEdit& BlockCaret(bool b)              { block_caret = b; return *this; }
	
	LineEdit& SetScrollBarStyle(const ScrollBar::Style& s)   { sb.SetStyle(s); return *this; }

	typedef LineEdit CLASSNAME;

	LineEdit();
	virtual ~LineEdit();
};

class DocEdit : public TextCtrl {
public:
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual bool  Key(dword key, int count);
	virtual void  LeftDown(Point p, dword flags);
	virtual void  LeftDouble(Point p, dword keyflags);
	virtual void  LeftTriple(Point p, dword keyflags);
	virtual void  LeftUp(Point p, dword flags);
	virtual void  RightDown(Point p, dword w);
	virtual void  MouseMove(Point p, dword flags);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  RefreshLine(int i);
	virtual void  SetSb();

	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();
	virtual void  LeftDrag(Point p, dword flags);

	virtual Rect  GetCaret() const;

protected:
	virtual void  ClearLines();
	virtual void  InsertLines(int line, int count);
	virtual void  RemoveLines(int line, int count);
	virtual void  PlaceCaret(int64 pos, bool select = false);
	virtual void  InvalidateLine(int i);

	struct Para : Moveable<Para> {
		int    cx, cy;

		Para()         { cx = -1; cy = 0; }
	};

	Vector<Para> para;
	Font         font;
	int          after;
	CharFilter   filter;
	ScrollBar    sb;
	int          cx;
	bool         updownleave, eofline;
	Rect         caret;

	struct Fmt {
		FontInfo      fi;
		int           len;
		Buffer<wchar> text;
		Buffer<int>   width;
		Vector<int>   line;
		int           LineEnd(int i)  {
			return i < line.GetCount() - 1 ? line[i + 1] : len;
		}
	};
	Fmt    Format(const WString& text) const;

	void   Invalidate();
	int    GetHeight(int i);
	void   Scroll();
	void   PlaceCaret(bool scroll);
	int    GetY(int parai);
	int    GetCursorPos(Point p);
	Point  GetCaret(int pos);
	void   VertMove(int delta, bool select, bool scs);
	void   HomeEnd(int x, bool select);
	void   RefreshStyle();
	Rect   DropCaret();
	void   RefreshDropCaret();
	int    GetMousePos(Point p);

public:
	Event<>   WhenLeftUp;
	
	DocEdit&  After(int a)                                   { after = a; RefreshStyle(); return *this; }
	DocEdit&  SetFont(Font f)                                { font = f; RefreshStyle(); return *this; }
	DocEdit&  SetFilter(int (*f)(int c))                     { filter = f; return *this; }
	DocEdit&  AutoHideSb(bool b = true)                      { sb.AutoHide(b); return *this; }
	bool      IsAutoHideSb() const                           { return sb.IsAutoHide(); }
	DocEdit&  UpDownLeave(bool u = true)                     { updownleave = u; return *this; }
	DocEdit&  NoUpDownLeave()                                { return UpDownLeave(false); }
	bool      IsUpDownLeave() const                          { return updownleave; }
	DocEdit&  SetScrollBarStyle(const ScrollBar::Style& s)   { sb.SetStyle(s); return *this; }
	DocEdit&  EofLine(bool b = true)                         { eofline = b; return *this; }
	DocEdit&  NoEofLine()                                    { return EofLine(false); }
	bool      IsEofLine() const                              { return eofline; }

	EditPos   GetEditPos() const;
	void      SetEditPos(const TextCtrl::EditPos& pos);

	typedef DocEdit CLASSNAME;

	DocEdit();
	virtual ~DocEdit();
};
