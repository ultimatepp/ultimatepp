#include "Draw.h"

namespace Upp {

#define IMAGECLASS DrawImg
#define IMAGEFILE <Draw/DrawImg.iml>
#include <Draw/iml_source.h>

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

bool AttrText::operator==(const AttrText& f) const
{
	return text == f.text && value == f.value && font == f.font && ink == f.ink &&
	       normalink == f.normalink && paper == f.paper && normalpaper == f.normalpaper &&
	       align == f.align && img == f.img && imgspc == f.imgspc;
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

// With CtrlLib, support qtf '\1' escape

Size (*extGetSmartTextSize)(const char *text, Font font, int cx);

Size ExtGetSmartTextSize(const char *text, Font font, int cx)
{
	return extGetSmartTextSize ? extGetSmartTextSize(text, font, cx)
	                           : GetTLTextSize(ToUnicode(text, CHARSET_DEFAULT), font);
}

void (*extDrawSmartText)(Draw& draw, int x, int y, int cx, const char *text, Font font,
                         Color ink, int accesskey, Color qtf_ink);

void ExtDrawSmartText(Draw& draw, int x, int y, int cx, const char *text, Font font,
                      Color ink, int accesskey, Color qtf_ink) {
	if(extDrawSmartText)
		extDrawSmartText(draw, x, y, cx, text, font, ink, accesskey, qtf_ink);
	else
		DrawTLText(draw, x, y, cx, ToUnicode(text, CHARSET_DEFAULT), font, ink, accesskey);
}

void StdDisplayClass::Paint0(Draw& w, const Rect& r, const Value& q,
                             Color ink, Color paper, dword s) const {
	LLOG("StdDisplay::Paint0: " << q << " ink:" << ink << " paper:" << paper);
	String txt;
	Font font = StdFont();
	int a = align;
	int x = r.left;
	int width = r.GetWidth();
	bool alter_color = s & (CURSOR|SELECT|READONLY);
	if(q.Is<Image>()) {
		const Image& m = q.To<Image>();
		Size isz = m.GetSize();
		w.DrawImage(r.left + max((r.Width() - isz.cx) / 2, 0),
		            r.top + max((r.Height() - isz.cy) / 2, 0),
		            m);
		return;
	}
	if(q.Is<AttrText>()) {
		const AttrText& t = q.To<AttrText>();
		txt = t.text.ToString();
		font = t.font;
		if(!IsNull(t.paper))
			paper = t.paper;
		if(!IsNull(t.ink))
			ink = t.ink;
		if(!IsNull(t.normalink) && !alter_color)
			ink = t.normalink;
		if(!IsNull(t.normalpaper) && !alter_color)
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
	Size tsz = ExtGetSmartTextSize(txt, font, INT_MAX);
	if(txt.GetCount() == 0)
		tsz.cy = 0;
	if(a == ALIGN_RIGHT)
		x = r.right - tsz.cx;
	if(a == ALIGN_CENTER)
		x += (width - tsz.cx) / 2;
	int tcy = tsz.cy;
	int tt = r.top + (tcy < 4 * r.GetHeight() / 3 ?  (r.Height() - tcy) / 2 : 0); // allow negative tt if only slightly bigger
	if(tsz.cx > width) {
		Size isz = DrawImg::threedots().GetSize();
		int wd = width - isz.cx;
		w.Clip(r.left, r.top, wd, r.GetHeight());
		ExtDrawSmartText(w, x, tt, tsz.cx, txt, font, ink, 0, alter_color ? ink : Null);
		w.End();
		w.DrawImage(r.left + wd, tt + font.Info().GetAscent() - isz.cy, DrawImg::threedots(), ink);
	}
	else
		ExtDrawSmartText(w, x, tt, tsz.cx, txt, font, ink, 0, alter_color ? ink : Null);
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
	String txt;
	Size isz(0, 0);
	if(q.Is<Image>()) {
		const Image& m = q.To<Image>();
		return m.GetSize();
	}
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text.ToString();
		font = t.font;
		if(!IsNull(t.img)) {
			isz = t.img.GetSize();
			isz.cx += t.imgspc;
		}
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	Size sz = ExtGetSmartTextSize(txt, font, INT_MAX);
	if(txt.GetCount() == 0)
		sz.cy = 0;
	return Size(sz.cx + isz.cx, max(sz.cy, isz.cy));
}

Display::~Display() {}

const Display& StdDisplay()
{
	return Single<StdDisplayClass>();
}

const Display& StdCenterDisplay() { static StdDisplayClass h(ALIGN_CENTER); return h; }
const Display& StdRightDisplay() { static StdDisplayClass h(ALIGN_RIGHT); return h; }

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

}
