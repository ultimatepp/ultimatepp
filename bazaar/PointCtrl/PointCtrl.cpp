#include "PointCtrl.h"

CH_STYLE(PointCtrl, Style, StyleDefault)
{
	backcol = SColorPaper();
	linecol = Blue();
	pointcol = LtRed();
	linesize = 1;
	pointsize = 4;
}

PointCtrl::PointCtrl()
	: pressed(false), moving(false), di(-1)
{
	style = &StyleDefault();
	sxp.Clear();
	xp.SetNull();

	vsx.MinMax(0,100);
	vsy.MinMax(0,100);

	for(int i = 0; i < 10; i++)
		vpd.Add().col = HsvColorf(i/(double)10, 1.0, 1.0);
	
	Update();
}

void PointCtrl::Paint(Draw& w)
{
	w.DrawRect(sz, style->backcol);

	//points
	for(int i = 0; i < vp.GetCount(); i++)
	{
		const XY& _dp = vp[i];
		XY _p((int) psx(vsx, _dp.x), (int) psy(vsy, _dp.y));
		const PData& pd = vpd.At(i);
		Rect r(_p,_p); r.Inflate(pd.size);
		w.DrawEllipse(r, pd.col);
	}

	//cross
	if(!xp.IsNullInstance() && IsEnabled() && IsEditable()) 
	{
		w.DrawLine(xp.x-3,xp.y, xp.x+3+1,xp.y, style->linesize, style->linecol);
		w.DrawLine(xp.x,xp.y-3, xp.x,xp.y+3+1, style->linesize, style->linecol);

		//target lines
		if(pressed && moving) 
		{
			w.DrawLine(0,xp.y, sz.cx,xp.y, style->linesize, style->linecol);
			w.DrawLine(xp.x,0, xp.x,sz.cy, style->linesize, style->linecol);
		}
	}

	//square
	Rect r(sxp, sxp); r.Inflate(style->pointsize);
	Vector<Point> vp;
	vp << r.TopLeft() << r.TopRight() << r.BottomRight() << r.BottomLeft() << r.TopLeft();
	w.DrawPolyline(vp, style->linesize, style->pointcol);

	//dot
	w.DrawLine(sxp.x, sxp.y, sxp.x, sxp.y, 2, style->pointcol);
}

void PointCtrl::LeftDown(Point p, dword keyflags)
{
	moving = false;
	if(IsReadOnly() || !IsEnabled()) return;
	SetCapture();
	pressed = (keyflags & K_MOUSELEFT);

	xp = p;
	sxp = xp;

	//point drag
	for(int i = 0; i < vp.GetCount(); i++)
	{
		XY& _dp = vp[i];
		Point _p(int(psx(vsx, _dp.x)), int(psy(vsy, _dp.y)));
		Rect r(_p, _p); r.Inflate(4);
		if(!r.Contains(p)) continue;
		di = i; break;
	}
	UpdateActionRefresh();
}

void PointCtrl::LeftRepeat(Point p, dword keyflags)
{
	if(moving) return;
	if(IsReadOnly() || !IsEnabled()) return;
	
	if(di<0 && !(keyflags & K_CTRL))
	{
		di = vp.GetCount();
		XY& _dp = vp.Add(XY(vsx(psx, sxp.x), vsy(psy, sxp.y)));
		WhenAddPoint();
	}
	else if(di>=0 && (keyflags & K_CTRL))
	{
		vp.Remove(di);
		WhenRemovePoint(di);
		di = -1;
		moving = true;
	}
	else return;
	
	Refresh();
}

void PointCtrl::MouseMove(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;
	moving = true;
	pressed = (keyflags & K_MOUSELEFT);

	//clip
	Point pp(minmax(p.x, 0, sz.cx), minmax(p.y, 0, sz.cy));
	if(xp == pp) return;
	xp = pp; 

	if(pressed) 
	{
		sxp = xp;
		if(di>=0)
		{
			vp[di] = XY(vsx(psx, sxp.x), vsy(psy, sxp.y));
			WhenMovePoint(di);
		}
		UpdateActionRefresh();
	}
	else Refresh();
}

void PointCtrl::LeftUp(Point p, dword keyflags)
{
	pressed = false;
	moving = false;
	if(IsReadOnly() || !IsEnabled()) return;
	ReleaseCapture();
	di = -1;
	xp.SetNull();
	Refresh();
}

void PointCtrl::Layout()
{
	sz = GetSize();
	psx.MinMax(0, sz.cx);
	psy.MinMax(0, sz.cy);
	UpdatedBack();
	Refresh();
}

void PointCtrl::Updated()
{
	vxp = XY(vsx(psx, sxp.x), vsy(psy, sxp.y));
}

void PointCtrl::UpdatedBack()
{
	sxp = Point(int(psx(vsx, vxp.x)), int(psy(vsy, vxp.y)));
}
