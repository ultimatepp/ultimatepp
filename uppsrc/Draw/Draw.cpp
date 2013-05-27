#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x)    // RLOG(x)
#define LTIMING(x) // RTIMING(x)
//#define BENCH

static StaticMutex sDrawLock;

INITBLOCK {
	Value::Register<Painting>("Painting");
	Value::Register<Drawing>("Drawing");
}

void Draw::SinCos(int angle, double& sina, double& cosa)
{
	angle = angle % 3600;
	switch(angle) {
	case    0: sina =  0; cosa =  1; break;
	case  900: sina =  1; cosa =  0; break;
	case 1800: sina =  0; cosa = -1; break;
	case 2700: sina = -1; cosa =  0; break;
	default:
		double a = angle * M_PI / 1800.0;
		sina = sin(a);
		cosa = cos(a);
		break;
	}
}

Draw::~Draw() {}

Size Draw::GetPixelsPerInch() const
{
	return IsNative() ? GetNativeDpi() : Dots() ? Size(600, 600) : Size(96, 96);
}

Size Draw::GetPageMMs() const
{
	return GetPageSize() * 254 / (10 * GetPixelsPerInch());
}

int Draw::GetNativeX(int x) const
{
	Size sz = GetNativeDpi();
	return Dots() && sz.cx != 600 ? iscale(x, sz.cx, 600) : x;
}

int Draw::GetNativeY(int y) const
{
	Size sz = GetNativeDpi();
	return Dots() && sz.cx != 600 ? iscale(y, sz.cy, 600) : y;
}

void Draw::Native(int& x, int& y) const
{
	x = GetNativeX(x);
	y = GetNativeY(y);
}

void Draw::Native(Point& p) const
{
	Native(p.x, p.y);
}

void Draw::Native(Size& sz) const
{
	Native(sz.cx, sz.cy);
}

void Draw::Native(Rect& r) const
{
	Native(r.left, r.top);
	Native(r.right, r.bottom);
}

#ifdef BENCH
static TimingInspector sDrawTiming("DRAW");
#endif

void EnterDraw() {
	sDrawLock.Enter();
#ifdef BENCH
	sDrawTiming.Start();
#endif
}

void LeaveDraw() {
#ifdef BENCH
	sDrawTiming.End();
#endif
	sDrawLock.Leave();
}

Size Draw::GetPageSize() const
{
	return Size(INT_MAX / 2, INT_MAX / 2);
}

Rect Draw::GetPaintRect() const
{
	return Rect(-(INT_MAX / 2), -(INT_MAX / 2), INT_MAX / 2, INT_MAX / 2);
}


void Draw::StartPage() {}
void Draw::EndPage() {}

Size Draw::GetNativeDpi() const { return Dots() ? Size(600, 600) : Size(96, 96); }
void Draw::BeginNative() {}
void Draw::EndNative() {}

int Draw::GetCloffLevel() const { return 0; }

// -------------------------------

void Draw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	NEVER();
}

void Draw::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	if(src == Rect(img.GetSize()))
		SysDrawImageOp(x, y, img, color);
	else {
		Clipoff(x, y, src.GetWidth(), src.GetHeight());
		SysDrawImageOp(-src.left, -src.top, img, color);
		End();
	}
}

void Draw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	LTIMING("DrawImageOp");
	bool tonative = !IsNative();
	if(tonative) {
		BeginNative();
		Native(x, y);
		Native(cx, cy);
	}
	Size sz = Size(cx, cy);
	if((cx > 2000 || cy > 1500) && IsNull(color) && IsPrinter()) {
		int yy = 0;
		ImageRaster ir(img);
		RescaleImage rm;
		rm.Create(Size(cx, cy), ir, src);
		while(yy < cy) {
			int ccy = min(cy - yy, 16);
			ImageBuffer ib(cx, ccy);
			for(int q = 0; q < ccy; q++)
				rm.Get(ib[q]);
			DrawImageBandRLE(*this, x, y + yy, ib, 16);
			yy += ccy;
		}
	}
	else
	if(src.GetSize() == sz)
		SysDrawImageOp(x, y, img, src, color);
	else {
		Image h = Rescale(img, Size(cx, cy), src);
		SysDrawImageOp(x, y, h, h.GetSize(), color);
	}
	if(tonative)
		EndNative();
}

// -------------------------------

void Draw::DrawRect(const Rect& rect, Color color)
{
	DrawRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight(), color);
}

void Draw::DrawRect(int x, int y, int cx, int cy, Color color)
{
	DrawRectOp(x, y, cx, cy, color);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img, const Rect& src)
{
	DrawImageOp(x, y, cx, cy, img, src, Null);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img)
{
	DrawImage(x, y, cx, cy, img, img.GetSize());
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	if(IsNull(color)) return;
	DrawImageOp(x, y, cx, cy, img, src, color);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img, Color color)
{
	if(IsNull(color)) return;
	DrawImage(x, y, cx, cy, img, img.GetSize(), color);
}

void Draw::DrawImage(const Rect& r, const Image& img, const Rect& src)
{
	DrawImage(r.left, r.top, r.Width(), r.Height(), img, src);
}

void Draw::DrawImage(const Rect& r, const Image& img)
{
	DrawImage(r.left, r.top, r.Width(), r.Height(), img);
}

void Draw::DrawImage(const Rect& r, const Image& img, const Rect& src, Color color)
{
	if(IsNull(color)) return;
	DrawImage(r.left, r.top, r.Width(), r.Height(), img, src, color);
}

void Draw::DrawImage(const Rect& r, const Image& img, Color color)
{
	if(IsNull(color)) return;
	DrawImage(r.left, r.top, r.Width(), r.Height(), img, color);
}

void Draw::DrawImage(int x, int y, const Image& img, const Rect& src)
{
	Size sz = src.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, src, Null);
}

void Draw::DrawImage(int x, int y, const Image& img)
{
	Size sz = img.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, img.GetSize(), Null);
}

void Draw::DrawImage(int x, int y, const Image& img, const Rect& src, Color color)
{
	if(IsNull(color)) return;
	DrawImageOp(x, y, src.Width(), src.Height(), img, src, color);
}

void Draw::DrawImage(int x, int y, const Image& img, Color color)
{
	if(IsNull(color)) return;
	Size sz = img.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, sz, color);
}

void Draw::DrawData(int x, int y, int cx, int cy, const String& data, const char *type)
{
	DrawDataOp(x, y, cx, cy, data, type);
}

void Draw::DrawData(const Rect& r, const String& data, const char *type)
{
	DrawDataOp(r.left, r.top, r.GetWidth(), r.GetHeight(), data, type);
}

void Draw::DrawLine(Point p1, Point p2, int width, Color color)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y, width, color);
}

void Draw::DrawLine(int x1, int y1, int x2, int y2, int width, Color color)
{
	DrawLineOp(x1, y1, x2, y2, width, color);
}

void Draw::DrawPolyPolyline(const Point *vertices, int vertex_count,
                            const int *counts, int count_count,
                            int width, Color color, Color doxor)
{
	DrawPolyPolylineOp(vertices, vertex_count, counts, count_count, width,
	                 color, doxor);
}

void Draw::DrawPolyPolyline(const Vector<Point>& vertices, const Vector<int>& counts,
                            int width, Color color, Color doxor)
{
	DrawPolyPolyline(vertices.Begin(), vertices.GetCount(),
		             counts.Begin(), counts.GetCount(),
		             width, color, doxor);
}

void Draw::DrawPolyline(const Point *vertices, int count,
                        int width, Color color, Color doxor)
{
	DrawPolyPolyline(vertices, count, &count, 1, width, color, doxor);
}

void Draw::DrawPolyline(const Vector<Point>& vertices,
                        int width, Color color, Color doxor)
{
	DrawPolyline(vertices.Begin(), vertices.GetCount(), width, color, doxor);
}

void Draw::DrawPolyPolyPolygon(const Point *vertices, int vertex_count,
                              const int *subpolygon_counts, int subpolygon_count_count,
                              const int *disjunct_polygon_counts,
                              int disjunct_polygon_count_count, Color color,
                              int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygonOp(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
	                      disjunct_polygon_counts, disjunct_polygon_count_count, color,
	                      width, outline, pattern, doxor);
}

void Draw::DrawPolyPolyPolygon(const Vector<Point>& vertices,
                           const Vector<int>& subpolygon_counts,
                           const Vector<int>& disjunct_polygon_counts,
                           Color color, int width, Color outline,
                           uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices.Begin(), vertices.GetCount(),
	                    subpolygon_counts.Begin(), subpolygon_counts.GetCount(),
	                    disjunct_polygon_counts.Begin(), disjunct_polygon_counts.GetCount(),
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolyPolygon(const Point *vertices, int vertex_count,
                       const int *subpolygon_counts, int subpolygon_count_count,
                       Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count,
	                    subpolygon_counts, subpolygon_count_count, &vertex_count, 1,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolyPolygon(const Vector<Point>& vertices, const Vector<int>& subpolygon_counts,
                       Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolygon(vertices.Begin(), vertices.GetCount(),
	                subpolygon_counts.Begin(), subpolygon_counts.GetCount(),
	                color, width, outline, pattern, doxor);
}

void Draw::DrawPolygons(const Point *vertices, int vertex_count,
                    const int *polygon_counts, int polygon_count_count,
                    Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count,
	                    polygon_counts, polygon_count_count,
	                    polygon_counts, polygon_count_count,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolygons(const Vector<Point>& vertices, const Vector<int>& polygon_counts,
                    Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolygons(vertices.Begin(), vertices.GetCount(),
                 polygon_counts.Begin(), polygon_counts.GetCount(),
                 color, width, outline, pattern, doxor);
}

void Draw::DrawPolygon(const Point *vertices, int vertex_count,
                   Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count, &vertex_count, 1, &vertex_count, 1,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolygon(const Vector<Point>& vertices,
                       Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolygon(vertices.Begin(), vertices.GetCount(), color, width, outline, pattern, doxor);
}

void Draw::DrawEllipse(int x, int y, int cx, int cy, Color color, int pen, Color pencolor)
{
	DrawEllipse(RectC(x, y, cx, cy), color, pen, pencolor);
}

void Draw::DrawEllipse(const Rect& r, Color color, int pen, Color pencolor)
{
	DrawEllipseOp(r, color, pen, pencolor);
}

void Draw::DrawArc(const Rect& rc, Point start, Point end, int width, Color color)
{
	DrawArcOp(rc, start, end, width, color);
}

void Draw::Offset(int x, int y)
{
	Offset(Point(x, y));
}

bool Draw::Clip(int x, int y, int cx, int cy)
{
	return Clip(RectC(x, y, cx, cy));
}

bool Draw::Clipoff(int x, int y, int cx, int cy)
{
	return Clipoff(RectC(x, y, cx, cy));
}

bool Draw::ExcludeClip(int x, int y, int cx, int cy)
{
	return ExcludeClip(RectC(x, y, cx, cy));
}

bool Draw::IntersectClip(int x, int y, int cx, int cy)
{
	return IntersectClip(RectC(x, y, cx, cy));
}

bool Draw::IsPainting(int x, int y, int cx, int cy) const
{
	return IsPainting(RectC(x, y, cx, cy));
}

static void  (*sIgfn)(ImageBuffer& ib, const Painting& pw, Size sz, Point pos, int mode);
static void  (*sIwfn)(ImageBuffer& ib, const Drawing& p, int mode);
static Image (*sRG)(Point at, int angle, int chr, Font fnt, Color color, Size sz);

void RegisterPaintingFns__(void (*ig)(ImageBuffer& ib, const Painting& pw, Size sz, Point pos, int mode),
                           void (*iw)(ImageBuffer& ib, const Drawing& p, int mode),
                           Image (*rg)(Point at, int angle, int chr, Font fnt, Color color, Size sz))
{
	sIgfn = ig;
	sIwfn = iw;
	sRG = rg;
}

bool HasPainter()
{
	return sIgfn && sIwfn && sRG;
}

Image RenderGlyphByPainter(Point at, int angle, int chr, Font fnt, Color color, Size sz)
{
	return sRG ? (*sRG)(at, angle, chr, fnt, color, sz) : Image();
}

void PaintImageBuffer(ImageBuffer& ib, const Painting& p, Size sz, Point pos, int mode)
{
	if(sIgfn)
		(*sIgfn)(ib, p, sz, pos, mode);
}

void PaintImageBuffer(ImageBuffer& ib, const Painting& p, int mode)
{
	PaintImageBuffer(ib, p, ib.GetSize(), Point(0, 0), mode);
}

void PaintImageBuffer(ImageBuffer& ib, const Drawing& iw, int mode)
{
	if(sIwfn)
		(*sIwfn)(ib, iw, mode);
}

Drawing AsDrawing(const Painting& pw)
{
	Size sz = pw.GetSize();
	DrawingDraw dw(sz);
	dw.DrawPainting(sz, pw);
	return dw.GetResult();
}

void Draw::DrawPaintingOp(const Rect& target, const Painting& pw)
{
	if(!HasPainter())
		return;
	Size sz = target.GetSize();
	if((sz.cx > 2000 || sz.cy > 1500) && IsPrinter()) {
		int yy = 0;
		while(yy < sz.cy) {
			int ccy = min(sz.cy - yy, 100);
			ImageBuffer ib(sz.cx, ccy);
			Fill(~ib, IsPrinter() ? White() : Null, ib.GetLength());
			PaintImageBuffer(ib, pw, sz, Point(0, yy), true);
			DrawImageBandRLE(*this, target.left, target.top + yy, ib, 16);
			yy += ccy;
		}
	}
	else {
		ImageBuffer ib(sz);
		Fill(~ib, IsPrinter() ? White() : Null, ib.GetLength());
		PaintImageBuffer(ib, pw, sz, Point(0, 0), IsPrinter());
		DrawImage(target.left, target.top, ib);
	}
}

void Draw::DrawPainting(int x, int y, int cx, int cy, const Painting& ig)
{
	DrawPainting(RectC(x, y, cx, cy), ig);
}

void Draw::DrawPainting(int x, int y, const Painting& iw)
{
	Size sz = iw.GetSize();
	DrawPainting(RectC(x, y, sz.cx, sz.cy), iw);
}

// ---------------------------

dword NilDraw::GetInfo() const { return DOTS; }
Size NilDraw::GetPageSize() const { return Size(0, 0); }
void NilDraw::BeginOp() {}
void NilDraw::EndOp() {}
void NilDraw::OffsetOp(Point p) {}
bool NilDraw::ClipOp(const Rect& r) { return false; }
bool NilDraw::ClipoffOp(const Rect& r) { return false; }
bool NilDraw::ExcludeClipOp(const Rect& r) { return false; }
bool NilDraw::IntersectClipOp(const Rect& r) { return false; }
bool NilDraw::IsPaintingOp(const Rect& r) const { return false; }
Rect NilDraw::GetPaintRect() const { return Rect(0, 0, 0, 0); }
void NilDraw::DrawRectOp(int x, int y, int cx, int cy, Color color) {}
void NilDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color) {}
void NilDraw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id) {}
void NilDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color) {}
void NilDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor) {}
void NilDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor) {}
void NilDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color) {}
void NilDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor) {}
void NilDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx) {}
void NilDraw::DrawDrawingOp(const Rect& target, const Drawing& w) {}
void NilDraw::DrawPaintingOp(const Rect& target, const Painting& w) {}

END_UPP_NAMESPACE
