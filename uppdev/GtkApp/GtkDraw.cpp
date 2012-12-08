#include "GtkApp.h"

#include <cairo/cairo-ft.h>

#define LLOG(x)

void CairoDraw::SetColor(Color c)
{
	cairo_set_source_rgb(cr, c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

dword CairoDraw::GetInfo() const
{
	return DRAWTEXTLINES;
}

Point CairoDraw::GetOffset() const
{
	return offset.GetCount() ? offset.Top() : Point(0, 0);
}

void CairoDraw::Push()
{
	cairo_save(cr);
	offset.Add(GetOffset());
}

void CairoDraw::Pop()
{
	if(offset.GetCount())
		offset.Drop();
	cairo_restore(cr);
}

void CairoDraw::BeginOp()
{
	Push();
}

void CairoDraw::EndOp()
{
	Pop();
}

void CairoDraw::OffsetOp(Point p)
{
	Push();
	offset.Top() += p;
	cairo_translate(cr, p.x, p.y);
}

void CairoDraw::RectPath(const Rect& r)
{
	cairo_rectangle(cr, r.left, r.top, r.GetWidth(), r.GetHeight());
}

bool CairoDraw::ClipOp(const Rect& r)
{
	Push();
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool CairoDraw::ClipoffOp(const Rect& r)
{
	Push();
	offset.Top() += r.TopLeft();
	RectPath(r);
	cairo_clip(cr);
	cairo_translate(cr, r.left, r.top);
	return true;
}

bool CairoDraw::ExcludeClipOp(const Rect& r)
{
	RectPath(Rect(-99999, -99999, 99999, r.top));
	RectPath(Rect(-99999, r.top, r.left, 99999));
	RectPath(Rect(r.right, r.top, 99999, 99999));
	RectPath(Rect(r.left, r.bottom, r.right, 99999));
	cairo_clip(cr);
	return true;
}

bool CairoDraw::IntersectClipOp(const Rect& r)
{
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool CairoDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

Rect CairoDraw::GetPaintRect() const
{
	return Rect(0, 0, INT_MAX, INT_MAX);
}

void CairoDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	SetColor(color);
	cairo_rectangle(cr, x, y, cx, cy);
	cairo_fill(cr);
}

struct ImageSysData {
	Image            img;
	cairo_surface_t *surface;
	
	void Init(const Image& img);
	~ImageSysData();
};

void ImageSysData::Init(const Image& m)
{
	img = m;
	Size isz = img.GetSize();
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cairo_surface_flush(surface);
	byte *a = (byte *)cairo_image_surface_get_data(surface);
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, isz.cx);
	for(int yy = 0; yy < isz.cy; yy++) {
		Copy((RGBA *)a, img[yy], isz.cx);
		a += stride;
	}
	cairo_surface_mark_dirty(surface);
	SysImageRealized(img);
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	cairo_surface_destroy(surface);
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Maker {
	Image img;

	virtual int64  Key() const                      { return img.GetSerialId(); }
	virtual int    Make(ImageSysData& object) const { object.Init(img); return img.GetLength(); }
};

void CairoDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	GuiLock __;
	if(img.GetLength() == 0)
		return;
	LLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << img.GetSize());
	ImageSysDataMaker m;
	static LRUCache<ImageSysData, int64> cache;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = img;
	ImageSysData& sd = cache.Get(m);
	if(!IsNull(color)) {
		SetColor(color);
		cairo_mask_surface(cr, sd.surface, x, y);
	}
	else {
		cairo_set_source_surface(cr, sd.surface, x, y);
		cairo_paint(cr);
	}
	cache.Shrink(4 * 1024 * 768, 1000); // Cache must be after Paint because of PaintOnly!
}

void CairoDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	SetColor(color);
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_set_line_width (cr, width);
	cairo_stroke(cr);
}

void CairoDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}

void CairoDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void CairoDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void CairoDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

namespace Upp {
FcPattern *CreateFcPattern(Font font);
FT_Face    FTFace(Font fnt, String *rpath = NULL);
};

struct FontSysData {
	cairo_scaled_font_t *scaled_font;
	
	void Init(Font font, int angle);
	~FontSysData() { cairo_scaled_font_destroy(scaled_font); }
};

void FontSysData::Init(Font font, int angle)
{
	DLOG("FontSysData::Init " << font << ", " << angle);
	FcPattern *p = CreateFcPattern(font);
	cairo_font_face_t *font_face = cairo_ft_font_face_create_for_pattern(p);
	FcPatternDestroy(p);
	
	cairo_matrix_t font_matrix[1], ctm[1];
	cairo_matrix_init_identity(ctm);
	cairo_matrix_init_scale(font_matrix, font.GetHeight(), font.GetHeight());
	if(angle)
		cairo_matrix_rotate(font_matrix, -angle * M_2PI / 3600);
	cairo_font_options_t *opt = cairo_font_options_create();
	scaled_font = cairo_scaled_font_create(font_face, font_matrix, ctm, opt);

	cairo_font_options_destroy(opt);
 	cairo_font_face_destroy(font_face);
}

struct FontDataSysMaker : LRUCache<FontSysData, Tuple2<Font, int> >::Maker {
	Font font;
	int  angle;

	virtual Tuple2<Font, int>  Key() const        { return MakeTuple(font, angle); }
	virtual int Make(FontSysData& object) const   { object.Init(font, angle); return 1; }
};

void CairoDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	GuiLock __;
	
	int ascent = font.GetAscent();
	double sina = 0;
	double cosa = 1;
	if(angle)
		Draw::SinCos(angle, sina, cosa);
	int xpos = 0;	
	Buffer<cairo_glyph_t> gs(n);
	for(int i = 0; i < n; i++) {
		cairo_glyph_t& g = gs[i];
		g.index = GetGlyphInfo(font, text[i]).glyphi;
		g.x = fround(x + cosa * xpos + sina * ascent);
		g.y = fround(y + cosa * ascent - sina * xpos);
		xpos += dx ? dx[i] : font[text[i]];
	}

	static LRUCache<FontSysData, Tuple2<Font, int> > cache;
	FontDataSysMaker m;
	m.font = font;
	m.angle = angle;
	FontSysData& sf = cache.Get(m);
	
	cairo_set_scaled_font(cr, sf.scaled_font);

	SetColor(ink);
 	cairo_show_glyphs(cr, gs, n);
 	
 	cache.Shrink(64);
}
