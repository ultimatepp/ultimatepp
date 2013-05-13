#include "glyph.h"

void FDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
}

void FDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void FDraw::DrawDrawingOp(const Rect& target, const Drawing& w)
{
}

void FDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

void FDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
}

void FDraw::DrawPaintingOp(const Rect& target, const Painting& w)
{
}

void FDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void FDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}
