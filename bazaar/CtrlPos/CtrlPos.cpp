#include "CtrlPos.h"

void CtrlPos::DrawAlignHandle(Draw& w, const Rect& _r, const Rect& r, const Ctrl::LogPos& pos, const Color& col)
{
	Point p;
	bool q;
	Size rz = r.GetSize();
	Color col2 = Blend(col, Black);

	p = r.CenterLeft();
	p.y -= rz.cy/4;
	q = (pos.x.GetAlign()&Ctrl::LEFT);
	w.DrawLine(p, Point(_r.left, p.y), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));

	p = r.CenterRight();
	p.y += rz.cy/4;
	q = (pos.x.GetAlign()&Ctrl::RIGHT);
	w.DrawLine(p, Point(_r.right, p.y), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));

	p = r.TopCenter();
	p.x -= rz.cx/4;
	q = (pos.y.GetAlign()&Ctrl::TOP);
	w.DrawLine(p, Point(p.x, _r.top), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));

	p = r.BottomCenter();
	p.x += rz.cx/4;
	q = (pos.y.GetAlign()&Ctrl::BOTTOM);
	w.DrawLine(p, Point(p.x, _r.bottom), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));
}

bool CtrlPos::GetAlignMode(const Rect& _r, const Rect& r, const Point& pp, Ctrl::LogPos& pos, int handsize)
{
	int size = handsize+2;
	Rect c;
	Point p;
	int q;
	Size rz = r.GetSize();
	
	q = pos.x.GetAlign();
	
	p = r.CenterLeft();
	p.y -= rz.cy/4;
	c = Rect(p, Point(_r.left, p.y)); c.InflateVert(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::LEFT; q|= (pos.x.GetAlign()&Ctrl::LEFT)?(0):(Ctrl::LEFT); pos.x.SetAlign(q); return true; }

	p = r.CenterRight();
	p.y += rz.cy/4;
	c = Rect(p, Point(_r.right, p.y)); c.InflateVert(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::RIGHT; q|= (pos.x.GetAlign()&Ctrl::RIGHT)?(0):(Ctrl::RIGHT); pos.x.SetAlign(q); return true; }

	q = pos.y.GetAlign();
	
	p = r.TopCenter();
	p.x -= rz.cx/4;
	c = Rect(p, Point(p.x, _r.top)); c.InflateHorz(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::TOP; q|= (pos.y.GetAlign()&Ctrl::TOP)?(0):(Ctrl::TOP); pos.y.SetAlign(q); return true; }

	p = r.BottomCenter();
	p.x += rz.cx/4;
	c = Rect(p, Point(p.x, _r.bottom)); c.InflateHorz(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::BOTTOM; q|= (pos.y.GetAlign()&Ctrl::BOTTOM)?(0):(Ctrl::BOTTOM); pos.y.SetAlign(q); return true; }
	
	return false;
}

void CtrlPos::GetAlignRects(const Ctrl& ctxuser, const Ctrl& finalctx, Rect& r, Rect& _r)
{
	r = ctxuser.GetRect();
	Point op = GetOffset(ctxuser, finalctx);
	r.Offset(op);
	
	if(ctxuser.InView())
	{
		_r = ctxuser.GetParent()->GetView();
		_r.Offset(op - _r.TopLeft()); //revert own offset which has been added by GetOffset
	}
	else //in frame, support makes no sense, actually, frames shouldnt be moveable
	{
		_r = ctxuser.GetParent()->GetRect();
		Point _op = GetOffset(*ctxuser.GetParent(), finalctx); //without view, so real parents's offset
		_r.Offset(_op);
	}
}

void CtrlPos::DrawHintFrame(Draw& w, const Ctrl& g, const Ctrl& q, const Color& hintcol, const CtrlFilterType& filter, int flags)
{
	Ctrl* c = g.GetFirstChild();
	while(c)
	{
		Ctrl* e(c);
		Point p; p.Clear();
		int f(flags);
		filter(e, f);
		if(e && c->InView())
		{
			Rect r = c->GetRect();
			r.Offset(GetOffset(*c, q));
			r.Inflate(1);
			RectCtrl::DrawHandleFrame(w, r, hintcol, 1);
		}
		if(f & CtrlFinder::DEEP)
			DrawHintFrame(w, *c, q, hintcol, filter, f);
		c = c->GetNext();	
	}
}

//remove each element of _c in c, if found
void CtrlPos::CombineSubtract(Vector<Ctrl*>& c, pick_ Vector<Ctrl*> _c)
{
	//top is the currently editable ctrl
	Vector<Ctrl*> __c(_c);
	while(__c.GetCount() > 0)
	{
		int i = FindIndex(c, __c.Top()); //with this we are O(N^2)
		if(i>=0) c.Remove(i);
		__c.Drop();
	}
}

void CtrlPos::CombineAdd(Vector<Ctrl*>& c, pick_ Vector<Ctrl*> _c)
{
	//top is the currently editable ctrl

	//first, we remove every duplicate, before we take everything else
	CombineSubtract(c, Vector<Ctrl*>(_c, 1)); //deep copy to keep our copy
	c.Append(_c);
}

void CtrlPos::DrawSelected(Draw& w, const Vector<Ctrl*>& ctrls)
{
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		Ctrl& c = *ctrls[i];
		if(&c == pctrl) return; //this will be the only one in the vector

		Rect r; //the rect of the ctrl
		Rect _r; //the outer parent ctrl rect
		GetAlignRects(c, *pctrl, r, _r);		
		
		if(&c == ctrls.Top())
		{
			//top, the one that all controling info comes from or goes to in a multi select context
			DrawAlignHandle(w, _r, r, c.GetPos(), style->handcol);
			RectCtrl::DrawHandleFrame(w, r, style->handcol, style->framesize);
			RectCtrl::DrawHandle(w, r, style->handcol, style->handsize);
			
			RectCtrl::DrawHandleFrame(w, _r, LtRed(), style->framesize); //debug only maybe?
		}
		else
		{
			RectCtrl::DrawHandleFrame(w, r, style->framecol, style->framesize);
		}
	}
}

void CtrlPos::Paint(Draw& w)
{
	Size sz = GetSize();
	
	if(!IsTransparent())
		w.DrawRect(0,0,sz.cx,sz.cy, SColorFace());

	if(!IsEnabled()) return;	

	if(pctrl)
		DrawHintFrame(w, *pctrl, *pctrl, LtGray(), filter, flags);

	if(ctrls.IsEmpty()) return;
	
	DrawSelected(w, ctrls);
	
	if(pressed)// && moving)
		RectCtrl::DrawRectInfo(w, Point(10,10), ctrls.Top()->GetRect(), style->framecol, style->textcol);

	//RectCtrl::DrawHandleFrame(w, rd, LtGreen(), style->framesize); //debug
	//RectCtrl::DrawHandleFrame(w, rd2, Yellow(), style->framesize); //debug
}

void CtrlPos::LeftDown(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;

	SetCapture();
	moving = false;
	pressed = (keyflags & K_MOUSELEFT);

	if(!ctrls.IsEmpty())
	{
		//if already found prepare moving or dragging handles
		//will be dealing with .Top() as the handled Ctrl

		Ctrl& c = *ctrls.Top();
		
		//save old data
		xp = p;
		xpos.SetCount(ctrls.GetCount());
		xpars.SetCount(ctrls.GetCount());
		xop.SetCount(ctrls.GetCount());
		xr.SetCount(ctrls.GetCount());
		for(int i = 0; i < ctrls.GetCount(); i++)
		{
			Ctrl& c = *ctrls[i];
			ASSERT(!c.InFrame());
			xpos[i] = c.GetPos();
			xpars[i] = c.GetParent();
			xop[i] = GetOffset(c, *pctrl);
			xr[i] = c.GetRect();
		}
	
		Rect r; //the rect of the ctrl
		Rect _r; //the outer parent ctrl rect
		GetAlignRects(c, *pctrl, r, _r);		

		Ctrl::LogPos pos = xpos.Top();
		Rect rr(r); rr.Inflate(style->handsize+2);

		//process alignment or prepare cursor for handles
		if(GetAlignMode(_r, rr, p, pos, style->handsize))
		{
			//derive new alignment from top to others
			for(int i = 0; i < ctrls.GetCount(); i++)
			{
				Ctrl& c = *ctrls[i];
				Ctrl::LogPos _pos = LogPosPopUp::MakeLogPos(pos, c);		
				c.SetPos(_pos);
			}
		
			Action();
			Refresh();
			return;
		}
		else if((mode = RectCtrl::GetMode(r, p, keyflags, style->handsize)) != RectCtrl::NONE)
		{
			ci = RectCtrl::SetCursor(mode, keyflags, ci);
			//no action, nothing happened yet
			Refresh();
			return;
		}
		//else fallback to finding a ctrl
	}

	if(!pctrl) return;
	
	//start finding
	
	Point pt(p);
	int f(flags);
	Rect r; r.Clear();

	//we can try to find multiple first
	if(multi)
	{
		RectTracker tr(*this);
		r = tr.Track(Rect(p,p), ALIGN_NULL, ALIGN_NULL);
	}

	if(!r.IsEmpty())
	{
		//try find multiple
		Vector<Ctrl*> _ctrls;
		_ctrls = GetCtrls(*pctrl, r, f, filter);
		if(_ctrls.IsEmpty() || !_ctrls.Top())
			ctrls.Clear();
		else if(keyflags & K_CTRL)
		{
			//add or remove probable found additional controls
			if(keyflags & K_SHIFT)
				CombineSubtract(ctrls, _ctrls);
			else
				CombineAdd(ctrls, _ctrls);
		}
		else
			ctrls = _ctrls;
		if(!ctrls.IsEmpty())
			WhenLeftSelectMulti(&ctrls, r, keyflags);
	}
	else
	{
		//try find single
		
		Vector<Ctrl*> _ctrls;
		_ctrls << GetCtrl(*pctrl, pt, f, filter);
		if(_ctrls.IsEmpty() || !_ctrls.Top()
			|| _ctrls.Top()->InFrame() //dont allow moving frame around
			)
			ctrls.Clear();
		else if(keyflags & K_CTRL)
		{
			//add or remove probable found additional controls
			if(keyflags & K_SHIFT)
				CombineSubtract(ctrls, _ctrls);
			else
				CombineAdd(ctrls, _ctrls);
		}
		else
			ctrls = _ctrls;
		if(!ctrls.IsEmpty())
			WhenLeftSelect(*ctrls.Top(), p, keyflags);
	}

	Action();
	Refresh();
}

void CtrlPos::MouseMove(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;

	moving = true;
	pressed = (keyflags & K_MOUSELEFT);
	
	if(ctrls.IsEmpty()) return;
	Ctrl& c = *ctrls.Top(); //we are always editing top, even if more selected. they will be calculated accordingly

#if 1 //for instant showing where handle dragging is met
	Rect r; //the rect of the ctrl
	Rect _r; //the outer parent ctrl rect
	GetAlignRects(c, *pctrl, r, _r);		
	int m = RectCtrl::GetMode(r, p, keyflags, style->handsize);
	ci = RectCtrl::SetCursor(m, keyflags, ci);
#endif

	if(pressed && mode != RectCtrl::NONE) 
	{
		Point dp(p-xp);

		if(keyflags & K_ALT)
		{
			//move into another parent to be found parent

			//calculate some things not accessible when Removed()
			//then, the Ctrls are removed to not take part in GetCtrl search as new destination
			//then, ctrls are added to new destination or restored to old, if no new found

			//r will be moving ctrl's source rect in pctrl frame coords ctx calculated from its origin parent
			//then, the dp vector offset, from draggin the handles/moving is applied
			//the new parent's ofset is calculated and applied
			//finally, the new logpos is generated using old alignment values, but new rect and new parent's size

 			//cant use r = c.GetRect().Offseted(GetOffset(c, *pctrl));
 			//because parent changes as we move. would need to correct the xpos every time, what we dont want.
 			//so we calculate from the fixed base

			Ctrl* q = c.GetParent(); //to determine later if we stayed within same parent

			Vector<Ctrl*> _xpars;
			Vector<Ctrl*> _xprevc;
			_xpars.SetCount(ctrls.GetCount()); //keep track of current old parent
			//_xprevc.SetCount(ctrls.GetCount()); //and the order
			for(int i = 0; i < ctrls.GetCount(); i++)
			{
				Ctrl& _c = *ctrls[i];
				_xpars[i] = _c.GetParent();
				//_xprevc[i] = _c.GetPrev();
				_c.Remove(); //prevent moving controls from finding when searching new parent
			}
			
			Point pt(p);
			int ft(flags); flags |= (DEEP | NEST);
			Ctrl* pc = GetCtrl(*pctrl, pt, flags, filter);
			flags &= ~(DEEP | NEST); flags |= (ft & DEEP); //restore DEEP flag from save, NEST is ours

			if(!pc) pc = pctrl; //enable fallback to global parent
			if(pc && pc != q)
			{
				//add to new parent
				for(int i = 0; i < ctrls.GetCount(); i++)
					pc->AddChild(ctrls[i]);
			}
			else
			{
				//restore adding to previous
				//we cant add to origin xpars here, since we need to keep track of the very last parrent (would oscilate)
				//means that ctrls content from different parents ends up beeing added to ctrls.Top()'s parent,

				for(int i = 0; i < ctrls.GetCount(); i++)
					_xpars[i]->AddChild(ctrls[i]);//, xprevc[i]); //cant restore full order, 
													//since prev children may be moving as well and may not be added yet
#if 0
				//restore order	now would be possible		
				for(int i = 0; i < ctrls.GetCount(); i++)
					_xpars[i]->AddChild(ctrls[i], _xprevc[i]);
#endif
			}

			//recalculate the new positions for all Ctrls
			for(int i = 0; i < ctrls.GetCount(); i++)
			{
				Ctrl& c = *ctrls[i];
				Rect r = xr[i];
	
				RectCtrl::CalcRect(r, dp, keyflags, mode, g);
				r.Normalize(); //in case r had been moved into flipped
				
				//apply offsets
				//transform ctrl's rect to pctrl context seen from old parent
				//retransform to new parent
				Point opd = GetOffset(c, *pctrl);
				r.Offset(xop[i]-opd);
				
				Ctrl::LogPos pos = LogPosPopUp::MakeLogPos(xpos[i], r, c.GetParent()->GetSize());
				c.SetPos(pos);
			}
		}
		else
		{
			//recalculate the new positions for all Ctrls
			for(int i = 0; i < ctrls.GetCount(); i++)
			{
				Ctrl& c = *ctrls[i];
				Rect r = xr[i];
			
				RectCtrl::CalcRect(r, dp, keyflags, mode, g);
				r.Normalize(); //in case r had been moved into flipped

				Ctrl::LogPos pos = LogPosPopUp::MakeLogPos(xpos[i], r, c.GetParent()->GetSize());
				c.SetPos(pos);
			}
		}
		Action();
		Refresh();
	}
}

void CtrlPos::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	pressed = false;
	moving = false;

	xp.SetNull();
	mode = RectCtrl::NONE;
	if(IsReadOnly() || !IsEnabled()) return;

	ci = RectCtrl::SetCursor(mode, keyflags, ci);
	Refresh();
}

void CtrlPos::RightDown(Point p, dword keyflags)
{
	ReleaseCapture();
	pressed = (keyflags & K_MOUSELEFT);
	moving = false;

	xp.SetNull();
	int m = mode;
	mode = RectCtrl::NONE;
	if(IsReadOnly() || !IsEnabled()) return;

	ci = RectCtrl::SetCursor(mode, keyflags, ci);

	if(ctrls.IsEmpty()) return;

	if(pressed)
	{
		//cancel, restore backup of parent context and logpos
		for(int i = 0; i < ctrls.GetCount(); i++)
		{
			if(ctrls[i]->GetParent() != xpars[i])
				xpars[i]->Add(*ctrls[i]); //@todo: old order not kept
			ctrls[i]->SetPos(xpos[i]);
		}
		Action();
		Refresh();
	}
	else if(WhenBar)
		MenuBar::Execute(WhenBar);
	pressed = false;
}

void CtrlPos::MouseWheel(Point p, int zdelta, dword keyflags)
{
	//if(!IsEditable()) return;
	//if(!HasFocus()) SetFocus();

	if(ctrls.IsEmpty()) return;
	
	//change order of children for top only, drop remaining
	ctrls[0] = ctrls.Top();
	ctrls.SetCount(1);
	
	Ctrl& c = *ctrls.Top();
	
	if(ctrls.GetCount() > 1)
	{
		
	}
	else
	{
		int i = zdelta/120;
	
		for(; i<0; ++i)
		{
			Ctrl* p = c.GetNext();
			c.GetParent()->AddChild(&c, p);
			c.Refresh();
		}
		for(; i>0; --i)
		{
			Ctrl* p = c.GetPrev();
			c.GetParent()->AddChildBefore(&c, p);
			c.Refresh();
		}
	}
}

void CtrlPos::LeftDouble(Point p, dword flags)
{
	//LeftDown already called
	WhenLeftDouble();
}

bool CtrlPos::Key(dword key, int count)
{
	if(key == K_SPACE)
	{
		ClearCtrl();
		Action();
		Refresh();
		return true;	
	}
	return false;
}

void CtrlPos::Updated()
{
	//refresh the view of the currently selected ctrl
	CtrlFinder::Updated();
}

CtrlPos::CtrlPos()
	: pressed(false), moving(false), mode(RectCtrl::NONE), g(4,4)
{
	BackPaint();
	NoIgnoreMouse();
	style = &RectCtrl::StyleDefault();
	xp.SetNull();
}
