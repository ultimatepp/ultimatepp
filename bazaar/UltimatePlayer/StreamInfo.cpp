#include <CtrlLib/CtrlLib.h>
#include "StreamInfo.h"

StreamInfo::StreamInfo()
{
	noname = true;
	name = "Ultimate Player";
	time = "00:00:00";
}


void StreamInfo::Paint(Draw &w)
{
	Size sz = GetSize();

	w.DrawRect(0, 0, sz.cx, 1, Black);
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Black);
	w.DrawRect(0, 0, 1, sz.cy, Black);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, Black);
	
	
	WString text = name;	
	Font fnt = Arial(18);//.Bold();
	Size nsz = GetTextSize(text, fnt);
	Size tsz = GetTextSize(time, fnt);
		
	if(noname)
	{
		w.DrawRect(1, 1, sz.cx - 2, sz.cy - 2, Color(245, 245, 255));
		w.Clip(5, 0, sz.cx - 10, sz.cy);
		w.DrawText((sz.cx - nsz.cx) / 2, (sz.cy - nsz.cy) / 2, text, fnt);
		w.End();
	}
	else
	{
		fnt.NoBold();
		w.DrawRect(Rect(1, 1, sz.cx - tsz.cx - 10, sz.cy - 1), Color(245, 245, 255));
		w.DrawRect(Rect(sz.cx - tsz.cx - 10, 1, sz.cx - 1, sz.cy - 1), Color(200, 200, 240));
		w.DrawRect(sz.cx - tsz.cx - 10, 1, 1, sz.cy - 2, Black);
		
		w.Clip(5, 0, sz.cx - 20 - tsz.cx, sz.cy);
		w.DrawText(5, (sz.cy - nsz.cy) / 2 + 1, text, fnt);
		w.End();
		w.Clip(sz.cx - tsz.cx - 5, 0, sz.cx - 5, sz.cy);
//		w.DrawText(sz.cx - 5 - tsz.cx, (sz.cy - tsz.cy) / 2 + 1, time, fnt);

		//fnt.Bold();
		int x = sz.cx - 5 - tsz.cx;
		int y = (sz.cy - tsz.cy) / 2 + 1;
		w.DrawText(x, y, time, fnt);
		
		w.End();
	}	
}


