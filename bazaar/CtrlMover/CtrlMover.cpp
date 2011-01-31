#include "CtrlMover.h"

void CtrlMover::CalcOffset(Ctrl& c, Ctrl& par, Point& p)
{
	Rect r = c.GetView();
	p += r.TopLeft();
	if(&c == &par) return;

	Ctrl* cp = c.GetParent();
	if(!cp) { return; }

	Rect rr = c.GetRect();
	p += rr.TopLeft();
	CalcOffset(*cp, par, p);
}

//calculate topleft offset of c w.r. up to par recursively
Point CtrlMover::GetOffset(Ctrl& c, Ctrl& par)
{
	Point p;
	p.Clear();
	CalcOffset(c, par, p);
	return p;
}

void CtrlMover::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &rc) { rc.Remove(); return;}
	rc.Remove();
	Add(rc.SizePos());

	Rect r = c.GetRect();
	r.Offset(GetOffset(*(c.GetParent()), Get()));

	rc.SetData(r);
	Action();
}

void CtrlMover::OnRectChange()
{
	if(IsEmpty()) { rc.Remove(); return; }
	Ctrl* c = GetCtrl();
	if(!c) return;

	Rect r = rc.GetData(); //FIXME recurse the parents of GetCtrl, whole tree until meeting Get()
	r.Offset(-GetOffset(*(c->GetParent()), Get()));
	
	c->SetRect(r);
	Action();
}

void CtrlMover::OnMissed(Point p, dword keyflags)
{
	rc.Remove();
	c = NULL; //from CtrlFinder
	Action();
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
	WhenMissed = THISBACK(OnMissed);
	rcst = RectCtrl::StyleDefault();	
	rcst.rectcol = Null;
	rc.SetStyle(rcst);
	rc <<= THISBACK(OnRectChange);
	rc.WhenMissed = THISBACK(OnMissed);
}
