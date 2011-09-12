#include "CtrlMover.h"

//returns the offset of a ctrl's drawing area w.r.t. its parent's drawing area
void CtrlMover::GetOffset(const Ctrl& c, Point& p)
{
	p += c.GetRect().TopLeft();
	if(c.InView())
		p += c.GetParent()->GetView().TopLeft();
}

//returns the offset of a ctrl's drawing area w.r.t an upper parent's drawing area.
void CtrlMover::GetOffset(const Ctrl& c, const Ctrl& q, Point& p)
{
	if(&c == &q) return;
	GetOffset(c, p);
	if(Ctrl* pc = c.GetParent())
		GetOffset(*pc, q, p);
}

//calculate topleft offset of c w.r. up to par recursively
Point CtrlMover::GetOffset(const Ctrl& c, const Ctrl& q)
{
	Point p; p.Clear();
	GetOffset(c, q, p);
	return p;
}

void CtrlMover::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(c.InFrame()) return; //may not move frames
	rc.Remove();
	if(&c == &rc) return;
	if(&c == pctrl) return; //mat not move base
	UpdateAction();
}

void CtrlMover::OnRectChange()
{
	if(!pctrl || !ctrl) { rc.Remove(); return; }

	Rect r = rc.GetData();
	r.Offset(-GetOffset(*ctrl, *pctrl) + ctrl->GetRect().TopLeft());
	
	ctrl->SetRect(r);
	Action();
}

void CtrlMover::OnMissed(Point p, dword keyflags)
{
	CtrlFinder::LeftDown(p, keyflags);
}

void CtrlMover::Updated()
{
	CtrlFinder::Updated();

	if(!pctrl || !ctrl) { rc.Remove(); return; }

	Add(rc.SizePos());

	//if c is frame: rect in parents window, 
	//if is in view, then rect in view area, which itself is subarea of parent
	//using only GetOffset we wouldnt be able to get the rect, only the offset point
	//but we do not need the offset of the drawing area, but the offesstet drawing area itself
	//thats why ctrl->GetParent, it replics part of GetOffset
	//revert the GetRect.TopLeft offset addition, because we specify the rect directly in parent coords
	//be it in parents view or frame, we dont specify it in childrens rect coords
	Rect r = ctrl->GetRect();
	r.Offset(GetOffset(*ctrl, *pctrl) - r.TopLeft());

	rc.SetData(r);
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
