#include "CtrlMover.h"

void CtrlMover::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(c.InFrame()) return; //may not move frames
	rc.Remove();
	if(&c == pctrl) return; //mat not move base
	if(&c == &rc) return;
	Update();
}

void CtrlMover::OnCtrlLeftMulti(const Vector<Ctrl*>* pctrls, Rect r, dword keyflags)
{
	if(pctrls->Top()->InFrame()) return; //may not move frames
	rc.Remove();
	if(pctrls->Top() == pctrl) return; //mat not move base
	if(pctrls->Top() == &rc) return;

	//for(int i = 0; i < pctrls->GetCount(); i++) {}

	Update();
}

void CtrlMover::OnRectChange()
{
	if(!pctrl || ctrls.IsEmpty()) { rc.Remove(); return; }

	Rect r = rc.GetData();

	if(ctrls.GetCount() > 1)
	{
		//not supported
	}
	else
	{
		r.Offset(-GetOffset(*ctrls.Top(), *pctrl));
		ctrls.Top()->SetRect(r);
	}

	Action();
}

void CtrlMover::OnMissed(Point p, dword keyflags)
{
	CtrlFinder::LeftDown(p, keyflags);
}

void CtrlMover::Updated()
{
	CtrlFinder::Updated();

	if(!pctrl || ctrls.IsEmpty()) { rc.Remove(); return; }

	Add(rc.SizePos());

	Rect r = SurroundingRect(ctrls);
	r.Offset( GetOffset(*ctrls.Top(), *pctrl)); //any of the parttakers is good to get the offset of their branch
	rc.SetData(r);
}

void CtrlMover::State(int reason)
{
	if(reason != ENABLE) return;
	if(!IsEnabled()) rc.Remove();
}

CtrlMover::CtrlMover()
{
	WhenLeftSelect = THISBACK(OnCtrlLeft);
	rcst = RectCtrl::StyleDefault();
	rcst.rectcol = Null;
	rc.SetStyle(rcst);
	rc <<= THISBACK(OnRectChange);
	rc.WhenMissed = THISBACK(OnMissed);
}
