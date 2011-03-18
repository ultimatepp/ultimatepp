#include "CtrlFinder.h"

Ctrl* ChildAtPoint(Ctrl& par, Point& pt, bool ignoreframe)
{
	GuiLock __;
	Ctrl *q;
	Point p = pt;
	Rect view = par.GetView();

	if(view.Contains(p)) {
		Point vp = p - view.TopLeft();
		for(q = par.GetLastChild(); q; q = q->GetPrev()) {
			if(q->InView()) {
				Rect r = q->GetRect();
				if(r.Contains(vp)) {
					pt = vp - r.TopLeft();
					return q;
				}
			}
		}
		return NULL;
	}
	if(ignoreframe) return NULL;

	for(q = par.GetLastChild(); q; q = q->GetPrev()) {
		if(q->InFrame()) {
			Rect r = q->GetRect();
			if(r.Contains(p)) {
				pt = p - r.TopLeft();
				return q;
			}
		}
	}
	return NULL;
}

Ctrl* GetCtrl(Ctrl& c, Point& p, bool ignoreframe, bool deep)
{
	Ctrl* q = ChildAtPoint(c, p, ignoreframe);
	if(q && deep) 
	{
		Point pt(p);
		Ctrl* qc = GetCtrl(*q, pt, ignoreframe, deep);
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

void CtrlFinder::OnCtrlLeft(Point p, dword keyflags)
{
	ctrl = NULL;
	if(IsEmpty()) return;
	Point pt(p);
	ctrl = ::GetCtrl(Get(), pt, ignoreframe, deep);
	if(ctrl) WhenLeftDown(*ctrl, p, keyflags);
	else WhenMissed(p, keyflags);
	Action();
}
void CtrlFinder::OnCtrlRight(Point p, dword keyflags)
{
	ctrl = NULL;
	if(IsEmpty()) return;
	Point pt(p);
	ctrl = ::GetCtrl(Get(), pt, ignoreframe, deep);
	if(ctrl) WhenRightDown(*ctrl, p, keyflags);
	else WhenMissed(p, keyflags);
	Action();
}
