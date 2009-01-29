#include "LineClip.h"

bool LineClip::Clip(Painter& sw, double x1, double y1, double x2, double y2)
{
	if(x1 < cliprect.left) {
		if(x2 < cliprect.left)
			return false;
		y1 = (y2 - y1) * (cliprect.left - x1) / (x2 - x1) + y1;
		x1 = cliprect.left;
	}
	else
	if(x1 > cliprect.right) {
		if(x2 > cliprect.right)
			return false;
		y1 = (y2 - y1) * (cliprect.right - x1) / (x2 - x1) + y1;
		x1 = cliprect.right;
	}
	if(x2 < cliprect.left) {
		y2 = (y2 - y1) * (cliprect.left - x1) / (x2 - x1) + y1;
		x2 = cliprect.left;
	}
	else
	if(x2 > cliprect.right) {
		y2 = (y2 - y1) * (cliprect.right - x1) / (x2 - x1) + y1;
		x2 = cliprect.right;
	}
	if(y1 < cliprect.top) {
		if(y2 < cliprect.top)
			return false;
		x1 = (x2 - x1) * (cliprect.top - y1) / (y2 - y1) + x1;
		y1 = cliprect.top;
	}
	else
	if(y1 > cliprect.bottom) {
		if(y2 > cliprect.bottom)
			return false;
		x1 = (x2 - x1) * (cliprect.bottom - y1) / (y2 - y1) + x1;
		y1 = cliprect.bottom;			
	}
	if(y2 < cliprect.top) {
		x2 = (x2 - x1) * (cliprect.top - y1) / (y2 - y1) + x1;
		y2 = cliprect.top;
	}
	else
	if(y2 > cliprect.bottom) {
		x2 = (x2 - x1) * (cliprect.bottom - y1) / (y2 - y1) + x1;
		y2 = cliprect.bottom;
	}
	sw.Move(x1, y1).Line(x2, y2).Stroke(1, LtRed());
	return true;
}

void LineClip::Paint(Draw& w)
{
	ImageBuffer ib(GetSize());
	BufferPainter sw(ib);
	sw.Clear(White());
	sw.Rectangle(100, 100, 100, 100).Fill(WhiteGray());
	sw.Move(x1, y1).Line(x2, y2).Stroke(1, Gray());
	if(!Clip(sw, x1, y1, x2, y2))
		sw.Text(0, 0, "No clip", Arial(20)).Fill(Black());
	w.DrawImage(0, 0, ib);
}

void LineClip::LeftDown(Point p, dword keyflags)
{
	x1 = p.x;
	y1 = p.y;
	Refresh();
}

void LineClip::MouseMove(Point p, dword keyflags)
{
	x2 = p.x;
	y2 = p.y;
	Refresh();
}

LineClip::LineClip()
{
	cliprect = RectfC(100, 100, 100, 100);
	x1 = y1 = x2 = y2 = 0;
	Refresh();
}

GUI_APP_MAIN
{
	LineClip().Run();
}
