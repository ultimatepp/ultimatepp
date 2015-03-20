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
		String text;
		bool   typing;
	};

	struct UndoData {
		int              undoserial;
		BiArray<UndoRec> undo;
		BiArray<UndoRec> redo;

		void Clear()                     { undo.Clear(); redo.Clear(); }
	};

	enum {
		INK_NORMAL,
		INK_DISABLED,
		INK_SELECTED,
		PAPER_NORMAL,
		PAPER_READONLY,
		PAPER_SELECTED,
		COLOR_COUNT,
	};

protected:
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
	virtual void    PlaceCaret(int newcursor, bool sel = false);
	virtual void    InvalidateLine(int i);
	virtual int     RemoveRectSelection();
	virtual WString CopyRectSelection();
	virtual int     PasteRectSelection(const WString& s);
	virtual String  GetPasteText();

	struct Ln : Moveable<Ln> {
		int    len;
		String text;

		int      GetLength() const       { return len; }
		operator WString() const         { return FromUtf8(text); }

		Ln(const WString& wtext)         { text = ToUtf8(wtext); len = wtext.GetLength(); }
		Ln()                             { len = 0; }
	};

	Vector<Ln>       line;
	int              total;
	int              cline, cpos;
	int              cursor, anchor;
	int              undoserial;
	bool             rectsel;
	bool             incundoserial;
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

public:
	virtual void   RefreshLine(int i);

	Callback1<Bar&> WhenBar;
	Callback        WhenState;
	Callback        WhenSel;

	void   CachePos(int pos);
	void   CacheLinePos(int linei);

	enum { CHARSET_UTF8_BOM = 250 };
	enum { LE_DEFAULT, LE_CRLF, LE_LF };

	int    Load(Stream& s, byte charset = CHARSET_DEFAULT);
	bool   IsTruncated() const                                { return false; }
	void   Save(Stream& s, byte charset = CHARSET_DEFAULT, int line_endings = LE_DEFAULT) const;

	int    GetInvalidCharPos(byte charset = CHARSET_DEFAULT) const;
	bool   CheckCharset(byte charset = CHARSET_DEFAULT) const { return GetInvalidCharPos(charset) < 0; }

	void    Set(const WString& s);
	void    Set(const String& s, byte charset = CHARSET_DEFAULT);
	String  Get(byte charset = CHARSET_DEFAULT) const;
	String  Get(int pos, int size, byte charset = CHARSET_DEFAULT) const;
	WString GetW(int pos, int size) const;
	WString GetW() const                      { return GetW(0, GetLength()); }

	void   ClearDirty();
	bool   IsDirty() const                    { return dirty; }

	void   Clear();

	int    GetLinePos(int& pos) const;
	int    GetPos(int line, int column) const;
	int    GetPos(int line) const             { return GetPos(line, 0); }
	int    GetLine(int pos) const             { return GetLinePos(pos); }

	const String& GetUtf8Line(int i) const    { return line[i].text; }
	WString       GetWLine(int i) const       { return FromUtf8(line[i].text); }
	String        GetEncodedLine(int i, byte charset = CHARSET_DEFAULT) const;
	int           GetLineLength(int i) const  { return line[i].GetLength(); }

	int    GetLineCount() const               { return line.GetCount(); }
	int    GetChar(int pos) const;
	int    operator[](int pos) const          { return GetChar(pos); }
	int    GetLength() const                  { return total; }

	int     GetCursor() const                 { return cursor; }
	int     GetCursorLine()                   { return GetLine(GetCursor()); }

	void    SetSelection(int anchor = 0, int cursor = INT_MAX);
	bool    IsSelection() const               { return IsAnySelection() && !rectsel; }
	bool    IsRectSelection() const           { return IsAnySelection() && rectsel; }
	bool    IsAnySelection() const            { return anchor >= 0 && anchor != cursor; }
	bool    GetSelection(int& l, int& h) const;
	String  GetSelection(byte charset = CHARSET_DEFAULT) const;
	WString GetSelectionW() const;
	void    ClearSelection();
	bool    RemoveSelection();
	void    SetCursor(int cursor)                { PlaceCaret(cursor); }
	int     Paste(const WString& text);

	int     Insert(int pos, const WString& txt)  { return Insert(pos, txt, false); }
	int     Insert(int pos, const String& txt, byte charset = CHARSET_DEFAULT);
	int     Insert(int pos, const char *txt)     { return Insert(pos, WString(txt)); }
	void    Remove(int pos, int size);

	void      NextUndo();
	void      Undo();
	void      Redo();
	bool      IsUndo() const                  { return undo.GetCount(); }
	bool      IsRedo() const                  { return redo.GetCount(); }
	void      ClearUndo()                     { undo.Clear(); redo.Clear(); }
	void      ClearRedo()                     { redo.Clear(); }
	bool      IsUndoOp() const                { return undo_op; }
	UndoData  PickUndoData();
	void      SetPickUndoData(UndoData rval_ data);

	void      Cut();
	void      Copy();
	void      Paste();
	void      SelectAll();

	void      StdBar(Bar& menu);

	void      SetCharset(byte cs)              { charset = ResolveCharset(cs); }
	byte      GetCharset() const               { return charset; }

	void      SetColor(int i, Color c)         { color[i] = c; Refresh(); }
	Color     GetColor(int i) const            { return color[i]; }

	TextCtrl& UndoSteps(int n)                 { undosteps = n; Undodo(); return *this; }
	int       GetUndoSteps() const             { return undosteps; }
	TextCtrl& ProcessTab(bool b = true)        { processtab = b; return *this; }
	TextCtrl& NoProcessTab()                   { return ProcessTab(false); }
	TextCtrl& ProcessEnter(bool b = true)      { processenter = b; return *this; }
	TextCtrl& NoProcessEnter()                 { return ProcessEnter(false); }
	TextCtrl& NoBackground(bool b = true)      { nobg = b; Transparent(); Refresh(); return *this; }
	TextCtrl& MaxLength(int len)               { max_total = len; return *this; }
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
	virtual Image  CursorImage(Point, dword);
	virtual void   DragAndDrop(Point p, PasteClip& d);
	virtual void   DragRepeat(Point p);
	virtual void   DragLeave();
	virtual void   Layout();
	virtual void   RefreshLine(int i);

protected:
	virtual void    SetSb();
	virtual void    PlaceCaret(int newcursor, bool sel = false);
	virtual int     RemoveRectSelection();
	virtual WString CopyRectSelection();
	virtual int     PasteRectSelection(const WString& s);

public:
	enum Flags {
		SHIFT_L = 1,
		SHIFT_R = 2,
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

	struct EditPos : Moveable<EditPos> {
		int sby;
		int cursor;

		void Serialize(Stream& s);
		void Clear()                      { sby = 0; cursor = 0; }
		EditPos()                         { Clear(); }
	};

protected:
	virtual void  HighlightLine(int line, Vector<Highlight>& h, int pos);
	virtual void  NewScrollPos();

	ScrollBars       sb;
	int              gcolumn;
	int              mpos;


	Font             font;
	CharFilter       filter;
	int              tabsize;
	int              bordercolumn;
	Color            bordercolor;
	Color            hline;
	Scroller         scroller;
	Point            caretpos;
	bool             nohbar;
	bool             showtabs;
	bool             cutline;
	bool             overwrite;
	bool             showspaces;
	bool             showlines;
	bool             showreadonly;
	bool             dorectsel; // TODO: Refactor this ugly hack!

	void   Paint0(Draw& w);

	void   AlignChar();

	void   MovePage(int dir, bool sel);

	void   PlaceCaret0(Point p);
	int    PlaceCaretNoG(int newcursor, bool sel = false);

	void   Scroll();
	void   SetHBar();
	Rect   DropCaret();
	void   RefreshDropCaret();
	void   DoPasteColumn() { PasteColumn(); }

	struct RefreshDraw;
	friend class TextCtrl;

public:
	Size   GetFontSize() const;
	int    GetGPos(int ln, int cl) const;
	int    GetMousePos(Point p) const;
	Point  GetColumnLine(int pos) const;
	int    GetColumnLinePos(Point pos) const  { return GetGPos(pos.y, pos.x); }
	Point  GetIndexLine(int pos) const;
	int    GetIndexLinePos(Point pos) const;

	void   SetRectSelection(int l, int h);
	void   SetRectSelection(const Rect& rect);
	Rect   GetRectSelection() const;
	bool   GetRectSelection(const Rect& rect, int line, int& l, int &h);

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

	void    ScrollIntoCursor();

	Point   GetCaretPoint() const             { return caretpos; }

	void    Clear();

	void    OverWriteMode(bool o = true)      { overwrite = o; PlaceCaret(cursor, false); }
	bool    IsOverWriteMode() const           { return overwrite; }

	void    RefreshChars(bool (*predicate)(int c));

	LineEdit& TabSize(int n);
	int       GetTabSize() const              { return tabsize; }
	LineEdit& BorderColumn(int col, Color c = SColorFace());
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
	LineEdit& WithCutLine(bool b)             { cutline = b; return *this; }
	LineEdit& NoCutLine()                     { return WithCutLine(false); }
	bool      IsWithCutLine() const           { return cutline; }
	LineEdit& SetFilter(int (*f)(int c))      { filter = f; return *this; }
	LineEdit& ShowReadOnly(bool b)            { showreadonly = b; Refresh(); return *this; }
	LineEdit& NoShowReadOnly()                { return ShowReadOnly(false); }
	bool      IsShowReadOnly()                { return showreadonly; }
	LineEdit& ShowCurrentLine(Color color)    { hline = color; Refresh(); return *this; }
	
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

protected:
	virtual void  ClearLines();
	virtual void  InsertLines(int line, int count);
	virtual void  RemoveLines(int line, int count);
	virtual void  PlaceCaret(int pos, bool select = false);
	virtual void  InvalidateLine(int i);

	struct Para : Moveable<Para> {
		int    cx, cy;

		Para()         { cx = -1; }
	};

	Vector<Para> para;
	Font         font;
	int          after;
	CharFilter   filter;
	ScrollBar    sb;
	int          cx;
	bool         updownleave, eofline;

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

	typedef DocEdit CLASSNAME;

	DocEdit();
	virtual ~DocEdit();
};
