#include "GLDraw.h"

namespace Upp {

void DrawGL::Init(Size sz, double alpha)
{
	Cloff& c = cloff.Add();
	c.clip = view_size = sz;
	c.offset = Pointf(0, 0);
	dd.Set(sz);
	dd.alpha = alpha;
	glEnable(GL_SCISSOR_TEST);
	scissor = Null;
	SyncScissor();
	prev = Point(0, 0);
	path_done = false;
}

DrawGL::~DrawGL()
{
	Flush();
	glDisable(GL_SCISSOR_TEST);
}

dword DrawGL::GetInfo() const
{
	return DRAWTEXTLINES;
}

void DrawGL::Push()
{
	auto& s = state.Add();
	s.dash = clone(dash);
	s.dash_start = clone(dash_start);
	s.alpha = dd.alpha;
}

void DrawGL::BeginOp()
{
	Flush();
	Cloff c = cloff.Top();
	cloff.Add(c);
	Push();
}

bool DrawGL::ClipOp(const Rect& r)
{
	Cloff c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = c.clip & (r + c.offset);
	c1.offset = c.offset;
	SyncScissor();
	Push();
	return !c1.clip.IsEmpty();
}

bool DrawGL::ClipoffOp(const Rect& r)
{
	Cloff c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = c.clip & (r + c.offset);
	c1.offset = c.offset + (Pointf)r.TopLeft();
	SyncScissor();
	Push();
	return !c1.clip.IsEmpty();
}

bool DrawGL::IntersectClipOp(const Rect& r)
{
	Cloff& c = cloff.Top();
	c.clip = c.clip & (r + c.offset);
	SyncScissor();
	Push();
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
	Push();
}

void DrawGL::EndOp()
{
	ASSERT(cloff.GetCount());
	if(cloff.GetCount())
		cloff.Drop();
	if(state.GetCount()) {
		auto& s = state.Top();
		dash = pick(s.dash);
		dash_start = s.dash_start;
		dd.alpha = s.alpha;
		state.Drop();
	}
	SyncScissor();
}

void DrawGL::SyncScissor()
{
	GL_TIMING("SyncScissor");
	Rect clip = cloff.Top().clip;
	if(clip != scissor) {
		Flush();
		Size sz = clip.GetSize();
		glScissor(clip.left, view_size.cy - sz.cy - clip.top, sz.cx, sz.cy);
	}
}

Pointf DrawGL::Off(int x, int y)
{
	Pointf o = cloff.Top().offset;
	return Pointf(x + o.x, y + o.y);
}

Rectf DrawGL::Off(int x, int y, int cx, int cy)
{
	Point o = cloff.Top().offset;
	return RectfC(x + o.x, y + o.y, cx, cy);
}

Rectf DrawGL::Off(int x, int y, Size sz)
{
	return Off(x, y, sz.cx, sz.cy);
}

void DrawGL::DrawImageOp(int x, int y, int cx, int cy, const Image& image, const Rect& src, Color color)
{
	Flush();
	GLDrawImage(dd, Off(x, y, cx, cy), IsNull(color) ? image : CachedSetColorKeepAlpha(image, color),
	            src);
}

void DrawGL::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	Flush();
	GLDrawText(dd, Off(x, y), angle * M_2PI / 3600, text, font, ink, n, dx);
}

void DrawGL::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	Point off = cloff.Top().offset;
	int a = Vertex(x + off.x, y + off.y, color, dd.alpha);
	int b = Vertex(x + off.x + cx, y + off.y, color, dd.alpha);
	int c = Vertex(x + off.x + cx, y + off.y + cy, color, dd.alpha);
	int d = Vertex(x + off.x, y + off.y + cy, color, dd.alpha);

	Triangle(a, b, c);
	Triangle(a, c, d);
}

const Vector<double>& DrawGL::GetDash(int& width)
{
	static Vector<double> nodash;
	static Vector<double> dash = { 18, 6 };
	static Vector<double> dot = { 3, 3 };
	static Vector<double> dashdot = { 9, 6, 3, 6 };
	static Vector<double> dashdotdot = { 9, 3, 3, 3, 3, 3 };

	if(width == 0)
		width = 1;
	if(width > 0)
		return nodash;
	if(width == PEN_NULL || IsNull(width)) {
		width = 0;
		return nodash;
	}
	int w = width;
	width = 1;
	return *decode(w, PEN_DASH, &dash,
	                  PEN_DOT, &dot,
	                  PEN_DASHDOT, &dashdot,
	                  PEN_DASHDOTDOT, &dashdotdot,
	                  &nodash);
}

void DrawGL::ApplyDash(Vector<Vector<Pointf>>& polyline, int& width)
{
	const Vector<double>& dash = GetDash(width);
	GetDash(width);
	if(dash.GetCount()) {
		Vector<Vector<Pointf>> r;
		for(auto& l : polyline)
			DashPolyline(r, l, dash);
		polyline = pick(r);
	}
}

void DrawGL::DoDrawPolylines(Vector<Vector<Pointf>>& poly, int width, Color color, bool close)
{
	ApplyDash(poly, width);
	for(const auto& p : poly)
		Polyline(*this, p, width, color, dd.alpha, close);
}

void DrawGL::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	Vector<Vector<Pointf>> poly;
	poly.Add().Add(Off(x1, y1));
	poly.Top().Add(Off(x2, y2));
	DoDrawPolylines(poly, width, color);
}

void DrawGL::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	GL_TIMING("DrawGL::DrawArcOp");
	Vector<Vector<Pointf>> poly;
	GLArc(poly, rc, start, end);
	DoDrawPolylines(poly, width, color);
}

void DrawGL::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	// TODO: Dash, ellipse stroke
	if(!r.IsEmpty())
		Upp::Ellipse(*this, Pointf(r.CenterPoint()) + cloff.Top().offset,
		             Sizef(r.GetSize()) / 2, color, pen, pencolor, dd.alpha);
}

void DrawGL::DoPath(Vector<Vector<Pointf>>& poly, const Point *pp, const Point *end)
{
	poly.Add().Add(Off(*pp++));
	while(pp < end)
		poly.Top().Add(Off(*pp++));
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
	
	DoDrawPolylines(poly, width, color);
}

extern int sTesselateCounter;

void DrawGL::DoDrawPolygons(const Vector<Vector<Pointf>>& path, Color color)
{
	const int TESS_LIMIT = 200;
	int n = 0;
	for(const auto& p : path) {
		n += p.GetCount();
		if(n > TESS_LIMIT) {
			Flush();
			GLVertexData data;
			GLPolygons(data, path);
			GLDrawPolygons(dd, Pointf(0, 0), data, Sizef(1, 1), color);
			return;
		}
	}
	Vector<Pointf> vertex;
	Vector<Tuple<int, int, int>> triangle;
	sTesselateCounter++;
	Tesselate(path, vertex, triangle, false);
	int ii0;
	for(int i = 0; i < vertex.GetCount(); i++) {
		int q = Vertex(vertex[i], color, dd.alpha);
		if(i == 0) ii0 = q;
	}
	for(const auto& t : triangle)
		Triangle(t.a + ii0, t.b + ii0, t.c + ii0);
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

	if(poly.GetCount() == 0)
		return;

	if(!IsNull(color))
		DoDrawPolygons(poly, color);

	if(!IsNull(outline))
		DoDrawPolylines(poly, width, outline, true);
}


void DrawGL::Flush()
{
	GLTriangles::Draw(dd);
	GLTriangles::Clear();
}

};
