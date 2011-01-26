#include "CtrlMover.h"

void CtrlMover::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &rc) return;
	if(&c == this) return;
	Visit(c);
	Action();
}

void CtrlMover::OnRectChange()
{
	if(IsEmpty()) return;
	Get().SetRect(rc.GetData());
	Action();
}

void CtrlMover::OnMissed(Point p, dword keyflags)
{
	Clear();
	Action();
	LeftDown(p, keyflags);
}

void CtrlMover::Updated()
{
	Reload();
}

void CtrlMover::State(int reason)
{
	if(reason != ENABLE) return;
	if(!IsEnabled()) Clear();
}

void CtrlMover::Visit(Ctrl& c)
{
	rc.Remove();
	Add(rc.SizePos());
	V::Visit(c);
}

void CtrlMover::Clear()
{
	V::Clear();
	rc.Remove();
}

void CtrlMover::Reload()
{
	V::Reload();
	rc.SetData(Get().GetRect());
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
