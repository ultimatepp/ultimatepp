#include "CtrlCore.h"

#ifdef GUI_WIN

namespace Upp {

#define LLOG(x)      // LOG(x)
#define LTIMING(x)   // RTIMING(x)

void SystemDraw::BeginOp()
{
	LTIMING("Begin");
	GuiLock __;
	Cloff& w = cloff.Add();
	w.org = actual_offset;
	w.drawingclip = drawingclip;
	w.hrgn = CreateRectRgn(0, 0, 0, 0);
	ASSERT(w.hrgn);
	int	q = ::GetClipRgn(handle, w.hrgn);
	ASSERT(q >= 0);
	if(q == 0) {
		DeleteObject(w.hrgn);
		w.hrgn = NULL;
	}
}

void SystemDraw::OffsetOp(Point p)
{
	GuiLock __;
	Begin();
	actual_offset += p;
	drawingclip -= p;
	LTIMING("Offset");
	SetOrg();
}

bool SystemDraw::ClipOp(const Rect& r)
{
	GuiLock __;
	Begin();
	LTIMING("Clip");
	return IntersectClip(r);
}

bool SystemDraw::ClipoffOp(const Rect& r)
{
	GuiLock __;
	Begin();
	LTIMING("Clipoff");
	LLOG("ClipoffOp " << r << ", GetClip() = " << GetClip() << ", actual_offset = " << actual_offset);
	actual_offset += r.TopLeft();
	bool q = IntersectClip(r);
	drawingclip -= r.TopLeft();
	SetOrg();
	LLOG("//ClipoffOp, GetClip() = " << GetClip() << ", actual_offset = " << actual_offset);
	return q;
}

void SystemDraw::EndOp()
{
	GuiLock __;
	LTIMING("End");
	ASSERT(cloff.GetCount());
	Cloff& w = cloff.Top();
	actual_offset = w.org;
	drawingclip = w.drawingclip;
	::SelectClipRgn(handle, w.hrgn);
	SetOrg();
	if(w.hrgn)
		::DeleteObject(w.hrgn);
	cloff.Drop();
}

bool SystemDraw::ExcludeClipOp(const Rect& r)
{
	GuiLock __;
#ifdef PLATFORM_WINCE
	int q = ExcludeClipRect(handle, r.left, r.top, r.right, r.bottom);
#else
	LTIMING("ExcludeClip");
	if(r.Contains(drawingclip))
		drawingclip = Rect(0, 0, 0, 0);
	Rect rr = LPtoDP(r);
	HRGN hrgn = ::CreateRectRgnIndirect(rr);
	int q = ::ExtSelectClipRgn(handle, hrgn, RGN_DIFF);
	ASSERT(q != ERROR);
	::DeleteObject(hrgn);
#endif
	return q == SIMPLEREGION || q == COMPLEXREGION;
}

bool SystemDraw::IntersectClipOp(const Rect& r)
{
	GuiLock __;
#ifdef PLATFORM_WINCE
	int q = IntersectClipRect(handle, r.left, r.top, r.right, r.bottom);
#else
	LTIMING("Intersect");
	drawingclip &= r;
	Rect rr = LPtoDP(r);
	HRGN hrgn = ::CreateRectRgnIndirect(rr);
	int q = ::ExtSelectClipRgn(handle, hrgn, RGN_AND);
	ASSERT(q != ERROR);
	::DeleteObject(hrgn);
#endif
	return q == SIMPLEREGION || q == COMPLEXREGION;
}

bool SystemDraw::IsPaintingOp(const Rect& r) const
{
	GuiLock __;
	LTIMING("IsPainting");
	return ::RectVisible(handle, r);
}

Rect SystemDraw::GetPaintRect() const
{
	GuiLock __;
	LTIMING("GetPaintRect");
	return drawingclip;
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	GuiLock __;
	LTIMING("DrawRect");
	LLOG("DrawRect " << RectC(x, y, cx, cy) << ": " << color);
	if(IsNull(color)) return;
	if(cx <= 0 || cy <= 0) return;
	if(color == InvertColor)
		::PatBlt(handle, x, y, cx, cy, DSTINVERT);
	else {
		SetColor(color);
		::PatBlt(handle, x, y, cx, cy, PATCOPY);
	}
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	GuiLock __;
	if(IsNull(width) || IsNull(color)) return;
	SetDrawPen(width, color);
	::MoveToEx(handle, x1, y1, NULL);
	::LineTo(handle, x2, y2);
}

#ifndef PLATFORM_WINCE

void SystemDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                            const int *counts, int count_count,
	                        int width, Color color, Color doxor)
{
	GuiLock __;
	ASSERT(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || IsNull(color) || IsNull(width))
		return;
	bool is_xor = !IsNull(doxor);
	if(is_xor)
		color = Color(color.GetR() ^ doxor.GetR(), color.GetG() ^ doxor.GetG(), color.GetB() ^ doxor.GetB());
	if(is_xor)
		SetROP2(GetHandle(), R2_XORPEN);
	SetDrawPen(width, color);
	if(count_count == 1)
		::Polyline(GetHandle(), (const POINT *)vertices, vertex_count);
	else
		::PolyPolyline(GetHandle(), (const POINT *)vertices,
		               (const dword *)counts, count_count);
	if(is_xor)
		SetROP2(GetHandle(), R2_COPYPEN);
}

static void DrawPolyPolyPolygonRaw(
	SystemDraw& draw, const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count)
{
	GuiLock __;
	for(int i = 0; i < disjunct_polygon_count_count; i++, disjunct_polygon_counts++)
	{
		int poly = *disjunct_polygon_counts;
		int sub = 1;
		if(*subpolygon_counts < poly) {
			if(disjunct_polygon_count_count > 1)
			{
				const int *se = subpolygon_counts;
				int total = 0;
				while(total < poly)
					total += *se++;
				sub = (int)(se - subpolygon_counts);
			}
			else
				sub = subpolygon_count_count;
		}
		ASSERT(sizeof(POINT) == sizeof(Point)); // modify algorithm when not
		if(sub == 1)
			Polygon(draw, (const POINT *)vertices, poly);
		else
			PolyPolygon(draw, (const POINT *)vertices, subpolygon_counts, sub);
		vertices += poly;
		subpolygon_counts += sub;
	}
}

void SystemDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	GuiLock __;
	if(vertex_count == 0)
		return;
	bool is_xor = !IsNull(doxor);
	HDC hdc = GetHandle();
	if(pattern) {
		int old_rop = GetROP2(hdc);
		HGDIOBJ old_brush = GetCurrentObject(hdc, OBJ_BRUSH);
		word wpat[8] = {
			(byte)(pattern >> 56), (byte)(pattern >> 48), (byte)(pattern >> 40), (byte)(pattern >> 32),
			(byte)(pattern >> 24), (byte)(pattern >> 16), (byte)(pattern >> 8), (byte)(pattern >> 0),
		};
		HBITMAP bitmap = CreateBitmap(8, 8, 1, 1, wpat);
		HBRUSH brush = ::CreatePatternBrush(bitmap);
		COLORREF old_bk = GetBkColor(hdc);
		COLORREF old_fg = GetTextColor(hdc);
		if(!is_xor) {
			SetROP2(hdc, R2_MASKPEN);
			SelectObject(hdc, brush);
			SetTextColor(hdc, Black());
			SetBkColor(hdc, White());
			SetDrawPen(PEN_NULL, Black);
			DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
				subpolygon_counts, subpolygon_count_count,
				disjunct_polygon_counts, disjunct_polygon_count_count);
			SetROP2(hdc, R2_MERGEPEN);
			SetTextColor(hdc, color);
			SetBkColor(hdc, Black());
		}
		else {
			SetROP2(hdc, R2_XORPEN);
			SetTextColor(hdc, COLORREF(color) ^ COLORREF(doxor));
			SelectObject(hdc, brush);
		}
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
		SelectObject(hdc, old_brush);
		SetTextColor(hdc, old_fg);
		SetBkColor(hdc, old_bk);
		SetROP2(hdc, old_rop);
		DeleteObject(brush);
		DeleteObject(bitmap);
		if(!IsNull(outline)) {
			SetColor(Null);
			SetDrawPen(width, outline);
			ASSERT(sizeof(POINT) == sizeof(Point));
			DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
				subpolygon_counts, subpolygon_count_count,
				disjunct_polygon_counts, disjunct_polygon_count_count);
		}
	}
	else { // simple fill
		SetDrawPen(IsNull(outline) ? PEN_NULL : width, Nvl(outline, Black));
		int old_rop2;
		if(is_xor) {
			color = Color(color.GetR() ^ doxor.GetR(), color.GetG() ^ doxor.GetG(), color.GetB() ^ doxor.GetB());
			old_rop2 = SetROP2(hdc, R2_XORPEN);
		}
		SetColor(color);
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
		if(is_xor)
			SetROP2(hdc, old_rop2);
	}
}

void SystemDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	GuiLock __;
	SetDrawPen(width, color);
	::Arc(GetHandle(), rc.left, rc.top, rc.right, rc.bottom, start.x, start.y, end.x, end.y);
}

#endif

void SystemDraw::DrawEllipseOp(const Rect& r, Color color, int width, Color pencolor)
{
	GuiLock __;
	SetColor(color);
	SetDrawPen(width, pencolor);
	::Ellipse(GetHandle(), r.left, r.top, r.right, r.bottom);
}

}

#endif
