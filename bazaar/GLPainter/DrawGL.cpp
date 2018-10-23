#include "GLPainter.h"

namespace Upp {

void DrawGL::Init(Size sz, double alpha)
{
	Cloff& c = cloff.Add();
	c.clip = view_size = sz;
	c.offset = Pointf(0.5, 0.5);
	dd.Set(sz);
	dd.alpha = alpha;
	glEnable(GL_SCISSOR_TEST);
	SyncScissor();
}

DrawGL::~DrawGL()
{
	glDisable(GL_SCISSOR_TEST);
}

dword DrawGL::GetInfo() const
{
	return DRAWTEXTLINES;
}

void DrawGL::BeginOp()
{
	Cloff c = cloff.Top();
	cloff.Add(c);
}

bool DrawGL::ClipOp(const Rect& r)
{
	Cloff c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = c.clip & (r + c.offset);
	c1.offset = c.offset;
	SyncScissor();
	return !c1.clip.IsEmpty();
}

bool DrawGL::ClipoffOp(const Rect& r)
{
	Cloff c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = c.clip & (r + c.offset);
	c1.offset = c.offset + (Pointf)r.TopLeft();
	SyncScissor();
	return !c1.clip.IsEmpty();
}

bool DrawGL::IntersectClipOp(const Rect& r)
{
	Cloff& c = cloff.Top();
	c.clip = c.clip & (r + c.offset);
	SyncScissor();
	return !c.clip.IsEmpty();
}

bool DrawGL::ExcludeClipOp(const Rect& r)
{
	// does not work with DrawGL
	return true;
}

bool DrawGL::IsPaintingOp(const Rect& r) const
{
	return true;
}

void DrawGL::OffsetOp(Point p)
{
	Cloff c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = c.clip;
	c1.offset = c.offset + (Pointf)p;
}

void DrawGL::EndOp()
{
	ASSERT(cloff.GetCount());
	if(cloff.GetCount())
		cloff.Drop();
	SyncScissor();
}

void DrawGL::SyncScissor()
{
	Rect clip = cloff.Top().clip;
	Size sz = clip.GetSize();
	glScissor(clip.left, view_size.cy - sz.cy - clip.top, sz.cx, sz.cy);
}

Pointf DrawGL::Offset(int x, int y)
{
	Pointf o = cloff.Top().offset;
	return Pointf(x + o.x, y + o.y);
}

Rectf DrawGL::Offset(int x, int y, int cx, int cy)
{
	Point o = cloff.Top().offset;
	return RectfC(x + o.x, y + o.y, cx, cy);
}

Rectf DrawGL::Offset(int x, int y, Size sz)
{
	return Offset(x, y, sz.cx, sz.cy);
}

void DrawGL::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	GLDrawImage(dd, Offset(x, y, img.GetSize()),
	            IsNull(color) ? img : CachedSetColorKeepAlpha(img, color));
}

void DrawGL::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	GLDrawImage(dd, Offset(x, y, img.GetSize()), IsNull(color) ? img : CachedSetColorKeepAlpha(img, color),
	            src);
}

void DrawGL::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	Vector<Vector<Pointf>> polygon;
	polygon.Add().Add(Offset(x, y));
	polygon.Top().Add(Offset(x + cx, y));
	polygon.Top().Add(Offset(x + cx, y + cy));
	polygon.Top().Add(Offset(x, y + cy));

	GLVertexData data;
	GLPolygons(data, polygon);
	
	GLDrawConvexPolygons(dd, Pointf(0, 0), data, Sizef(1, 1), color);
}

void DrawGL::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	GLDrawText(dd, Offset(x, y), angle * M_2PI / 3600, text, font, ink, n, dx);
}

void DrawGL::ApplyDash(Vector<Vector<Pointf>>& polyline, int& width)
{
	if(width == 0)
		width = 1;
	if(width > 0)
		return;
	if(width == PEN_NULL) {
		width = 0;
		return;
	}

	static Vector<double> dash = { 18, 6 };
	static Vector<double> dot = { 3, 3 };
	static Vector<double> dashdot = { 9, 6, 3, 6 };
	static Vector<double> dashdotdot = { 9, 3, 3, 3, 3, 3 };
	
	Vector<double>& d = *decode(width, PEN_DASH, &dash,
	                                   PEN_DOT, &dot,
	                                   PEN_DASHDOT, &dashdot,
	                                   &dashdotdot);

	Vector<Vector<Pointf>> r;
	for(auto& l : polyline)
		DashPolyline(r, l, d);
	
	polyline = pick(r);
	width = 1;
}

void DrawGL::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	Vector<Vector<Pointf>> poly;
	poly.Add().Add(Offset(x1, y1));
	poly.Top().Add(Offset(x2, y2));
	
	ApplyDash(poly, width);
	
	GLVertexData data;
	GLPolylines(data, poly);
	
	if(width > 0)
		GLDrawPolylines(dd, Pointf(0, 0), data, Sizef(1, 1), width, color);
}

void DrawGL::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	Vector<Vector<Pointf>> poly;
	GLArc(poly, rc, start, end);
	ApplyDash(poly, width);
	GLVertexData data;
	GLPolylines(data, poly);
	if(width > 0)
		GLDrawPolylines(dd, Pointf(0, 0), data, Sizef(1, 1), width, color);
}

void DrawGL::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	GLDrawEllipse(dd, Offset(r.CenterPoint()), Sizef(r.GetSize()) / 2, color, pen, pencolor);
}

void DrawGL::DoPath(Vector<Vector<Pointf>>& poly, const Point *pp, const Point *end)
{
	poly.Add().Add(Offset(*pp++));
	while(pp < end)
		poly.Top().Add(Offset(*pp++));
}

void DrawGL::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
	if(vertex_count < 2 || IsNull(color))
		return;
	Vector<Vector<Pointf>> poly;
	while(--count_count >= 0) {
		const Point *pp = vertices;
		vertices += *counts++;
		DoPath(poly, pp, vertices);
	}
	
	GLVertexData data;
	ApplyDash(poly, width);
	GLPolylines(data, poly);
	if(width > 0)
		GLDrawPolylines(dd, Pointf(0, 0), data, Sizef(1, 1), width, color);
}

void DrawGL::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc,
                                   const int *disjunct_polygon_counts, int dpcc, Color color,
                                   int width, Color outline, uint64 pattern, Color doxor)
{
	Vector<Vector<Pointf>> poly;
	while(--dpcc >= 0) {
		const Point *sp = vertices;
		vertices += *disjunct_polygon_counts++;
		while(sp < vertices) {
			const Point *pp = sp;
			sp += *subpolygon_counts++;
			DoPath(poly, pp, sp);
		}
	}

	if(!IsNull(color)) {
		GLVertexData data;
		GLPolygons(data, poly);
		GLDrawPolygons(dd, Pointf(0, 0), data, Sizef(1, 1), color);
	}
	if(!IsNull(outline)) {
		GLVertexData data;
		for(auto& pl : poly)
			pl.Add(pl[0]);
		ApplyDash(poly, width);
		GLPolylines(data, poly);
		if(width > 0)
			GLDrawPolylines(dd, Pointf(0, 0), data, Sizef(1, 1), width, outline);
	}
}

};