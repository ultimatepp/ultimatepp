#include "ExpandFrame.h"

void ExpandFrame::ImgButton::Paint(Draw &w)
{
	Size sz = GetSize();
	Size isz = img.GetSize();

	const ExpandFrame* p = (ExpandFrame*)GetParent();
	const Style* st = p->St();
	if (HasMouse() || st->buttonstyle == 1)
		w.DrawRect(sz, hl);

	if (st->buttonstyle == 0 || st->buttonstyle == 1)
		UPP::DrawFrame(w, sz, fg);

	if (st->buttonstyle == 2)
	{
		Rect r = sz;
		int i = !IsShowEnabled() ? CTRL_DISABLED :
	         IsPush() ? CTRL_PRESSED :
	         HasMouse() || HasFocus() ? CTRL_HOT :
	         CTRL_NORMAL;
		if(p->IsExpanded()) i = CTRL_PRESSED;
		ChPaint(w, sz, Button::StyleNormal().look[i]);
	}

	int dx = IsPush() * (left ? -1 : 1);
	int dy = IsPush();
	if (st->buttonstyle == 0)
	{
		if (p->hasarrow)
			w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);
	}
	else if (st->buttonstyle == 1 || st->buttonstyle == 2)
	{
		if (p->type == TOP || p->type == BOTTOM)
		{
			if (p->hasarrow)
				w.DrawImage(sz.cx - isz.cx + dx - 5, (sz.cy - isz.cy) / 2 + dy, img);
			w.DrawText(4, 1, p->GetTitle());
		}
		else
		{
			if (p->hasarrow)
				w.DrawImage((sz.cx - isz.cx) / 2, sz.cy - isz.cy + dy - 5, img);
			w.DrawText(p->TitleSz() - 1, 4, 2700, p->GetTitle());
		}
	}
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
	bool lr = (type == LEFT || type == RIGHT);
	sz.cx += lr ? TitleSz()+ExpandSz() : 0;
	sz.cy += lr ? 0 : TitleSz()+ExpandSz();
}

void ExpandFrame::FrameLayout(Rect& r)
{
	if (!IsSet()) return;
	switch (type) {
	case LEFT:
		LayoutFrameLeft(r, this, TitleSz() + ExpandSz());
		break;
	case RIGHT:
		LayoutFrameRight(r, this, TitleSz() + ExpandSz());
		break;
	case TOP:
		LayoutFrameTop(r, this, TitleSz() + ExpandSz());
		break;
	case BOTTOM:
		LayoutFrameBottom(r, this, TitleSz() + ExpandSz());
		break;
	}
}

void ExpandFrame::Paint(Draw& w)
{
	if (!IsSet() || !IsShown()) return;
	const Style *st = St();
	Rect r = GetSize();
	int focus = (childfocus || ignorefocus) ? 1 : 0;
	bool lr = (type == LEFT || type == RIGHT);

	if (expand) {
		Rect rr = r;
		switch (type) {
		case LEFT:
			r.right = TitleSz();
			rr.left = r.right;
			break;
		case RIGHT:
			r.left = r.right - TitleSz();
			rr.right = r.left;
			break;
		case TOP:
			r.bottom = TitleSz();
			rr.top = r.bottom;
			break;
		case BOTTOM:
			r.top = r.bottom - TitleSz();
			rr.bottom = r.top;
			break;
		}
		w.DrawRect(rr, SColorFace());
	}

	ChPaint(w, r, st->background[focus]);

	PaintBorder(w);

	if (st->buttonstyle == 0)
	{
		if (!lr) w.Clip(0, 0, r.right - TitleSz() + 2, r.bottom);

		Point pt;
		if (lr) {
			pt = Point(r.right - 1, r.top + TitleSz());
			if (img) {
				w.DrawImage(r.left + (r.Width() - img.GetWidth())/2, pt.y, img);
				pt.y += img.GetHeight() + 2;
			}
		}
		else {
			pt = Point(r.left + 2, r.top + 1);
			if (img) {
				w.DrawImage(pt.x, pt.y + (r.Height() - img.GetHeight())/2, img);
				pt.x += img.GetWidth() + 2;
			}
		}
		w.DrawText(pt.x, pt.y, lr ? 2700 : 0, title, st->font, st->text[focus]);
		if (!lr) w.End();
	}
}

void ExpandFrame::PaintBorder(Draw& w)
{
	const Style *st = St();
	int focus = (childfocus || ignorefocus) ? 1 : 0;

	if (st->buttonstyle == 0)
	{
		if (expand)
			DrawFrame(w, GetSize(), st->border[focus]);
		else
			DrawFrame(w, GetSize(), st->border[focus], Null, st->border[focus], st->border[focus]);
	}
	else
	{
		const int d = 5;

		Rect r = GetSize();
		r.left++;
		r.top++;
		if (type == TOP)
		{
			r.top += d;
			if (!expand)
				r.bottom -= d - 2;
		}
		else if (type == BOTTOM)
		{
			r.bottom -= d - 1;
			if (!expand)
				r.top += d - 1;
		}
		else if (type == LEFT)
		{
			r.left += d;
			if (!expand)
				r.right -= d - 2;
		}
		else if (type == RIGHT)
		{
			r.right -= d - 2;
			if (!expand)
				r.left += d;
		}
		DrawFrame(w, r, SColorLight);
		r.Offset(-1, -1);
		DrawFrame(w, r, SColorShadow);
	}
}

void ExpandFrame::UpdateButton()
{
	Image image;
	const Style *st = St();

	switch (type) {
	case LEFT:
		image = expand ? st->btnimage[0] : st->btnimage[2];
		break;
	case RIGHT:
		image = expand ? st->btnimage[2] : st->btnimage[0];
		break;
	case TOP:
		image = expand ? st->btnimage[1] : st->btnimage[3];
		break;
	case BOTTOM:
		image = expand ? st->btnimage[3] : st->btnimage[1];
		break;
	}
	btn.SetImage(image) <<= THISBACK1(Expand0, !expand);
	if (st->buttoncenter == 0)
	{
		int sz = TitleSz() - 2;
		if (type == LEFT || type == RIGHT) {
			btn.TopPos(1, sz);
			(type == LEFT) ? btn.LeftPos(1, sz) : btn.RightPos(1, sz);
		}
		else {
			btn.RightPos(1, sz);
			(type == TOP) ? btn.TopPos(1, sz) : btn.BottomPos(1, sz);
		}
	}
	else
	{
		int sz = TitleSz();
		if (type == LEFT) {
			btn.LeftPos(0, sz);
			btn.VSizePos(5, 5);
		}
		else if (type == RIGHT) {
			btn.RightPos(0, sz);
			btn.VSizePos(5, 5);
		}
		else if (type == TOP) {
			btn.TopPos(0, sz);
			btn.HSizePos(5, 5);
		}
		else if (type == BOTTOM) {
			btn.BottomPos(0, sz);
			btn.HSizePos(5, 5);
		}
	}
}

void ExpandFrame::SetChildPos()
{
	if (!IsSet()) return;
	Ctrl *c = GetLastChild();

	const Style *st = St();
	int d = st->borderwidth;

	switch (type) {
	case LEFT:
		c->VSizePos(d, d).HSizePos(TitleSz(), d);
		break;
	case RIGHT:
		c->VSizePos(d, d).HSizePos(d, TitleSz());
		break;
	case TOP:
		c->HSizePos(d, d).VSizePos(TitleSz(), d);
		break;
	case BOTTOM:
		c->HSizePos(d, d).VSizePos(d, TitleSz());
		break;
	}
}

ExpandFrame& ExpandFrame::Set(Ctrl& c, int size, int _type)
{
	ASSERT(type >= LEFT && type <= BOTTOM);
	ASSERT(size > 0);
	type = _type;
	bool lr = (type == LEFT || type == RIGHT);
	if (IsSet())
		GetLastChild()->Remove();
	child_size = size;
	UpdateButton();
	Add(c);
	SetChildPos();
	c.Show(expand);
	RefreshParentLayout();
	return *this;
}

ExpandFrame& ExpandFrame::Expand(bool _expand)
{
	expand = _expand;
	UpdateButton();
	if (!IsSet()) return *this;
	GetLastChild()->Show(expand);
	if (!expand && childfocus)
		Ctrl::IterateFocusForward(GetFocusCtrl(), GetParent());
	else
		GetLastChild()->SetFocus();
	childfocus = expand;
	RefreshParentLayout();
	return *this;
}

ExpandFrame::ExpandFrame()
{
	style = NULL;
	child_size = 0;
	childfocus = false;
	ignorefocus = false;
	expand = false;
	type = 0;
	hasarrow = true;
	btn.Transparent();
	Add(btn);
}

CH_STYLE(ExpandFrame, Style, StyleDefault)
{
	btnimage[3] = CtrlsImg::DA(); // DOWN
	btnimage[0] = RotateClockwise(btnimage[3]); // LEFT
	btnimage[1] = RotateClockwise(btnimage[0]); // UP
	btnimage[2] = RotateClockwise(btnimage[1]); // RIGHT
	background[0] = LtGray(); // No focus
	background[1] = Blend(LtBlue(), White(), 60); // Focus
	text[0]	= Black(); // No focus
	text[1]	= White(); // Focus
	border[0] = Gray(); // No focus
	border[1] = LtBlue(); // Focus
	font = StdFont(12);
	buttonstyle = 0;
	buttoncenter = 0;
	borderwidth = 1;
}

CH_STYLE(ExpandFrame, Style, StyleWide)
{
	btnimage[3] = CtrlsImg::DA(); // DOWN
	btnimage[0] = RotateClockwise(btnimage[3]); // LEFT
	btnimage[1] = RotateClockwise(btnimage[0]); // UP
	btnimage[2] = RotateClockwise(btnimage[1]); // RIGHT
	background[0] = SColorFace(); // No focus
	background[1] = SColorFace(); // Focus
	text[0]	= Black(); // No focus
	text[1]	= Black(); // Focus
	border[0] = Gray(); // No focus
	border[1] = LtBlue(); // Focus
	font = StdFont(12);
	buttonstyle = 1;
	buttoncenter = 1;
	borderwidth = 2;
}

CH_STYLE(ExpandFrame, Style, StyleButton)
{
	btnimage[3] = CtrlsImg::DA(); // DOWN
	btnimage[0] = RotateClockwise(btnimage[3]); // LEFT
	btnimage[1] = RotateClockwise(btnimage[0]); // UP
	btnimage[2] = RotateClockwise(btnimage[1]); // RIGHT
	background[0] = SColorFace(); // No focus
	background[1] = SColorFace(); // Focus
	text[0]	= Black(); // No focus
	text[1]	= Black(); // Focus
	border[0] = Gray(); // No focus
	border[1] = LtBlue(); // Focus
	font = StdFont(12);
	buttonstyle = 2;
	buttoncenter = 1;
	borderwidth = 2;
}
