#include "Draw.h"

NAMESPACE_UPP

#define IMAGECLASS DrawImg
#define IMAGEFILE <Draw/DrawImg.iml>
#include <Draw/iml.h>

#define LLOG(x) // RLOG(x)

AttrText::operator Value() const
{
	return RawToValue(*this);
}

void AttrText::Init()
{
	ink = Null;
	paper = Null;
	font = Null;
	align = Null;
}

AttrText::AttrText(const char *_text)
{
	text = _text;
	Init();
}

AttrText::AttrText(const wchar *_text)
{
	text = _text;
	Init();
}

AttrText::AttrText(const WString& _text)
{
	text = _text;
	Init();
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
	}
	w.DrawRect(r, paper);
}

void Display::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	StdDisplay().Paint(w, r, q, ink, paper, style);
}

Size Display::RatioSize(const Value& q, int cx, int cy) const {
	return Size(cx, cy);
}

Size Display::GetStdSize(const Value& q) const
{
	return Size(8, 8);
}

void StdDisplayClass::Paint0(Draw& w, const Rect& r, const Value& q,
                             Color ink, Color paper, dword s) const {
	LLOG("Display::Paint0: " << q << " ink:" << ink << " paper:" << paper);
	WString txt;
	Font font = StdFont();
	int a = align;
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		if(!IsNull(t.paper))
			paper = t.paper;
		if(!IsNull(t.ink))
			ink = t.ink;
		if(!IsNull(t.font))
			font = t.font;
		if(!IsNull(t.align))
			a = t.align;
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	int x = r.left;
	Size tsz = GetTLTextSize(txt, font);
	if(a == ALIGN_RIGHT)
		x = r.right - tsz.cx;
	if(a == ALIGN_CENTER)
		x += (r.Width() - tsz.cx) / 2;
	int tcy = GetTLTextHeight(txt, font);
	int tt = r.top + max((r.Height() - tcy) / 2, 0);
	if(tsz.cx > r.GetWidth()) {
		Size isz = DrawImg::threedots().GetSize();
		int wd = r.GetWidth() - isz.cx;
		w.Clip(r.left, r.top, wd, r.GetHeight());
		DrawTLText(w, x, tt, r.Width(), txt, font, ink);
		w.End();
		w.DrawImage(r.left + wd, tt + font.Info().GetAscent() - isz.cy, DrawImg::threedots(), ink);
	}
	else
		DrawTLText(w, x, tt, r.Width(), txt, font, ink);
}

void StdDisplayClass::Paint(Draw& w, const Rect& r, const Value& q,
                    Color ink, Color paper, dword s) const {
	LLOG("Display::Paint: " << q << " ink:" << ink << " paper:" << paper);
	PaintBackground(w, r, q, ink, paper, s);
	Paint0(w, r, q, ink, paper, s);
}

Size StdDisplayClass::GetStdSize(const Value& q) const
{
	Font font = StdFont();
	WString txt;
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		if(!IsNull(t.font))
			font = t.font;
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	return GetTLTextSize(txt, font);
}

#ifdef flagSO
Display::Display() {}
#endif

Display::~Display() {}

const Display& GLOBAL_V_INIT(StdDisplayClass, StdDisplay)
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
			w.DrawImage(p.x, p.y, m);
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
