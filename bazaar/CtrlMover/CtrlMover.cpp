#include "CtrlMover.h"

void CtrlMover::CalcOffset(const Ctrl& c, const Ctrl& q, Point& p)
{
	if(&c == &q) return;

	Rect r = c.GetRect();
	p += r.TopLeft();

	Ctrl* pc = c.GetParent();
	if(!pc) { return; }

	if(c.InView())
	{
		Rect rv = pc->GetView();
		p += rv.TopLeft();
	}
	CalcOffset(*pc, q, p);
}

//calculate topleft offset of c w.r. up to par recursively
Point CtrlMover::GetOffset(const Ctrl& c, const Ctrl& q)
{
	Point p; p.Clear();
	CalcOffset(c, q, p);
	return p;
}

void CtrlMover::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &rc) { rc.Remove(); return; }
	if(&c == &Get()) { rc.Remove(); return; } //mat not move base
	if(c.InFrame()) return; //may not move frames
	rc.Remove();
	Add(rc.SizePos());

	//if c is frame: rect in parents window, 
	//if is in view, then rect in view area, which itself is subarea of parent
	Rect r = c.GetRect();
	if(c.InView())
		r.Offset(c.GetParent()->GetView().TopLeft());
	r.Offset(GetOffset(*c.GetParent(), Get()));

	rc.SetData(r);
	Action();
}

void CtrlMover::OnRectChange()
{
	if(IsEmpty()) { rc.Remove(); return; }
	if(!GetCtrl()) return;
	Ctrl& c = *GetCtrl();

	Rect r = rc.GetData();
	r.Offset(-GetOffset(*c.GetParent(), Get()));
	if(c.InView())
		r.Offset(-c.GetParent()->GetView().TopLeft());
	
	c.SetRect(r);
	Action();
}

void CtrlMover::OnMissed(Point p, dword keyflags)
{
	CtrlFinder::LeftDown(p, keyflags);
}

void CtrlMover::Updated()
{
	Ctrl* c = GetCtrl();
	if(!c) return;
	rc.SetData(c->GetRect());
}

void CtrlMover::State(int reason)
{
	if(reason != ENABLE) return;
	if(!IsEnabled()) rc.Remove();
}

void CtrlMover::Visit(Ctrl& c)
{
	rc.Remove();
	V::Visit(c);
}

void CtrlMover::Clear()
{
	V::Clear();
	rc.Remove();
}

void CtrlMover::Reload()
{
	if(IsEmpty()) return;
	V::Reload();
}

CtrlMover::CtrlMover()
{
	WhenLeftDown = THISBACK(OnCtrlLeft);
	rcst = RectCtrl::StyleDefault();
	rcst.rectcol = Null;
	rc.SetStyle(rcst);
	rc <<= THISBACK(OnRectChange);
	rc.WhenMissed = THISBACK(OnMissed);
}
