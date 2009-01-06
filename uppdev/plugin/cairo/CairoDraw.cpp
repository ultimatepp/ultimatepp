#include "CairoDraw.h"

NAMESPACE_UPP

void CairoDraw::BeginOp()
{
	_cairo.Save();
	
	_offsetStack.Add(actual_offset);
	_clipStack.Add(_actualClip);
}

void CairoDraw::EndOp()
{
	_cairo.Restore();
	
	actual_offset = _offsetStack.Pop();
	_actualClip = _clipStack.Pop();
}

void CairoDraw::OffsetOp(Point p)
{
	Begin();
	
	_cairo.Translate(p.x, p.y);
	
	actual_offset += p;
	_actualClip -= p;
}

bool CairoDraw::ClipOp(const Rect& r)
{
	Begin();
	
	_cairo.Rectangle(r.left, r.top, r.Width(), r.Height());
	_cairo.Clip();
	
	_actualClip &= r;
	return !_actualClip.IsEmpty();
}

bool CairoDraw::ClipoffOp(const Rect& r)
{
	Begin();
	
	_cairo.Translate(r.left, r.top);
	
	_cairo.Rectangle(0, 0, r.Width(), r.Height());
	_cairo.Clip();
	
	actual_offset += r.TopLeft();
	_actualClip &= r;
	_actualClip -= r.TopLeft();
	return !_actualClip.IsEmpty();
}

bool CairoDraw::ExcludeClipOp(const Rect& r)
{
	_cairo.MoveTo(0, 0);
	_cairo.LineTo(_cairo.GetWidth(), 0);
	_cairo.LineTo(_cairo.GetWidth(), _cairo.GetHeight());
	_cairo.LineTo(0, _cairo.GetHeight());
	
	_cairo.LineTo(0, r.bottom);
	_cairo.LineTo(r.right, r.bottom);
	_cairo.LineTo(r.right, r.top);
	_cairo.LineTo(r.left, r.top);
	_cairo.LineTo(r.left, r.bottom);
	
	_cairo.LineTo(0, r.bottom);
	_cairo.ClosePath();
	
	_cairo.Clip();
	
	return !_actualClip.IsEmpty();
}

bool CairoDraw::IntersectClipOp(const Rect& r)
{
	_cairo.Rectangle(r.left, r.top, r.Width(), r.Height());
	_cairo.Clip();
	
	_actualClip &= r;
	return !_actualClip.IsEmpty();
}

Rect CairoDraw::GetClipOp() const
{
	return _actualClip;
}

bool CairoDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}


void CairoDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	_cairo.Rectangle(x, y, cx, cy);
	_cairo.SetSourceRGB(color);
	_cairo.Fill();
}

void CairoDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	_cairo.Save();
	
	cairo_t *context = _cairo.GetContext();
	ASSERT(context != NULL);
	
	cairo_surface_t *imgSurface = cairo_image_surface_create_for_data((unsigned char *)&(img[0]->b),
																		CAIRO_FORMAT_ARGB32,
																		img.GetWidth(),
																		img.GetHeight(),
																		img.GetWidth() * 4);
	
	_cairo.Translate(x, y);
	_cairo.Scale((double)cx / img.GetWidth(), (double)cy / img.GetHeight());
	cairo_set_source_surface(context, imgSurface, 0, 0);
	cairo_rectangle(context, 0, 0, cx, cy); //todo: src rect
	_cairo.Fill();
	cairo_surface_destroy(imgSurface);
	
	_cairo.Restore();
}

void CairoDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	if (width == 0) width = 1;
	
	_cairo.SetLineCap(CAIRO_LINE_CAP_ROUND);
	_cairo.SetSourceRGB(color);
	_cairo.SetLineWidth(width);
	_cairo.MoveTo(x1 + 0.5, y1 + 0.5);
	_cairo.LineTo(x2 + 0.5, y2 + 0.5);
	_cairo.Stroke();
}

void CairoDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                                   const int *counts, int count_count,
                                   int width, Color color, Color doxor)
{
	if (width == 0) width = 1;
	
	for (int i = 0; i < count_count; i++)
	{
		ASSERT(counts[i] >= 1);
		_cairo.MoveTo(vertices->x + 0.5, vertices->y + 0.5);
		vertices++;
		
		for (int j = 1; j < counts[i]; j++, vertices++)
			_cairo.LineTo(vertices->x + 0.5, vertices->y + 0.5);
	}
	
	_cairo.SetLineCap(CAIRO_LINE_CAP_ROUND);
	_cairo.SetLineJoin(CAIRO_LINE_JOIN_ROUND);
	_cairo.SetLineWidth(width);
	_cairo.SetSourceRGB(color);
	_cairo.Stroke();
}

void CairoDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
                                      const int *subpolygon_counts, int scc,
                                      const int *disjunct_polygon_counts, int dpcc,
                                      Color color, int width, Color outline,
                                      uint64 pattern, Color doxor)
{
	if (width == 0) width = 1;
	
	for (int i = 0; i < scc; i++)
	{
		ASSERT(subpolygon_counts[i] >= 1);
		_cairo.MoveTo(vertices->x, vertices->y);
		vertices++;
		
		for (int j = 1; j < subpolygon_counts[i]; j++, vertices++)
			_cairo.LineTo(vertices->x, vertices->y);
		
		_cairo.ClosePath();
	}
	
	_cairo.SetSourceRGB(color);
	_cairo.Fill(true);
	_cairo.SetLineCap(CAIRO_LINE_CAP_ROUND);
	_cairo.SetLineJoin(CAIRO_LINE_JOIN_ROUND);
	_cairo.SetLineWidth(width);
	_cairo.SetSourceRGB(outline);
	_cairo.Stroke();
}

void CairoDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	if (pen == 0) pen = 1;
	
	_cairo.Save();
	_cairo.Translate(r.left + (r.Width() / 2.0), r.top + (r.Height() / 2.0));
	_cairo.Scale(r.Width() / 2.0, r.Height() / 2.0);
	_cairo.Arc(0.0, 0.0, 1.0, 0.0, 2 * M_PI);
	_cairo.Restore();
	
	_cairo.SetSourceRGB(color);
	
	_cairo.Fill(pen != (int)Null);
	
	if (pen != (int)Null)
	{
		_cairo.SetLineWidth(pen);
		_cairo.SetSourceRGB(pencolor);
		_cairo.Stroke();
	}
}

void CairoDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	if (width == 0) width = 1;
	
	//TODO
}

void CairoDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
                           Color ink, int n, const int *dx)
{
	cairo_t *context = _cairo.GetContext();
	ASSERT(context != NULL);
	
	/*HFONT hFont = CreateFont(font.GetHeight() ? -abs(font.GetHeight()) : -12,
		                     font.GetWidth(), angle, angle, font.IsBold() ? FW_BOLD : FW_NORMAL,
		                     font.IsItalic(), font.IsUnderline(), font.IsStrikeout(),
	                         Font::GetFaceInfo(font.GetFace()) & Font::SYMBOLTYPE ? SYMBOL_CHARSET : DEFAULT_CHARSET,
	                         font.IsTrueTypeOnly() ? OUT_TT_ONLY_PRECIS : OUT_DEFAULT_PRECIS,
	                         CLIP_DEFAULT_PRECIS,
	                         font.IsNonAntiAliased() ? NONANTIALIASED_QUALITY
						                          : DEFAULT_QUALITY,
	                         DEFAULT_PITCH|FF_DONTCARE,
	                         Font::GetFaceName(font.GetFace())/*sFontFace().GetKey(font.GetFace())* /);
	
	cairo_font_face_t *fontFace = cairo_win32_font_face_create_for_hfont(hFont);
	cairo_set_font_face(context, fontFace);*/
	cairo_set_font_size(context, font.GetHeight());
	
	_cairo.Save();
	
	_cairo.Translate(x, y + (font.GetHeight() ? abs(font.GetHeight()) : 12));
	_cairo.Rotate(angle * (-3.14159 / 1800.0));
	
	_cairo.SetSourceRGB(ink);
	WString tempWString = text;
	cairo_show_text(context, tempWString.ToString());
	
	_cairo.NewPath();
	
	_cairo.Restore();
	
	/*cairo_font_face_destroy(fontFace);*/
}

END_UPP_NAMESPACE

