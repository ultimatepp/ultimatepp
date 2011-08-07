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
	if(c.InFrame()) return; //may not move frames
	rc.Remove();
	if(&c == &rc) return;
	if(&c == ~pctrl) return; //mat not move base
	Add(rc.SizePos());

	//if c is frame: rect in parents window, 
	//if is in view, then rect in view area, which itself is subarea of parent
	Rect r = c.GetRect();
	if(c.InView())
		r.Offset(c.GetParent()->GetView().TopLeft());
	r.Offset(GetOffset(*c.GetParent(), *pctrl));

	rc.SetData(r);
	Action();
}

void CtrlMover::OnRectChange()
{
	if(!pctrl || !ctrl) { rc.Remove(); return; }

	Rect r = rc.GetData();
	r.Offset(-GetOffset(*ctrl->GetParent(), *pctrl));
	if(ctrl->InView())
		r.Offset(-ctrl->GetParent()->GetView().TopLeft());
	
	ctrl->SetRect(r);
	Action();
}

void CtrlMover::OnMissed(Point p, dword keyflags)
{
	CtrlFinder::LeftDown(p, keyflags);
}

void CtrlMover::Updated()
{
	if(!pctrl || !ctrl) { rc.Remove(); return; }
	rc.SetData(ctrl->GetRect());
}

void CtrlMover::State(int reason)
{
	if(reason != ENABLE) return;
	if(!IsEnabled()) rc.Remove();
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
