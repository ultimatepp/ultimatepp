enum {
	CTRL_NORMAL, CTRL_HOT, CTRL_PRESSED, CTRL_DISABLED,
	CTRL_CHECKED, CTRL_HOTCHECKED
};

void CtrlsImageLook(Value *look, int i, int n = 4);
void CtrlsImageLook(Value *look, int i, const Image& image, const Color *color, int n = 4);
void CtrlsImageLook(Value *look, int i, const Image& image, int n = 4);

String DeAmp(const char *s);

Size GetSmartTextSize(const char *text, Font font = StdFont(), int cx = INT_MAX);
int  GetSmartTextHeight(const char *s, int cx, Font font = StdFont());
void DrawSmartText(Draw& w, int x, int y, int cx, const char *text,
                   Font font = StdFont(), Color ink = DefaultInk, int accesskey = 0);

int   ExtractAccessKey(const char *s, String& label);
bool  CompareAccessKey(int accesskey, dword key);
int   ChooseAccessKey(const char *s, dword used);

void DrawFocus(Draw& w, int x, int y, int cx, int cy, Color c = SColorText());
void DrawFocus(Draw& w, const Rect& r, Color c = SColorText());

void DrawHorzDrop(Draw& w, int x, int y, int cx);
void DrawVertDrop(Draw& w, int x, int y, int cy);

Point GetDragScroll(Ctrl *ctrl, Point p, Size max);
Point GetDragScroll(Ctrl *ctrl, Point p, int max = 16);

struct DrawLabel {
	bool      push;
	bool      focus;
	bool      disabled;
	bool      limg_never_hide;
	bool      rimg_never_hide;

	PaintRect paintrect;
	Image     limg;
	Color     lcolor;
	int       lspc;
	String    text;
	Font      font;
	Color     ink, disabledink;
	Image     rimg;
	Color     rcolor;
	int       rspc;

	int       align, valign;
	
	bool      nowrap;

	int       accesskey;
	int       accesspos;

	Size      GetSize(int txtcx, Size sz1, int lspc, Size sz2, int rspc) const;
	Size      GetSize(int txtcx = INT_MAX) const;
	Size      Paint(Ctrl *ctrl, Draw& w, const Rect& r, bool visibleaccesskey = true) const;
	Size      Paint(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;
	Size      Paint(Draw& w, const Rect& r, bool visibleaccesskey = true) const;
	Size      Paint(Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;

	DrawLabel();
};

Image DisabledImage(const Image& img, bool disabled = true);
Color GetLabelTextColor(const Ctrl *ctrl);

class LabelBase {
protected:
	virtual void  LabelUpdate();

	DrawLabel   lbl;

public:
	LabelBase&  SetLeftImage(const Image& bmp1, int spc = 0, bool never_hide = false);
	LabelBase&  SetPaintRect(const PaintRect& pr);
	LabelBase&  SetText(const char *text);
	LabelBase&  SetFont(Font font);
	LabelBase&  SetInk(Color color, Color disabledink);
	LabelBase&  SetInk(Color color)                          { return SetInk(color, color); }
	LabelBase&  SetRightImage(const Image& bmp2, int spc = 0, bool never_hide = false);
	LabelBase&  SetAlign(int align);
	LabelBase&  AlignLeft()                                  { return SetAlign(ALIGN_CENTER); }
	LabelBase&  AlignCenter()                                { return SetAlign(ALIGN_CENTER); }
	LabelBase&  AlignRight()                                 { return SetAlign(ALIGN_RIGHT); }
	LabelBase&  SetVAlign(int align);
	LabelBase&  AlignTop()                                   { return SetAlign(ALIGN_TOP); }
	LabelBase&  AlignVCenter()                               { return SetAlign(ALIGN_CENTER); }
	LabelBase&  AlignBottom()                                { return SetAlign(ALIGN_BOTTOM); }
	LabelBase&  SetImage(const Image& bmp, int spc = 0, bool never_hide = false)
	{ SetLeftImage(bmp, spc, never_hide); return *this; }
	LabelBase&  NoWrap(bool b = true);

	int         GetAlign() const                             { return lbl.align; }
	int         GetVAlign() const                            { return lbl.valign; }
	PaintRect   GetPaintRect() const                         { return lbl.paintrect; }
	String      GetText() const                              { return lbl.text; }
	Font        GetFont() const                              { return lbl.font; }
	Color       GetInk() const                               { return lbl.ink; }

	Size        PaintLabel(Ctrl *ctrl, Draw& w, const Rect& r,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Size        PaintLabel(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Size        PaintLabel(Draw& w, const Rect& r,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Size        PaintLabel(Draw& w, int x, int y, int cx, int cy,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Size        GetLabelSize() const;

	virtual ~LabelBase();
};

class DisplayPopup : public Ctrl, public Link<DisplayPopup> {
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftDrag(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  RightDown(Point p, dword);
	virtual void  LeftUp(Point p, dword);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void  MouseLeave();
	virtual void  MouseMove(Point p, dword);

private:
	Ptr<Ctrl>      ctrl;
	Rect           item;
	Rect           slim;

	Value          value;
	Color          paper, ink;
	dword          style;
	const Display *display;
	int            margin;

	Point   Op(Point p);
	void    Sync();

	static Link<DisplayPopup> *all();
	static bool StateHook(Ctrl *, int reason);
	static bool MouseHook(Ctrl *, bool, int, Point, int, dword);
	static void SyncAll();
	
	typedef DisplayPopup CLASSNAME;

public:
	void Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display,
	         Color ink, Color paper, dword style, int margin = 0);
	void Cancel();
	bool IsOpen();
	bool HasMouse();

	DisplayPopup();
	~DisplayPopup();
};
