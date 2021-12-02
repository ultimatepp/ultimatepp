#include "Painter.h"

namespace Upp {

dword Painter::GetInfo() const
{
	return DOTS;
}

void Painter::OffsetOp(Point p)
{
	Begin();
	Translate(p.x, p.y);
}

Painter& Painter::RectPath(int x, int y, int cx, int cy)
{
	Move(x, y).Line(x + cx, y).Line(x + cx, y + cy).Line(x, y + cy).Close();
	return *this;
}

Painter& Painter::RectPath(const Rect& r)
{
	RectPath(r.left, r.top, r.GetWidth(), r.GetHeight());
	return *this;
}

Painter& Painter::RectPath(double x, double y, double cx, double cy)
{
	Move(x, y).Line(x + cx, y).Line(x + cx, y + cy).Line(x, y + cy).Close();
	return *this;
}

Painter& Painter::RectPath(const Rectf& r)
{
	RectPath(r.left, r.top, r.GetWidth(), r.GetHeight());
	return *this;
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
	RectPath(Rect(-99999, -99999, 99999, r.top));
	RectPath(Rect(-99999, r.top, r.left, 99999));
	RectPath(Rect(r.right, r.top, 99999, 99999));
	RectPath(Rect(r.left, r.bottom, r.right, 99999));
	Clip();
	return true;
}

bool Painter::IntersectClipOp(const Rect& r)
{
	RectPath(r);
	Clip();
	return true;
}

bool Painter::IsPaintingOp(const Rect& r) const
{
	return true;
}

void Painter::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	RectPath(x, y, cx, cy);
	if(color == InvertColor)
		Invert();
	Fill(color);
}

void Painter::DrawImageOp(int x, int y, int cx, int cy, const Image& image, const Rect& src, Color color)
{
	if(src.GetWidth() * src.GetHeight() == 0)
		return;
	Image img = IsNull(color) ? image : SetColorKeepAlpha(image, color);
	RectPath(x, y, cx, cy);
	double sw = (double)cx / src.GetWidth();
	double sh = (double)cy / src.GetHeight();
	Fill(img, Xform2D::Scale(sw, sh) * Xform2D::Translation(x - sw * src.left, y - sh * src.top));
}

void Painter::DrawLineStroke(int width, Color color)
{
	if(IsNull(width) || IsNull(color))
		return;
	Begin();
	LineCap(LINECAP_ROUND);
	switch(width) {
	case PEN_NULL:
		Stroke(0, color);
		End();
		return;
	case PEN_SOLID:
		Stroke(1, color);
		break;
	case PEN_DASH:
		Dash("18 6");
		break;
	case PEN_DOT:
		Dash("3 3");
		break;
	case PEN_DASHDOT:
		Dash("9 6 3 6");
		break;
	case PEN_DASHDOTDOT:
		Dash("9 3 3 3 3 3");
		break;
	default:
		Stroke(width == 0 ? 1 : width, color);
		End();
		return;
	}
	Stroke(1, color);
	End();
}

void Painter::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	Move(x1, y1);
	Line(x2, y2);
	DrawLineStroke(width, color);
}

void Painter::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts,
                                 int count_count, int width, Color color, Color doxor)
{
	while(--count_count >= 0) {
		const Point *lp = vertices;
		vertices += *counts++;
		Move(*lp);
		while(++lp < vertices)
			Line(*lp);
		DrawLineStroke(width, color);
	}
}

void Painter::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
                                  const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts,
                                  int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	Image fill_img;
	if(pattern && !IsNull(color)) {
		ImageBuffer ibuf(8, 8);
		RGBA r[2] = { color, RGBAZero() };
		for(RGBA *out = ibuf, *end = out + 64; out < end; pattern >>= 1)
			*out++ = r[(byte)pattern & 1];
		fill_img = ibuf;
	}
	
	while(--dpcc >= 0) {
		const Point *sp = vertices;
		vertices += *disjunct_polygon_counts++;
		while(sp < vertices) {
			const Point *pp = sp;
			sp += *subpolygon_counts++;
			Move(*pp);
			while(++pp < sp)
				Line(*pp);
			Close();
		}
		if(!IsNull(fill_img))
			Fill(fill_img, Xform2D::Identity(), FILL_HREPEAT | FILL_VREPEAT);
		else if(!IsNull(color))
			Fill(color);
		if(!IsNull(outline))
			DrawLineStroke(width, outline);
	}
}

void Painter::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	if(rc.Width() <= 0 || rc.Height() <= 0)
		return;
	Sizef radius = Sizef(rc.Size()) / 2.0;
	Pointf center = Pointf(rc.TopLeft()) + radius;
	double ang1 = Bearing((Pointf(start) - center) / radius);
	double ang2 = Bearing((Pointf(end) - center) / radius);
	double sweep = ang1 - ang2;
	if(sweep <= 0)
		sweep += 2 * M_PI;
	Move(center.x + radius.cx * cos(ang1), center.y + radius.cy * sin(ang1));
	Arc(center, radius.cx, radius.cy, ang1, -sweep);
	DrawLineStroke(width, color);
}

void Painter::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	Sizef sz = r.GetSize();
	Ellipse(r.left + sz.cx / 2, r.top + sz.cy / 2, sz.cx / 2, sz.cy / 2);
	Fill(color);
	DrawLineStroke(pen, pencolor);
}

void Painter::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	Begin();
	EvenOdd(true);
	Translate(x, y);
	if(angle)
		Rotate(-angle * M_2PI / 3600);
	if(n < 0)
		n = strlen__(text);
	double *ddx = NULL;
	Buffer<double> h;
	if(dx) {
		h.Alloc(n);
		ddx = h;
		for(int i = 0; i < n; i++)
			ddx[i] = dx[i];
	}
	Text(0, 0, text, font, n, ddx);
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

}
