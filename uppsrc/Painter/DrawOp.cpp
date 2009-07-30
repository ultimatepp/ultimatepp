#include "Painter.h"

NAMESPACE_UPP

dword Painter::GetInfo() const
{
	return DOTS;
}

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
	while(--count_count >= 0) {
		const Point *lp = vertices;
		vertices += *counts++;
		Move(*lp);
		while(++lp < vertices)
			Line(*lp);
		Stroke(max(width, 0), color);
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
			Stroke(max(width, 0), outline);
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
	Stroke(max(width, 0), color);
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

void ImageAnyDrawPainter(Draw *(*f)(Size sz), Image (*e)(Draw *w));

static Draw *sCP(Size sz)
{
	return new ImagePainter(sz);
}

static Image sEP(Draw *w)
{
	ImagePainter *ip = dynamic_cast<ImagePainter *>(w);
	return ip ? (Image)(*ip) : Image();
}

INITBLOCK {
	ImageAnyDrawPainter(sCP, sEP);
}

END_UPP_NAMESPACE
