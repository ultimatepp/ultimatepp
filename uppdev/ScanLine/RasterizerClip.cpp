#include "ScanLine.h"

void Rasterizer::Move(double x, double y)
{
	x1 = x;
	y1 = y;
}

inline int Cv(double x)
{
	return int(minmax(x * 256 + 65536.5, 0.0, 200000.0)) - 65536;
}

void Rasterizer::LineClip(double x1, double y1, double x2, double y2)
{
	if(y1 < cliprect.top) {
		if(y2 < cliprect.top)
			return;
		x1 = (x2 - x1) * (cliprect.top - y1) / (y2 - y1) + x1;
		y1 = cliprect.top;
	}
	else
	if(y1 > cliprect.bottom) {
		if(y2 > cliprect.bottom)
			return;
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
		DDUMP(Cv(y2));
	}
/*
	if(x1 < cliprect.left) {
		if(x2 < cliprect.left)
			return;
		y1 = (y2 - y1) * (cliprect.left - x1) / (x2 - x1) + y1;
		x1 = cliprect.left;
	}
	else
	if(x1 > cliprect.right) {
		if(x2 > cliprect.right)
			return;
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
*/
	int ymax = (sz.cy << 8) - 1;
	LineRaw(Cv(x1), min(Cv(y1), ymax), Cv(x2), min(Cv(y2), ymax));
}

void Rasterizer::Line(double x2, double y2)
{
	LineClip(x1, y1, x2, y2);
	x1 = x2;
	y1 = y2;
}
