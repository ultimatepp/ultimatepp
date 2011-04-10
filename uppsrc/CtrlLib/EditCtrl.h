struct TextArrayOps {
	virtual int GetLength() const = 0;
	virtual int GetChar(int i) const = 0;

	bool GetWordSelection(int c, int& sell, int& selh);
	int  GetNextWord(int c);
	int  GetPrevWord(int c);

	virtual ~TextArrayOps() {}
};

class LookFrame : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAddSize(Size& sz);

private:
	Value (*look)();
	Value lookv;
	Value Get() const { return look ? (*look)() : lookv; }

public:
	void  Set(const Value& v)                  { look = NULL; lookv = v; }
	void  Set(Value (*l)())                    { look = l; }
	LookFrame(Value (*look)()) : look(look)    {}
	LookFrame(const Value& v) : lookv(v) { look = NULL; }
	LookFrame() { look = NULL; }
};

Value EditFieldEdge();
Value ViewEdge();

void ViewEdge_Write(Value);

CtrlFrame& EditFieldFrame();
CtrlFrame& ViewFrame();

class ActiveEdgeFrame : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAddSize(Size& sz);

private:
	const Value *edge;
	const Ctrl  *ctrl;
	bool  mousein;

public:
	void Set(const Ctrl *ctrl, const Value *edge, bool active);
	void Mouse(bool in) { mousein = in; }

	ActiveEdgeFrame() { edge = NULL; mousein = false; }
};

class EditField : public Ctrl, private TextArrayOps {
public:
	virtual void  Layout();
	virtual void  Paint(Draw& draw);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MiddleDown(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  LeftDouble(Point p, dword keyflags);
	virtual void  LeftTriple(Point p, dword keyflags);
	virtual void  LeftDrag(Point p, dword flags);
	virtual void  LeftUp(Point p, dword flags);
	virtual void  RightDown(Point p, dword keyflags);
	virtual void  MouseEnter(Point p, dword keyflags);
	virtual void  MouseLeave();
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();
	virtual bool  Key(dword key, int rep);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual Size  GetMinSize() const;
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;
	virtual void  CancelMode();
	virtual String GetSelectionData(const String& fmt) const;

public:
	struct Style : ChStyle<Style> {
		Color paper;
		Color disabled;
		Color focus;
		Color invalid;
		Color text, textdisabled;
		Color selected, selectedtext;
		Value edge[4];
		bool  activeedge;
		int   vfm;
	};

protected:
	const Style *style;

	ActiveEdgeFrame edge;

	WString    text;
	int        sc;
	int        cursor, anchor;

	WString    undotext;
	int        undocursor, undoanchor;

	CharFilter      filter;
	const Convert  *convert;
	const Convert  *inactive_convert;
	Font            font;

	WString         nulltext;
	Color           nullink;
	Font            nullfont;
	Image           nullicon;
	int             maxlen;
	int             autosize;
	byte            charset;

	int        dropcursor;
	Rect       dropcaret;
	bool       selclick;

	bool       password:1;
	bool       autoformat:1;
	bool       initcaps:1;
	bool       keep_selection:1;
	bool       clickselect:1;
	bool       nobg:1;
	bool       alignright:1;
	bool       errorbg:1;

	bool    FrameIsEdge();
	void    SetEdge(int i);
	void    SyncEdge();
	int     GetTextCx(const wchar *text, int n, bool password, Font fnt) const;
	void    Paints(Draw& w, int& x, int fcy, const wchar *&txt,
		           Color ink, Color paper, int n, bool pwd, Font fnt);
	int     GetStringCx(const wchar *text, int n);
	int     GetCaret(int cursor) const;
	int     GetCursor(int posx);
	void    SyncCaret();
	void    Finish(bool refresh = true);
	void    SaveUndo();
	void    DoAutoFormat();
	int     GetTy() const;

public:
	Callback1<Bar&>   WhenBar;
	Callback          WhenEnter;

	static const Style& StyleDefault();
	EditField&  SetStyle(const Style& s);

	static  int   GetViewHeight(Font font = StdFont());
	static  int   GetStdHeight(Font font = StdFont());

	int     Insert(int pos, const WString& text);
	int     Insert(int pos, const String& text)         { return Insert(pos, text.ToWString()); }
	int     Insert(int pos, const char *text)           { return Insert(pos, WString(text)); }
	void    Remove(int pos, int n);

	void    Insert(const WString& text);
	void    Insert(const String& text)                  { return Insert(text.ToWString()); }
	void    Insert(const char *text)                    { return Insert(WString(text)); }
	void    Insert(int chr);

	void    Move(int newpos, bool select = false);

	void    SetSelection(int l = 0, int h = INT_MAX);
	bool    GetSelection(int& l, int& h) const;
	bool    IsSelection() const;
	bool    RemoveSelection();
	void    CancelSelection();
	void    Copy();
	void    Undo();
	void    Cut();
	void    Paste();
	void    Erase();
	void    SelectAll();

	void    StdBar(Bar& menu);

	void           SetText(const WString& text);
	void           SetText(const String& t)  { SetText(t.ToWString()); }
	void           SetText(const char *t)    { SetText(WString(t)); }
	const WString& GetText() const           { return text; }

	operator const WString&() const          { return text; }
	operator String() const                  { return text.ToString(); }
	void operator=(const WString& s)         { SetText(s); }
	int     GetLength() const                { return text.GetLength(); }
	int     GetChar(int i) const             { return text[i]; }

	Rect    GetCaretRect(int pos) const;
	Rect    GetCaretRect() const             { return GetCaretRect(cursor); }

	void    Clear();
	void    Reset();
	
	void    Error(bool error = true)         { errorbg = error; }
	
	EditField& Password(bool pwd = true)     { password = pwd; Finish(); return *this; }
	bool       IsPassword() const            { return password; }
	EditField& SetFilter(int (*f)(int))      { filter = f; return *this; }
	EditField& SetConvert(const Convert& c)  { convert = &c; Refresh(); return *this; }
	EditField& SetInactiveConvert(const Convert& c) { inactive_convert = &c; Refresh(); return *this; }
	EditField& AutoFormat(bool b = true)     { autoformat = b; return *this; }
	EditField& NoAutoFormat()                { return AutoFormat(false); }
	bool       IsAutoFormat() const          { return autoformat; }
	EditField& SetCharset(byte cs)           { charset = cs; return *this; }
	EditField& SetFont(Font _font);
	EditField& ClickSelect(bool b = true)    { clickselect = b; return *this; }
	bool       IsClickSelect() const         { return clickselect; }
	EditField& InitCaps(bool b = true)       { initcaps = b; return *this; }
	bool       IsInitCaps() const            { return initcaps; }
	EditField& NullText(const Image& icon, const char *text = t_("(default)"), Color ink = SColorDisabled);
	EditField& NullText(const Image& icon, const char *text, Font fnt, Color ink);
	EditField& NullText(const char *text = t_("(default)"), Color ink = SColorDisabled);
	EditField& NullText(const char *text, Font fnt, Color ink);
	EditField& MaxChars(int mc)              { maxlen = mc; return *this; }
	int        GetMaxChars() const           { return maxlen; }
	EditField& AutoSize(int maxcx = INT_MAX) { autosize = maxcx; Finish(); return *this; }
	EditField& NoBackground(bool b = true)   { nobg = b; Transparent(); Refresh(); return *this; }
	EditField& AlignRight(bool b = true)     { alignright = b; Refresh(); return *this; }

	CharFilter     GetFilter() const         { return filter; }
	const Convert& GetConvert() const        { return *convert; }
	Font           GetFont() const           { return font; }

	typedef EditField CLASSNAME;

	EditField();
	virtual ~EditField();
};

template <class DataType, class Cv>
class EditValue : public EditField, public Cv {
public:
	EditValue& operator=(const DataType& t)  { EditField::SetData(t); return *this; }
	operator DataType() const                { return EditField::GetData(); }

	EditValue()                              { SetConvert(*this); }
};

template <class DataType, class Cv>
class EditMinMax : public EditValue<DataType, Cv> {
public:
	EditMinMax& operator=(const DataType& t)          { EditField::SetData(t); return *this; }

	EditMinMax() {}
	EditMinMax(DataType min, DataType max)            { Cv::MinMax(min, max); }

	EditMinMax& Min(DataType min)                     { Cv::Min(min); Ctrl::Refresh(); return *this; }
	EditMinMax& Max(DataType max)                     { Cv::Max(max); Ctrl::Refresh(); return *this; }
	EditMinMax& MinMax(DataType min, DataType max)    { Min(min); return Max(max); }
	EditMinMax& NotNull(bool nn = true)               { Cv::NotNull(nn); Ctrl::Refresh(); return *this; }
};

template <class DataType, class Base>
class EditMinMaxNotNull : public Base {
public:
	EditMinMaxNotNull& operator=(const DataType& t)   { EditField::SetData(t); return *this; }

	EditMinMaxNotNull()                               { Base::NotNull(); }
	EditMinMaxNotNull(DataType min, DataType max)     { Base::NotNull(); Base::MinMax(min, max); }

	EditMinMaxNotNull& Min(DataType min)              { Base::Min(min); return *this; }
	EditMinMaxNotNull& Max(DataType max)              { Base::Max(max); return *this; }
	EditMinMaxNotNull& MinMax(DataType max)           { Base::MinMax(min, max); return *this; }
	EditMinMaxNotNull& NotNull(bool nn = true)        { Base::NotNull(nn); return *this; }
};

typedef EditMinMax<int, ConvertInt>              EditInt;
typedef EditMinMax<int64, ConvertInt64>          EditInt64;
typedef EditMinMax<double, ConvertDouble>        EditDouble;
typedef EditMinMax<Date, ConvertDate>            EditDate;
typedef EditMinMax<Time, ConvertTime>            EditTime;
typedef EditMinMaxNotNull<int, EditInt>          EditIntNotNull;
typedef EditMinMaxNotNull<double, EditDouble>    EditDoubleNotNull;
typedef EditMinMaxNotNull<Date, EditDate>        EditDateNotNull;
typedef EditMinMaxNotNull<Time, EditTime>        EditTimeNotNull;

class EditString : public EditValue<WString, ConvertString> {
public:
	operator const WString&() const                  { return GetText(); }

	EditString& operator=(const WString& data)       { SetData(data); return *this; }
	EditString& operator=(const String& data)        { SetData(data); return *this; }

	EditString() {}
	EditString(int maxlen)                           { MaxLen(maxlen); }

	EditString& MaxLen(int maxlen)                   { ConvertString::MaxLen(maxlen); Ctrl::Refresh(); return *this; }
	EditString& NotNull(bool nn = true)              { ConvertString::NotNull(nn); Ctrl::Refresh(); return *this; }
	EditString& TrimLeft(bool b)                     { ConvertString::TrimLeft(b); Ctrl::Refresh(); return *this; }
	EditString& TrimRight(bool b)                    { ConvertString::TrimRight(b); Ctrl::Refresh(); return *this; }
	EditString& TrimBoth(bool b)                     { ConvertString::TrimBoth(b); Ctrl::Refresh(); return *this; }
};

class EditStringNotNull : public EditString {
public:
	EditStringNotNull& operator=(const WString& data) { SetData(data); return *this; }
	EditStringNotNull& operator=(const String& data)  { SetData(data); return *this; }

	EditStringNotNull()                               { NotNull(); }
	EditStringNotNull(int maxlen)                     { NotNull(); MaxLen(maxlen); }
};

class EditIntSpin : public EditInt {
public:
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool Key(dword key, int repcnt);


protected:
	SpinButtons sb;
	int         inc;

	void   Inc();
	void   Dec();
	void   Init();

public:
	EditIntSpin&    SetInc(int _inc)             { inc = _inc; return *this; }
	int             GetInc() const               { return inc; }
	EditIntSpin&    OnSides(bool b = true)       { sb.OnSides(b); return *this; }
	bool            IsOnSides() const            { return sb.IsOnSides(); } 

	EditIntSpin&    ShowSpin(bool s = true)      { sb.Show(s); return *this; }

	SpinButtons&    SpinButtonsObject()          { return sb; }
	const SpinButtons& SpinButtonsObject() const { return sb; }

	EditIntSpin();
	EditIntSpin(int min, int max);
	virtual ~EditIntSpin();
};

class EditDoubleSpin : public EditDouble
{
public:
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool Key(dword key, int repcnt);

protected:
	void            Inc();
	void            Dec();
	void            Init();

private:
	SpinButtons     sb;
	double          inc;

	typedef EditDoubleSpin CLASSNAME;
public:

	EditDoubleSpin& SetInc(double _inc = 0.1)    { inc = _inc; return *this; }
	double          GetInc() const               { return inc; }

	EditDoubleSpin& OnSides(bool b = true)       { sb.OnSides(b); return *this; }
	bool            IsOnSides() const            { return sb.IsOnSides(); }
	
	EditDoubleSpin& ShowSpin(bool s = true)      { sb.Show(s); return *this; }

	SpinButtons&       SpinButtonsObject()       { return sb; }
	const SpinButtons& SpinButtonsObject() const { return sb; }

	EditDoubleSpin(double inc = 0.1);
	EditDoubleSpin(double min, double max, double inc = 0.1);
	virtual ~EditDoubleSpin();
};

class EditDoubleNotNullSpin : public EditDoubleSpin
{
public:
	EditDoubleNotNullSpin(double inc = 0.1) : EditDoubleSpin(inc) { NotNull(); }
	EditDoubleNotNullSpin(double min, double max, double inc = 0.1) : EditDoubleSpin(min, max, inc) { NotNull(); }
};
