#include "IconDes.h"

NAMESPACE_UPP

void IconDraw::PutHorz(int x, int y, int cx)
{
	if(y < 0 || y >= image.GetHeight())
		return;
	int r = minmax(x + cx, 0, image.GetWidth());
	x = minmax(x, 0, image.GetWidth());
	cx = r - x;
	if(cx <= 0)
		return;
	Upp::Fill(image[y] + x, docolor, cx);
}

void IconDraw::PutVert(int x, int y, int cy)
{
	if(x < 0 || x >= image.GetWidth())
		return;
	int b = minmax(y + cy, 0, image.GetHeight());
	y = minmax(y, 0, image.GetHeight());
	cy = b - y;
	if(cy <= 0)
		return;
	RGBA *t = image[y] + x;
	while(cy-- > 0) {
		*t = docolor;
		t += image.GetWidth();
	}
}

void IconDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	docolor = color;
	int b = minmax(y + cy, 0, image.GetHeight());
	y = minmax(y, 0, image.GetHeight());
	cy = b - y;
	while(cy-- > 0)
		PutHorz(x, y++, cx);
}

void IconDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	docolor = color;
	Width(width);
	Move(Point(x1, y1));
	Line(Point(x2, y2));
}

void IconDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	if(!IsNull(color)) {
		docolor = color;
		Polygon().Ellipse(r).Fill();
	}
	if(!IsNull(pen) && !IsNull(pencolor)) {
		docolor = pencolor;
		Width(pen);
		Ellipse(r);
	}
}

END_UPP_NAMESPACE
