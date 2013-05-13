#include "glyph.h"

dword FDraw::GetInfo() const
{
	return DRAWTEXTLINES;
}

void FDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void FDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

void FDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	Width(width);
	docolor = color;
	Move(Point(x1, y1));
	Line(Point(x2, y2));
}

void FDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void FDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}
