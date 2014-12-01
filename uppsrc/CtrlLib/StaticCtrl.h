class StaticText : public Ctrl, public LabelBase {
public:
	virtual void   Paint(Draw& w);
	virtual Size   GetMinSize() const;
	virtual void   LabelUpdate();

public:
	StaticText& SetFont(Font font)                      { LabelBase::SetFont(font); return *this; }
	StaticText& SetInk(Color color)                     { LabelBase::SetInk(color); return *this; }
	StaticText& SetAlign(int align)                     { LabelBase::SetAlign(align); return *this; }
	StaticText& SetImage(const Image& img, int spc = 0) { LabelBase::SetImage(img, spc); return *this; }
	StaticText& SetText(const char *text)               { LabelBase::SetText(text); return *this; }

	StaticText& operator=(const char *s)                { SetText(s); return *this; }

	StaticText();
};

class Label : public StaticText {
public:
	virtual bool   HotKey(dword key);
	virtual dword  GetAccessKeys() const;
	virtual void   AssignAccessKeys(dword used);

private:
	bool   noac;

public:
	Label& SetText(const char *text);
	Label& SetLabel(const char *lbl);

	Label& operator=(const char *s)                     { SetText(s); return *this; }

	Label();
	virtual ~Label();
};

class LabelBox : public Label {
public:
	virtual void   Paint(Draw& w);
	virtual void   AssignAccessKeys(dword used);
	virtual Rect   GetVoidRect() const;

	Color color;

public:
	LabelBox& SetColor(Color c)          { color = c; return *this; }

	LabelBox();
	virtual ~LabelBox();
	LabelBox& operator=(const char *s)   { SetText(s); return *this; }
};

Color LabelBoxTextColor();
Color LabelBoxColor();

void LabelBoxTextColor_Write(Color c);
void LabelBoxColor_Write(Color c);

class ParentCtrl : public Ctrl {
	Size minsize;

public:
	virtual Rect   GetVoidRect() const;
	virtual Size   GetStdSize() const;
	virtual Size   GetMinSize() const;

	void    SetMinSize(Size sz)         { minsize = sz; }

	ParentCtrl();
};

class StaticRect : public Ctrl {
public:
	virtual void   Paint(Draw& w);

protected:
	Value bg;

public:
	StaticRect& Background(const Value& chvalue);
	StaticRect& Color(class Color c)                   { Background(c); return *this; }

	Value GetBackground() const                        { return bg; }

	StaticRect();
	virtual ~StaticRect();
};

class ImageCtrl : public Ctrl {
public:
	virtual void   Paint(Draw& w);
	virtual Size   GetStdSize() const;
	virtual Size   GetMinSize() const;

protected:
	Image   img;

public:
	ImageCtrl&   SetImage(const Image& _img)          { img = _img; Refresh(); return *this; }

	ImageCtrl()                                       { Transparent(); NoWantFocus(); }
};

class DisplayCtrl : public Ctrl {
public:
	virtual void   Paint(Draw& w);
	virtual Size   GetMinSize() const;
	virtual void   SetData(const Value& v);
	virtual Value  GetData() const;

private:
	PaintRect pr;

public:
	void SetDisplay(const Display& d);
};

// BWC
typedef ImageCtrl Icon;

class Picture : public Ctrl {
public:
	virtual void   Paint(Draw& w);

protected:
	Drawing picture;
	Color   background;
	bool    ratio;

public:
	Drawing  Get() const                         { return picture; }

	Picture& Background(Color color);
	Picture& KeepRatio(bool keep = true)         { ratio = keep; Refresh(); return *this; }
	Picture& NoKeepRatio()                       { return KeepRatio(false); }
	Picture& Set(const Drawing& _picture)        { picture = _picture; Refresh(); return *this; }

	Picture& operator=(const Drawing& _picture)  { return Set(_picture); }
	Picture& operator=(const Painting& _picture) { return Set(AsDrawing(_picture)); }

	Picture();
};

class SeparatorCtrl : public Ctrl {
public:
	virtual Size GetMinSize() const;
	virtual void Paint(Draw& w);

	struct Style : ChStyle<Style> {
		Value l1, l2;
	};

private:
	int          lmargin, rmargin;
	int          size;
	const Style *style;

public:
	static const Style& StyleDefault();

	SeparatorCtrl& Margin(int l, int r);
	SeparatorCtrl& Margin(int w)                { return Margin(w, w); }
	SeparatorCtrl& SetSize(int w);
	SeparatorCtrl& SetStyle(const Style& s);

	SeparatorCtrl();
};
