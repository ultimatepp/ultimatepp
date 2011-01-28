#include "CtrlMover.h"

void CtrlMover::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &rc) { rc.Remove(); return;}
	rc.Remove();
	Add(rc.SizePos());
	rc.SetData(c.GetRect());
	Action();
}

void CtrlMover::OnRectChange()
{
	if(IsEmpty()) { rc.Remove(); return; }
	Ctrl* c = GetCtrl();
	if(!c) return;
	c->SetRect(rc.GetData());
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
