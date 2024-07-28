#include "IconDes.h"

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

void IconDraw::DrawRect(int x, int y, int cx, int cy, RGBA color)
{
	docolor = color;
	int b = minmax(y + cy, 0, image.GetHeight());
	y = minmax(y, 0, image.GetHeight());
	cy = b - y;
	while(cy-- > 0)
		PutHorz(x, y++, cx);
}

void IconDraw::DrawFrame(int x, int y, int cx, int cy, RGBA color, int n)
{
	DrawRect(x, y, cx, n, color);
	DrawRect(x, y + n, n, cy - n, color);
	DrawRect(x + cx - n, y + n, n, cy - n, color);
	DrawRect(x + n, y + cy - n, cx - 2 * n, n, color);
}

void IconDraw::DrawLine(Point p1, Point p2, int width, RGBA color)
{
	docolor = color;
	Width(width);
	Move(p1);
	Line(p2);
}

void IconDraw::DrawEllipse(const Rect& r_, bool fill_empty, RGBA color, int pen, RGBA pencolor)
{
	Rect r = r_.Normalized();
	if(r.IsEmpty() && Rect(image.GetSize()).Contains(r.TopLeft())) {
		image[r.top][r.left] = pencolor;
		return;
	}
	r.right++;
	r.bottom++;
	if(fill_empty) {
		docolor = color;
		Ellipse(r, -1);
	}
	if(!IsNull(pen)) {
		docolor = pencolor;
		Ellipse(r, pen);
	}
}
