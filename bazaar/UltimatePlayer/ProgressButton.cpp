#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include "ProgressButton.h"

ProgressButton::ProgressButton()
{
	pos = 0;
	shaded = true;
	immediate = false;
	src = Color(255, 255, 0);
	dst = Color(255, 0, 0);
}

void ProgressButton::Paint(Draw &w)
{
	Size sz = GetSize();

	w.DrawRect(0, 0, sz.cx, 1, Black);
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Black);
	w.DrawRect(0, 0, 1, sz.cy, Black);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, Black);
		
	int t = (int) ((pos * sz.cx) / 100.0f);
	if(t < 1) t = 1;
	if(t > sz.cx - 1) t = sz.cx - 1;
	
	if(shaded)
	{
		for(int i = 1; i < t; i++)
			w.DrawRect(i, 1, 1, sz.cy - 2, Blend(src, dst, 256 * i / (sz.cx - 1)));
	}
	else
	{
		w.DrawRect(Rect(1, 1, t, sz.cy - 1), dst);
	}

	if(t < sz.cx - 1)
		w.DrawRect(Rect(t, 1, sz.cx - 1, sz.cy - 1), Color(245, 245, 255));

	String s = Format("%s : %.2f %%", text, pos);
	Size tsz = GetTextSize(s, StdFont());
	w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, s);
}

void ProgressButton::LeftDown(Point p, dword keyflags)
{
	pos = (p.x * 100) / (float) GetSize().cx;
	Refresh();
	SetCapture();
	if(immediate)
		WhenAction();
}

void ProgressButton::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	if(!immediate)
		WhenAction();
}

void ProgressButton::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		pos = (p.x * 100) / (float) GetSize().cx;
		if(pos > 100) pos = 100;
		if(pos < 0) pos = 0;
		Refresh();
		if(immediate)
			WhenAction();
	}
	
}

void ProgressButton::SetPos(float p)
{
	pos = p;
	Refresh();
}

float ProgressButton::GetPos()
{
	return pos;
}


