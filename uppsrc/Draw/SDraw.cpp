#include "Draw.h"

NAMESPACE_UPP

dword SDraw::GetInfo() const
{
	return DRAWTEXTLINES;
}

void SDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	// TODO
}

void SDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	// TODO
}

void SDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	Width(width);
	docolor = color;
	Move(Point(x1, y1));
	Line(Point(x2, y2));
}

void SDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	// TODO
}

void SDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
	// TODO
}

END_UPP_NAMESPACE
