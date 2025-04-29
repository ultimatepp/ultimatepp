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
                   Font font = StdFont(), Color ink = SBlack(), int accesskey = 0,
                   Color qtf_ink = Null, int dark_theme = Null);

enum { ORIENTATION_NORMAL, ORIENTATION_CLOCKWISE, ORIENTATION_ANTICLOCKWISE };

void DrawSmartText(Draw& w, int x, int y, int cx, const char *text, int orientation,
                   Font font = StdFont(), Color ink = SBlack(), int accesskey = 0,
                   Color qtf_ink = Null, int dark_theme = Null);

int   ExtractAccessKey(const char *s, String& label);
bool  CompareAccessKey(int accesskey, dword key);
int   ChooseAccessKey(const char *s, dword used);

void DrawFocus(Draw& w, int x, int y, int cx, int cy, Color c = SColorText());
void DrawFocus(Draw& w, const Rect& r, Color c = SColorText());

void DrawHorzDrop(Draw& w, int x, int y, int cx);
void DrawVertDrop(Draw& w, int x, int y, int cy);

Point GetDragScroll(Ctrl *ctrl, Point p, Size max);
Point GetDragScroll(Ctrl *ctrl, Point p, int max = 16);

struct DrawLabelBasic {
	String    text;
	Font      font;
	Color     ink;
	Color     disabledink;

	int       accesskey;

	int       align:4;
	int       valign:4;
	int       orientation:4;
	
	bool      nowrap:1;
	
	DrawLabelBasic() { align = valign = ALIGN_CENTER; orientation = ORIENTATION_NORMAL; nowrap = false; accesskey = 0; font = StdFont(); }
};

struct DrawLabelExt {
	PaintRect paintrect;

	Image     limg;
	Image     rimg;

	Color     lcolor;
	int       lspc = 0;
	Color     rcolor;
	int       rspc = 0;

	bool      limg_never_hide = false;
	bool      rimg_never_hide = false;
};

struct DrawLabel : DrawLabelBasic, DrawLabelExt {
	bool      push = false;
	bool      focus = false;
	bool      disabled = false;

	Size      GetSize(int txtcx, Size sz1, int lspc, Size sz2, int rspc) const;
	Size      GetSize(int txtcx = INT_MAX) const;
	Rect      PaintRect(Ctrl *ctrl, Draw& w, const Rect& r, bool visibleaccesskey) const;
	Size      Paint(Ctrl *ctrl, Draw& w, const Rect& r, bool visibleaccesskey = true) const;
	Size      Paint(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;
	Rect      PaintRect(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;
	Size      Paint(Draw& w, const Rect& r, bool visibleaccesskey = true) const;
	Size      Paint(Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;
	Rect      PaintRect(Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;
};

Image DisabledImage(const Image& img, bool disabled = true);
Color GetLabelTextColor(const Ctrl *ctrl);

class LabelBase {
protected:
	virtual void  LabelUpdate();

	DrawLabelBasic    lbl;
	One<DrawLabelExt> ext;
	
	DrawLabelExt& Ext()                                      { if(!ext) ext.Create() ; return *ext; }
	DrawLabel     Make() const;

public:
	LabelBase&  SetLeftImage(const Image& bmp1, int spc = 0, bool never_hide = false);
	LabelBase&  SetPaintRect(const PaintRect& pr);
	LabelBase&  SetText(const char *text);
	LabelBase&  SetFont(Font font);
	LabelBase&  SetInk(Color color, Color disabledink);
	LabelBase&  SetInk(Color color)                          { return SetInk(color, color); }
	LabelBase&  SetRightImage(const Image& bmp2, int spc = 0, bool never_hide = false);
	LabelBase&  SetAlign(int align);
	LabelBase&  AlignLeft()                                  { return SetAlign(ALIGN_LEFT); }
	LabelBase&  AlignCenter()                                { return SetAlign(ALIGN_CENTER); }
	LabelBase&  AlignRight()                                 { return SetAlign(ALIGN_RIGHT); }
	LabelBase&  SetVAlign(int align);
	LabelBase&  AlignTop()                                   { return SetVAlign(ALIGN_TOP); }
	LabelBase&  AlignVCenter()                               { return SetVAlign(ALIGN_CENTER); }
	LabelBase&  AlignBottom()                                { return SetVAlign(ALIGN_BOTTOM); }
	LabelBase&  SetImage(const Image& bmp, int spc = 0, bool never_hide = false)
	{ SetLeftImage(bmp, spc, never_hide); return *this; }
	LabelBase&  NoWrap(bool b = true);

	int         GetAlign() const                             { return lbl.align; }
	int         GetVAlign() const                            { return lbl.valign; }
	PaintRect   GetPaintRect() const                         { return ext ? ext->paintrect : PaintRect(); }
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
	
	LabelBase(const LabelBase& src)   { lbl = src.lbl; if(src.ext) ext = clone(src.ext); }
	LabelBase()                       {}

	virtual ~LabelBase();
};

Rect LookMargins(const Rect& r, const Value& ch);

class ActiveEdgeFrame : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAddSize(Size& sz);

private:
	Value        coloredge;
	const Value *edge;
	const Ctrl  *ctrl;
	Color        color;
	bool         mousein:1;
	bool         push:1;
	bool         button:1;

public:
	void Set(const Ctrl *ctrl, const Value *edge, bool active);
	void Mouse(bool in)                     { mousein = in; }
	void Push(bool b)                       { button = true; push = b; }
	void SetColor(const Value& ce, Color c) { coloredge = ce; color = c; }

	ActiveEdgeFrame() { edge = NULL; mousein = push = button = false; }
};
