#include "SDraw.h"

NAMESPACE_UPP

void SDraw::BeginOp()
{
	Begin();
}

void SDraw::EndOp()
{
	End();
}

void SDraw::OffsetOp(Point p)
{
	Begin();
	Translate(p.x, p.y);
}

bool SDraw::ClipOp(const Rect& r)
{
	Begin();
	// todo
	return true;
}

bool SDraw::ClipoffOp(const Rect& r)
{
	Begin();
	Translate(r.left, r.top);
	return true;
}

bool SDraw::ExcludeClipOp(const Rect& r)
{
	// ToDo
	return true;
}

bool SDraw::IntersectClipOp(const Rect& r)
{
	// ToDo
	return true;
}

Rect SDraw::GetClipOp() const
{
	return Rect(0, 0, 0, 0);
}

bool SDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

void SDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	RTIMING("Rect");
	MoveTo(x, y).LineTo(x + cx - 1, y).LineTo(x + cx - 1, y + cy - 1).LineTo(x, y + cy - 1);
	Fill(color);
}

void SDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	MoveTo(x, y).LineTo(x + cx - 1, y).LineTo(x + cx - 1, y + cy - 1).LineTo(x, y + cy - 1);
	Fill(img, Translate2D(x, y));
}

void SDraw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
}

void SDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	MoveTo(x1, y1);
	LineTo(x2, y2);
	Stroke(color, width);
}

void SDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}

void SDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void SDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void SDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

void SDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	RTIMING("DrawTextOp");
	Begin();
	EvenOdd(true);
	if(angle)
		Rotate(angle * M_2PI / 36000);
	if(n < 0)
		n = wstrlen(text);
	double *ddx = NULL;
	Buffer<double> h;
	if(dx) {
		h.Alloc(n);
		ddx = h;
		for(int i = 0; i < n; i++)
			ddx[i] = dx[i];
	}
	Text(*this, x, y, text, font, n, ddx);
	Fill(ink);
	End();
}

END_UPP_NAMESPACE
