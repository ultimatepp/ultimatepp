#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)

void SystemDraw::SetColor(Color c)
{
	cairo_set_source_rgb(cr, c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

dword SystemDraw::GetInfo() const
{
	return DRAWTEXTLINES;
}

Point SystemDraw::GetOffset() const
{
	return offset.GetCount() ? offset.Top() : Point(0, 0);
}

void SystemDraw::Push()
{
	cairo_save(cr);
	offset.Add(GetOffset());
}

void SystemDraw::Pop()
{
	if(offset.GetCount())
		offset.Drop();
	cairo_restore(cr);
}

void SystemDraw::BeginOp()
{
	Push();
}

void SystemDraw::EndOp()
{
	Pop();
}

void SystemDraw::OffsetOp(Point p)
{
	Push();
	offset.Top() += p;
	cairo_translate(cr, p.x, p.y);
}

void SystemDraw::RectPath(const Rect& r)
{
	cairo_rectangle(cr, r.left, r.top, r.GetWidth(), r.GetHeight());
}

bool SystemDraw::ClipOp(const Rect& r)
{
	Push();
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool SystemDraw::ClipoffOp(const Rect& r)
{
	Push();
	offset.Top() += r.TopLeft();
	RectPath(r);
	cairo_clip(cr);
	cairo_translate(cr, r.left, r.top);
	return true;
}

bool SystemDraw::ExcludeClipOp(const Rect& r)
{
	RectPath(Rect(-99999, -99999, 99999, r.top));
	RectPath(Rect(-99999, r.top, r.left, 99999));
	RectPath(Rect(r.right, r.top, 99999, 99999));
	RectPath(Rect(r.left, r.bottom, r.right, 99999));
	cairo_clip(cr);
	return true;
}

bool SystemDraw::IntersectClipOp(const Rect& r)
{
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool SystemDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

Rect SystemDraw::GetPaintRect() const
{
	return Rect(0, 0, INT_MAX, INT_MAX);
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	if(IsNull(color))
		return;
	cairo_rectangle(cr, x, y, cx, cy);
	if(color == InvertColor()) {
		SetColor(White());
		cairo_set_operator(cr, CAIRO_OPERATOR_DIFFERENCE);
		cairo_fill(cr);
		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
	}
	else {
		SetColor(color);
		cairo_fill(cr);
	}
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	SetColor(color);
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_set_line_width (cr, width);
	cairo_stroke(cr);
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
}

END_UPP_NAMESPACE

#endif
