#include "ExpandFrame.h"

void ExpandFrame::ExpandButton::Paint(Draw &w)
{
	Size sz = GetSize();
	int state = GetVisualState();
	bool lr = (align == ExpandFrame::LEFT || align == ExpandFrame::RIGHT);
	int margin = style->handle_margin[state];
	int x = margin;
	// BG
	ChPaint(w, sz, style->handle_look[state]);

	if (lr) 
		Swap(sz.cx, sz.cy);
	
	// Image
	Image img = style->image[expand ? 1 : 0];
	if (!IsNull(img)) {
		Size isz = img.GetSize();
		if (lr)
			Swap(isz.cx, isz.cy);
		Point p(x, (sz.cy - isz.cy)/2);

		if (style->imagealign == ExpandFrame::RIGHT) {
			sz.cx -= isz.cx + margin;
			p.x = sz.cx;			
		}
		else
			x += isz.cx+margin;
		if (lr)
			w.DrawImage(p.y, p.x, img);
		else
			w.DrawImage(p.x, p.y, img);
	}
	// Text
	if (label.GetLength()) {
		Size tsz = GetTextSize(label, style->font[state]);
		if (lr)
			Swap(tsz.cx, tsz.cy);
		Point p(x, (sz.cy - tsz.cy)/2);

		if (style->textalign == ExpandFrame::RIGHT)
			p.x = sz.cx - margin - tsz.cx;
		
		if (lr)
			w.DrawText(sz.cy + p.y, p.x, 2700, label, style->font[state], style->textcolor[state]);
		else
			w.DrawText(p.x, p.y, label, style->font[state], style->textcolor[state]);
	}
}

int ExpandFrame::ExpandButton::GetVisualState() const
{
	int i = FlatButton::GetVisualState();
	return (i == CTRL_NORMAL && expand) ? CTRL_PRESSED : i;
}

void ExpandFrame::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void ExpandFrame::FrameRemove()
{
	this->Ctrl::Remove();
}

void ExpandFrame::FrameAddSize(Size& sz)
{
	if (!IsSet() || !IsShown()) return;
	bool lr = (handle.align == LEFT || handle.align == RIGHT);
	sz.cx += lr ? ExpandSz() : 0;
	sz.cy += lr ? 0 : ExpandSz();
}

void ExpandFrame::FrameLayout(Rect& r)
{
	if (!IsSet()) return;
	switch (handle.align) {
	case LEFT:
		LayoutFrameLeft(r, this, ExpandSz());
		break;
	case RIGHT:
		LayoutFrameRight(r, this, ExpandSz());
		break;
	case TOP:
		LayoutFrameTop(r, this, ExpandSz());
		break;
	case BOTTOM:
		LayoutFrameBottom(r, this, ExpandSz());
		break;
	}
}

void ExpandFrame::Paint(Draw& w)
{
	if (!IsSet() || !IsShown()) return;
	
	if (!IsTransparent())
		w.DrawRect(GetSize(), SColorFace());
	
	// Paint Border
	int state = handle.GetVisualState();
	if (handle.style->border[state]) {
		Rect r = GetSize();
		r.Deflate(handle.style->border_inset);
		DrawBorder(w, r, handle.style->border[state]);
	}
}

void ExpandFrame::UpdateButton()
{
	Rect r = handle.style->handle_border[0];
	switch (handle.align) {
	case TOP:
		handle.HSizePosZ(r.left, r.right).TopPosZ(r.top, handle.style->handle_size);
		break;
	case LEFT:
		handle.VSizePosZ(r.right, r.left).LeftPosZ(r.top, handle.style->handle_size);
		break;
	case BOTTOM:
		handle.HSizePosZ(r.right, r.left).BottomPosZ(r.top, handle.style->handle_size);
		break;
	case RIGHT:
		handle.VSizePosZ(r.left, r.right).RightPosZ(r.top, handle.style->handle_size);
		break;
	}
}

int ExpandFrame::ExpandSz() const
{
	if (IsExpanded()) {
		Rect r = BorderSz();
		return max(r.top, HandleSz()) + r.bottom + child_size;
	}
	else
		return HandleSz();
}

Rect ExpandFrame::BorderSz() const
{
	int sz = (int)(uintptr_t)(handle.style->border[0][0]);
	const Rect &in = handle.style->border_inset;
	const Rect &out = handle.style->border_outset;
	return Rect(in.left + out.left + sz, in.top + out.top + sz,
		in.right + out.right + sz, in.bottom + out.bottom + sz);
}

void ExpandFrame::SetChildPos()
{
	if (!IsSet()) return;
	Ctrl *c = GetLastChild();
	
	Rect r = BorderSz();
	r.top = max(r.top, HandleSz());
		
	switch (handle.align) {
	case TOP:
		c->VSizePos(r.top, r.bottom).HSizePos(r.left, r.right);
		break;
	case LEFT:
		c->HSizePos(r.top, r.bottom).VSizePos(r.right, r.left);
		break;
	case BOTTOM:
		c->VSizePos(r.bottom, r.top).HSizePos(r.left, r.right);
		break;
	case RIGHT:
		c->HSizePos(r.top, r.bottom).VSizePos(r.left, r.right);
		break;
	}
}

ExpandFrame& ExpandFrame::Set(Ctrl& c, int size, int _type)
{
	ASSERT(handle.align >= LEFT && handle.align <= BOTTOM);
	ASSERT(size > 0);
	handle.align = _type;
	bool lr = (handle.align == LEFT || handle.align == RIGHT);
	if (IsSet())
		GetLastChild()->Remove();
	child_size = size;
	UpdateButton();
	Add(c);
	SetChildPos();
	c.Show(IsExpanded());
	RefreshParentLayout();
	return *this; 
}

ExpandFrame& ExpandFrame::Expand(bool _expand)
{
	handle.expand = _expand;
	UpdateButton();
	if (!IsSet()) return *this;
	bool hasfocus = HasFocusDeep();
	GetLastChild()->Show(_expand);
/*	if (!expand && childfocus)
		Ctrl::IterateFocusForward(GetFocusCtrl(), GetParent());		
	else*/
	if (_expand && childfocus)
		GetLastChild()->SetFocus();
	childfocus = _expand;
	RefreshParentLayout();
	return *this; 
}

Size ExpandFrame::GetMinSize() const
{
	return IsSet() ? Size(handle.style->handle_size, handle.style->handle_size) : Ctrl::GetMinSize();	
}

Size ExpandFrame::GetStdSize() const
{
	int t = ExpandSz();
	return IsSet() ? Size(t, t) : Ctrl::GetStdSize();	
}

ExpandFrame::ExpandFrame()
{
	handle.style = &StyleDefault();
	child_size = 0;
	childfocus = false;
	ignorefocus = false;
	handle.expand = false;
	handle.align = 0;
	handle <<= THISBACK(Toggle);
	hasarrow = true;
	Add(handle);
}

const ColorF *LabelBoxBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&LabelBoxColor, &LabelBoxColor, &LabelBoxColor, &LabelBoxColor,
		&LabelBoxColor, &LabelBoxColor, &LabelBoxColor, &LabelBoxColor,
	};
	return data;
}

CH_STYLE(ExpandFrame, Style, StyleDefault)
{
	handle_size = 20;
	for (int i =0; i < 4; i++) {
		handle_look[i] = Button::StyleNormal().look[i];
		handle_margin[i] = 5;
		handle_border[i] = Rect(10, 0, 10, 0);
		font[i] = StdFont();
		textcolor[i] = Button::StyleNormal().textcolor[i];
	}
	textalign = ExpandFrame::LEFT;
	image[0] = CtrlsImg::kDA(); // DOWN
	image[1] = CtrlsImg::kUA(); // UP
	imagealign = ExpandFrame::RIGHT;
	for (int i =0; i < 4; i++)
		border[i] = LabelBoxBorder();
	border_inset = Rect(1, handle_size/3, 1, handle_size/3);
	border_outset = Rect(1, 2, 2, 1);
}

CH_STYLE(ExpandFrame, Style, StyleFlat)
{
	handle_size = 20;
	for (int i =0; i < 4; i++) {
		handle_look[i] = ButtonOption::StyleFlat().look[i];
		handle_margin[i] = 3;
		handle_border[i] = Rect(10, 0, 10, 0);
		font[i] = StdFont();
		textcolor[i] = ButtonOption::StyleFlat().textcolor[i];
	}
	textalign = ExpandFrame::LEFT;
	image[0] = CtrlsImg::DA(); // DOWN
	image[1] = CtrlsImg::UA(); // UP
	imagealign = ExpandFrame::RIGHT;
	for (int i =0; i < 4; i++)
		border[i] = NULL;
	border_inset = Rect(1, handle_size/2, 1, 1);
	border_outset = Rect(2, 2, 2, 2);
}
