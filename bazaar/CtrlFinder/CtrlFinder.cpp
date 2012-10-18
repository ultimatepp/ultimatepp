#include "CtrlFinder.h"

//this one filters
//check if the found ctrl is to be kept and returned as found
//according to the selection flags f
//set q = NULL will continue the search.
//keeping q will return it as found, but probably continue a DEEP search in it if flag set
void CtrlFinder::StdCtrlFilter(Ctrl*& q, int& f)
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
	Point p(pt);
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
					filt(w, f);
					if(w) return w;
				}
			}
		}
	}

	if(f & FRAME)
	for(q = par.GetLastChild(); q; q = q->GetPrev()) {
		if((f & FRAME) && q->InFrame()) 
		{
			Rect r = q->GetRect();
			if(r.Contains(p)) {
				pt = p - r.TopLeft();
				Ctrl* w(q);
				filt(w, f);
				if(w) return w;
			}
		}
	}
	return NULL;
}

//find a ctrl at point (in pctrl coords). The search can go deep, if specified.
//a filter can prohibit to look deep in a returned found ctrl, resetting a DEEP flag
//or prohibit a deep look in a fellow to be found child resetting DEEP flag and returning NULL
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

Ctrl* CtrlFinder::ChildInRect(Ctrl& par, Rect& rect, int& f, const CtrlFilterType& filt)
{
	GuiLock __;
	Ctrl *q;
	Rect rc(rect);
	Rect view = par.GetView();

	if(f & VIEW)
	if(view.Contains(rc)) {
		Rect vrc = rc - view.TopLeft();
		for(q = par.GetLastChild(); q; q = q->GetPrev()) {
			if((f & VIEW) && q->InView())
			{
				Rect r = q->GetRect();
				if(r.Contains(vrc)) {
					rect = vrc - r.TopLeft();
					Ctrl* w(q);
					filt(w, f);
					if(w) return w;
				}
			}
		}
	}

	if(f & FRAME)
	for(q = par.GetLastChild(); q; q = q->GetPrev()) {
		if((f & FRAME) && q->InFrame()) 
		{
			Rect r = q->GetRect();
			if(r.Contains(rc)) {
				rect = rc - r.TopLeft();
				Ctrl* w(q);
				filt(w, f);
				if(w) return w;
			}
		}
	}
	return NULL;
}

//see rect as a big Point, the found Ctrl needs to Contain it.
Ctrl* CtrlFinder::GetCtrl(Ctrl& c, Rect& rect, int& f, const CtrlFilterType& filt)
{
	Ctrl* q = ChildInRect(c, rect, f, filt);
	if(q && (f & DEEP)) 
	{
		Rect rc(rect);
		Ctrl* qc = GetCtrl(*q, rc, f, filt);
		if(qc) { rect = rc; return qc;	}
	}
	return q;
}

Vector<Ctrl*> CtrlFinder::ChildrenInRect(Ctrl& par, Rect& rect, int& f, const CtrlFilterType& filt)
{
	GuiLock __;
	Vector<Ctrl*> vc;
	Ctrl *q;
	Rect rc(rect);
	Rect view = par.GetView();

	if(f & VIEW)
	if(view.Contains(rc)) {
		Rect vrc = rc - view.TopLeft();
		for(q = par.GetLastChild(); q; q = q->GetPrev()) {
			if((f & VIEW) && q->InView())
			{
				Rect r = q->GetRect();
				if(vrc.Contains(r)) {
					Ctrl* w(q);
					filt(w, f);
					if(w) vc.Add(w);
				}
			}
		}
	}

	if(f & FRAME)
	for(q = par.GetLastChild(); q; q = q->GetPrev()) {
		if((f & FRAME) && q->InFrame()) 
		{
			Rect r = q->GetRect();
			if(rc.Contains(r)) {
				Ctrl* w(q);
				filt(w, f);
				if(w) vc.Add(w);
			}
		}
	}
	return vc;
}

Vector<Ctrl*> CtrlFinder::GetCtrls(Ctrl& c, Rect& rect, int& f, const CtrlFilterType& filt)
{
	Vector<Ctrl*> vq = ChildrenInRect(c, rect, f, filt);
	if(vq.IsEmpty() && (f & DEEP)) //difference between child and children, we stop if we found some in rect
	{
		//find a child, that has rect fully within, so we can try in there
		//if no recursable child is found, we can return the empty vector
		Rect rc(rect);
		
		Ctrl* q = ChildInRect(c, rc, f, filt);
		if(q)
		{
			vq = GetCtrls(*q, rc, f, filt);
			if(!vq.IsEmpty()) { rect = rc; }
		}
	}
	return vq;
}

bool CtrlFinder::IsParentR(const Ctrl* p, const Ctrl* c)
{
	while(c && p) {
		if(c == p) return true;
		c = c->GetParent();
	}
	return false;
}

//Offset calculation:
// A Rect is always related to it housing/encapsulating/parent context.

//adds the view offset of a ctrl's parent context, if the ctrl happens to sit in its parent's view
//effectively, it's the frame to view coord. offset of the parent context of c
void CtrlFinder::AddParentViewOffset(const Ctrl& c, Point& p)
{
	if(c.InView())
		p += c.GetParent()->GetView().TopLeft();
}

//adds the offset of a source context to a (far) parent context q, all frame coordinates
void CtrlFinder::AddContextOffset(const Ctrl& c, const Ctrl& q, Point& p)
{
	if(&c == &q) return;

	p += c.GetRect().TopLeft();
	AddParentViewOffset(c, p);
	AddContextOffset(*c.GetParent(), q, p); //recurse
}

Point CtrlFinder::GetContextOffset(const Ctrl& c, const Ctrl& q)
{
	Point p; p.Clear();
	AddContextOffset(c, q, p);
	return p;
}

Point CtrlFinder::GetOffset(const Ctrl& ctxuser, const Ctrl& finalctx)
{
	ASSERT(&ctxuser != &finalctx); //a user of a context cant be the context itself
	Point p; p.Clear();
	AddParentViewOffset(ctxuser, p);
	AddContextOffset(*ctxuser.GetParent(), finalctx, p);
	return p;
}

Rect CtrlFinder::SurroundingRect(const Vector<Ctrl*>& ctrls)
{
	Rect r; r.Clear();
	
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		Rect rc = ctrls[i]->GetRect();
		if(r.IsEmpty()) r = rc;
		else r.Union(rc);
	}
	return r;
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

void CtrlFinder::SetCtrl(Ctrl* c)
{
	if(pctrl && c) { ASSERT(IsParentR(pctrl, c)); }
	if(c)
	{
		ctrls.SetCount(1);
		ctrls.Top() = c;
	}
	else ctrls.Clear();
	Update();
}

void CtrlFinder::LeftDown(Point p, dword keyflags)
{
	if(!pctrl) return;
	Point pt(p);
	int f(flags);

	Rect r; r.Clear();

	//try to find multiple first
	if(multi)
	{
		RectTracker tr(*this);
		r = tr.Track(Rect(p,p), ALIGN_NULL, ALIGN_NULL);
	}
	
	if(!r.IsEmpty())
	{
		ctrls = GetCtrls(*pctrl, r, f, filter);
		if(ctrls.IsEmpty())
			ctrls << pctrl;
		WhenLeftSelectMulti(&ctrls, r, keyflags);
	}
	else
	{
		ctrls.SetCount(1);
		ctrls.Top() = GetCtrl(*pctrl, pt, f, filter);
		if(!ctrls.Top())
			ctrls.Top() = pctrl;
		WhenLeftSelect(*ctrls.Top(), p, keyflags);
	}

	Action();
}
void CtrlFinder::RightDown(Point p, dword keyflags)
{
	if(!pctrl) return;
	Point pt(p);
	int f(flags);

	Rect r; r.Clear();

	//try to find multiple first
	if(multi)
	{
		RectTracker tr(*this);
		r = tr.Track(Rect(p,p), ALIGN_NULL, ALIGN_NULL);
	}
	
	if(!r.IsEmpty())
	{
		ctrls = GetCtrls(*pctrl, r, f, filter);
		if(ctrls.IsEmpty())
			ctrls << pctrl;
		WhenRightSelectMulti(&ctrls, r, keyflags);
	}
	else
	{
		ctrls.SetCount(1);
		ctrls.Top() = GetCtrl(*pctrl, pt, f, filter);
		if(!ctrls.Top())
			ctrls.Top() = pctrl;
		WhenRightSelect(*ctrls.Top(), p, keyflags);
	}
	
	Action();

	if(WhenBar)
		MenuBar::Execute(WhenBar);
}

