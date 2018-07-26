#include "CocoMM.h"

#ifdef PLATFORM_COCOA

namespace Upp {

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	if(IsNull(color))
		return;
	CGPoint p[2];
	p[0] = Convert(x1, y1);
	p[1] = Convert(x2, y2);
    CGContextSetLineWidth(cgHandle, width > 0 ? width : 1);
	CGContextAddLines(cgHandle, p, 2);
    SetStroke(color);
    CGContextDrawPath(cgHandle, kCGPathStroke);
}

void SystemDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}

void SystemDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void SystemDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void SystemDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	if(IsNull(color) && (pen <= 0 || IsNull(pencolor)))
		return;
	Set(color);
    CGContextSetLineWidth(cgHandle, pen);
    if(pen > 0)
	    SetStroke(pencolor);
    CGContextBeginPath(cgHandle);
    CGContextAddEllipseInRect(cgHandle, Convert(r));
    CGContextDrawPath(cgHandle, IsNull(color) ? kCGPathStroke
                                              : pen > 0 ? kCGPathFillStroke
                                                        : kCGPathFill);
}

};

#endif
