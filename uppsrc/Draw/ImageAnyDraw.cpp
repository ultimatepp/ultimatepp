#include "Draw.h"

namespace Upp {

dword ImageAnyDraw::GetInfo() const
{
	return draw->GetInfo();
}

Size ImageAnyDraw::GetPageSize() const
{
	return draw->GetPageSize();
}

void ImageAnyDraw::BeginOp()
{
	return draw->BeginOp();
}

void ImageAnyDraw::EndOp()
{
	return draw->EndOp();
}

void ImageAnyDraw::OffsetOp(Point p)
{
	return draw->OffsetOp(p);
}

bool ImageAnyDraw::ClipOp(const Rect& r)
{
	return draw->ClipOp(r);
}

bool ImageAnyDraw::ClipoffOp(const Rect& r)
{
	return draw->ClipoffOp(r);
}

bool ImageAnyDraw::ExcludeClipOp(const Rect& r)
{
	return draw->ExcludeClipOp(r);
}

bool ImageAnyDraw::IntersectClipOp(const Rect& r)
{
	return draw->IntersectClipOp(r);
}

bool ImageAnyDraw::IsPaintingOp(const Rect& r) const
{
	return draw->IsPaintingOp(r);
}

Rect ImageAnyDraw::GetPaintRect() const
{
	return draw->GetPaintRect();
}

void ImageAnyDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	draw->DrawRectOp(x, y, cx, cy, color);
}

void ImageAnyDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	draw->DrawImageOp(x, y, cx, cy, img, src, color);
}

void ImageAnyDraw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
	draw->DrawDataOp(x, y, cx, cy, data, id);
}

void ImageAnyDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	draw->DrawLineOp(x1, y1, x2, y2, width, color);
}

void ImageAnyDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                                      const int *counts, int count_count, int width,
                                      Color color, Color doxor)
{
	draw->DrawPolyPolylineOp(vertices, vertex_count, counts, count_count, width, color, doxor);
}

void ImageAnyDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	draw->DrawPolyPolyPolygonOp(vertices, vertex_count, subpolygon_counts, scc,
	                            disjunct_polygon_counts, dpcc, color, width, outline,
	                            pattern, doxor);
}

void ImageAnyDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	draw->DrawArcOp(rc, start, end, width, color);
}

void ImageAnyDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	draw->DrawEllipseOp(r, color, pen, pencolor);
}

void ImageAnyDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                              int n, const int *dx)
{
	draw->DrawTextOp(x, y, angle, text, font, ink, n, dx);
}

void ImageAnyDraw::DrawDrawingOp(const Rect& target, const Drawing& w)
{
	draw->DrawDrawingOp(target, w);
}

void ImageAnyDraw::DrawPaintingOp(const Rect& target, const Painting& w)
{
	draw->DrawPaintingOp(target, w);
}

Draw *(*sCreateImageDraw)(Size sz);
Image (*sExtractImageDraw)(Draw *w);

void ImageAnyDrawPainter(Draw *(*f)(Size sz), Image (*e)(Draw *w))
{
	sCreateImageDraw = f;
	sExtractImageDraw = e;
}

void ImageAnyDrawSystem(Draw *(*f)(Size sz), Image (*e)(Draw *w))
{
	if(!sCreateImageDraw) {
		sCreateImageDraw = f;
		sExtractImageDraw = e;
	}
}

bool ImageAnyDraw::IsAvailable()
{
	return sCreateImageDraw;
}

void ImageAnyDraw::Init(Size sz)
{
	ASSERT(sCreateImageDraw);
	draw = (*sCreateImageDraw)(sz);
	ASSERT(draw);
}

ImageAnyDraw::operator Image() const
{
	return (*sExtractImageDraw)(draw);
}

ImageAnyDraw::ImageAnyDraw(Size sz)
{
	Init(sz);
}

ImageAnyDraw::ImageAnyDraw(int cx, int cy)
{
	Init(Size(cx, cy));
}

ImageAnyDraw::~ImageAnyDraw()
{
	delete draw;
}

}
