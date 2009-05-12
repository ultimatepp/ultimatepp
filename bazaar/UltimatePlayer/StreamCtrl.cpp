#include <CtrlLib/CtrlLib.h>
using namespace Upp;
#include "StreamCtrl.h"

StreamCtrl::StreamCtrl()
{
	pos = 0;
	info_width = 100;
	progress_height = 18;
	shaded = true;
	immediate = false;
	src = Color(255, 255, 0);
	dst = Color(255, 0, 0);
	noname = true;
	name = "Ultimate Player";
	time = 0;
	bitrate = "0 kbps";
	time_direction = 0;
	
}

void StreamCtrl::Paint(Draw &w)
{
	Size sz = GetSize();
	
	Color fill = Color(245, 245, 255);

	int width = sz.cx - info_width;
	int height = sz.cy - progress_height;

	/* black border */
	w.DrawRect(0, 0, sz.cx, 1, Black);
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Black);
	w.DrawRect(0, 0, 1, sz.cy, Black);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, Black);
	
	/* progress separator */
	w.DrawRect(1, progress_height, width - 1, 1, Gray);
	           
	/* info separator */
	w.DrawRect(width, 1, 1, sz.cy - 2, Gray);
			
	int t = max(1, fceil((pos * width) / 100.0f));
	//if(t < 1) t = 1;
	//if(t > width) t = width;
	
	if(shaded)
	{
		for(int i = 1; i < t; i++)
			w.DrawRect(i, 1, 1, progress_height - 1, Blend(src, dst, 256 * i / (width - 1)));
	}
	else
	{
		w.DrawRect(Rect(1, 1, t, progress_height), dst);
	}

	w.DrawRect(Rect(t, 1, width, progress_height), fill);

	String s = Format("%s : %d %%", postext, (int) pos);
	Size psz = GetTextSize(s, StdFont());
	w.DrawText((width - psz.cx) / 2, 1 + (progress_height - psz.cy) / 2, s);

	/* Title */
	//WString text = name;	
	Font fnt = Arial(18);
	Size nsz = GetTextSize(name, fnt);
		
	w.DrawRect(1, progress_height + 1, width - 1, height - 2, fill);	
	w.Clip(5, progress_height, width - 10, height);
	w.DrawText(noname ? (width - nsz.cx) / 2 : 5, progress_height + (height - nsz.cy) / 2 + 1, name, fnt);
	w.End();

	/* Time */
	w.DrawRect(width + 1, 1, info_width - 2, sz.cy - 2, fill);
	
	String ts;
	if(time_direction > 0)
	{
		int tms = time_total - time;
		int min = tms / 1000 / 60;
		int sec = tms / 1000 % 60;
		if(min == 0 && sec == 0)
			ts = "00:00";
		else
			ts = ::Format("-%02d:%02d", min, sec);
	}
	else
	{
		int tms = time;
		ts = ::Format("%02d:%02d", tms / 1000 / 60, tms / 1000 % 60);
	}
	Size tsz = GetTextSize(ts, fnt);	
	w.DrawText(width + (info_width - tsz.cx) / 2, sz.cy - (sz.cy - progress_height + tsz.cy) / 2, ts, fnt);

	/* Bitrate */
	
	fnt = Arial(11);
	String inf = bitrate + " / stereo";
	Size isz = GetTextSize(inf, fnt);	
	w.DrawText(width + (info_width - isz.cx) / 2, 3, inf, fnt);
	//w.DrawText(width + 5, 3, bitrate, fnt);
	//w.DrawText(width + 5 + bsz.cx + 5, 3, "stereo", fnt);

}

void StreamCtrl::LeftDown(Point p, dword keyflags)
{
	Size sz = GetSize();
	int width = sz.cx - info_width;
	if(p.x < width && p.y < progress_height)
	{
		pos = (p.x * 100) / (float) (sz.cx - info_width);
		Refresh();
		SetCapture();
		if(immediate)
			WhenAction();
	}
	else if(p.x > width && p.y > progress_height)
	{
		time_direction = 1 - time_direction;
		Refresh();
	}
}

void StreamCtrl::LeftUp(Point p, dword keyflags)
{
	if(HasCapture())
	{
		ReleaseCapture();
		if(!immediate)
			WhenAction();
	}
}

void StreamCtrl::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		pos = (p.x * 100) / (float) (GetSize().cx - info_width);
		if(pos > 100) pos = 100;
		if(pos < 0) pos = 0;
		Refresh();
		if(immediate)
			WhenAction();
	}
	
}

void StreamCtrl::SetPos(float p)
{
	pos = p;
	Refresh();
}

float StreamCtrl::GetPos()
{
	return pos;
}

