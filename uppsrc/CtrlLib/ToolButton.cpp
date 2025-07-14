#include "CtrlLib.h"

namespace Upp {

#define LTIMING(x)  // TIMING(x)

CH_STYLE(ToolButton, Style, StyleDefault)
{
	CtrlsImageLook(look, CtrlsImg::I_TB, 6);
	font = StdFont();
	for(int i = 0; i < 4; i++)
		textcolor[i] = Button::StyleNormal().textcolor[i];
	textcolor[CTRL_CHECKED] = textcolor[CTRL_NORMAL];
	textcolor[CTRL_HOTCHECKED] = textcolor[CTRL_HOT];
	for(int i = 0; i < 6; i++) {
		light[i] = false;
		contrast[i] = 0;
	}
	light[CTRL_PRESSED] = light[CTRL_HOT] = light[CTRL_HOTCHECKED] = true;
	overpaint = 0;
}

CH_STYLE(ToolButton, Style, StyleSolid)
{
	const Button::Style& bs = Button::StyleNormal();
	look[0] = bs.look[0];
	look[1] = bs.look[1];
	look[2] = bs.look[2];
	look[3] = bs.look[3];
	look[4] = bs.look[2];
	look[5] = bs.look[1];
	font = StdFont();
	for(int i = 0; i < 4; i++)
		textcolor[i] = Button::StyleNormal().textcolor[i];
	textcolor[CTRL_CHECKED] = textcolor[CTRL_NORMAL];
	textcolor[CTRL_HOTCHECKED] = textcolor[CTRL_HOT];
	for(int i = 0; i < 6; i++) {
		light[i] = false;
		contrast[i] = 0;
	}
	light[CTRL_PRESSED] = light[CTRL_HOT] = light[CTRL_HOTCHECKED] = true;
}

ToolButton::ToolButton()
{
	Reset();
	checked = false;
	paint_checked = false;
	nodarkadjust = false;
	Transparent();
	kind = NOLABEL;
}

ToolButton::~ToolButton() {}

void ToolButton::ResetKeepStyle()
{
	repeat = false;
	accel = 0;
	checked = false;
	NoWantFocus();
	minsize = Size(0, 0);
	maxiconsize = Size(INT_MAX, INT_MAX);
	Tip("");
	Help("");
	Topic("");
	Description("");
}

void ToolButton::Reset()
{
	ResetKeepStyle();
	style = &StyleDefault();
}

void ToolButton::UpdateTip()
{
	LTIMING("UpdateTip");
	String s = tiptext;
	if(IsNull(s) && kind == NOLABEL)
		s = text;
	if(accel) {
		if(s.GetCount())
			s << ' ';
		s << '(' << GetKeyDesc(accel) << ')';
	}
	Ctrl::Tip(s);
}

Bar::Item& ToolButton::Text(const char *txt)
{
	String newtext;
	ExtractAccessKey(txt, newtext);
	if(newtext != text) {
		text = newtext;
		UpdateTip();
		Refresh();
	}
	return *this;
}

ToolButton& ToolButton::SetStyle(const Style& s)
{
	if(style != &s) {
		style = &s;
		Refresh();
	}
	return *this;
}

Bar::Item& ToolButton::Check(bool check)
{
	if(checked != check) {
		checked = check;
		Refresh();
	}
	return *this;
}

Bar::Item& ToolButton::Radio(bool check)
{
	return Check(check);
}

Bar::Item& ToolButton::Key(dword key)
{
	if(key) {
		accel = key;
		UpdateTip();
	}
	return *this;
}

Bar::Item& ToolButton::Repeat(bool r)
{
	repeat = r;
	return *this;
}

ToolButton& ToolButton::Label(const char *text, int _kind)
{
	if(kind != _kind) {
		kind = _kind;
		Refresh();
	}
	Text(text);
	return *this;
}

ToolButton& ToolButton::Label(const char *text)
{
	Label(text, kind == NOLABEL ? RIGHTLABEL : kind);
	return *this;
}

ToolButton& ToolButton::Kind(int _kind)
{
	if(kind != _kind) {
		kind = _kind;
		Refresh();
	}
	return *this;
}

Image ToolButton::GetImage() const
{
	return CachedRescale(img, min(img.GetSize(), maxiconsize));
}

Bar::Item& ToolButton::Image(const UPP::Image& img_)
{
	Upp::Image m = img_;
	if(!m.IsSame(img)) {
		img = m;
		Refresh();
	}
	return *this;
}

Bar::Item& ToolButton::Enable(bool enable)
{
	Ctrl::Enable(enable);
	return *this;
}

Bar::Item& ToolButton::Tip(const char *tip)
{
	if(tiptext != tip) {
		tiptext = tip;
		UpdateTip();
	}
	return *this;
}

Bar::Item& ToolButton::Help(const char *help)
{
	HelpLine(help);
	return *this;
}

Bar::Item& ToolButton::Topic(const char *help)
{
	HelpTopic(help);
	return *this;
}

Bar::Item& ToolButton::Description(const char *desc)
{
	Ctrl::Description(desc);
	return *this;
}

struct sCachedContrast : public ImageMaker
{
	int   d;
	Image img;

	virtual String Key() const {
		String s;
		RawCat(s, d);
		RawCat(s, img.GetSerialId());
		return s;
	}

	virtual Image Make() const {
		return Contrast(img, 256 + d);
	}
};

Image CachedContrast(const Image& m, int d)
{
	if(d == 0)
		return m;
	sCachedContrast cr;
	cr.d = d;
	cr.img = m;
	return MakeImage(cr);
}

void  ToolButton::Paint(Draw& w)
{
	LTIMING("ToolButton::Paint");
	paint_checked = checked;
	Size sz = GetSize();
	UPP::Image image = GetImage();
	Size isz = image.GetSize();
//	Ctrl *q = GetParent()->GetParent();
//	if(!q || !q->IsTransparent())
//		w.DrawRect(sz, checked && !HasMouse() ? Blend(SColorFace, SColorLight) : SColorFace);
	int li = IsEnabled() ? HasMouse() ? GetMouseLeft() ? CTRL_PRESSED
						                               : checked ? CTRL_HOTCHECKED : CTRL_HOT
				                       : checked ? CTRL_CHECKED : CTRL_NORMAL
					     : CTRL_DISABLED;
	ChPaint(w, sz, style->look[li]);
	Point off = style->offset[li];
	Point ip = (sz - isz) / 2 + off;
	Size tsz(0, 0);
	if(kind != NOLABEL)
		tsz = GetTextSize(text, style->font);
	if(kind == BOTTOMLABEL) {
		ip.y  -= tsz.cy / 2 + 1;
		w.DrawText((sz.cx - tsz.cx) / 2 + off.x, ip.y + isz.cy + 2 + off.y, text, style->font, style->textcolor[li]);
	}
	if(kind == RIGHTLABEL) {
		ip.x -= tsz.cx / 2 + 2;
		w.DrawText(ip.x + isz.cx + 3 + off.x, (sz.cy - tsz.cy) / 2 + off.y, text, style->font, style->textcolor[li]);
	}
	UPP::Image img = CachedContrast(image, style->contrast[li]);
	if(!IsEnabled())
		img = DisabledImage(img);
	if(IsEnabled() && style->light[li])
		DrawHighlightImage(w, ip.x, ip.y, img, true);
	else
		w.DrawImage(ip.x, ip.y, img);
}

void  ToolButton::MouseEnter(Point, dword)
{
	BarCtrl::SendHelpLine(this);
	Refresh();
}

void  ToolButton::MouseLeave()
{
	BarCtrl::ClearHelpLine(this);
	Refresh();
}

Size  ToolButton::GetMinSize() const
{
	UPP::Image image = GetImage();
	Size sz = image.GetSize();
	if(sz.IsEmpty())
		sz = Size(16, 16);
	sz += DPI(6);
	if(text.GetCount()) {
		Size tsz = GetTextSize(text, style->font);
		if(kind == BOTTOMLABEL) {
			sz.cy += tsz.cy + DPI(3);
			sz.cx = max(sz.cx, tsz.cx + DPI(9));
		}
		if(kind == RIGHTLABEL) {
			sz.cx += tsz.cx + DPI(6);
			sz.cy = max(sz.cy, tsz.cy + DPI(6));
		}
	}
	return max(sz, minsize);
}

void  ToolButton::LeftDown(Point, dword)
{
	Refresh();
	if(repeat)
		Action();
}

void  ToolButton::LeftRepeat(Point, dword)
{
	Refresh();
	if(repeat)
		Action();
}

void  ToolButton::LeftUp(Point, dword)
{
	Refresh();
	if(!repeat)
		Action();
}

bool  ToolButton::HotKey(dword key)
{
	if(key == accel) {
		Action();
		return true;
	}
	return false;
}

void ToolButton::FinalSync()
{
	if(checked != paint_checked)
		Refresh();
}

String ToolButton::GetDesc() const
{
	return tiptext;
}

int ToolButton::OverPaint() const
{
	return style->overpaint;
}

}
