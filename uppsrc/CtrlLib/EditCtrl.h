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
		Color selected0, selectedtext0; // If does not have focus...
		Value edge[4];
		bool  activeedge;
		int   vfm;
	};
	
	struct Highlight : Moveable<Highlight> {
		Color ink;
		Color paper;
		Color underline;

		bool operator!=(const Highlight& b) const { return ink != b.ink || paper != b.paper || underline != b.underline; }
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
	Color           textcolor;

	WString         nulltext;
	Color           nullink;
	Font            nullfont;
	Image           nullicon;
	int             maxlen;
	int             autosize;
	byte            charset;
	int             fsell, fselh; // used to hold selection after LostFocus for X11 middle mouse copy

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
	bool       showspaces:1;
	bool       no_internal_margin:1;

	bool    FrameIsEdge();
	void    SetEdge(int i);
	void    SyncEdge();
	int     LowChar(int c) const { return 0x25af /*c + 0x2400*/; }
	int     GetCharWidth(int c) const { return font[c < 32 ? LowChar(c) : c]; }
	int     GetTextCx(const wchar *text, int n, bool password, Font fnt) const;
	void    Paints(Draw& w, int& x, int fcy, const wchar *&txt,
		           Color ink, Color paper, int n, bool pwd, Font fnt, Color underline, bool showspaces);
	int     GetStringCx(const wchar *text, int n);
	int     GetCaret(int cursor) const;
	int     GetCursor(int posx);
	void    SyncCaret();
	void    Finish(bool refresh = true);
	void    SaveUndo();
	void    DoAutoFormat();
	int     GetTy() const;
	void    StdPasteFilter(WString&);
	void    SelSource();

protected:
	virtual void  HighlightText(Vector<Highlight>& hl);

public:
	Callback1<Bar&>               WhenBar;
	Callback                      WhenEnter;
	Callback1<WString&>           WhenPasteFilter;
	Callback1<Vector<Highlight>&> WhenHighlight;

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
	void operator=(const WString& s)         { SetText(s); } // Deprecated, use operator<<=
	int     GetLength() const                { return text.GetLength(); }
	int     GetChar(int i) const             { return text[i]; }

	Rect    GetCaretRect(int pos) const;
	Rect    GetCaretRect() const             { return GetCaretRect(cursor); }

	void    Clear();
	void    Reset();
	
	void    Error(bool error = true)         { if(errorbg != error) { errorbg = error; Refresh(); } }
	
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
	EditField& SetColor(Color c);
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
	bool       IsNoBackground() const        { return nobg; }
	bool       IsAlignRight() const          { return alignright; }
	EditField& ShowSpaces(bool b = true)     { showspaces = b; Refresh(); return *this; }
	EditField& NoInternalMargin(bool b = true) { no_internal_margin = b; return *this; }

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
	EditValue& operator=(const DataType& t)  { EditField::SetData(t); return *this; }  // Deprecated, use operator<<=
	operator DataType() const                { return EditField::GetData(); }

	EditValue()                              { SetConvert(*this); }
};

template <class DataType, class Cv>
class EditMinMax : public EditValue<DataType, Cv> {
public:
	EditMinMax& operator=(const DataType& t)          { EditField::SetData(t); return *this; }  // Deprecated, use operator<<=

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
	EditMinMaxNotNull& operator=(const DataType& t)   { EditField::SetData(t); return *this; }  // Deprecated, use operator<<=

	EditMinMaxNotNull()                               { Base::NotNull(); }
	EditMinMaxNotNull(DataType min, DataType max)     { Base::NotNull(); Base::MinMax(min, max); }

	EditMinMaxNotNull& Min(DataType min)              { Base::Min(min); return *this; }
	EditMinMaxNotNull& Max(DataType max)              { Base::Max(max); return *this; }
	EditMinMaxNotNull& MinMax(DataType min, DataType max) { Base::MinMax(min, max); return *this; }
	EditMinMaxNotNull& NotNull(bool nn = true)        { Base::NotNull(nn); return *this; }
};

typedef EditMinMax<int, ConvertInt>              EditInt;
typedef EditMinMax<int64, ConvertInt64>          EditInt64;
typedef EditMinMax<double, ConvertDouble>        EditDouble;
typedef EditMinMax<Date, ConvertDate>            EditDate;
typedef EditMinMax<Time, ConvertTime>            EditTime;
typedef EditMinMaxNotNull<int, EditInt>          EditIntNotNull;
typedef EditMinMaxNotNull<int64, EditInt64>      EditInt64NotNull;
typedef EditMinMaxNotNull<double, EditDouble>    EditDoubleNotNull;
typedef EditMinMaxNotNull<Date, EditDate>        EditDateNotNull;
typedef EditMinMaxNotNull<Time, EditTime>        EditTimeNotNull;

class EditString : public EditValue<WString, ConvertString> {
public:
	operator const WString&() const                  { return GetText(); }

	EditString& operator=(const WString& data)       { SetData(data); return *this; } // Deprecated, use operator<<=
	EditString& operator=(const String& data)        { SetData(data); return *this; } // Deprecated, use operator<<=

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
	EditStringNotNull& operator=(const WString& data) { SetData(data); return *this; } // Deprecated, use operator<<=
	EditStringNotNull& operator=(const String& data)  { SetData(data); return *this; } // Deprecated, use operator<<=

	EditStringNotNull()                               { NotNull(); }
	EditStringNotNull(int maxlen)                     { NotNull(); MaxLen(maxlen); }
};

template <class IncType> IncType WithSpin_DefaultIncValue() { return 1; }
template <> inline       double  WithSpin_DefaultIncValue() { return 0.1; }

template <class DataType, class IncType>
void WithSpin_Add(DataType& value, IncType inc)
{
	value += inc;
}

template <> inline
void WithSpin_Add(double& value, double inc) {
	if(inc < 0) {
		inc = -inc;
		value = (ceil(value / inc - inc / 100) - 1) * inc;
	}
	else
		value = (floor(value / inc + inc / 100) + 1) * inc;
}

template <class DataType, class Base, class IncType = DataType>
class WithSpin : public Base {
public:
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool Key(dword key, int repcnt);

protected:
	void            Inc();
	void            Dec();
	void            Init();

private:
	SpinButtons     sb;
	IncType         inc;

	typedef WithSpin CLASSNAME;
public:

	WithSpin&          SetInc(IncType _inc = 1)     { inc = _inc; return *this; }
	DataType           GetInc() const               { return inc; }

	WithSpin&          OnSides(bool b = true)       { sb.OnSides(b); return *this; }
	bool               IsOnSides() const            { return sb.IsOnSides(); }
	
	WithSpin&          ShowSpin(bool s = true)      { sb.Show(s); return *this; }
	bool               IsSpinVisible() const        { return sb.IsVisible(); }

	SpinButtons&       SpinButtonsObject()          { return sb; }
	const SpinButtons& SpinButtonsObject() const    { return sb; }

	WithSpin();
	WithSpin(IncType inc); // deprecated
	WithSpin(DataType min, DataType max, IncType inc); // deprecated
	virtual ~WithSpin() {}
};

template <class DataType, class Base, class IncType>
WithSpin<DataType, Base, IncType>::WithSpin()
:	inc(WithSpin_DefaultIncValue<IncType>())
{
	Init();
}

template <class DataType, class Base, class IncType>
WithSpin<DataType, Base, IncType>::WithSpin(IncType inc)
:	inc(inc)
{
	Init();
}

template <class DataType, class Base, class IncType>
WithSpin<DataType, Base, IncType>::WithSpin(DataType min, DataType max, IncType inc)
:	inc(WithSpin_DefaultIncValue<IncType>())
{
	Base::MinMax(min, max);
	Init();
}

template <class DataType, class Base, class IncType>
void WithSpin<DataType, Base, IncType>::Init()
{
	Ctrl::AddFrame(sb);
	sb.inc.WhenRepeat = sb.inc.WhenAction = THISBACK(Inc);
	sb.dec.WhenRepeat = sb.dec.WhenAction = THISBACK(Dec);
}

template <class DataType, class Base, class IncType>
void WithSpin<DataType, Base, IncType>::Inc()
{
	if(Ctrl::IsReadOnly()) {
		BeepExclamation();
		return;
	}
	DataType d = Base::GetData();
	if(!IsNull(d)) {
		WithSpin_Add(d, inc);
		if(IsNull(Base::GetMax()) || d <= Base::GetMax()) {
			Base::SetData(d);
			Ctrl::Action();
		}
	}
	else {
		DataType min = Base::GetMin();
		if(!IsNull(min))
			Base::SetData(min);
	}
	Ctrl::SetFocus();
}

template <class DataType, class Base, class IncType>
void WithSpin<DataType, Base, IncType>::Dec()
{
	if(Ctrl::IsReadOnly()) {
		BeepExclamation();
		return;
	}
	DataType d = Base::GetData();
	if(!IsNull(d)) {
		WithSpin_Add(d, -inc);
		if(IsNull(Base::GetMin()) || d >= Base::GetMin()) {
			Base::SetData(d);
			Ctrl::Action();
		}
	}
	else {
		DataType max = Base::GetMax();
		if(!IsNull(max))
			Base::SetData(Base::maxval);
	}
	Ctrl::SetFocus();
}

template <class DataType, class Base, class IncType>
bool WithSpin<DataType, Base, IncType>::Key(dword key, int repcnt)
{
	if(key == K_UP) {
		Inc();
		return true;
	}
	if(key == K_DOWN) {
		Dec();
		return true;
	}
	return Base::Key(key, repcnt);
}

template <class DataType, class Base, class IncType>
void WithSpin<DataType, Base, IncType>::MouseWheel(Point, int zdelta, dword)
{
	if(zdelta < 0)
		Dec();
	else
		Inc();
}

typedef WithSpin<int, EditInt>               EditIntSpin;
typedef WithSpin<int64, EditInt64>           EditInt64Spin;
typedef WithSpin<double, EditDouble>         EditDoubleSpin;
typedef WithSpin<Date, EditDate, int>        EditDateSpin;
typedef WithSpin<Time, EditTime, int>        EditTimeSpin;

typedef WithSpin<int, EditIntNotNull>        EditIntNotNullSpin;
typedef WithSpin<int64, EditInt64NotNull>    EditInt64NotNullSpin;
typedef WithSpin<double, EditDoubleNotNull>  EditDoubleNotNullSpin;
typedef WithSpin<Date, EditDateNotNull, int> EditDateNotNullSpin;
typedef WithSpin<Time, EditTimeNotNull, int> EditTimeNotNullSpin;
