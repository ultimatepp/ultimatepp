#include "Painter.h"

NAMESPACE_UPP

void Painter::OffsetOp(Point p)
{
	Begin();
	Translate(p.x, p.y);
}

void Painter::RectPath(int x, int y, int cx, int cy)
{
	Move(x, y).Line(x + cx, y).Line(x + cx, y + cy).Line(x, y + cy).Close();
}

void Painter::RectPath(const Rect& r)
{
	RectPath(r.left, r.top, r.GetWidth(), r.GetHeight());
}

bool Painter::ClipOp(const Rect& r)
{
	Begin();
	RectPath(r);
	Clip();
	return true;
}

bool Painter::ClipoffOp(const Rect& r)
{
	Begin();
	RectPath(r);
	Clip();
	Translate(r.left, r.top);
	return true;
}

bool Painter::ExcludeClipOp(const Rect& r)
{
	return true;
}

bool Painter::IntersectClipOp(const Rect& r)
{
	return true;
	RectPath(r);
	Clip();
	return true;
}

Rect Painter::GetClipOp() const
{
	return Rect(0, 0, 0, 0);
}

bool Painter::IsPaintingOp(const Rect& r) const
{
	return true;
}

void Painter::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	RectPath(x, y, cx, cy);
	Fill(color);
}

void Painter::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	// Color and src support!!!
	RectPath(x, y, cx, cy);
	Sizef sz = img.GetSize();

	Fill(img, Xform2D::Scale(cx / sz.cx, cy / sz.cy) * Xform2D::Translation(x, y));


//	Fill(img, Xform2D::Scale(sz.cx / cx, sz.cy / cy) * Xform2D::Translation(x, y));
}

void Painter::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	double h = width / 2;
	Move(x1 + h, y1 + h);
	Line(x2 + h, y2 + h);
	Stroke(max(width, 0), color);
}

void Painter::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts,
                                 int count_count, int width, Color color, Color doxor)
{
}

void Painter::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
                                  const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts,
                                  int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void Painter::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void Painter::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	Sizef sz = r.GetSize();
	Ellipse(r.left + sz.cx / 2, r.top + sz.cy / 2, sz.cx / 2, sz.cy / 2);
	Fill(color);
	Stroke(max(pen, 0), pencolor);
}

void Painter::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
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
	Text(x, y, text, font, n, ddx);
	Fill(ink);
	End();
}

void Painter::DrawPaintingOp(const Rect& target, const Painting& p)
{
	Size sz = target.GetSize();
	Sizef psz = p.GetSize();
	Begin();
	Translate(target.left, target.top);
	Scale(sz.cx / psz.cx, sz.cy / psz.cy);
	Paint(p);
	End();
}

END_UPP_NAMESPACE
