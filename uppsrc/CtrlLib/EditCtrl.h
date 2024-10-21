struct TextArrayOps {
	virtual int64 GetTotal() const = 0;
	virtual int   GetCharAt(int64 i) const = 0;

	bool   GetWordSelection(int64 c, int64& sell, int64& selh);
	int64  GetNextWord(int64 c);
	int64  GetPrevWord(int64 c);

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
	virtual void   State(int);
	virtual Rect   GetCaret() const;

public:
	struct Style : ChStyle<Style> {
		Color paper;
		Color disabled;
		Color focus;
		Color invalid;
		Color text, textdisabled;
		Color selected, selectedtext;
		Color selected0, selectedtext0; // If does not have focus...
		Value edge[4]; // border (for various active edge states)
		Value coloredge; // border mask for adding color, e.g. round borders with red Error
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
	enum {
		ATTR_TEXTCOLOR = Ctrl::ATTR_LAST,
		ATTR_INACTIVE_CONVERT,
		ATTR_CHARFILTER,
		ATTR_NULLICON,
		ATTR_NULLTEXT,
		ATTR_NULLINK,
		ATTR_NULLFONT,
		ATTR_BACKGROUND,
		ATTR_LAST,
	};
	
	ActiveEdgeFrame edge;

	WString    text;
	WString    undotext;
	Rect16     dropcaret;

	const Style    *style;
	CharFilter      filter;
	const Convert  *convert;

	Font            font;

	int             sc;
	int             cursor, anchor;

	int             undocursor, undoanchor;

	int             maxlen;
	int             autosize;
	int             fsell, fselh; // used to hold selection after LostFocus for X11 middle mouse copy

	int             dropcursor;

	byte            charset;

	bool       selclick:1;

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
	void    RefreshAll();
	int     LowChar(int c) const { return 0x25af /*c + 0x2400*/; }
	int     GetCharWidth(int c) const { return font[c < 32 ? LowChar(c) : c]; }
	int     GetTextCx(const wchar *text, int n, bool password) const;
	void    Paints(Draw& w, int& x, int fcy, const wchar *&txt,
		           Color ink, Color paper, int n, bool pwd, Font fnt, Color underline, bool showspaces);
	int     GetStringCx(const wchar *text, int n);
	int     GetCaret(int cursor) const;
	int     GetCursor(int posx);
	void    Finish(bool refresh = true);
	void    SaveUndo();
	void    DoAutoFormat();
	int     GetTy() const;
	void    StdPasteFilter(WString&);
	void    SelSource();
	Color   GetPaper();
	int     GetRightSpace() const;

protected:
	virtual void  HighlightText(Vector<Highlight>& hl);
	virtual int64 GetTotal() const             { return text.GetLength(); }
	virtual int   GetCharAt(int64 pos) const   { return text[(int)pos]; }
	
	// Spin support
	virtual void  PaintSpace(Draw& w);
	virtual int   GetSpaceLeft() const;
	virtual int   GetSpaceRight() const;
	virtual void  EditCapture();
	virtual bool  HasEditCapture();

public:
	Event<Bar&>               WhenBar;
	Event<>                   WhenEnter;
	Event<WString&>           WhenPasteFilter;
	Event<Vector<Highlight>&> WhenHighlight;
	Event<Color>              WhenPaper; // needed to improve visuals of DropChoice

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

	operator const WString&() const          { return text; } // Deprecated, use ~
	void operator=(const WString& s)         { SetText(s); } // Deprecated, use operator<<=
	int     GetLength() const                { return text.GetLength(); }
	int     GetChar(int i) const             { return text[i]; }

	Rect    GetCaretRect(int pos) const;
	Rect    GetCaretRect() const             { return GetCaretRect(cursor); }

	void    Clear();
	void    Reset();
	
	void    Error(bool error = true)         { if(errorbg != error) { errorbg = error; RefreshAll(); } }
	
	EditField& Password(bool pwd = true)     { password = pwd; Finish(); return *this; }
	bool       IsPassword() const            { return password; }
	EditField& SetFilter(int (*f)(int))      { filter = f; return *this; }
	EditField& SetConvert(const Convert& c)  { convert = &c; Refresh(); return *this; }
	EditField& SetInactiveConvert(const Convert& c) { SetVoidPtrAttr(ATTR_INACTIVE_CONVERT, &c); Refresh(); return *this; }
	EditField& AutoFormat(bool b = true)     { autoformat = b; return *this; }
	EditField& NoAutoFormat()                { return AutoFormat(false); }
	bool       IsAutoFormat() const          { return autoformat; }
	EditField& SetCharset(byte cs)           { charset = cs; return *this; }
	EditField& SetFont(Font _font);
	EditField& SetColor(Color c);
	EditField& SetBackground(Color c);
	EditField& ClickSelect(bool b = true)    { clickselect = b; return *this; }
	bool       IsClickSelect() const         { return clickselect; }
	EditField& InitCaps(bool b = true)       { initcaps = b; return *this; }
	bool       IsInitCaps() const            { return initcaps; }
	EditField& NullText(const Image& icon, const char *text = t_("(default)"), Color ink = Null);
	EditField& NullText(const Image& icon, const char *text, Font fnt, Color ink);
	EditField& NullText(const char *text = t_("(default)"), Color ink = Null);
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
	operator DataType() const                { return EditField::GetData(); } // Deprecated, use ~

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
typedef EditMinMax<double, ConvertFloat>         EditFloat;
typedef EditMinMax<Date, ConvertDate>            EditDate;
typedef EditMinMax<Time, ConvertTime>            EditTime;
typedef EditMinMaxNotNull<int, EditInt>          EditIntNotNull;
typedef EditMinMaxNotNull<int64, EditInt64>      EditInt64NotNull;
typedef EditMinMaxNotNull<double, EditFloat>     EditFloatNotNull;
typedef EditMinMaxNotNull<double, EditDouble>    EditDoubleNotNull;
typedef EditMinMaxNotNull<Date, EditDate>        EditDateNotNull;
typedef EditMinMaxNotNull<Time, EditTime>        EditTimeNotNull;

class EditString : public EditValue<WString, ConvertString> {
public:
	operator const WString&() const                  { return GetText(); }  // Deprecated, use ~

	EditString& operator=(const WString& data)       { SetData(data); return *this; } // Deprecated, use operator<<=
	EditString& operator=(const String& data)        { SetData(data); return *this; } // Deprecated, use operator<<=

	EditString() {}
	EditString(int maxlen)                           { MaxLen(maxlen); }

	EditString& MaxLen(int maxlen)                   { ConvertString::MaxLen(maxlen); Ctrl::Refresh(); return *this; }
	EditString& NotNull(bool nn = true)              { ConvertString::NotNull(nn); Ctrl::Refresh(); return *this; }
	EditString& TrimLeft(bool b = true)              { ConvertString::TrimLeft(b); Ctrl::Refresh(); return *this; }
	EditString& TrimRight(bool b = true)             { ConvertString::TrimRight(b); Ctrl::Refresh(); return *this; }
	EditString& TrimBoth(bool b = true)              { ConvertString::TrimBoth(b); Ctrl::Refresh(); return *this; }
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

template <class IncType> IncType WithSpin_DefaultStartValue() { return 0; }
template <> inline       Date    WithSpin_DefaultStartValue() { return GetSysDate(); }
template <> inline       Time    WithSpin_DefaultStartValue() { return GetSysTime(); }

template <class DataType, class IncType>
void WithSpin_Add(DataType& value, IncType inc, DataType min, bool roundfrommin)
{
	value += inc;
}

template <> inline
void WithSpin_Add(int& value, int inc, int min, bool roundfrommin) {
	if(roundfrommin)
		value -= min;
	if(inc < 0) {
		inc = -inc;
		value = (value - inc) / inc * inc;
	}
	else
		value = (value + inc) / inc * inc;
	if(roundfrommin)
		value += min;
}

template <> inline
void WithSpin_Add(double& value, double inc, double min, bool roundfrommin) {
	if(roundfrommin)
		value -= min;
	if(inc < 0) {
		inc = -inc;
		value = (ceil(value / inc - inc / 100) - 1) * inc;
	}
	else
		value = (floor(value / inc + inc / 100) + 1) * inc;
	if(roundfrommin)
		value += min;
}

template <class DataType, class Base, class IncType = DataType>
class WithSpin : public Base, private VirtualButtons {
public:
	virtual void  CancelMode();
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool  Key(dword key, int repcnt);
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);

	virtual int   GetSpaceLeft() const;
	virtual int   GetSpaceRight() const;
	virtual void  PaintSpace(Draw& w);
	virtual void  EditCapture();
	virtual bool  HasEditCapture();

	virtual int   ButtonCount() const;
	virtual Rect  ButtonRect(int i) const;
	virtual const Button::Style& ButtonStyle(int i) const;
	virtual void  ButtonPush(int i);
	virtual void  ButtonRepeat(int i);

protected:
	void            Inc();
	void            Dec();
	void            Init();

private:
	const SpinButtons::Style *style;
	IncType         inc;
	bool            roundfrommin = false;
	bool            visible = true;
	bool            mousewheel = true;
	bool            keys = true;

	typedef WithSpin CLASSNAME;
public:

	WithSpin&          SetInc(IncType _inc = 1)     { inc = _inc; return *this; }
	DataType           GetInc() const               { return inc; }
	
	WithSpin&          SetSpinStyle(const SpinButtons::Style& s) { style = &s; return *this; }

	WithSpin&          OnSides(bool b = true);
	bool               IsOnSides() const            { return style->onsides; }
	
	WithSpin&          ShowSpin(bool b = true)      { visible = b; Base::RefreshLayout(); return *this; }
	bool               IsSpinVisible() const        { return visible; }
	
	WithSpin&          RoundFromMin(bool b = true)  { roundfrommin = b; return *this; }
	
	WithSpin&          MouseWheelSpin(bool b = true){ mousewheel = b; return *this; }
	WithSpin&          NoMouseWheelSpin()           { return MouseWheelSpin(false); }
	
	WithSpin&          KeySpin(bool b = true)       { keys = b; return *this; }
	WithSpin&          NoKeySpin()                  { return KeySpin(false); }

	WithSpin();
	WithSpin(IncType inc); // deprecated
	WithSpin(DataType min, DataType max, IncType inc); // deprecated
	virtual ~WithSpin() {}
};

#include "EditCtrl.hpp"

typedef WithSpin<int, EditInt>               EditIntSpin;
typedef WithSpin<int64, EditInt64>           EditInt64Spin;
typedef WithSpin<double, EditDouble>         EditDoubleSpin;
typedef WithSpin<double, EditFloat>          EditFloatSpin;
typedef WithSpin<Date, EditDate, int>        EditDateSpin;
typedef WithSpin<Time, EditTime, int>        EditTimeSpin;

typedef WithSpin<int, EditIntNotNull>        EditIntNotNullSpin;
typedef WithSpin<int64, EditInt64NotNull>    EditInt64NotNullSpin;
typedef WithSpin<double, EditDoubleNotNull>  EditDoubleNotNullSpin;
typedef WithSpin<double, EditFloatNotNull>   EditFloatNotNullSpin;
typedef WithSpin<Date, EditDateNotNull, int> EditDateNotNullSpin;
typedef WithSpin<Time, EditTimeNotNull, int> EditTimeNotNullSpin;

