#include "Draw.h"

namespace Upp {

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
	if(!IsNull(color)) {
		docolor = color;
		Ellipse(r, -1);
	}
	if(!IsNull(pen) && !IsNull(pencolor)) {
		docolor = pencolor;
		Width(pen);
		Ellipse(r, max(pen, 0));
	}
}

void SDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	if(IsNull(width) || IsNull(color))
		return;
	Width(width);
	docolor = color;
	Move(Point(x1, y1));
	Line(Point(x2, y2));
}

void SDraw::DrawPolyPolyPolygonOp(const Point *vertices0, int /*vertex_count*/,
                                  const int *subpolygon_counts0,
                                  int /*scc*/, const int *disjunct_polygon_counts0, int dpcc0,
                                  Color color, int width, Color outline,
                                  uint64 /*pattern*/, Color /*doxor*/) // Last two parameters ignored
{
	for(int pass = 0; pass < 1 + (width > 0); pass++) {
		const Point *vertices = vertices0;
		const int *subpolygon_counts = subpolygon_counts0;
		const int *disjunct_polygon_counts = disjunct_polygon_counts0;
		int dpcc = dpcc0;
		while(--dpcc >= 0) {
			const Point *sp = vertices;
			vertices += *disjunct_polygon_counts++;
			if(pass == 0)
				Polygon();
			else {
				docolor = outline;
				Width(width);
			}
			while(sp < vertices) {
				const Point *pp = sp;
				sp += *subpolygon_counts++;
				Move(*pp);
				while(++pp < sp)
					Line(*pp);
				Close();
			}
			if(pass == 0 && !IsNull(color)) {
				docolor = color;
				Fill();
			}
		}
	}
}

void SDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                               const int *counts, int count_count, int width, Color color,
                               Color /*doxor*/) // Last parameter ignored
{
	if(width == 0)
		width = 1;
	if(IsNull(color))
		return;
	Width(width);
	docolor = color;
	while(--count_count >= 0) {
		const Point *lp = vertices;
		vertices += *counts++;
		Move(*lp);
		while(++lp < vertices)
			Line(*lp);
	}
}

}
