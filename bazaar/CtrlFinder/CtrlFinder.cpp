#include "CtrlFinder.h"

//this one filters 
void CtrlFinder::StdCtrlFilter(Ctrl*& q, Point& pt, int& f)
{
	if((f & VIEW) && q->InView())
	{
		if( 	((f & VISIBLE) && q->IsShown())
			|| ((f & INVISIBLE) && !q->IsShown())
			|| ((f & ENABLED) && q->IsEnabled())
			|| ((f & DISABLED) && !q->IsEnabled())
		) return;
	}

	if((f & FRAME) && q->InFrame()) 
		if(		((f & VISIBLE) && q->IsShown())
			|| ((f & INVISIBLE) && !q->IsShown())
			|| ((f & ENABLED) && q->IsEnabled())
			|| ((f & DISABLED) && !q->IsEnabled())
		) return;
	q = NULL;
}

Ctrl* CtrlFinder::ChildAtPoint(Ctrl& par, Point& pt, int& f, const CtrlFilterType& filt)
{
	GuiLock __;
	Ctrl *q;
	Point p = pt;
	Rect view = par.GetView();

	if(f & VIEW)
	if(view.Contains(p)) {
		Point vp = p - view.TopLeft();
		for(q = par.GetLastChild(); q; q = q->GetPrev()) {
			if((f & VIEW) && q->InView())
			{
				Rect r = q->GetRect();
				if(r.Contains(vp)) {
					pt = vp - r.TopLeft();
					Ctrl* w(q);
					filt(w, pt, f);
					if(w) return w;
				}
			}
		}
		return NULL;
	}

	if(f & FRAME)
	for(q = par.GetLastChild(); q; q = q->GetPrev()) {
		if((f & FRAME) && q->InFrame()) 
		{
			Rect r = q->GetRect();
			if(r.Contains(p)) {
				pt = p - r.TopLeft();
				Ctrl* w(q);
				filt(w, pt, f);
				if(w) return w;
			}
		}
	}
	return NULL;
}

Ctrl* CtrlFinder::GetCtrl(Ctrl& c, Point& p, int& f, const CtrlFilterType& filt)
{
	Ctrl* q = ChildAtPoint(c, p, f, filt);
	if(q && (f & DEEP)) 
	{
		Point pt(p);
		Ctrl* qc = GetCtrl(*q, pt, f, filt);
		if(qc) { p = pt; return qc;	}
	}
	return q;
}

bool CtrlFinder::IsParentR(const Ctrl* p, const Ctrl* c)
{
	while(c && p) {
		if(c == p) return true;
		c = c->GetParent();
	}
	return false;
}

void CtrlFinder::Updated()
{
	if(!pctrl) return;
	SetPos(pctrl->GetPos());
}

void CtrlFinder::UpdatedSource()
{
	Remove();
	ClearCtrl();
	if(!pctrl) return;
	ASSERT(pctrl->GetParent());
	//finder adds itself on top of any child in the search ctrl's parent
	pctrl->GetParent()->AddChild(this, pctrl);
}

void CtrlFinder::LeftDown(Point p, dword keyflags)
{
	if(!pctrl) return;
	Point pt(p);
	ctrl = GetCtrl(*pctrl, pt, flags, filter);
	if(!ctrl) ctrl = pctrl;
	if(!ctrl) return;
	WhenLeftDown(*ctrl, p, keyflags);
	Action();
}
void CtrlFinder::RightDown(Point p, dword keyflags)
{
	if(!pctrl) return;
	Point pt(p);
	ctrl = GetCtrl(*pctrl, pt, flags, filter);
	if(!ctrl) ctrl = pctrl;
	if(!ctrl) return;
	WhenRightDown(*ctrl, p, keyflags);
	Action();
	if(WhenBar)
		MenuBar::Execute(WhenBar);
}
