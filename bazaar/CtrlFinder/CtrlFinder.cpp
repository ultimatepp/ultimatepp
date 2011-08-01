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

Ctrl* CtrlFinder::ChildAtPoint(Ctrl& par, Point& pt, int& f, const CtrlFilterType& fil)
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
					fil(w, pt, f);
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
				fil(w, pt, f);
				if(w) return w;
			}
		}
	}
	return NULL;
}

Ctrl* CtrlFinder::GetCtrl(Ctrl& c, Point& p, int& f, const CtrlFilterType& fil)
{
	Ctrl* q = ChildAtPoint(c, p, f, fil);
	if(q && (f & DEEP)) 
	{
		Point pt(p);
		Ctrl* qc = GetCtrl(*q, pt, f, fil);
		if(qc)
		{
			p = pt;
			return qc;	
		}
	}
	return q;
}

void CtrlFinder::Reload()
{
	if(IsEmpty()) return;
	V::Reload();
	
	Remove();
	Get().GetParent()->AddChild(&SetPos(Get().GetPos()), &Get());
}

void CtrlFinder::Visit(Ctrl& c)
{
	ASSERT(c.GetParent());
	V::Visit(c);
	Enable();	
}

void CtrlFinder::Clear()
{
	ctrl = NULL;	
	Remove();
	V::Clear();
}

void CtrlFinder::LeftDown(Point p, dword keyflags)
{
	ctrl = NULL;
	if(IsEmpty()) return;
	Point pt(p);
	ctrl = GetCtrl(Get(), pt, flags, filter);
	if(!ctrl) ctrl = &Get();
	WhenLeftDown(*ctrl, p, keyflags);
	Action();
}
void CtrlFinder::RightDown(Point p, dword keyflags)
{
	ctrl = NULL;
	if(IsEmpty()) return;
	Point pt(p);
	ctrl = GetCtrl(Get(), pt, flags, filter);
	if(!ctrl) ctrl = &Get();
	WhenRightDown(*ctrl, p, keyflags);
	Action();
}
