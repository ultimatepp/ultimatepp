#include "ExpandFrame.h"

void ExpanderCtrl::Layout()
{
	Repos();
	scroll.SetPage(Hv(GetSize()));
	UpdateScrollBar();
}

void ExpanderCtrl::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	int i = -1;
	if (event == MOUSEWHEEL)
		scroll.Wheel(zdelta);
}

ExpandFrame & ExpanderCtrl::AddExpander(Ctrl &c, bool expand, int size)
{
	int sz = (size < 0) ? Hv(c.GetMinSize()) 
					    : (IsHorz() ? HorzLayoutZoom(size) : VertLayoutZoom(size));
	ExpandFrame &e = exp.Add();
	e.Set(c, sz, IsHorz() ? ExpandFrame::LEFT : ExpandFrame::TOP);
	if (IsHorz()) {
		e.VSizePos();
		e.LeftPos(0, 0);
	}
	else {
		e.HSizePos();
		e.TopPos(0, 0);
	}
	e <<= THISBACK(OnExpand);
	Add(e);
	e.Expand(expand);
	Repos();
	if (GetCount() == 1) scroll.SetLine(Hv(e.GetMinSize()));
	return e;
}

ExpanderCtrl & ExpanderCtrl::Horz(bool v)
{
	if (v == horz) return *this;

	horz = v;
	scroll.Horz(v);
	if (horz)
		for (int i = 0; i < exp.GetCount(); i++)
			exp[i].Left(*exp[i].GetLastChild());
	else
		for (int i = 0; i < exp.GetCount(); i++)
			exp[i].Top(*exp[i].GetLastChild());
	Repos();
	return *this;
}

void ExpanderCtrl::UpdateScrollBar()
{
	Size sz(0, 0);
	for (int i = 0; i < GetCount(); i++)
		sz += exp[i].GetStdSize();
	scroll.SetTotal(Hv(sz));
}

void ExpanderCtrl::OnScroll()
{
	Repos();
	scroller.Scroll(*this, scroll.Get());
}

void ExpanderCtrl::OnExpand()
{
	int p = scroll.Get();
	Repos();
	if (p != scroll.Get())
		OnScroll();
}

void ExpanderCtrl::Repos()
{
	if (!GetCount()) return;
	Rect r = GetSize();
	Size sz = r.GetSize();
	int &var = IsHorz() ? sz.cx : sz.cy;
	int total = 0;
	Point off = IsHorz() ? Point(-scroll.Get(), 0) : Point(0, -scroll.Get());
	for (int i = 0; i < GetCount(); i++) {
		ExpandFrame &e = exp[i];
		int intsz = Hv(e.GetStdSize()); 
		total += intsz;
		var = intsz;
		e.SetRect(Rect(off, sz));
		if (IsHorz())
			off.x += intsz;
		else
			off.y += intsz;
	}
	scroll.SetTotal(total);
}

ExpanderCtrl::ExpanderCtrl()
{
	Vert();
	AddFrame(scroll);
	scroll <<= THISBACK(OnScroll);
	scroll.AutoHide(true);
}

