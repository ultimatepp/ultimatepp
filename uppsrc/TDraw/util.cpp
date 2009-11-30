#include "TDraw.h"
#pragma hdrstop

#ifdef PLATFORM_WIN32
//#include <PdfDraw/PdfDraw.h>

#include <commdlg.h>
#else
enum { DOC_SCREEN_ZOOM = 140 };
#endif

NAMESPACE_UPP

#ifdef SYSTEMDRAW

Size GetPixelsPerMeter(const Draw& draw)
{
	if(draw.Dots())
		return Size(DOTS_PER_METER_INT, DOTS_PER_METER_INT);
	else
		return Size(96 * DOTS_PER_METER_INT / 600, 96 * DOTS_PER_METER_INT / 600);
}

int GetHorzPixelsPerMeter(const Draw& draw)
{
	return draw.Dots() ? DOTS_PER_METER_INT : 96 * DOTS_PER_METER_INT / 600;
}

int GetVertPixelsPerMeter(const Draw& draw)
{
	return draw.Dots() ? DOTS_PER_METER_INT : 96 * DOTS_PER_METER_INT / 600;
}

int GetAvgPixelsPerMeter(const Draw& draw)
{
	Size mp = GetPixelsPerMeter(draw);
	return (mp.cx + mp.cy) >> 1;
}

double GetHorzPixelsPerDot(const Draw& draw)
{
	return draw.Dots() ? 1 : GetHorzPixelsPerMeter(draw) / DOTS_PER_METER_DBL;
}

double GetVertPixelsPerDot(const Draw& draw)
{
	return draw.Dots() ? 1 : GetVertPixelsPerMeter(draw) / DOTS_PER_METER_DBL;
}

double GetAvgPixelsPerDot(const Draw& draw)
{
	if(draw.Dots())
		return 1;
	return GetAvgPixelsPerMeter(draw) / DOTS_PER_METER_DBL;
}

Font GetPixelFont(Font dot_font, const Draw& draw)
{
	if(draw.Pixels())
		dot_font.Height(dot_font.GetHeight() * 140 / 1024);
	return dot_font;
}

int DotsToPixels(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots * GetAvgPixelsPerDot(draw));
}

Size DotsToPixels(const Draw& draw, Size size)
{
	return draw.Dots()
		? size
		: iscale(size, GetPixelsPerMeter(draw), Size(DOTS_PER_METER_INT, DOTS_PER_METER_INT));
}

int HorzDotsToPixels(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots * GetHorzPixelsPerDot(draw));
}

int VertDotsToPixels(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots * GetVertPixelsPerDot(draw));
}

int PixelsToDots(const Draw& draw, int pixels)
{
	return draw.Dots() ? pixels : fround(pixels / GetAvgPixelsPerDot(draw));
}

Size PixelsToDots(const Draw& draw, Size size)
{
	return draw.Dots() ? size
		: iscale(size, Size(DOTS_PER_METER_INT, DOTS_PER_METER_INT), GetPixelsPerMeter(draw));
}

int HorzPixelsToDots(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots / GetHorzPixelsPerDot(draw));
}

int VertPixelsToDots(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots / GetVertPixelsPerDot(draw));
}

int PixelsToPoints(const Draw& draw, int pixels)
{
	if(draw.Dots())
		return pixels * 72 / 600;
	return iscale(pixels, 72, draw.GetPixelsPerInch().cx);
}

Size PixelsToPoints(const Draw& draw, Size pixels)
{
	if(draw.Dots())
		return pixels * 72 / 600;
	return iscale(pixels, Size(72, 72), draw.GetPixelsPerInch());
}

int PointsToPixels(const Draw& draw, int points)
{
	if(draw.Dots())
		return points * 600 / 72;
	return iscale(points, draw.GetPixelsPerInch().cx, 72);
}

Size PointsToPixels(const Draw& draw, Size points)
{
	if(draw.Dots())
		return points * 600 / 72;
	return iscale(points, draw.GetPixelsPerInch(), Size(72, 72));
}

#else
Size GetPixelsPerMeter(const Draw& draw)
{
	if(draw.Dots())
		return Size(DOTS_PER_METER_INT, DOTS_PER_METER_INT);
	else
		return iscale(draw.GetPagePixels(), Size(1000, 1000), max(draw.GetPageMMs(), Size(1, 1)));
}

int GetHorzPixelsPerMeter(const Draw& draw)
{
	return draw.Dots() ? DOTS_PER_METER_INT
		: iscale(draw.GetPagePixels().cx, 1000, max(draw.GetPageMMs().cx, 1));
}

int GetVertPixelsPerMeter(const Draw& draw)
{
	return draw.Dots() ? DOTS_PER_METER_INT
		: iscale(draw.GetPagePixels().cy, 1000, max(draw.GetPageMMs().cy, 1));
}

int GetAvgPixelsPerMeter(const Draw& draw)
{
	Size mp = GetPixelsPerMeter(draw);
	return (mp.cx + mp.cy) >> 1;
}

double GetHorzPixelsPerDot(const Draw& draw)
{
	return draw.Dots() ? 1 : GetHorzPixelsPerMeter(draw) / DOTS_PER_METER_DBL;
}

double GetVertPixelsPerDot(const Draw& draw)
{
	return draw.Dots() ? 1 : GetVertPixelsPerMeter(draw) / DOTS_PER_METER_DBL;
}

double GetAvgPixelsPerDot(const Draw& draw)
{
	if(draw.Dots())
		return 1;
	return GetAvgPixelsPerMeter(draw) / DOTS_PER_METER_DBL;
}

Font GetPixelFont(Font dot_font, const Draw& draw)
{
	if(draw.Pixels())
		dot_font.Height(dot_font.GetHeight() * 140 / 1024);
	return dot_font;
}

int DotsToPixels(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots * GetAvgPixelsPerDot(draw));
}

Size DotsToPixels(const Draw& draw, Size size)
{
	return draw.Dots()
		? size
		: iscale(size, GetPixelsPerMeter(draw), Size(DOTS_PER_METER_INT, DOTS_PER_METER_INT));
}

int HorzDotsToPixels(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots * GetHorzPixelsPerDot(draw));
}

int VertDotsToPixels(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots * GetVertPixelsPerDot(draw));
}

int PixelsToDots(const Draw& draw, int pixels)
{
	return draw.Dots() ? pixels : fround(pixels / GetAvgPixelsPerDot(draw));
}

Size PixelsToDots(const Draw& draw, Size size)
{
	return draw.Dots() ? size
		: iscale(size, Size(DOTS_PER_METER_INT, DOTS_PER_METER_INT), GetPixelsPerMeter(draw));
}

int HorzPixelsToDots(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots / GetHorzPixelsPerDot(draw));
}

int VertPixelsToDots(const Draw& draw, int dots)
{
	return draw.Dots() ? dots : fround(dots / GetVertPixelsPerDot(draw));
}

int PixelsToPoints(const Draw& draw, int pixels)
{
	if(draw.Dots())
		return pixels * 72 / 600;
	return iscale(pixels, 72, draw.GetPixelsPerInch().cx);
}

Size PixelsToPoints(const Draw& draw, Size pixels)
{
	if(draw.Dots())
		return pixels * 72 / 600;
	return iscale(pixels, Size(72, 72), draw.GetPixelsPerInch());
}

int PointsToPixels(const Draw& draw, int points)
{
	if(draw.Dots())
		return points * 600 / 72;
	return iscale(points, draw.GetPixelsPerInch().cx, 72);
}

Size PointsToPixels(const Draw& draw, Size points)
{
	if(draw.Dots())
		return points * 600 / 72;
	return iscale(points, draw.GetPixelsPerInch(), Size(72, 72));
}

#endif

void RGBtoHSV(Color c, double& h, double& s, double& v)
{
	RGBtoHSV(c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0, h, s, v);
}

Color HSVtoRGB(double h, double s, double v)
{
	double r, g, b;
	HSVtoRGB(h, s, v, r, g, b);
	return Color(fround(r * 255.0), fround(g * 255.0), fround(b * 255.0));
}

Color GetColorGradient(Color c1, Color c2, int ratio1, int ratio2, double gamma)
{
	double h1, s1, v1;
	double h2, s2, v2;
	RGBtoHSV(pow(c1.GetR() / 255.0, gamma), pow(c1.GetG() / 255.0, gamma), pow(c1.GetB() / 255.0, gamma), h1, s1, v1);
	RGBtoHSV(pow(c2.GetR() / 255.0, gamma), pow(c2.GetG() / 255.0, gamma), pow(c2.GetB() / 255.0, gamma), h2, s2, v2);
	if(s1 == 0)
		h1 = h2;
	else if(s2 == 0)
		h2 = h1;
	double r = ratio1 / (double)ratio2;
	gamma = 1 / gamma;
	if(h2 > h1 + 0.5)
		h1 = ((h1 += (h2 - 1 - h1) * r) < 0 ? h1 + 1 : h1);
	else if(h1 > h2 + 0.5)
		h1 = ((h1 += (h2 + 1 - h1) * r) > 1 ? h1 - 1 : h1);
	else
		h1 += (h2 - h1) * r;
	HSVtoRGB(h1, s1 + (s2 - s1) * r, v1 + (v2 - v1) * r, h1, s1, v1);
	return Color(fround(pow(h1, gamma) * 255.0), fround(pow(s1, gamma) * 255.0), fround(pow(v1, gamma) * 255.0));
}

#ifdef PLATFORM_WIN32
static void DrawDragDropRectRaw(Draw& draw, const Rect& rc, HBRUSH brush, int width)
{
#ifdef SYSTEMDRAW
	SystemDraw *w = dynamic_cast<SystemDraw *>(&draw);
	if(w) {
		SystemDraw& draw = *w;
		UnrealizeObject(brush);
		HGDIOBJ old_brush = SelectObject(draw, brush);
		SetBrushOrgEx(draw, ~rc.left, ~rc.top, NULL);
		Size size = rc.Size();
		enum { NOTPATINVERT = 0x00A50065 };
		if(2 * width >= min(size.cx, size.cy))
			::PatBlt(draw, rc.left, rc.top, size.cx, size.cy, NOTPATINVERT);
		else
		{
			::PatBlt(draw, rc.left, rc.top, size.cx, width, NOTPATINVERT);
			::PatBlt(draw, rc.left, rc.bottom - width, size.cx, width, NOTPATINVERT);
			::PatBlt(draw, rc.left, rc.top + width, width, size.cy - 2 * width, NOTPATINVERT);
			::PatBlt(draw, rc.right - width, rc.top + width, width, size.cy - 2 * width, NOTPATINVERT);
		}
		SelectObject(draw, old_brush);
	}
#else
	UnrealizeObject(brush);
	HGDIOBJ old_brush = SelectObject(draw, brush);
	SetBrushOrgEx(draw, ~rc.left, ~rc.top, NULL);
	Size size = rc.Size();
	enum { NOTPATINVERT = 0x00A50065 };
	if(2 * width >= min(size.cx, size.cy))
		::PatBlt(draw, rc.left, rc.top, size.cx, size.cy, NOTPATINVERT);
	else
	{
		::PatBlt(draw, rc.left, rc.top, size.cx, width, NOTPATINVERT);
		::PatBlt(draw, rc.left, rc.bottom - width, size.cx, width, NOTPATINVERT);
		::PatBlt(draw, rc.left, rc.top + width, width, size.cy - 2 * width, NOTPATINVERT);
		::PatBlt(draw, rc.right - width, rc.top + width, width, size.cy - 2 * width, NOTPATINVERT);
	}
	SelectObject(draw, old_brush);
#endif
}
#endif

#ifdef PLATFORM_X11
static void DrawDragDropRectRaw(Draw& draw, const Rect& rc, int width)
{
	Size size = rc.Size();
	if(2 * width <= min(size.cx, size.cy))
		XFillRectangle(Xdisplay, draw.GetDrawable(), draw.GetGC(), rc.left, rc.top, size.cx, size.cy);
	else
	{
		XFillRectangle(Xdisplay, draw.GetDrawable(), draw.GetGC(), rc.left, rc.top, size.cx, width);
		XFillRectangle(Xdisplay, draw.GetDrawable(), draw.GetGC(), rc.left, rc.bottom - width, size.cx, width);
		XFillRectangle(Xdisplay, draw.GetDrawable(), draw.GetGC(), rc.left, rc.top + width, width, size.cy - 2 * width);
		XFillRectangle(Xdisplay, draw.GetDrawable(), draw.GetGC(), rc.right - width, rc.top + width, width, size.cy - 2 * width);
	}
}
#endif

void DrawDragDropRect(Draw& draw, const Rect& rc_old, const Rect& rc_new, int width, Color c1, Color c2)
{
#ifdef PLATFORM_WIN32
#ifdef SYSTEMDRAW
	SystemDraw *w = dynamic_cast<SystemDraw *>(&draw);
	if(w) { SystemDraw& draw = *w;
#endif
	static const word halftone[] = { 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa };
	HBITMAP bitmap = CreateBitmap(8, 8, 1, 1, halftone);
	HBRUSH brush = CreatePatternBrush(bitmap);
	DeleteObject(bitmap);
	draw.BeginGdi();
	SetTextColor(draw, c1);
	SetBkColor(draw, c2);
	if(!IsNull(rc_old))
		DrawDragDropRectRaw(draw, rc_old, brush, width);
	if(!IsNull(rc_new))
		DrawDragDropRectRaw(draw, rc_new, brush, width);
	DeleteObject(brush);
	draw.EndGdi();
#ifdef SYSTEMDRAW
	}
#endif
#endif
#ifdef PLATFORM_X11
	XGCValues gcv_old, gcv_new;
	XGetGCValues(Xdisplay, draw.GetGC(), GCForeground | GCFunction, &gcv_old);
	gcv_new.function = X11_ROP2_XOR;
	gcv_new.foreground = GetXPixel(c1);
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCFunction, &gcv_new);
	if(!IsNull(rc_old))
		DrawDragDropRectRaw(draw, rc_old, width);
	if(!IsNull(rc_new))
		DrawDragDropRectRaw(draw, rc_new, width);
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCFunction, &gcv_old);
#endif
}

void DrawDragDropRect(Draw& draw, const Rect& rc_old, const Rect& rc_new, Color c1, Color c2)
{
	DrawDragDropRect(draw, rc_old, rc_new, max(rc_old.Width(), rc_new.Width()), c1, c2);
}

enum
{
	wPolyPolyline    = 100,
	wPolyPolyPolygon = 101,
	wEllipse         = 102,
	wArc             = 103,
};

static void StreamUnpackPoints(Stream& stream, Point *out, int count)
{
	ASSERT(sizeof(Point) >= 8);
	if(count == 0)
		return;
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(Point) == 8) {
		stream.Get(out, count * 8);
		return;
	}
#endif
	Point *end = out + count;
	byte *top = reinterpret_cast<byte *>(end) - count * 8;
	stream.Get(top, count * 8);
	for(; out < end; out++, top += 8)
	{
		out -> x = Peek32le(top + 0);
		out -> y = Peek32le(top + 4);
	}
}

static void StreamPackPoints(Stream& stream, const Point *in, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(Point) == 8) {
		stream.Put(in, count * 8);
		return;
	}
#endif
	enum { PART = 1024 };
	byte part[PART * 8];
	while(count > 0) {
		int part_count = min<int>(count, PART);
		for(byte *pp = part, *pe = pp + 8 * part_count; pp < pe; pp += 8, in++) {
			Poke32le(pp + 0, in -> x);
			Poke32le(pp + 4, in -> y);
		}
		stream.Put(part, part_count * 8);
		count -= part_count;
	}
}

static void StreamUnpackInts(Stream& stream, int *out, int count)
{
	ASSERT(sizeof(int) >= 4);
	int *end = out + count;
	byte *top = reinterpret_cast<byte *>(end) - count * 4;
	stream.Get(top, count * 4);
	if(sizeof(int) > 4)
	{
		for(; out < end; out++, top += 4)
			*out = Peek32le(top);
	}
	else
	{
#ifndef CPU_LITTLE_ENDIAN
		BltSwapEndian4(out, out, count);
#endif
	}
}

static void StreamPackInts(Stream& stream, const int *in, int count)
{
	if(sizeof(int) == 4)
	{
#ifdef CPU_LITTLE_ENDIAN
		stream.Put(in, count * 4);
		return;
#endif
	}
	enum { PART = 1024 };
	byte part[PART * 4];
	while(count > 0)
	{
		int part_count = min<int>(count, PART);
		for(byte *pp = part, *pe = pp + 4 * part_count; pp < pe; pp += 4)
			Poke32le(pp, *in++);
		stream.Put(part, part_count * 4);
		count -= part_count;
	}
}

/*
static void StreamPolyPolyline(Stream& stream,
	Vector<Point>& out_vertices, const Point *in_vertices, int vertex_count,
	Vector<int>& out_counts, const int *in_counts, int count_count,
	int& width, Color& color, Color& doxor)
{
	int version = 2;
	stream / version;
	stream % width % color % doxor;
	stream % vertex_count % count_count;
	if(stream.IsLoading())
	{
		out_vertices.SetCount(vertex_count);
		out_counts.SetCount(count_count);
		StreamUnpackPoints(stream, out_vertices.Begin(), vertex_count);
		StreamUnpackInts(stream, out_counts.Begin(), count_count);
	}
	else
	{
		StreamPackPoints(stream, in_vertices, vertex_count);
		StreamPackInts(stream, in_counts, count_count);
	}
}
*/

#if STAT_LINES
static int vertex_count = 0;
static int segment_count = 0;
int GetVertexCount()
{
	return vertex_count;
}

int GetSegmentCount()
{
#if STAT_LINES >= 2
	static int prev = 0;
	RLOG("GetSegmentCount() -> " << segment_count << ", delta = " << (segment_count - prev));
	prev = segment_count;
#endif
	return segment_count;
}
#endif

/*
void DrawPolyPolyline(Draw& draw, const Point *vertices, int vertex_count,
	const int *counts, int count_count, int width, Color color, Color doxor)
{
	//RTIMING("DrawPolyPolyline");

	ASSERT(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || IsNull(color))
		return;
#if STAT_LINES
	::vertex_count += vertex_count;
	::segment_count += count_count;
#if STAT_LINES >= 2
	{ // log vertices
		RLOG("Polyline (" << count_count << " segments, " << vertex_count << " vertices)");
		const Point *p = vertices;
		for(int s = 0; s < count_count; s++)
		{
			for(const Point *e = p + counts[s]; p < e; p++)
				RLOG(*p);
			RLOG("-------- (end of segment #" << s << ")");
		}
	}
#endif
#endif
	if(draw.IsDrawing())
	{
//		TIMING("DrawPolyPolyline/stream")
		Vector<Point> out_vertices;
		Vector<int> out_counts;
		StreamPolyPolyline(draw.DrawingOp(wPolyPolyline),
			out_vertices, vertices, vertex_count, out_counts, counts, count_count, width, color, doxor);
		return;
	}
//	TIMING("DrawPolyPolyline/hdc")
	bool is_xor = !IsNull(doxor);
	if(is_xor)
		color = Color(color.GetR() ^ doxor.GetR(), color.GetG() ^ doxor.GetG(), color.GetB() ^ doxor.GetB());
#if defined(PLATFORM_WIN32)
	if(is_xor)
		SetROP2(draw, R2_XORPEN);
	draw.SetDrawPen(width, color);
	enum { MAX_COUNT = 8000 };
	if(count_count == 1)
		Polyline(draw, (const POINT *)vertices, vertex_count);
	else if(vertex_count <= MAX_COUNT)
		PolyPolyline(draw, (const POINT *)vertices, (const dword *)counts, count_count);
	else
	{ // too many vertices / parts
		const int *ce = counts + count_count;
		while(counts < ce)
		{
			const int *cs = counts;
			int total = 0;
			while(counts < ce && total + *counts <= MAX_COUNT)
				total += *counts++;
			if(total)
			{
				PolyPolyline(draw, (const POINT *)vertices, (const dword *)cs, counts - cs);
				vertices += total;
			}
			else
			{
				const Point *pts = vertices;
				vertices += (total = *counts++);
				for(; total > MAX_COUNT; pts += MAX_COUNT, total -= MAX_COUNT)
					Polyline(draw, (const POINT *)pts, MAX_COUNT + 1);
				if(total >= 2)
					Polyline(draw, (const POINT *)pts, total);
			}
		}
	}
	if(is_xor)
		SetROP2(draw, R2_COPYPEN);
#elif defined(PLATFORM_X11)
	XGCValues gcv_old, gcv_new;
	XGetGCValues(Xdisplay, draw.GetGC(), GCForeground | GCLineWidth | GCFunction, &gcv_old);
	gcv_new.function = is_xor ? X11_ROP2_COPY : X11_ROP2_XOR;
	gcv_new.foreground = GetXPixel(color);
	gcv_new.line_width = width;
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCLineWidth | GCFunction, &gcv_new);
	enum { REQUEST_LENGTH = 256 }; // X server XDrawLines request length (heuristic)
	Point offset = draw.GetOffset();
	if(vertex_count == 2)
		XDrawLine(Xdisplay, draw.GetDrawable(), draw.GetGC(),
			vertices[0].x + offset.x, vertices[0].y + offset.y,
			vertices[1].x + offset.x, vertices[1].y + offset.y);
	else if(count_count == 1 || vertex_count > count_count * (REQUEST_LENGTH + 2))
	{
		for(; --count_count >= 0; counts++)
		{
			Buffer<XPoint> part(*counts);
			for(XPoint *vo = part, *ve = vo + *counts; vo < ve; vo++, vertices++)
			{
				vo -> x = (short)(vertices -> x + offset.x);
				vo -> y = (short)(vertices -> y + offset.y);
			}
			XDrawLines(Xdisplay, draw.GetDrawable(), draw.GetGC(), part, *counts, CoordModeOrigin);
		}
	}
	else
	{
		int segment_count = vertex_count - count_count;
		Buffer<XSegment> segments(segment_count);
		XSegment *so = segments;
		while(--count_count >= 0)
		{
			const Point *end = vertices + *counts++;
			so -> x1 = (short)(vertices -> x + offset.x);
			so -> y1 = (short)(vertices -> y + offset.y);
			vertices++;
			so -> x2 = (short)(vertices -> x + offset.x);
			so -> y2 = (short)(vertices -> y + offset.y);
			so++;
			while(++vertices < end) {
				so -> x1 = so[-1].x2;
				so -> y1 = so[-1].y2;
				so -> x2 = (short)(vertices -> x + offset.x);
				so -> y2 = (short)(vertices -> y + offset.y);
				so++;
			}
		}
		ASSERT(so == segments + segment_count);
		XDrawSegments(Xdisplay, draw.GetDrawable(), draw.GetGC(), segments, segment_count);
	}
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCLineWidth | GCFunction, &gcv_old);
#else
	#error
#endif
}
*/

/*
static void wsPolyPolyline(Draw& draw, Stream& stream, const DrawingPos& dp)
{
//	TIMING("wsPolyPolyline")
	int width;
	Color color, doxor;
	Vector<Point> vertices;
	Vector<int> counts;
	StreamPolyPolyline(stream, vertices, 0, 0, counts, 0, 0, width, color, doxor);
	if(!dp.Identity())
	{
		for(int i = 0; i < vertices.GetCount(); i++)
			dp.Transform(vertices[i]);
		if(width > 0)
			dp.TransformW(width);
	}
	DrawPolyPolyline(draw, vertices, counts, width, color, doxor);
}
*/

//static DrawerRegistrator MK__s(wPolyPolyline, wsPolyPolyline);

/*
void DrawPolyPolyline(Draw& draw, const Vector<Point>& vertices, const Vector<int>& counts, int width, Color color, Color doxor)
{
	DrawPolyPolyline(draw, vertices.Begin(), vertices.GetCount(),
		counts.Begin(), counts.GetCount(), width, color, doxor);
}
*/

/*
void DrawPolyline(Draw& draw, const Point *vertices, int count, int width, Color color, Color doxor)
{
	DrawPolyPolyline(draw, vertices, count, &count, 1, width, color, doxor);
}
*/

/*
void DrawPolyline(Draw& draw, const Vector<Point>& vertices, int width, Color color, Color doxor)
{
	DrawPolyline(draw, vertices.Begin(), vertices.GetCount(), width, color, doxor);
}
*/

static void StreamPolyPolyPolygon(Stream& stream,
	Vector<Point>& out_vertices, const Point *in_vertices, int vertex_count,
	Vector<int>& out_subpolygon_counts, const int *in_subpolygon_counts, int subpolygon_count_count,
	Vector<int>& out_polygon_counts, const int *in_polygon_counts, int polygon_count_count,
	Color& color, int& width, Color& outline, uint64& pattern, Color& doxor)
{
//	static TimingInspector ti("StreamPolyPolygon");
//	ti.Start();
	int version = 2;
	stream / version;
	stream % color % width % outline % pattern % doxor;
//	ti.End();
	stream % vertex_count % subpolygon_count_count % polygon_count_count;
	if(polygon_count_count <= 0 || polygon_count_count > subpolygon_count_count
	|| subpolygon_count_count <= 0 || subpolygon_count_count > vertex_count)
	{
		stream.SetError();
		return;
	}
	if(stream.IsLoading())
	{
		out_vertices.SetCount(vertex_count);
		out_subpolygon_counts.SetCount(subpolygon_count_count);
		out_polygon_counts.SetCount(polygon_count_count);
		StreamUnpackPoints(stream, out_vertices, vertex_count);
		StreamUnpackInts(stream, out_subpolygon_counts, subpolygon_count_count);
		StreamUnpackInts(stream, out_polygon_counts, polygon_count_count);
	}
	else
	{
		StreamPackPoints(stream, in_vertices, vertex_count);
		StreamPackInts(stream, in_subpolygon_counts, subpolygon_count_count);
		StreamPackInts(stream, in_polygon_counts, polygon_count_count);
	}
}

#ifdef PLATFORM_WIN32
static void DrawPolyPolygonRaw(Draw& draw, const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	bool is_inside, int outline_width, Color outline_color
)
{
#ifdef SYSTEMDRAW
	SystemDraw *w = dynamic_cast<SystemDraw *>(&draw);
	if(w) { SystemDraw& draw = *w;
#endif
	draw.SetDrawPen(outline_width, outline_color);
	ASSERT(sizeof(POINT) == sizeof(Point)); // modify algorithm when not
	enum { MAX_POLY = 8000 };
	if(subpolygon_count_count == 1 && vertex_count < MAX_POLY)
		Polygon(draw, (const POINT *)vertices, vertex_count);
	else if(vertex_count < MAX_POLY)
		PolyPolygon(draw, (const POINT *)vertices, subpolygon_counts, subpolygon_count_count);
	else {
		if(is_inside) {
			draw.SetDrawPen(PEN_NULL, Black);
			Vector<Point> split_vertices;
			Vector<int> split_counts;
		#ifdef SYSTEMDRAW
			SplitPolygon(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
				split_vertices, split_counts, Size(9999, 9999));
		#else
			SplitPolygon(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
				split_vertices, split_counts, draw.GetClip());
		#endif
			//!! todo: maxcount for splitpolygon
			const Point *sv = split_vertices.Begin();
			for(const int *sc = split_counts.Begin(), *se = split_counts.End(); sc < se; sc++) {
				Polygon(draw, (const POINT *)sv, *sc);
				sv += *sc;
			}
		}
		if(outline_width != PEN_NULL) {
			draw.DrawPolyPolyline(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
				outline_width, outline_color, Null);
			Buffer<Point> finish(2 * subpolygon_count_count);
			Buffer<int> counts(subpolygon_count_count);
			Fill(&counts[0], &counts[subpolygon_count_count], 2);
			Point *d = finish;
			const Point *p = vertices;
			const int *c = subpolygon_counts, *e = c + subpolygon_count_count;
			while(c < e)
			{
				*d++ = *p;
				*d++ = (p += *c++)[-1];
			}
			draw.DrawPolyPolyline(finish, 2 * subpolygon_count_count,
				counts, subpolygon_count_count, outline_width, outline_color, Null);
		}
		draw.SetDrawPen(outline_width, outline_color);
	}
#ifdef SYSTEMDRAW
	}
#endif
}
#elif defined(PLATFORM_X11)
static void FillPolyPolygonRaw(GC gc, Drawable drawable, Rect clip, Point offset,
	const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count)
{
	enum { MAX_VERTEX_COUNT = 10000 };

	if(subpolygon_count_count == 1 && vertex_count <= MAX_VERTEX_COUNT) {
		Buffer<XPoint> out_points(vertex_count);
		const Point *in = vertices;
		for(XPoint *out = out_points, *end = out + vertex_count; out < end; out++, in++)
		{
			out -> x = (short)(in -> x + offset.x);
			out -> y = (short)(in -> y + offset.y);
		}
		XFillPolygon(Xdisplay, drawable, gc,
			out_points, vertex_count, Nonconvex, CoordModeOrigin);
	}
	else {
		Vector<Point> split_vertices;
		ASSERT(sizeof(XPoint) <= sizeof(Point)); // modify algorithm when not
		Vector<int> split_counts;
		SplitPolygon(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
			split_vertices, split_counts, clip);
		const Point *sv = split_vertices.Begin();
		XPoint *dv = reinterpret_cast<XPoint *>(split_vertices.Begin());
		for(const int *sc = split_counts.Begin(), *se = split_counts.End(); sc < se; sc++) {
			for(XPoint *db = dv, *de = dv + *sc; db < de; db++, sv++) {
				db -> x = (short)(sv -> x + offset.x);
				db -> y = (short)(sv -> y + offset.y);
			}
			XFillPolygon(Xdisplay, drawable, gc, dv, *sc, Nonconvex, CoordModeOrigin);
		}
	}
}

static void DrawPolyPolygonRaw(GC gc, Drawable drawable, Point offset,
	const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count)
{
	for(const int *sp = subpolygon_counts, *se = sp + subpolygon_count_count; sp < se; sp++) {
		Buffer<XPoint> segment(*sp + 1);
		XPoint *out = segment;
		for(const Point *end = vertices + *sp; vertices < end; vertices++, out++)
		{
			out -> x = (short)(vertices -> x + offset.x);
			out -> y = (short)(vertices -> y + offset.y);
		}
		*out = segment[0];
		XDrawLines(Xdisplay, drawable, gc, segment, *sp + 1, CoordModeOrigin);
	}
}
#endif

/*
#ifdef PLATFORM_X11
Pixmap GetPatternPixmap(int64 pattern)
{
	static VectorMap<int64, Image> cache;
	int f = cache.Find(pattern);
	if(f >= 0)
		return cache[f];
	if(cache.GetCount() >= 1000)
		cache.Clear();
	ImageBuffer new_image(Size(8, 8));
	for(int p = 0; p < 64; p++) {
		RGBA rgba = { 0, 0, 0, pattern & (1 << p) ? 255 : 0 };
		new_
		new_image[p] = (pattern & (1 << p) ? RGBAZero() :
	new_image.(8, 8);
	PixelArray px;
	px.CreateMono(8, 8, 1);
	for(int p = 0; p < 8; p++)
		*px.GetUpScan(p) = (byte)(~pattern >> (8 * p));
	ImageMaskDraw im(new_image);
	px.Paint(im);
	im.Close();
	cache.Add(pattern, new_image);
	return new_image;
}
#endif
*/

void DrawPolyPolyPolygon(Draw& draw, const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	if(vertex_count == 0)
		return;

#ifdef PLATFORM_WIN32_ //!!TODO
	if(PdfDraw *pdf = dynamic_cast<PdfDraw *>(&draw)) {
		pdf->DrawPolyPolyPolygon(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count, color, width, outline, pattern, doxor);
		return;
	}
#endif

//	LOG("DrawPolyPolyPolygon: drawing = " << (draw.IsDrawing() ? "yes" : "no")
//		<< ", backdraw = " << (draw.IsBack() ? "yes" : "no")
//		<< ", " << (draw.Dots() ? "dots" : "pixels")
//		<< ", printer = " << (draw.IsPrinter() ? "yes" : "no"));
//	LOG("#vertices = " << vertex_count << ", #subpolygons = " << subpolygon_count_count
//	<< ", #disjunct polygons = " << disjunct_polygon_count_count);
//	LOG("color = " << Dump(color) << ", width = " << width << ", outline = " << Dump(outline)
//	<< ", pattern = " << Dump(pattern) << ", doxor = " << doxor);

#ifdef _DEBUG
//	for(int v = 0; v < vertex_count; v++)
//		if(abs2(vertices[v] - vertices[(v ? v : vertex_count) - 1]) >= 500 * 500)
//			NEVER();
#endif

	if(!dynamic_cast<SystemDraw *>(&draw)) {
		draw.DrawPolyPolyPolygon(vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count,
			color, width, outline, pattern, doxor);
		return;
	}

//	TIMING("DrawPolyPolygon/hdc");
	bool is_xor = !IsNull(doxor);

#ifdef PLATFORM_X11
	unsigned xor_pixel = (is_xor ? GetXPixel(doxor) : 0);
	XGCValues gcv;
	gcv.function = is_xor ? X11_ROP2_XOR : X11_ROP2_COPY;
	GC fill_gc = NULL;
	Image pattern_image;
	if(!IsNull(color)) {
		gcv.foreground = GetXPixel(color) ^ xor_pixel;
		int fmask = GCForeground | GCFunction;
		if(pattern) {
			pattern_image = GetPatternImage(pattern);
			gcv.stipple = pattern_image.GetMaskPixmap();
			gcv.fill_style = FillStippled;
			fmask |= GCStipple | GCFillStyle;
		}
		fill_gc = XCreateGC(Xdisplay, draw.GetDrawable(), fmask, &gcv);
	}
	GC line_gc = NULL;
	if(!IsNull(outline)) {
		gcv.foreground = GetXPixel(outline) ^ xor_pixel;
		gcv.line_width = width;
		line_gc = XCreateGC(Xdisplay, draw.GetDrawable(), GCForeground | GCFunction | GCLineWidth, &gcv);
		Point offset = draw.GetOffset();
	}
#endif

	for(int i = 0; i < disjunct_polygon_count_count; i++, disjunct_polygon_counts++) {
		int poly = *disjunct_polygon_counts;
		int sub = 1;
		if(*subpolygon_counts < poly)
			if(disjunct_polygon_count_count > 1) {
				const int *se = subpolygon_counts;
				int total = 0;
				while(total < poly)
					total += *se++;
				sub = se - subpolygon_counts;
			}
			else
				sub = subpolygon_count_count;

		if(sub > poly) {
			vertices += poly;
			subpolygon_counts += sub;
			continue;
		}


#if defined(PLATFORM_WIN32)
	#ifdef SYSTEMDRAW
		SystemDraw *w = dynamic_cast<SystemDraw *>(&draw);
		if(w) { SystemDraw& draw = *w;
	#endif
		if(pattern) {
			int old_rop = GetROP2(draw);
			HGDIOBJ old_brush = GetCurrentObject(draw, OBJ_BRUSH);
			word wpat[8] = {
				(byte)(pattern >> 56), (byte)(pattern >> 48), (byte)(pattern >> 40), (byte)(pattern >> 32),
				(byte)(pattern >> 24), (byte)(pattern >> 16), (byte)(pattern >> 8), (byte)(pattern >> 0),
			};
			HBITMAP bitmap = CreateBitmap(8, 8, 1, 1, wpat);
			HBRUSH brush = ::CreatePatternBrush(bitmap);
			COLORREF old_bk = GetBkColor(draw);
			COLORREF old_fg = GetTextColor(draw);
			if(!is_xor) {
				SetROP2(draw, R2_MASKPEN);
				SelectObject(draw, brush);
				SetTextColor(draw, Black());
				SetBkColor(draw, White());
				DrawPolyPolygonRaw(draw, vertices, poly,
					subpolygon_counts, sub, true, PEN_NULL, Null);
				SetROP2(draw, R2_MERGEPEN);
				SetTextColor(draw, color);
				SetBkColor(draw, Black());
			}
			else { // xor fill with pattern data
				SetROP2(draw, R2_XORPEN);
				SelectObject(draw, brush);
			}
			DrawPolyPolygonRaw(draw, vertices, poly,
				subpolygon_counts, sub, true, PEN_NULL, Null);
			SelectObject(draw, old_brush);
			SetTextColor(draw, old_fg);
			SetBkColor(draw, old_bk);
			SetROP2(draw, old_rop);
			DeleteObject(brush);
			DeleteObject(bitmap);
			if(!IsNull(outline))
			{
				draw.SetColor(Null);
				draw.SetDrawPen(width, outline);
				ASSERT(sizeof(POINT) == sizeof(Point));
				PolyPolygon(draw, (const POINT *)vertices, subpolygon_counts, sub);
			}
		}
		else
		{ // simple fill
	//		RTIMING("AreaTool::Fill(solid color)");
			int out_wd = (IsNull(width) || IsNull(outline) ? PEN_NULL : width);
			Color out_co = Nvl(outline, Black);
			draw.SetDrawPen(out_wd, out_co);
			if(is_xor)
			{
				color = Color(color.GetR() ^ doxor.GetR(), color.GetG() ^ doxor.GetG(), color.GetB() ^ doxor.GetB());
				SetROP2(draw, R2_XORPEN);
			}
			HGDIOBJ old_brush = 0;
			if(IsNull(color))
			{
				static HGDIOBJ null_brush = GetStockObject(NULL_BRUSH);
				old_brush = SelectObject(draw, null_brush);
			}
			else
				draw.SetColor(color);
			DrawPolyPolygonRaw(draw, vertices, poly, subpolygon_counts, sub,
				!IsNull(color), out_wd, out_co);
			if(old_brush)
				SelectObject(draw, old_brush);
			if(is_xor)
				SetROP2(draw, R2_COPYPEN);
		}
	#ifdef SYSTEMDRAW
		}
	#endif
#elif defined(PLATFORM_X11)
		if(fill_gc)
			FillPolyPolygonRaw(fill_gc, draw.GetDrawable(), draw.GetClip(), draw.GetOffset(),
				vertices, poly, subpolygon_counts, sub);
		if(line_gc)
			DrawPolyPolygonRaw(line_gc, draw.GetDrawable(), draw.GetOffset(),
				vertices, poly, subpolygon_counts, sub);
#else
	#error
#endif
		vertices += poly;
		subpolygon_counts += sub;
	}

#ifdef PLATFORM_X11
	if(fill_gc)
		XFreeGC(Xdisplay, fill_gc);
	if(line_gc)
		XFreeGC(Xdisplay, line_gc);
#endif
}

/*
static void wsPolyPolyPolygon(Draw& draw, Stream& stream, const DrawingPos& dp)
{
//	TIMING("wsPolyPolygon")
	Color color, outline, doxor;
	int width;
	uint64 pattern;
	Vector<Point> vertices;
	Vector<int> subpolygon_counts, polygon_counts;
	StreamPolyPolyPolygon(stream, vertices, 0, 0,
		subpolygon_counts, 0, 0, polygon_counts, 0, 0, color, width, outline, pattern, doxor);
	if(!dp.Identity()) {
		for(int i = 0; i < vertices.GetCount(); i++)
			dp.Transform(vertices[i]);
		if(width > 0)
			dp.TransformW(width);
	}
	DrawPolyPolyPolygon(draw, vertices, subpolygon_counts, polygon_counts,
		color, width, outline, pattern, doxor);
}

static DrawerRegistrator MK__s(wPolyPolyPolygon, wsPolyPolyPolygon);
*/

void DrawPolyPolyPolygon(Draw& draw, const Vector<Point>& vertices,
	const Vector<int>& subpolygon_counts, const Vector<int>& disjunct_polygon_counts,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(draw, vertices.Begin(), vertices.GetCount(),
		subpolygon_counts.Begin(), subpolygon_counts.GetCount(),
		disjunct_polygon_counts.Begin(), disjunct_polygon_counts.GetCount(),
		color, width, outline, pattern, doxor);
}

void DrawPolyPolygon(Draw& draw, const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(draw, vertices, vertex_count,
		subpolygon_counts, subpolygon_count_count, &vertex_count, 1,
		color, width, outline, pattern, doxor);
}

void DrawPolyPolygon(Draw& draw, const Vector<Point>& vertices, const Vector<int>& subpolygon_counts,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolygon(draw, vertices.Begin(), vertices.GetCount(),
		subpolygon_counts.Begin(), subpolygon_counts.GetCount(),
		color, width, outline, pattern, doxor);
}

void DrawPolygons(Draw& draw, const Point *vertices, int vertex_count, const int *polygon_counts, int polygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(draw, vertices, vertex_count,
		polygon_counts, polygon_count_count, polygon_counts, polygon_count_count,
		color, width, outline, pattern, doxor);
}

void DrawPolygons(Draw& draw, const Vector<Point>& vertices, const Vector<int>& polygon_counts,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolygons(draw, vertices.Begin(), vertices.GetCount(),
		polygon_counts.Begin(), polygon_counts.GetCount(),
		color, width, outline, pattern, doxor);
}

void DrawPolygon(Draw& draw, const Point *vertices, int vertex_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(draw, vertices, vertex_count,
		&vertex_count, 1, &vertex_count, 1, color, width, outline, pattern, doxor);
}

void DrawPolygon(Draw& draw, const Vector<Point>& vertices,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolygon(draw, vertices.Begin(), vertices.GetCount(), color, width, outline, pattern, doxor);
}

/*
static void StreamEllipse(Stream& stream, Rect& rc, Color& color, Color& outline, int& outline_width)
{
	int version = 1;
	stream / version % rc % color % outline / outline_width;
}

static void wsEllipse(Draw& draw, Stream& stream, const DrawingPos& dp)
{
//	TIMING("wsEllipse")
	Rect rc;
	Color color, outline;
	int outline_width;
	StreamEllipse(stream, rc, color, outline, outline_width);
	dp.Transform(rc);
	dp.TransformW(outline_width);
	DrawEllipse(draw, rc, color, outline, outline_width);
}

static DrawerRegistrator MK__s(wEllipse, wsEllipse);

void DrawEllipse(Draw& draw, const Rect& rc, Color color, Color outline, int outline_width)
{
	if(draw.IsDrawing())
	{
		Rect rtmp = rc;
		StreamEllipse(draw.DrawingOp(wEllipse), rtmp, color, outline, outline_width);
		return;
	}

#if defined(PLATFORM_WIN32)
	draw.SetDrawPen(outline_width, outline);
	draw.SetColor(color);
	DrawEllipse(draw, rc);
#elif defined(PLATFORM_X11)
	XGCValues gcv, gcv_old;
	enum { GC_BACKUP = GCForeground | GCArcMode | GCLineWidth };
	XGetGCValues(Xdisplay, draw.GetGC(), GC_BACKUP, &gcv_old);
	Point offset = draw.GetOffset();
	if(!IsNull(color))
	{
		gcv.foreground = GetXPixel(color);
		gcv.arc_mode = ArcChord;
		XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCArcMode, &gcv);
		XFillArc(Xdisplay, draw.GetDrawable(), draw.GetGC(),
			rc.left + offset.x, rc.top + offset.y, rc.Width(), rc.Height(), 0, 360 * 64);
	}
	if(!IsNull(outline))
	{
		gcv.foreground = GetXPixel(outline);
		gcv.line_width = outline_width;
		XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCLineWidth, &gcv);
		XDrawArc(Xdisplay, draw.GetDrawable(), draw.GetGC(),
			rc.left + offset.x, rc.top + offset.y, rc.Width() - 1, rc.Height() - 1, 0, 360 * 64);
	}
	XChangeGC(Xdisplay, draw.GetGC(), GC_BACKUP, &gcv_old);
#else
	#error
#endif//PLATFORM
}
*/

/*
static void StreamArc(Stream& stream, Rect& rc, Point& start, Point& end, Color& color, int& width)
{
	int version = 1;
	stream / version % rc % start % end % color / width;
}

static void wsArc(Draw& draw, Stream& stream, const DrawingPos& dp)
{
//	TIMING("wsEllipse")
	Rect rc;
	Point start, end;
	Color color;
	int width;
	StreamArc(stream, rc, start, end, color, width);
	dp.Transform(start);
	dp.Transform(end);
	dp.TransformW(width);
	DrawArc(draw, rc, start, end, color, width);
}

static DrawerRegistrator MK__s(wArc, wsArc);

void DrawArc(Draw& draw, const Rect& rc, Point start, Point end, Color color, int width)
{
	if(draw.IsDrawing())
	{
		Rect rtmp = rc;
		StreamArc(draw.DrawingOp(wArc), rtmp, start, end, color, width);
		return;
	}

#if defined(PLATFORM_WIN32)
	draw.SetDrawPen(width, color);
	DrawArc(draw.GetHandle(), rc, start, end);
#elif defined(PLATFORM_X11)
	XGCValues gcv, gcv_old;
	XGetGCValues(Xdisplay, draw.GetGC(), GCForeground, &gcv_old);
	Point offset = draw.GetOffset();
	gcv.foreground = GetXPixel(color);
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground, &gcv);
	Point centre = rc.CenterPoint();
	int angle1 = fround(360 * 64 / TWOPI * atan2(start.x - centre.x, centre.y - start.y));
	int angle2 = fround(360 * 64 / TWOPI * atan2(end.x - centre.x, centre.y - end.y));
	if(angle2 <= angle1)
		angle2 += 360 * 64;
	XDrawArc(Xdisplay, draw.GetDrawable(), draw.GetGC(), rc.left + offset.x, rc.top + offset.y,
		rc.Width(), rc.Height(), angle1, angle2);
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground, &gcv_old);
#else
	#error
#endif
}
*/

/*
Size ImageDisplay::GetStdSize(const Value& q) const
{
	return Image(q).GetSize();
}
*/

/*
void ImageDisplay::Paint(Draw& draw, const Rect& rc, const Value& q, Color ink, Color paper, dword style) const
{
	draw.DrawRect(rc, paper);
	Image image(q);

	if(!image)
		return;

	Size size = image.GetSize();

	Point pos;
	switch(halign)
	{
	case ALIGN_LEFT:   pos.x = rc.left; break;
	case ALIGN_RIGHT:  pos.x = rc.right - size.cx; break;
	default:           pos.x = (rc.left + rc.right - size.cx) >> 1; break;
	}
	switch(valign)
	{
	case ALIGN_TOP:    pos.y = rc.top; break;
	case ALIGN_BOTTOM: pos.y = rc.bottom - size.cy; break;
	default:           pos.y = (rc.top + rc.bottom - size.cy) >> 1; break;
	}

	switch(layout)
	{
	default:
		NEVER();
		break;

	case SINGLE:
		draw.DrawImage(pos.x, pos.y, image);
		break;

	case STRETCH:
		draw.DrawImage(rc, image);
		break;

	case TILE:
		pos -= size * idivceil(pos - rc.TopLeft(), size);
		for(Point at = pos; at.y < rc.bottom; at.y += size.cy)
			for(at.x = pos.x; at.x < rc.right; at.x += size.cx)
				draw.DrawImage(at.x, at.y, image);
		break;
	}
}
*/

/*
const ImageDisplay& CenterImageDisplay()
{
	static ImageDisplay display(ImageDisplay::SINGLE);
	return display;
}
*/

/*
const ImageDisplay& StretchImageDisplay()
{
	static ImageDisplay display(ImageDisplay::STRETCH);
	return display;
}
*/

/*
const ImageDisplay& TileImageDisplay()
{
	static ImageDisplay display(ImageDisplay::TILE);
	return display;
}
*/

GradientDisplay::GradientDisplay(Point a, Color ca, Point b, Color cb, double gamma)
: a(a), ca(ca), b(b), cb(cb), gamma(gamma)
{
}

static void DrawGradient(Draw& draw, const Rect& rc, Point A, Color cA, Point B, Color cB, double gamma = 2.5, int step = 16)
{
#ifdef PLATFORM_WIN32
	Point N = B - A;
	if(!(N.x | N.y))
		N = Size(1, 1);
	int shift = ScalarProduct(A, N);
	int scale = ScalarProduct(B, N) - shift;

#ifndef SYSTEMDRAW
	if(draw.IsDrawing())
	{
		int block = 16;
		for(int x = rc.left; x < rc.right; x += block)
			for(int y = rc.top; y < rc.bottom; y += block)
				draw.DrawRect(x, y, block, block,
					GetColorGradient(cA, cB, ScalarProduct(Point(x, y), N) - shift, scale, gamma));
		return;
	}
#endif

#ifdef SYSTEMDRAW
	SystemDraw *w = dynamic_cast<SystemDraw *>(&draw);
	if(w) { SystemDraw& draw = *w;
#endif

	draw.BeginGdi();
	SelectObject(draw, GetStockObject(NULL_PEN));
	SelectObject(draw, GetStockObject(NULL_BRUSH));

	Point pt = draw.LPtoDP(Point(0, 0));
	SetBrushOrgEx(draw, pt.x, pt.y, NULL);

	Point poly[4];
	int l, h, d;
	bool horz = (abs(N.x) >= abs(N.y));
	if(horz)
	{ // major x: track x axis
		d = iscale(-N.y, rc.Height(), N.x);
		l = rc.left;
		h = rc.right;
		poly[0].y = poly[1].y = rc.top;
		poly[2].y = poly[3].y = rc.bottom;
	}
	else
	{
		d = iscale(-N.x, rc.Width(), N.y);
		l = rc.top;
		h = rc.bottom;
		poly[0].x = poly[1].x = rc.left;
		poly[2].x = poly[3].x = rc.right;
	}
	l -= max(d, 0);
	h -= min(d, 0);

	for(; l < h; l += step)
	{
		if(horz)
			poly[3].x = (poly[2].x = (poly[1].x = (poly[0].x = l) + step) + d) - step;
		else
			poly[3].y = (poly[2].y = (poly[1].y = (poly[0].y = l) + step) + d) - step;
		HGDIOBJ old_brush = SelectObject(draw, CreateSolidBrush(
			GetColorGradient(cA, cB, ScalarProduct(poly[0], N) - shift, scale, gamma)));
		Polygon(draw, *poly, 4);
		DeleteObject(SelectObject(draw, old_brush));
	}

	draw.EndGdi();

#ifdef SYSTEMDRAW
	}
#endif

#endif
}

Point GradientDisplay::GetRectPoint(Point P, const Rect& rc)
{
	P.x += (P.x >= 0 ? rc.left : rc.right);
	P.y += (P.y >= 0 ? rc.top : rc.bottom);
	return P;
}

void GradientDisplay::Paint(Draw& draw, const Rect& rc, const Value& q, Color ink, Color paper, dword style) const
{
	if(!IsNull(ca))
		ink = ca;
	if(!IsNull(cb))
		paper = cb;
	DrawGradient(draw, rc, GetRectPoint(a, rc), ink, GetRectPoint(b, rc), paper, gamma);
}

PrinterInfo::PrinterInfo()
: page_size(0, 0)
{
#if defined(PLATFORM_WIN32) && defined(flagGUI)
	devmode = NULL;
	PAGESETUPDLG psd;
	Zero(psd);
	psd.lStructSize = sizeof(psd);
	psd.Flags = PSD_RETURNDEFAULT | PSD_INHUNDREDTHSOFMILLIMETERS;
	if(!PageSetupDlg(&psd))
		return;
	if(psd.hDevMode)
	{
		const DEVMODE *dm = (const DEVMODE *)GlobalLock(psd.hDevMode);
		devmode_data = String((const char *)dm, dm -> dmSize + dm -> dmDriverExtra);
		devmode = (const DEVMODE *)devmode_data.Begin();
		GlobalUnlock(psd.hDevMode);
		GlobalFree(psd.hDevMode);
	}
	if(psd.hDevNames)
	{
		const DEVNAMES *dn = (const DEVNAMES *)GlobalLock(psd.hDevNames);
		const char *p = (const char *)dn;
		driver = String(p + dn -> wDriverOffset);
		device = String(p + dn -> wDeviceOffset);
		output = String(p + dn -> wOutputOffset);
		GlobalUnlock(psd.hDevNames);
		GlobalFree(psd.hDevNames);
	}
	HDC hdc = CreateIC(driver, device, output, devmode);
	if(hdc)
	{
		Size mm_size(GetDeviceCaps(hdc, HORZSIZE), GetDeviceCaps(hdc, VERTSIZE));
		page_size = iscale(mm_size, 6000, 254);
		DeleteDC(hdc);
	}
#endif
}

END_UPP_NAMESPACE
