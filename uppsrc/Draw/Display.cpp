#include "Draw.h"

NAMESPACE_UPP

#define IMAGECLASS DrawImg
#define IMAGEFILE <Draw/DrawImg.iml>
#include <Draw/iml.h>

#define LLOG(x) // RLOG(x)

AttrText& AttrText::Set(const Value& v)
{
	value = v;
	text = AsString(v).ToWString();
	return *this;
}

AttrText::operator Value() const
{
	return RichToValue(*this);
}

AttrText::AttrText(const Value& v)
{
	if(v.Is<AttrText>())
		*this = ValueTo<AttrText>(v);
	else {
		Init();
		Set(v);
	}
}

void AttrText::Serialize(Stream& s)
{
	int version = 0;
	s / version
	  % text % font % ink % normalink % paper % normalpaper % align % img % imgspc;
}

void AttrText::Jsonize(JsonIO& jio)
{
	jio
		("text", text)
		("value", value)
		("font", font)
		("ink", ink)
		("normalink", normalink)
		("paper", paper)
		("normalpaper", normalpaper)
		("align", align)
		("img", img)
		("imgspc", imgspc)
	;
}

void AttrText::Xmlize(XmlIO& xio)
{
	XmlizeByJsonize(xio, *this);
}

void AttrText::Init()
{
	ink = Null;
	normalink = Null;
	normalpaper = Null;
	paper = Null;
	font = StdFont();
	align = Null;
	imgspc = 0;
}

class StdDisplayClass : public Display
{
public:
	StdDisplayClass(int align = ALIGN_LEFT) : align(align) {}

	virtual void Paint0(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const;
	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;

private:
	int    align;
};

void Display::PaintBackground(Draw& w, const Rect& r, const Value& q,
                              Color ink, Color paper, dword style) const
{
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		if(!IsNull(t.paper))
			paper = t.paper;
		if(!IsNull(t.normalpaper) && !(style & (CURSOR|SELECT|READONLY)))
			paper = t.normalpaper;
	}
	w.DrawRect(r, paper);
}

void Display::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	PaintBackground(w, r, q, ink, paper, style);
	Single<StdDisplayClass>().Paint0(w, r, q, ink, paper, style);
}

Size Display::RatioSize(const Value& q, int cx, int cy) const {
	return Size(cx, cy);
}

Size Display::GetStdSize(const Value& q) const
{
	return Single<StdDisplayClass>().GetStdSize(q);
}


void StdDisplayClass::Paint0(Draw& w, const Rect& r, const Value& q,
                             Color ink, Color paper, dword s) const {
	LLOG("StdDisplay::Paint0: " << q << " ink:" << ink << " paper:" << paper);
	WString txt;
	Font font = StdFont();
	int a = align;
	int x = r.left;
	int width = r.GetWidth();
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		font = t.font;
		if(!IsNull(t.paper))
			paper = t.paper;
		if(!IsNull(t.ink))
			ink = t.ink;
		if(!IsNull(t.normalink) && !(s & (CURSOR|SELECT|READONLY)))
			ink = t.normalink;
		if(!IsNull(t.normalpaper) && !(s & (CURSOR|SELECT|READONLY)))
			paper = t.normalpaper;
		if(!IsNull(t.align))
			a = t.align;
		if(!IsNull(t.img)) {
			Size isz = t.img.GetSize();
			w.DrawImage(x, r.top + max((r.Height() - isz.cy) / 2, 0), t.img);
			x += isz.cx + t.imgspc;
		}
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	Size tsz = GetTLTextSize(txt, font);
	if(a == ALIGN_RIGHT)
		x = r.right - tsz.cx;
	if(a == ALIGN_CENTER)
		x += (width - tsz.cx) / 2;
	int tcy = GetTLTextHeight(txt, font);
	int tt = r.top + max((r.Height() - tcy) / 2, 0);
	if(tsz.cx > width) {
		Size isz = DrawImg::threedots().GetSize();
		int wd = width - isz.cx;
		w.Clip(r.left, r.top, wd, r.GetHeight());
		DrawTLText(w, x, tt, width, txt, font, ink);
		w.End();
		w.DrawImage(r.left + wd, tt + font.Info().GetAscent() - isz.cy, DrawImg::threedots(), ink);
	}
	else
		DrawTLText(w, x, tt, width, txt, font, ink);
}

void StdDisplayClass::Paint(Draw& w, const Rect& r, const Value& q,
                    Color ink, Color paper, dword s) const {
	LLOG("StdDisplay::Paint: " << q << " ink:" << ink << " paper:" << paper);
	PaintBackground(w, r, q, ink, paper, s);
	Paint0(w, r, q, ink, paper, s);
}

Size StdDisplayClass::GetStdSize(const Value& q) const
{
	Font font = StdFont();
	WString txt;
	Size isz(0, 0);
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		font = t.font;
		if(!IsNull(t.img)) {
			isz = t.img.GetSize();
			isz.cx += t.imgspc;
		}
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	Size sz = GetTLTextSize(txt, font);
	return Size(sz.cx + isz.cx, max(sz.cy, isz.cy));
}

#ifdef flagSO
Display::Display() {}
#endif

Display::~Display() {}

const Display& StdDisplay()
{
	return Single<StdDisplayClass>();
}

const Display& GLOBAL_VP_INIT(StdDisplayClass, StdCenterDisplay, (ALIGN_CENTER))
const Display& GLOBAL_VP_INIT(StdDisplayClass, StdRightDisplay, (ALIGN_RIGHT))

#ifdef flagSO
ColorDisplayNull::ColorDisplayNull(String nulltext) : nulltext(nulltext) {}
ColorDisplayNull::~ColorDisplayNull() {}
#endif

void  ColorDisplayNull::Paint(Draw& w, const Rect& r, const Value& q,
							 Color ink, Color paper, dword style) const
{
	if(IsNull(q))
		StdDisplay().Paint(w, r, nulltext, ink, paper, style);
	else
		w.DrawRect(r, Color(q));
}

const Display& ColorDisplay() { return Single<ColorDisplayNull>(); }

class SizeTextDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
};

void  SizeTextDisplayCls::Paint(Draw& w, const Rect& r, const Value& q,
								Color ink, Color, dword) const {
	w.DrawText(r.left, r.top, r.Width(), (String)q, Arial(-r.Height()), ink);
}

const Display& SizeTextDisplay() { return Single<SizeTextDisplayCls>(); }

static inline int NScale(int sz, int r) {
	return sz ? sz < r ? r / sz * sz : r : 0;
}

class CenteredImageDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = q;
		Size sz = m.GetSize();
		if(!IsNull(m))
			w.DrawImage(r.left + (r.Width() - sz.cx) / 2, r.top + (r.Height() - sz.cy) / 2, m);
	}
	virtual Size GetStdSize(const Value& q) const
	{
		return Image(q).GetSize();
	}
};

const Display& CenteredImageDisplay() { return Single<CenteredImageDisplayCls>(); }

class CenteredHighlightImageDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = q;
		Size sz = m.GetSize();
		if(!IsNull(m))
			DrawHighlightImage(w, r.left + (r.Width() - sz.cx) / 2,
			                      r.top + (r.Height() - sz.cy) / 2, m);
	}
	virtual Size GetStdSize(const Value& q) const
	{
		return Image(q).GetSize();
	}
};

const Display& CenteredHighlightImageDisplay()
{
	return Single<CenteredHighlightImageDisplayCls>();
}

class ImageDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = q;
		if(!IsNull(m))
			w.DrawImage(r.left, r.top, Rescale(m, r.GetSize()));
	}
	virtual Size GetStdSize(const Value& q) const
	{
		return Image(q).GetSize();
	}
};

const Display& ImageDisplay() { return Single<ImageDisplayCls>(); }

class FittedImageDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = q;
		if(!IsNull(m)) {
			Size sz = GetFitSize(m.GetSize(), r.Size());
			Point p = r.CenterPos(sz);
			w.DrawImage(p.x, p.y, CachedRescale(m, sz));
		}
	}
	virtual Size GetStdSize(const Value& q) const
	{
		return Image(q).GetSize();
	}
};

const Display& FittedImageDisplay() { return Single<FittedImageDisplayCls>(); }

class DrawingDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	virtual Size RatioSize(const Value& q, int cx, int cy) const;
};

void DrawingDisplayCls::Paint(Draw& w, const Rect& r, const Value& q,
							  Color, Color, dword) const {
	w.DrawDrawing(r, q);
}

Size DrawingDisplayCls::GetStdSize(const Value& q) const {
	return ((const Drawing&) q).GetSize();
}

Size DrawingDisplayCls::RatioSize(const Value& q, int cx, int cy) const {
	return ((const Drawing&) q).RatioSize(cx, cy);
}

const Display& DrawingDisplay() { return Single<DrawingDisplayCls>(); }

void DisplayWithIcon::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	display->PaintBackground(w, r, q, ink, paper, style);
}

void DisplayWithIcon::Paint(Draw& w, const Rect& r0, const Value& q, Color ink, Color paper, dword style) const
{
	Rect r = r0;
	if(!IsNull(icon)) {
		Size isz = icon.GetSize();
		w.DrawImage(r.left, r.top + max((r.Height() - isz.cy) / 2, 0), icon);
		r.left += isz.cx + lspc;
	}
	display->Paint(w, r, q, ink, paper, style);
}

Size DisplayWithIcon::GetStdSize(const Value& q) const
{
	Size sz = display->GetStdSize(q);
	if(!IsNull(icon)) {
		sz.cx += icon.GetSize().cx + lspc;
		sz.cy = max(sz.cy, icon.GetSize().cy);
	}
	return sz;
}

DisplayWithIcon::DisplayWithIcon()
{
	display = &StdDisplay();
	lspc = 0;
}

Size  PaintRect::GetStdSize() const {
	return display ? display->GetStdSize(value) : Size(0, 0);
}

Size  PaintRect::RatioSize(int cx, int cy) const {
	return display ? display->RatioSize(value, cx, cy) : Size(0, 0);
}

void  PaintRect::Paint(Draw& w, const Rect& r,
                       Color ink, Color paper, dword style) const {
	if(display)
		display->Paint(w, r, value, ink, paper, style);
}

void  PaintRect::Paint(Draw& w, int x, int y, int cx, int cy,
					   Color ink, Color paper, dword style) const {
	Paint(w, RectC(x, y, cx, cy), ink, paper, style);
}

PaintRect::PaintRect() {
	display = NULL;
}

PaintRect::PaintRect(const Display& _display) {
	display = &_display;
}

PaintRect::PaintRect(const Display& _display, const Value& _val) {
	display = &_display;
	value = _val;
}

END_UPP_NAMESPACE
