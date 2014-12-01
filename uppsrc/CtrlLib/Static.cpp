#include "CtrlLib.h"

NAMESPACE_UPP

void StaticText::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	PaintLabel(this, w, 0, 0, sz.cx, sz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
}

Size StaticText::GetMinSize() const
{
	return GetLabelSize();
}

void StaticText::LabelUpdate() {
	Refresh();
}

StaticText::StaticText()
{
	NoWantFocus();
	IgnoreMouse();
	Transparent();
	SetAlign(ALIGN_LEFT);
}

Label& Label::SetText(const char *text)
{
	LabelBase::SetText(text);
	lbl.accesskey = 0;
	noac = false;
	return *this;
}

Label& Label::SetLabel(const char *_text)
{
	String text;
	int accesskey = ExtractAccessKey(_text, text);
	LabelBase::SetText(text);
	lbl.accesskey = accesskey;
	return *this;
}

bool Label::HotKey(dword key) {
	if(CompareAccessKey(lbl.accesskey, key)) {
		IterateFocusForward(this, GetParent());
		return true;
	}
	return false;
}

dword Label::GetAccessKeys() const
{
	return AccessKeyBit(lbl.accesskey);
}

void  Label::AssignAccessKeys(dword used)
{
	if(noac)
		return;
	Ctrl *next = GetNext();
	if(!lbl.accesskey && next && next->IsInitFocus()) {
		next->AssignAccessKeys(used);
		if(!next->GetAccessKeysDeep()) {
			lbl.accesskey = ChooseAccessKey(GetText(), used);
			if(lbl.accesskey) Refresh();
			used |= AccessKeyBit(lbl.accesskey);
		}
	}
	Ctrl::AssignAccessKeys(used);
}

Label::Label() {
	noac = false;
}

Label::~Label() {}

CH_COLOR(LabelBoxTextColor, IsDark(SColorFace()) ? Blend(LtBlue(), White) : LtBlue());
CH_COLOR(LabelBoxDisabledTextColor, SColorDisabled());
CH_COLOR(LabelBoxColor, SColorShadow());

LabelBox::LabelBox()
{
	color = Null;
	LabelBase::SetInk(LabelBoxTextColor, LabelBoxDisabledTextColor);
	SetVAlign(ALIGN_TOP);
	#if defined(flagWINGL) || defined(flagLINUXGL)
	ClipToBounds(false);
	#endif
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

void LabelBox::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(sz, SColorFace);
	Size lsz = GetLabelSize();
	int d = lsz.cy >> 1;
	bool hline = sz.cy < 2 * Draw::GetStdFontCy();
	bool vline = sz.cx < 2 * Draw::GetStdFontCy();
	int ty = hline ? (sz.cy - lsz.cy) / 2 : 0;
	Size ts = PaintLabel(w, d + 2, ty, sz.cx, lsz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
	w.Begin();
	w.ExcludeClip(d, ty, ts.cx + 4, ts.cy);
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


Picture& Picture::Background(Color color)
{
	background = color;
	Transparent(IsNull(color));
	Refresh();
	return *this;
}

void Picture::Paint(Draw& w) {
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

Picture::Picture() {
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
	NoWantFocus();
	Transparent();
	Disable();
	lmargin = rmargin = 2;
	size = 7;
	SetStyle(StyleDefault());
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

END_UPP_NAMESPACE
