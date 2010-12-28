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

void RectCtrl::Paint(Draw& w)
{
	Size sz = GetSize();

	if(r.IsNullInstance()) return;
	
	//rect
	w.DrawRect(r, style->rectcol);

	//handle frame
	Vector<Point> vp;
	vp << r.TopLeft() << r.TopRight() << r.BottomRight() << r.BottomLeft() << r.TopLeft();
	w.DrawPolyline(vp, style->framesize, style->framecol);
	
	//handles
	int size = style->handsize;
	w.DrawRect(HandleAt(r.CenterPoint(), size), style->handcol);

	w.DrawRect(HandleAt(r.TopLeft(), size), style->handcol);
	w.DrawRect(HandleAt(r.BottomRight(), size), style->handcol);
	w.DrawRect(HandleAt(r.TopRight(), size), style->handcol);
	w.DrawRect(HandleAt(r.BottomLeft(), size), style->handcol);

	w.DrawRect(HandleAt(r.CenterLeft(), size), style->handcol);
	w.DrawRect(HandleAt(r.CenterRight(), size), style->handcol);
	w.DrawRect(HandleAt(r.TopCenter(), size), style->handcol);
	w.DrawRect(HandleAt(r.BottomCenter(), size), style->handcol);
	
	//text
	if(pressed && moving)
	{
		String t = AsString(r);
		Size tsz = GetTextSize(t, StdFont());
		w.DrawRect(10, 10, tsz.cx, tsz.cy, style->framecol);
		w.DrawText(10, 10, t, StdFont(), style->textcol);
	}
}

int RectCtrl::GetMode(const Point& p, dword keyflags)
{
	int size = style->handsize<<1;
	Rect q = r; q.Inflate(size);
	if(!q.Contains(p)) return NONE;

	if(HandleAt(r.TopLeft(), size).Contains(p)) return LEFTTOP;
	if(HandleAt(r.BottomRight(), size).Contains(p)) return RIGHTBOTTOM;
	if(HandleAt(r.TopRight(), size).Contains(p)) return RIGHTTOP;
	if(HandleAt(r.BottomLeft(), size).Contains(p)) return LEFTBOTTOM;

	if(HandleAt(r.CenterLeft(), size).Contains(p)) return LEFT;
	if(HandleAt(r.CenterRight(), size).Contains(p)) return RIGHT;
	if(HandleAt(r.TopCenter(), size).Contains(p)) return TOP;
	if(HandleAt(r.BottomCenter(), size).Contains(p)) return BOTTOM;

	if(HandleAt(r.CenterPoint(), size).Contains(p)) return CENTER;

	return MOVE;
}

void RectCtrl::CalcRect(const Point& dp, dword keyflags)
{
	if(mode == NONE) return;
	r = xr;

	Point p = dp;
	int m = mode;

	if(!(keyflags & K_ALT)) m |= GRID;
	if(m & GRIDX) p.x = dp.x/g.x*g.x;
	if(m & GRIDY) p.y = dp.y/g.y*g.y;

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

void RectCtrl::SetCursor(unsigned m, dword keyflags)
{
	switch(m & (ALL | _ALL))
	{
		case LEFTTOP: c = OverrideCursor(Img::ltrb()); break;
		case RIGHTBOTTOM: c = OverrideCursor(Img::ltrb()); break;
		case RIGHTTOP: c = OverrideCursor(Img::rtlb()); break; 
		case LEFTBOTTOM: c = OverrideCursor(Img::rtlb()); break;
		
		case LEFT: c = OverrideCursor(Img::lr()); break;
		case BOTTOM: c = OverrideCursor(Img::tb()); break;
		case RIGHT:  c = OverrideCursor(Img::lr()); break;
		case TOP: c = OverrideCursor(Img::tb()); break;

		case ALL: c = OverrideCursor(Img::mv()); break;
		case CENTER: c = OverrideCursor(Img::fr()); break;
		
		default: OverrideCursor(c); break;
	}
}

void RectCtrl::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	moving = false;
	pressed = (keyflags & K_MOUSELEFT);

	xr = r;
	xp = p;
	mode = GetMode(p, keyflags);

	SetCursor(mode, keyflags);
	
	if(mode == NONE) WhenMissed(p, keyflags);
}

void RectCtrl::MouseMove(Point p, dword keyflags)
{
	moving = true;
	pressed = (keyflags & K_MOUSELEFT);

	//int m = GetMode(p, keyflags);
	//SetCursor(m, keyflags);

	if(pressed && mode != NONE) 
	{
		CalcRect(p-xp, keyflags);	
		UpdateActionRefresh();
	}
	else Refresh();
}

void RectCtrl::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	pressed = false;
	moving = false;
	xr.SetNull();
	xp.SetNull();
	mode = NONE;
	SetCursor(mode, keyflags);
	Refresh();
}

void RectCtrl::RightDown(Point p, dword keyflags)
{
	//cancel
	ReleaseCapture();
	mode = NONE;
	SetCursor(mode, keyflags);
	if(xr.IsNullInstance()) return;
	r = xr;
	UpdateActionRefresh();
}

void RectCtrl::Updated()
{
	
}

RectCtrl::RectCtrl()
	: pressed(false), moving(false)
{
	Transparent();
	style = &StyleDefault();
	r.SetNull();
	g = Point(4,4);
	xr.SetNull();
	xp.SetNull();
}
