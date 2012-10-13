#include "RectCtrl.h"

#define IMAGECLASS Img
#define IMAGEFILE <RectCtrl/icons.iml>
#include <Draw/iml_source.h>

CH_STYLE(RectCtrl, Style, StyleDefault)
{
	rectcol = Cyan;
	handcol = SColorHighlight();
	handsize = 2;
	framecol = SColorHighlight();
	framesize = 1;
	textcol = SColorPaper();
}

Rect RectCtrl::HandleAt(const Point& p, int size)
{
	Rect q(p,p); q.Inflate(size); return q;
}

void RectCtrl::DrawHandle(Draw& w, const Rect& r, const Color& col, int size)
{
	w.DrawRect(HandleAt(r.CenterPoint(), size), col);

	w.DrawRect(HandleAt(r.TopLeft(), size), col);
	w.DrawRect(HandleAt(r.BottomRight(), size), col);
	w.DrawRect(HandleAt(r.TopRight(), size), col);
	w.DrawRect(HandleAt(r.BottomLeft(), size), col);

	w.DrawRect(HandleAt(r.CenterLeft(), size), col);
	w.DrawRect(HandleAt(r.CenterRight(), size), col);
	w.DrawRect(HandleAt(r.TopCenter(), size), col);
	w.DrawRect(HandleAt(r.BottomCenter(), size), col);
}

void RectCtrl::DrawHandleFrame(Draw& w, const Rect& _r, const Color& col, int size)
{
	Rect r(_r);
	--r.right;
	--r.bottom;
	Vector<Point> vp;
	vp << r.TopLeft() << r.TopRight() << r.BottomRight() << r.BottomLeft() << r.TopLeft();
	w.DrawPolyline(vp, size, col);
}

void RectCtrl::DrawRectInfo(Draw& w, const Point& p, const Rect&r, const Color& framecol, const Color& textcol)
{
	String t = AsString(r);
	Size tsz = GetTextSize(t, StdFont());
	w.DrawRect(p.x-2, p.y-2, tsz.cx+4, tsz.cy+4, framecol);
	w.DrawText(p.x, p.y, t, StdFont(), textcol);
}

int RectCtrl::GetMode(const Rect& r, const Point& p, dword keyflags, int handsize)
{
	int size = handsize+2;
	Rect q = r; q.Inflate(size);
	if(!q.Contains(p)) return NONE;

	if(HandleAt(r.CenterPoint(), size).Contains(p)) return CENTER;

	if(HandleAt(r.TopLeft(), size).Contains(p)) return LEFTTOP;
	if(HandleAt(r.BottomRight(), size).Contains(p)) return RIGHTBOTTOM;
	if(HandleAt(r.TopRight(), size).Contains(p)) return RIGHTTOP;
	if(HandleAt(r.BottomLeft(), size).Contains(p)) return LEFTBOTTOM;

	if(HandleAt(r.CenterLeft(), size).Contains(p)) return LEFT;
	if(HandleAt(r.CenterRight(), size).Contains(p)) return RIGHT;
	if(HandleAt(r.TopCenter(), size).Contains(p)) return TOP;
	if(HandleAt(r.BottomCenter(), size).Contains(p)) return BOTTOM;

	return MOVE;
}

void RectCtrl::CalcRect(Rect& r, const Point& dp, dword keyflags, int mode, const Point& g)
{
	if(mode == NONE) return;

	Point p = dp;
	int m = mode;

	if(!(keyflags & K_ALT)) m |= GRID;
	if(m & GRIDX) p.x = p.x/g.x*g.x;
	if(m & GRIDY) p.y = p.y/g.y*g.y;

	if(keyflags & K_CTRL)
	{
		double dYdX = double(r.Height())/r.Width();
		switch(m & ALL)
		{
			case LEFTTOP: 
			case RIGHTBOTTOM: p.y = int(p.x*double(r.Height())/r.Width()); break;
			case RIGHTTOP: 
			case LEFTBOTTOM: p.y = -int(p.x*double(r.Height())/r.Width()); break;
			
			case LEFT: m |= TOP; p.y = int(p.x*double(r.Height())/r.Width()); break;
			case BOTTOM: m |= RIGHT; p.x = int(p.y*double(r.Width())/r.Height()); break;
			case RIGHT: m |= BOTTOM; p.y = int(p.x*double(r.Height())/r.Width()); break;
			case TOP: m |= LEFT; p.x = int(p.y*double(r.Width())/r.Height()); break;
		}
	}

	if(keyflags & K_SHIFT)
	{
		if(m & LEFT)  m |= _RIGHT;
		if(m & RIGHT) m |= _LEFT;
		if(m & TOP)   m |= _BOTTOM;
		if(m & BOTTOM)m |= _TOP;
	}

	if(m & LEFT)   r.left += p.x;
	if(m & RIGHT)  r.right += p.x;
	if(m & TOP)    r.top += p.y;
	if(m & BOTTOM) r.bottom += p.y;

	if(m & _LEFT)  r.left -= p.x;
	if(m & _RIGHT) r.right -= p.x;
	if(m & _TOP)   r.top -= p.y;
	if(m & _BOTTOM)r.bottom -= p.y;
}

Image RectCtrl::SetCursor(unsigned m, dword keyflags, const Image& old)
{
	switch(m & (ALL | _ALL))
	{
		case LEFTTOP: return OverrideCursor(Img::ltrb());
		case RIGHTBOTTOM: return OverrideCursor(Img::ltrb());
		case RIGHTTOP: return OverrideCursor(Img::rtlb());
		case LEFTBOTTOM: return OverrideCursor(Img::rtlb());
		
		case LEFT: return OverrideCursor(Img::lr());
		case BOTTOM: return OverrideCursor(Img::tb());
		case RIGHT:  return OverrideCursor(Img::lr());
		case TOP: return OverrideCursor(Img::tb());

		case CENTER: return OverrideCursor(Img::fr());
		case ALL: if(keyflags & K_MOUSELEFT) return OverrideCursor(Img::mv()); //fall through
		
		default: OverrideCursor(old); return Null;
	}
}

void RectCtrl::Paint(Draw& w)
{
	Size sz = GetSize();
	if(r.IsNullInstance()) return;

	if(IsEnabled())
		w.DrawRect(r, style->rectcol);

	DrawHandleFrame(w, r, style->framecol, style->framesize);
	
	if(!IsEnabled() || IsReadOnly()) return;

	DrawHandle(w, r, style->handcol, style->handsize);
	if(pressed)// && moving)
		DrawRectInfo(w, Point(10,10), r, style->framecol, style->textcol);
}

void RectCtrl::LeftDown(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;
	
	SetCapture();
	moving = false;
	pressed = (keyflags & K_MOUSELEFT);

	xr = r;
	xp = p;
	mode = GetMode(r, p, keyflags, style->handsize);
	c = SetCursor(mode, keyflags, c);
	
	if(mode == NONE) WhenMissed(p, keyflags);
	Refresh();
}

void RectCtrl::MouseMove(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;

	moving = true;
	pressed = (keyflags & K_MOUSELEFT);

	if(pressed && mode != NONE) 
	{
		r = xr;
		CalcRect(r, p-xp, keyflags, mode, g);
		r.Normalize(); //if flipped
		UpdateActionRefresh();
	}
	else
	{
		int m = GetMode(r, p, keyflags, style->handsize);
		if(m != mode)
		{
			mode = m;
			c = SetCursor(mode, keyflags, c);
		}
	}
}

void RectCtrl::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	pressed = false;
	moving = false;
	xr.SetNull();
	xp.SetNull();
	mode = NONE;
	if(IsReadOnly() || !IsEnabled()) return;

	c = SetCursor(mode, keyflags, c);
	Refresh();
}

void RectCtrl::RightDown(Point p, dword keyflags)
{
	//cancel
	ReleaseCapture();
	int m = mode;
	mode = NONE;
	if(IsReadOnly() || !IsEnabled()) return;

	c = SetCursor(mode, keyflags, c);
	if(pressed)
	{
		if(m != NONE)
		{
			r = xr;
			UpdateActionRefresh();
		}
	}
	else
	{
		pressed = false;
		moving = false;
		xr.SetNull();
		xp.SetNull();
		//mode = NONE;
		RectTracker tr(*this);
		r = tr.Track(Rect(p,p));
		UpdateActionRefresh();
	}
}

void RectCtrl::Updated()
{
	
}

RectCtrl::RectCtrl()
	: pressed(false), moving(false), mode(NONE), g(4,4)
{
	Transparent();
	//BackPaint();
	style = &StyleDefault();
	r.SetNull();
	xr.SetNull();
	xp.SetNull();
}
