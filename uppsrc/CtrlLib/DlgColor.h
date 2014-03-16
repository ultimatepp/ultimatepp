#ifndef __CtrlLib_DlgColor__
#define __CtrlLib_DlgColor__

class WheelRampCtrl : public Ctrl
{
public:
	WheelRampCtrl(bool ramp);
	~WheelRampCtrl();

	virtual void  Layout();
	virtual void  Paint(Draw& draw);
	virtual void  SetData(const Value& value);
	virtual Value GetData() const                { return color; }

	virtual void  LeftDown(Point pt, dword keyflags);
	virtual void  LeftUp(Point pt, dword keyflags);
	virtual void  LeftDouble(Point pt, dword keyflags);
	virtual void  MouseMove(Point pt, dword keyflags);

	Callback      WhenLeftDouble;

private:
	void          SetColor(Color color, bool set_norm, bool set_hsv);

	Image         PaintRamp(Size size);
	Image         PaintWheel(Size size);
	void          PaintColumn(Draw& draw);

	int           ClientToLevel(int y) const;
	int           LevelToClient(int l) const;

private:
//	double        gamma;
	bool          ramp;
	Color         color;
	Color         normalized_color;
	int           h16, s16, v16;
	int           round_step;
	enum STYLE { S_WHEEL, S_RECT, S_HEXAGON };
	STYLE         style;
	Image         cache;
	int           cache_level;
	Image         wheel_cache;
	Rect          wheel_rect;
	Rect          column_rect;

	int           firstclick;

	struct WheelBuff
	{
		int arg;
		int l;
	};
};

struct ColorWheelCtrl : public WheelRampCtrl {
	ColorWheelCtrl() : WheelRampCtrl(false) {}
};

struct ColorRampCtrl : public WheelRampCtrl {
	ColorRampCtrl() : WheelRampCtrl(true) {}
};

class ColorSelector : public Ctrl
{
public:
	ColorSelector(bool not_null = true);

	ColorSelector& NotNull(bool nn = true)                  { impl->NotNull(nn); return *this; }
	ColorSelector& NoNotNull()                              { return NotNull(false); }
	bool           IsNotNull() const                        { return impl->IsNotNull(); }

	Color          Get() const                              { return impl->Get(); }
	void           Set(Color c)                             { impl->Set(c); }

	Vector<Color>  GetPalette() const                       { return impl->GetPalette(); }
	void           SetPalette(const Vector<Color>& palette) { impl->SetPalette(palette); }

	void           SerializeConfig(Stream& stream)          { impl->SerializeConfig(stream); }

	virtual void   SetData(const Value& color)              { Set(color); }
	virtual Value  GetData() const                          { return Get(); }

	Ctrl&          GetImplCtrl()                            { return impl->GetCtrl(); }

public:
	Callback       WhenSetColor;

public:
	class Impl
	{
	public:
		virtual ~Impl() {}

		virtual void          Set(Color c) = 0;
		virtual Color         Get() const = 0;
		virtual void          NotNull(bool nn) = 0;
		virtual bool          IsNotNull() const = 0;
		virtual Vector<Color> GetPalette() const = 0;
		virtual void          SetPalette(const Vector<Color>& pal) = 0;
		virtual void          SerializeConfig(Stream& stream) = 0;
		virtual Ctrl&         GetCtrl() = 0;

	protected:
		Impl() {}
	};

protected:
	One<Impl>      impl;
};

class ColorCtrl : public DataPusher
{
public:
	typedef ColorCtrl CLASSNAME;
	ColorCtrl(bool not_null = true);
	virtual ~ColorCtrl();

	ColorCtrl&     NotNull(bool _nn = true) { empty.Show(!_nn); return *this; }
	ColorCtrl&     NoNotNull()              { return NotNull(false); }
	bool           IsNotNull() const        { return !empty.IsVisible(); }

protected:
	virtual void   DoAction();
	void           OnClear()                { SetDataAction(Null); }

protected:
	FrameRight<Button> empty;
};

Color RunDlgSelectColor(Color init_color = Black, bool not_null = true, const char *title = 0, bool *ok = 0);
const Display& StdColorDisplayNull();

class ColorPopUp : public Ctrl {
public:
	virtual  void Paint(Draw& w);
	virtual  void LeftUp(Point p, dword);
	virtual  void LeftDown(Point p, dword);
	virtual  void MouseMove(Point p, dword);
	virtual  void MouseLeave();
	virtual  bool Key(dword key, int count);

private:
	void PopupDeactivate();

	struct Popup : Ctrl {
		ColorPopUp *color;
		
		virtual void Deactivate() { color->PopupDeactivate(); }		
	};

	int      Get(Point p);
	int      GetCy();
	void     Setup(Color c);
	void     Finish();
	void     Ramp();
	void     Wheel();
	int      GetColorCount() const;
	Color    GetColor(int i) const;
	void     Select();

	void 	 DrawFilledFrame(Draw &w, int x, int y, int cx, int cy, Color fcol, Color bcol);
	void 	 DrawFilledFrame(Draw &w, Rect &r, Color fcol, Color bcol);

	int      colori;
	bool     notnull;
	bool     scolors;
	bool     norampwheel;
	bool     animating;
	bool     hints;
	bool     open;
	bool     withvoid;
	String   nulltext;
	String   voidtext;
	Color    color;

	ColorRampCtrl  ramp;
	ColorWheelCtrl wheel;
	One<Popup>     popup;

	static Color   hint[18];
	
	friend void ColorPopUp_InitHint();

public:
	Callback WhenCancel;
	Callback WhenSelect;

	static void Hint(Color c);

	typedef ColorPopUp CLASSNAME;

	void     PopUp(Ctrl *owner, Color c = White);
	Color    Get() const;
	
	ColorPopUp& NotNull(bool b = true)               { notnull = b; return *this; }
	ColorPopUp& SColors(bool b = true)               { scolors = b; return *this; }//Deprecated
	ColorPopUp& NullText(const char *s)              { nulltext = s; Refresh(); return *this; }
	ColorPopUp& VoidText(const char *s)              { voidtext = s; Refresh(); return *this; }
	ColorPopUp& WithVoid(bool b = true)              { withvoid = b; Refresh(); return *this; }
	ColorPopUp& NoRampWheel(bool b = true)           { norampwheel = b; return *this; }
	ColorPopUp& Hints(bool b = true)                 { hints = b; return *this; }

	ColorPopUp();
	virtual ~ColorPopUp();
};

class ColorPusher : public Ctrl {
public:
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword);
	virtual bool  Key(dword key, int);
	virtual void  GotFocus()                  { Refresh(); }
	virtual void  LostFocus()                 { Refresh(); }
	virtual void  SetData(const Value& v);
	virtual Value GetData() const;

protected:
	bool       push;
	bool       withtext;
	bool       track;
	Color      color, saved_color;
	ColorPopUp colors;
	String     nulltext;
	String     voidtext;

	void AcceptColors();
	void CloseColors();
	void NewColor();
	void Drop();

public:
	typedef ColorPusher CLASSNAME;

	ColorPusher& NullText(const char *s)    { nulltext = s; colors.NullText(s); Refresh(); return *this; }
	ColorPusher& NotNull(bool b = true)     { colors.NotNull(b); return *this; }
	ColorPusher& VoidText(const char *s)    { voidtext = s; colors.VoidText(s); Refresh(); return *this; }
	ColorPusher& WithVoid(bool b = true)    { colors.WithVoid(b); return *this; }
	ColorPusher& SColors(bool b = true)     { colors.SColors(b); return *this; }
	ColorPusher& WithText()                 { withtext = true; return *this; }
	ColorPusher& Track(bool b = true)       { track = b; return *this; }
	ColorPusher& NoTrack()                  { return Track(false); }
	ColorPusher& NoRampWheel(bool b = true) { colors.NoRampWheel(b); return *this; }

	ColorPusher();
	virtual ~ColorPusher();
};

class ColorButton : public ColorPusher {
public:
	virtual void  Paint(Draw& w);
	virtual void  MouseEnter(Point p, dword keyflags);
	virtual void  MouseLeave();
	virtual Size  GetMinSize() const;

protected:
	Image      image, nullimage, staticimage;
	const ToolButton::Style *style;

public:
	ColorButton& ColorImage(const Image& img)             { image = img; Refresh(); return *this; }
	ColorButton& NullImage(const Image& img)              { nullimage = img; Refresh(); return *this; }
	ColorButton& StaticImage(const Image& img)            { staticimage = img; Refresh(); return *this; }
	ColorButton& SetStyle(const ToolButton::Style& s)     { style = &s; Refresh(); return *this; }

	ColorButton();
	virtual ~ColorButton();
};

String FormatColor(Color c);
Color  ReadColor(CParser& p);
Color  ColorFromText(const char *s);

#endif//__TCtrlLib_DlgColor__
