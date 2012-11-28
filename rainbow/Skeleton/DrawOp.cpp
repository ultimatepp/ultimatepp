#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

NAMESPACE_UPP

#define LLOG(x)      // LOG(x)
#define LTIMING(x)   // RTIMING(x)

void SystemDraw::BeginOp()
{
	LTIMING("Begin");
	GuiLock __;
}

void SystemDraw::OffsetOp(Point p)
{
	GuiLock __;
}

bool SystemDraw::ClipOp(const Rect& r)
{
	GuiLock __;
	return true;
}

bool SystemDraw::ClipoffOp(const Rect& r)
{
	GuiLock __;
	return true;
}

void SystemDraw::EndOp()
{
	GuiLock __;
}

bool SystemDraw::ExcludeClipOp(const Rect& r)
{
	GuiLock __;
	return true;
}

bool SystemDraw::IntersectClipOp(const Rect& r)
{
	GuiLock __;
	return true;
}

bool SystemDraw::IsPaintingOp(const Rect& r) const
{
	GuiLock __;
	return true;
}

Rect SystemDraw::GetPaintRect() const
{
	GuiLock __;
	return Rect(0, 0, 10000, 1000);
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	GuiLock __;
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	GuiLock __;
}

void SystemDraw::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	GuiLock __;
}

void SystemDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                            const int *counts, int count_count,
	                        int width, Color color, Color doxor)
{
	GuiLock __;
}

void SystemDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	GuiLock __;
}

void SystemDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	GuiLock __;
}

void SystemDraw::DrawEllipseOp(const Rect& r, Color color, int width, Color pencolor)
{
	GuiLock __;
}

END_UPP_NAMESPACE

#endif
