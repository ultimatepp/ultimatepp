#if defined(flagWINGL) || defined(flagLINUXGL)

#include "Scrollbar.h"

NAMESPACE_UPP

void AlignedFrame::FrameLayout(Rect &r)
{
	switch(layout)
	{
		case LEFT:
			LayoutFrameLeft(r, this, framesize);
			break;
		case TOP:
			LayoutFrameTop(r, this, framesize);
			break;
		case RIGHT:
			LayoutFrameRight(r, this, framesize);
			break;
		case BOTTOM:
			LayoutFrameBottom(r, this, framesize);
			break;
	}
	r.top += border;
	r.left += border;
	r.right -= border;
	r.bottom -= border;
}

void AlignedFrame::FrameAddSize(Size& sz)
{
	sz += border * 2;
	IsVert() ? sz.cx += framesize : sz.cy += framesize;
}

void AlignedFrame::FramePaint(Draw& w, const Rect& r)
{
	if(border > 0)
	{
		Rect n = r;
		switch(layout)
		{
			case LEFT:
				n.left += framesize;
				break;
			case TOP:
				n.top += framesize;
				break;
			case RIGHT:
				n.right -= framesize;
				break;
			case BOTTOM:
				n.bottom -= framesize;
				break;		
		}
		ViewFrame().FramePaint(w, n);
	}
	else
		FrameCtrl<Ctrl>::FramePaint(w, r);		
}

AlignedFrame& AlignedFrame::SetFrameSize(int sz, bool refresh)
{
	framesize = sz; 
	if (refresh) RefreshParentLayout(); 
	return *this;	
}

void AlignedFrame::Fix(Size& sz)
{
	if(IsVert())
		Swap(sz.cx, sz.cy);
}

void AlignedFrame::Fix(Point& p)
{
	if(IsVert())
		Swap(p.x, p.y);
}

Size AlignedFrame::Fixed(const Size& sz)
{
	return IsVert() ? Size(sz.cy, sz.cx) : Size(sz.cx, sz.cy);
}

Point AlignedFrame::Fixed(const Point& p)
{
	return IsVert() ? Point(p.y, p.x) : Point(p.x, p.y);
}

// SlimScrollBar
SlimScrollBar::SlimScrollBar()
{
	Clear();
}

void SlimScrollBar::Clear()
{
	total = 0;
	t = 0;
	pos = 0;
	prev_ps = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	isend = true;
	isbegin = true;
	sz.Clear();
	ready = false;	
}

void SlimScrollBar::UpdatePos(bool update, bool whenscroll)
{
	sz = GetSize();	
	Fix(sz);
	
	if(total <= 0 || sz.cx <= 0)
	{
		cs = ics = 0;
	}
	else
	{
		cs = sz.cx / double(t + 0.5);
		ics = t / double(sz.cx);
	}
	
	size = max(0.0, sz.cx * cs);
	
	if(update)
		pos = new_pos - start_pos;

	isbegin = t < sz.cx;
	isend = t < sz.cx;

	if(pos <= 0)
	{
		pos = 0;
		isbegin = true;
	}
	else if(pos + size >= sz.cx)
	{
		pos = sz.cx - size;
		isend = true;
	}
		
	ps = t > sz.cx ? pos * ics : 0;
	
	if(whenscroll && int(ps) != int(prev_ps))
		WhenScroll();
	
	prev_ps = ps;
}

void SlimScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}

	Ctrl* q = GetParent();
	if(!q)
		return;

	Size sz = GetSize();
	Size psz = q->GetSize();
	
	if(IsHorz() && total > psz.cx || IsVert() && total > psz.cy) {
		Point p(int(pos), 0);
		Size rsz(int(size), IsVert() ? sz.cx : sz.cy);
		Fix(p);
		Fix(rsz);
		#ifdef COLOR_ALPHA
		w.DrawRect(p.x, p.y, rsz.cx, rsz.cy, White().Alpha(150));
		#else
		SystemDraw& sw = (SystemDraw&) w;
		sw.alpha = 150.f;
		w.DrawRect(p.x, p.y, rsz.cx, rsz.cy, White);
		sw.alpha = 255.f;
		#endif		
	}
}

void SlimScrollBar::Layout()
{
	Size csz = GetSize();
	Fix(csz);
	if(ready && csz != sz)
	{
		sz = csz;
		UpdatePos(false);
	}
}

void SlimScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	Fix(p);
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = tabs(p.x - pos);
	UpdatePos();
	UpdateActionRefresh();	
}

void SlimScrollBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	Fix(p);
	old_pos = p.x;
}

void SlimScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;

	Fix(p);
	new_pos = p.x;
	UpdatePos();
	UpdateActionRefresh();
}

void SlimScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
	UpdateActionRefresh();
}

int SlimScrollBar::GetPos() const
{
	return int(ps);
}

void SlimScrollBar::SetPos(int p, bool dontscale, bool whenscroll)
{
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, t) : 0;
	UpdatePos(false, whenscroll);
	Refresh();
}

void SlimScrollBar::AddPos(int p, bool dontscale, bool whenscroll)
{
	pos += total > 0 ? dontscale ? p : iscale(p, sz.cx, t) : 0;
	UpdatePos(false, whenscroll);
	Refresh();
}

int SlimScrollBar::GetTotal() const
{
	return total;
}

void SlimScrollBar::SetTotal(int total, bool whenscroll)
{
	Ctrl* q = GetParent();
	
	Size sz = GetSize();
	Size psz = q ? q->GetSize() : Size(0, 0);

	Fix(sz);
	Fix(psz);

	this->total = total;
	this->t = total;
	if(q)
		this->t -= psz.cx - sz.cx;
	
	UpdatePos(false, whenscroll);
	Refresh();
}

void SlimScrollBar::AddTotal(int t)
{
	sz = GetSize();
	Fix(sz);
	total += t;
	if(total <= 0 || sz.cx <= 0)
		cs = ics = 0;
	else
		cs = sz.cx / ((double) t + 0.5);
	size = int(sz.cx * cs);
	ps = min(ps, (double)(t - sz.cx));
	pos = (int)(ps * cs);		
	old_pos = new_pos = (int)(pos - start_pos);
	Refresh();
}

void SlimScrollBar::GoEnd(bool whenscroll)
{
	pos = total;
	UpdatePos(false, whenscroll);
	Refresh();
}

void SlimScrollBar::GoBegin(bool whenscroll)
{
	pos = 0;
	UpdatePos(false, whenscroll);
	Refresh();
}

bool SlimScrollBar::IsEnd()
{
	return isend;
}

bool SlimScrollBar::IsBegin()
{
	return isbegin;
}

void SlimScrollBar::Set(const SlimScrollBar& t)
{
	total = t.total;
	pos = t.pos;
	ps = t.ps;
	Refresh();
}

bool SlimScrollBar::IsScrollable() const
{
	return t > sz.cx && sz.cx > 0;
}

END_UPP_NAMESPACE

#endif
