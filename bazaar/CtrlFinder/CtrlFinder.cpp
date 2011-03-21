#include "CtrlFinder.h"

Ctrl* CtrlFinder::ChildAtPoint(Ctrl& par, Point& pt, int f)
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
			if((f & VISIBLE) && q->IsVisible())
			if((f & ENABLED) && q->IsEnabled())
			{
				Rect r = q->GetRect();
				if(r.Contains(vp)) {
					pt = vp - r.TopLeft();
					return q;
				}
			}
		}
		return NULL;
	}

	if(f & FRAME)
	for(q = par.GetLastChild(); q; q = q->GetPrev()) {
		if((f & FRAME) && q->InFrame()) 
		if((f & VISIBLE) && q->IsVisible())
		if((f & ENABLED) && q->IsEnabled())
		{
			Rect r = q->GetRect();
			if(r.Contains(p)) {
				pt = p - r.TopLeft();
				return q;
			}
		}
	}
	return NULL;
}

Ctrl* CtrlFinder::GetCtrl(Ctrl& c, Point& p, int f)
{
	Ctrl* q = ChildAtPoint(c, p, f);
	if(q && (f & DEEP)) 
	{
		Point pt(p);
		Ctrl* qc = GetCtrl(*q, pt, f);
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
	ctrl = GetCtrl(Get(), pt, flags);
	if(!ctrl) ctrl = &Get();
	WhenLeftDown(*ctrl, p, keyflags);
	Action();
}
void CtrlFinder::RightDown(Point p, dword keyflags)
{
	ctrl = NULL;
	if(IsEmpty()) return;
	Point pt(p);
	ctrl = GetCtrl(Get(), pt, flags);
	if(!ctrl) ctrl = &Get();
	WhenRightDown(*ctrl, p, keyflags);
	Action();
}
