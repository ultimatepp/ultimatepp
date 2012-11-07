#include "GtkApp.h"

#include <cairo/cairo-ft.h>

void GtkDraw::SetColor(Color c)
{
	cairo_set_source_rgb(cr, c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

dword GtkDraw::GetInfo() const
{
	return 0;
}

Point GtkDraw::GetOffset() const
{
	return offset.GetCount() ? offset.Top() : Point(0, 0);
}

void GtkDraw::Push()
{
	cairo_save(cr);
	offset.Add(GetOffset());
}

void GtkDraw::Pop()
{
	if(offset.GetCount())
		offset.Drop();
	cairo_restore(cr);
}

void GtkDraw::BeginOp()
{
	Push();
}

void GtkDraw::EndOp()
{
	Pop();
}

void GtkDraw::OffsetOp(Point p)
{
	Push();
	offset.Top() += p;
	cairo_translate(cr, p.x, p.y);
}

void GtkDraw::RectPath(const Rect& r)
{
	cairo_rectangle(cr, r.left, r.top, r.GetWidth(), r.GetHeight());
}

bool GtkDraw::ClipOp(const Rect& r)
{
	Push();
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool GtkDraw::ClipoffOp(const Rect& r)
{
	Push();
	offset.Top() += r.TopLeft();
	RectPath(r);
	cairo_clip(cr);
	cairo_translate(cr, r.left, r.top);
	return true;
}

bool GtkDraw::ExcludeClipOp(const Rect& r)
{
	RectPath(Rect(-99999, -99999, 99999, r.top));
	RectPath(Rect(-99999, r.top, r.left, 99999));
	RectPath(Rect(r.right, r.top, 99999, 99999));
	RectPath(Rect(r.left, r.bottom, r.right, 99999));
	cairo_clip(cr);
	return true;
}

bool GtkDraw::IntersectClipOp(const Rect& r)
{
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool GtkDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

Rect GtkDraw::GetPaintRect() const
{
	return Rect(0, 0, INT_MAX, INT_MAX);
}

void GtkDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	SetColor(color);
	cairo_rectangle(cr, x, y, cx, cy);
	cairo_fill(cr);
}

void GtkDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	// TODO: cache cairo_surfaces
	Size isz = img.GetSize();
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cairo_surface_flush(surface);
	byte *a = (byte *)cairo_image_surface_get_data(surface);
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, isz.cx);
	for(int yy = 0; yy < isz.cy; yy++) {
		Copy((RGBA *)a, img[yy], isz.cx);
		a += stride;
	}
	cairo_surface_mark_dirty(surface);
	if(!IsNull(color)) {
		SetColor(color);
		cairo_mask_surface(cr, surface, x, y);
	}
	else {
		cairo_set_source_surface(cr, surface, x, y);
		cairo_paint(cr);
	}
	cairo_surface_destroy(surface);
}

void GtkDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
}

void GtkDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}

void GtkDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void GtkDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void GtkDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

namespace Upp {
FcPattern *CreateFcPattern(Font font);
};

void GtkDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	FcPattern *p = CreateFcPattern(font);
	cairo_font_face_t *face = cairo_ft_font_face_create_for_pattern(p);
	FcPatternDestroy(p);
	
	Buffer<cairo_glyph_t> gs(n);
	for(int i = 0; i < n; i++) {
		cairo_glyph_t& g = gs[i];
		g.index = GetGlyphInfo(font, text[i]).glyphi;
		g.x = x;
		g.y = y + font.GetAscent();
		x += dx ? dx[i] : font[text[i]];
	}
	
 	cairo_set_font_face(cr, face);
 	cairo_set_font_size(cr, font.GetHeight());
	SetColor(ink);
	DDUMP(y + font.GetAscent());
 	cairo_show_glyphs(cr, gs, n);
 	cairo_font_face_destroy(face);
}
