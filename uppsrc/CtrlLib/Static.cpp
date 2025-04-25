#include "CtrlLib.h"

namespace Upp {

CH_COLOR(LabelBoxColor, SColorShadow());

CH_COLOR(LabelBoxTextColor, SColorText());
CH_COLOR(LabelBoxDisabledTextColor, SColorDisabled());

StaticText& StaticText::SetFont(Font font)
{
	SetFontAttr(ATTR_FONT, font);
	Refresh();
	return *this;
}

StaticText& StaticText::SetInk(Color color)
{
	SetColorAttr(ATTR_INK, color);
	Refresh();
	return *this;
}

StaticText& StaticText::SetAlign(int align)
{
	SetIntAttr(ATTR_ALIGN, align);
	Refresh();
	return *this;
}

StaticText& StaticText::SetVAlign(int align)
{
	SetIntAttr(ATTR_VALIGN, align);
	Refresh();
	return *this;
}

StaticText& StaticText::SetImage(const Image& img, int spc)
{
	CreateAttr<Image>(ATTR_IMAGE) = img;
	if(spc)
		SetIntAttr(ATTR_IMAGE_SPC, spc);
	Refresh();
	return *this;
}

StaticText& StaticText::SetText(const char *s)
{
	text = s;
	Refresh();
	return *this;
}

StaticText& StaticText::SetOrientation(int orientation)
{
	SetIntAttr(ATTR_ORIENTATION, orientation);
	Refresh();
	return *this;
}

void StaticText::MakeDrawLabel(DrawLabel& l) const
{
	l.text = text;
	l.font = GetFont();
	l.ink = GetInk();
	l.align = GetAlign();
	l.valign = GetVAlign();
	l.limg = GetImage();
	l.lspc = Nvl(GetIntAttr(ATTR_IMAGE_SPC), 0);
	l.disabled = !IsShowEnabled();
	l.orientation = GetOrientation();
	l.accesskey = accesskey;
	if(dynamic_cast<const LabelBox *>(this)) {
		l.valign = ALIGN_TOP;
		l.orientation = ORIENTATION_NORMAL;
		l.disabledink = LabelBoxDisabledTextColor();
	}
}

void StaticText::Paint(Draw& w)
{
	DrawLabel l;
	MakeDrawLabel(l);
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);

	l.Paint(this, w, sz, VisibleAccessKeys());
}

Size StaticText::GetMinSize() const
{
	return GetLabelSize();
}

Size StaticText::GetLabelSize() const
{
	DrawLabel l;
	MakeDrawLabel(l);
	return l.GetSize();
}

StaticText::StaticText()
{
	NoWantFocus();
	IgnoreMouse();
	Transparent();
}

StaticText::~StaticText()
{
	DeleteAttr<Image>(ATTR_IMAGE);
}

Label& Label::SetText(const char *text)
{
	accesskey = 0;
	StaticText::SetText(text);
	noac = false;
	return *this;
}

Label& Label::SetLabel(const char *_text)
{
	String text;
	accesskey = ExtractAccessKey(_text, text);
	StaticText::SetText(text);
	return *this;
}

bool Label::HotKey(dword key) {
	if(CompareAccessKey(accesskey, key)) {
		IterateFocusForward(this, GetParent());
		return true;
	}
	return false;
}

dword Label::GetAccessKeys() const
{
	return AccessKeyBit(accesskey);
}

void  Label::AssignAccessKeys(dword used)
{
	if(noac)
		return;
	Ctrl *next = GetNext();
	if(!accesskey && next && next->IsInitFocus()) {
		next->AssignAccessKeys(used);
		if(!next->GetAccessKeysDeep()) {
			accesskey = ChooseAccessKey(text, used);
			if(accesskey) Refresh();
			used |= AccessKeyBit(accesskey);
		}
	}
	Ctrl::AssignAccessKeys(used);
}

Label::Label() {
	noac = false;
}

Label::~Label() {}

LabelBox::LabelBox()
{
	color = Null;
	SetInk(LabelBoxTextColor());
}

void  LabelBox::AssignAccessKeys(dword used)
{
	Ctrl::AssignAccessKeys(used);
}

LabelBox::~LabelBox() {}

Rect LabelBox::GetVoidRect() const
{
	Rect r = GetSize();
	r.left += 2;
	r.top += max(GetLabelSize().cy, 2);
	r.bottom -= 2;
	r.right -= 2;
	return r;
}

Value LabelBox::LabelBoxLook;

void PaintLabelBox(Draw& w, Size sz, Color color, int d)
{
	bool hline = sz.cy < 2 * Draw::GetStdFontCy();
	bool vline = sz.cx < 2 * Draw::GetStdFontCy();
	if(GUI_GlobalStyle() >= GUISTYLE_XP || !IsNull(color)) {
		if(hline) {
			d = sz.cy / 2;
			w.DrawRect(0, d - 1, sz.cx, 1, SColorLight);
			w.DrawRect(0, d, sz.cx, 1, SColorShadow);
			w.DrawRect(0, d + 1, sz.cx, 1, SColorLight);
		}
		else
		if(vline) {
			d = sz.cx / 2;
			w.DrawRect(d - 1, 0, 1, sz.cy, SColorLight);
			w.DrawRect(d, 0, 1, sz.cy, SColorShadow);
			w.DrawRect(d + 1, 0, 1, sz.cy, SColorLight);
		}
		else {
			Value look = LabelBox::GetLook();
			if(IsNull(look) || !IsNull(color)) {
				Color c = Nvl(color, LabelBoxColor);
				w.DrawRect(0, d + 2, 1, sz.cy - d - 4, c);
				w.DrawRect(sz.cx - 1, d + 2, 1, sz.cy - d - 4, c);
				w.DrawRect(2, sz.cy - 1, sz.cx - 4, 1, c);
				w.DrawRect(2, d, sz.cx - 4, 1, c);
	
				w.DrawRect(1, d + 1, 2, 1, c);
				w.DrawRect(1, d + 2, 1, 1, c);
	
				w.DrawRect(sz.cx - 3, d + 1, 2, 1, c);
				w.DrawRect(sz.cx - 2, d + 2, 1, 1, c);
	
				w.DrawRect(1, sz.cy - 2, 2, 1, c);
				w.DrawRect(1, sz.cy - 3, 1, 1, c);
	
				w.DrawRect(sz.cx - 3, sz.cy - 2, 2, 1, c);
				w.DrawRect(sz.cx - 2, sz.cy - 3, 1, 1, c);
			}
			else {
				Rect r = sz;
				r.top = d;
				ChPaintEdge(w, r, look);
			}
		}
	}
	else {
		if(hline) {
			d = sz.cy / 2;
			w.DrawRect(0, d, sz.cx, 1, SColorShadow);
			w.DrawRect(0, d + 1, sz.cx, 1, SColorLight);
		}
		else
		if(vline) {
			d = sz.cx / 2;
			w.DrawRect(d, 0, 1, sz.cy, SColorShadow);
			w.DrawRect(d - 1, 1, 0, sz.cy, SColorLight);
		}
		else {
			w.DrawRect(1, d, sz.cx - 2, 1, SColorShadow);
			w.DrawRect(1, d + 1, sz.cx - 2, 1, SColorLight);

			w.DrawRect(0, d, 1, sz.cy - d - 1, SColorShadow);
			w.DrawRect(1, d + 1, 1, sz.cy - d - 2, SColorLight);

			w.DrawRect(sz.cx - 2, d, 1, sz.cy - d, SColorShadow);
			w.DrawRect(sz.cx - 1, d, 1, sz.cy - d, SColorLight);

			w.DrawRect(1, sz.cy - 2, sz.cx - 2, 1, SColorShadow);
			w.DrawRect(1, sz.cy - 1, sz.cx - 2, 1, SColorLight);
		}
	}
}

void LabelBox::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(sz, SColorFace);
	Size lsz = GetLabelSize();
	int d = lsz.cy >> 1;
	int ty = sz.cy < 2 * Draw::GetStdFontCy() ? (sz.cy - lsz.cy) / 2 : 0;
	DrawLabel l;
	MakeDrawLabel(l);
	Rect tr = l.PaintRect(this, w, d + DPI(2), ty, sz.cx - 2 * (d + DPI(2)), lsz.cy);
	w.Begin();
	w.ExcludeClip(tr.left - DPI(2), ty, tr.GetWidth() + DPI(4), tr.GetHeight());
	PaintLabelBox(w, sz, color, d);
	w.End();
}

Rect ParentCtrl::GetVoidRect() const
{
	return GetSize();
}

Size ParentCtrl::GetStdSize() const
{
	return GetMinSize();
}

Size ParentCtrl::GetMinSize() const
{
	return IsNull(minsize) ? Ctrl::GetMinSize() : minsize;
}

ParentCtrl::ParentCtrl()
{
	NoWantFocus();
	Transparent();
	minsize = Null;
}

StaticRect& StaticRect::Background(const Value& chvalue)
{
	if(chvalue != bg) {
		bg = chvalue;
		Refresh();
	}
	return *this;
}

void StaticRect::Paint(Draw& w)
{
	ChPaint(w, GetSize(), bg);
}

StaticRect::StaticRect() {
	bg = SColorFace();
	NoWantFocus();
}

StaticRect::~StaticRect() {}

void ImageCtrl::Paint(Draw& w)
{
	if(!img) return;
	Size sz = GetSize();
	Size bsz = GetStdSize();
	w.DrawImage((sz.cx - bsz.cx) / 2, (sz.cy - bsz.cy) / 2, img);
}

Size ImageCtrl::GetStdSize() const
{
	return img.GetSize();
}

Size ImageCtrl::GetMinSize() const
{
	return img.GetSize();
}

ImageCtrl& ImageCtrl::SetImage(const Image& _img)
{
	if(img.IsSame(_img))
		return *this;
	img = _img;
	Refresh();
	return *this;
}

DrawingCtrl& DrawingCtrl::Background(Color color)
{
	background = color;
	Transparent(IsNull(color));
	Refresh();
	return *this;
}

void DrawingCtrl::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, background);
	if(!picture) return;
	int dx = 0;
	int dy = 0;
	Size rz = sz;
	if(ratio) {
		Size sr = picture.GetSize();
		if(sr.cy * sz.cx < sz.cy * sr.cx) {
			if(sr.cx) {
				rz.cy = sr.cy * sz.cx / sr.cx;
				dy = (sz.cy - rz.cy) / 2;
			}
		}
		else {
			if(sr.cy) {
				rz.cx = sr.cx * sz.cy / sr.cy;
				dx = (sz.cx - rz.cx) / 2;
			}
		}
	}
	w.Clipoff(dx, dy, rz.cx, rz.cy);
	w.DrawDrawing(0, 0, rz.cx, rz.cy, picture);
	w.End();
}

DrawingCtrl::DrawingCtrl() {
	ratio = true;
	background = White;
}

Size SeparatorCtrl::GetMinSize() const {
	return Size(size, size);
}

CH_STYLE(SeparatorCtrl, Style, StyleDefault)
{
	l1 = SColorShadow();
	l2 = Null;
}

void SeparatorCtrl::Paint(Draw& w) {
	Size sz = GetSize();
	if(sz.cx > sz.cy) {
		int q = sz.cy / 2;
		ChPaint(w, lmargin, q - 1, sz.cx - (lmargin + rmargin), 1, style->l1);
		ChPaint(w, lmargin, q, sz.cx - (lmargin + rmargin), 1, style->l2);
	}
	else {
		int q = sz.cx / 2;
		ChPaint(w, q - 1, lmargin, 1, sz.cy - (lmargin + rmargin), style->l1);
		ChPaint(w, q, lmargin, 1, sz.cy - (lmargin + rmargin), style->l2);
	}
};

SeparatorCtrl& SeparatorCtrl::Margin(int l, int r)
{
	if(l != lmargin || r != rmargin) {
		lmargin = l;
		rmargin = r;
		Refresh();
	}
	return *this;
}

SeparatorCtrl& SeparatorCtrl::SetSize(int w)
{
	if(w != size) {
		size = w;
		Refresh();
	}
	return *this;
}

SeparatorCtrl& SeparatorCtrl::SetStyle(const Style& s)
{
	if(&s != style) {
		style = &s;
		Refresh();
	}
	return *this;
}

SeparatorCtrl::SeparatorCtrl()
{
	lmargin = rmargin = 2;
	size = 7;
	NoWantFocus();
	Transparent();
	Disable();
	style = &StyleDefault();
}

void DisplayCtrl::Paint(Draw& w)
{
	pr.Paint(w, GetSize());
}

Size DisplayCtrl::GetMinSize() const
{
	return pr.GetStdSize();
}

void DisplayCtrl::SetData(const Value& v)
{
	pr.SetValue(v);
	Refresh();
}

Value DisplayCtrl::GetData() const
{
	return pr.GetValue();
}

void DisplayCtrl::SetDisplay(const Display& d)
{
	pr.SetDisplay(d);
}

}
