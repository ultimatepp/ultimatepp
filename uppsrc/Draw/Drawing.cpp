#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x)  // DLOG(x)

#ifdef COMPILER_MSC
#pragma warning(disable: 4700)
#endif

static void StreamUnpackPoints(Stream& stream, Point *out, int count)
{
	if(count == 0)
		return;
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(Point) == 8) {
		stream.Get(out, 8 * count);
		return;
	}
#endif
	Point *end = out + count;
	byte *top = reinterpret_cast<byte *>(end) - count * 8;
	stream.Get(top, count * 8);
	for(; out < end; out++, top += 8) {
		out -> x = (short)Peek32le(top + 0);
		out -> y = (short)Peek32le(top + 4);
	}
}

static void StreamPackPoints(Stream& stream, const Point *in, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(Point) == 8) {
		stream.Put(in, 8 * count);
		return;
	}
#endif
	enum { PART = 1024 };
	byte part[PART * 8];
	while(count > 0)
	{
		int part_count = min<int>(count, PART);
		for(byte *pp = part, *pe = pp + 8 * part_count; pp < pe; pp += 8, in++) {
			Poke32le(pp + 0, in -> x);
			Poke32le(pp + 4, in -> y);
		}
		stream.Put(part, part_count * 4);
		count -= part_count;
	}
}

static void StreamUnpackInts(Stream& stream, int *out, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == 4) {
		stream.Get(out, count * 4);
		return;
	}
#endif
	while(count--)
		*out++ = stream.Get32le();
}

static void StreamPackInts(Stream& stream, const int *in, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == 4) {
		stream.Put(in, count * 4);
		return;
	}
#endif
	while(count--)
		stream.Put32le(*in++);
}

// ------------------------------

Stream& DrawingDraw::DrawingOp(int code)
{
	ASSERT(IsDrawing());
	drawing / code;
	return drawing;
}

void DrawingDraw::DrawingBegin()
{
	Cloff& w = cloff.Add();
	w.org = actual_offset;
	w.drawingclip = drawingclip;
}

void DrawingDraw::BeginOp()
{
	DrawingOp(BEGIN);
	DrawingBegin();
}

void DrawingDraw::OffsetOp(Point p)
{
	DrawingOp(OFFSET) % p;
	DrawingBegin();
	actual_offset += p;
}

bool DrawingDraw::ClipOp(const Rect& r)
{
	DrawingOp(CLIP) % const_cast<Rect&>(r);
	DrawingBegin();
	drawingclip &= r + actual_offset;
	return !drawingclip.IsEmpty();
}

bool DrawingDraw::ClipoffOp(const Rect& r)
{
	DrawingOp(CLIPOFF) % const_cast<Rect&>(r);
	DrawingBegin();
	drawingclip &= r + actual_offset;
	actual_offset += r.TopLeft();
	return !drawingclip.IsEmpty();
}

void DrawingDraw::EndOp()
{
	DrawingOp(END);
	ASSERT(cloff.GetCount());
	Cloff& w = cloff.Top();
	actual_offset = w.org;
	drawingclip = w.drawingclip;
	cloff.Drop();
}

bool DrawingDraw::ExcludeClipOp(const Rect& r)
{
	DrawingOp(EXCLUDECLIP) % const_cast<Rect&>(r);
	return !drawingclip.IsEmpty();
}

bool DrawingDraw::IntersectClipOp(const Rect& r)
{
	DrawingOp(INTERSECTCLIP) % const_cast<Rect&>(r);
	drawingclip &= r;
	return !drawingclip.IsEmpty();
}

Rect DrawingDraw::GetClipOp() const
{
	return drawingclip - actual_offset;
}

bool DrawingDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

void DrawingDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	if(!IsNull(color))
		DrawingOp(DRAWRECT) % x % y % cx % cy % color;
}

void DrawingDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	Rect s = src;
	DrawingOp(DRAWIMAGE) % x % y % cx % cy % s % color;
	val.Add(img);
}

void DrawingDraw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
	String h = id;
	DrawingOp(DRAWDATA) % x % y % cx % cy % h;
	val.Add(data);
}

void DrawingDraw::DrawDrawingOp(const Rect& target, const Drawing& w)
{
	DrawingOp(DRAWDRAWING) % const_cast<Rect&>(target);
	val.Add(w);
}

void DrawingDraw::DrawPaintingOp(const Rect& target, const Painting& w)
{
	DrawingOp(DRAWPAINTING) % const_cast<Rect&>(target);
	val.Add(w);
}

void DrawingDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	DrawingOp(DRAWLINE) % x1 % y1 % x2 % y2 % width % color;
}

void DrawingDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                 const int *counts, int count_count,
	                                 int width, Color color, Color doxor)
{
	ASSERT(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || IsNull(color))
		return;
	DrawingOp(DRAWPOLYPOLYLINE);
	int version = 2;
	drawing / version;
	drawing % width % color % doxor;
	drawing % vertex_count % count_count;
	StreamPackPoints(drawing, vertices, vertex_count);
	StreamPackInts(drawing, counts, count_count);
}

void DrawingDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                   const int *subpolygon_counts, int subpolygon_count_count,
	                   const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	                   Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	if(vertex_count == 0)
		return;
	DrawingOp(DRAWPOLYPOLYPOLYGON);
	int version = 2;
	drawing / version;
	drawing % color % width % outline % pattern % doxor;
	drawing % vertex_count % subpolygon_count_count % disjunct_polygon_count_count;
	StreamPackPoints(drawing, vertices, vertex_count);
	StreamPackInts(drawing, subpolygon_counts, subpolygon_count_count);
	StreamPackInts(drawing, disjunct_polygon_counts, disjunct_polygon_count_count);
}

void DrawingDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	DrawingOp(DRAWELLIPSE) % const_cast<Rect&>(r) % color / pen % pencolor;
}

void DrawingDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	DrawingOp(DRAWARC) % const_cast<Rect&>(rc) % start % end % color % width;
}

void DrawingDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                             int n, const int *dx) {
	if(IsNull(ink)) return;
	if(n < 0)
		n = wstrlen((const wchar *)text);
	if(n == 0)
		return;
	Stream& s = DrawingOp(DRAWTEXT);
	byte cs = CHARSET_UNICODE;
	s % x % y % angle % font % ink / n % cs;
	s.PutW((wchar *)text, n);
	bool dxb = dx;
	s % dxb;
	if(dx) {
		int *w = const_cast<int *>(dx);
		while(n--)
			s / *w++;
	}
}

Drawing DrawingDraw::GetResult()
{
	Drawing out;
	out.size = size;
	LLOG("GetResult size: " << size);
	out.data = drawing.GetResult();
	out.val = val;
	return out;
}

// ------------------------------

struct Draw::DrawingPos : StringStream {
	Size    source;
	Size    target;
	Point   srcoff;
	Point   trgoff;
	Point   trgini;
	Vector<Point16> stack;

	bool  Identity() const                               { return source == target; }

	int   GetX(int x) const;
	int   GetY(int y) const;
	int   GetCx(int cx) const;
	int   GetCy(int cy) const;
	int   GetW(int w) const;
	Point Get(int x, int y) const;
	Point Get(Point p) const;
	Rect  Get(const Rect& r) const;
	Rect  Get(int x, int y, int cx, int cy) const;

	Point operator()(int x, int y) const                 { return Get(x, y); }
	Point operator()(Point p) const                      { return Get(p); }
	Rect  operator()(const Rect& r) const                { return Get(r); }
	Rect  operator()(int x, int y, int cx, int cy) const { return Get(x, y, cx, cy); }

	void TransformX(int& x) const                        { x = GetX(x); }
	void TransformY(int& y) const                        { y = GetY(y); }
	void TransformW(int& w) const                        { w = GetW(w); }
	void Transform(int& x, int& y) const                 { TransformX(x); TransformY(y); }
	void Transform(Point& p) const                       { p = Get(p); }
	void Transform(Rect& r) const                        { r = Get(r); }
	
	Rect  GetRect();
	
	DrawingPos(const String& src) : StringStream(src) {}
};

Rect Draw::DrawingPos::GetRect()
{
	Rect r;
	*this % r;
	return Get(r);
}

int   Draw::DrawingPos::GetX(int x) const {
	return iscale(x + srcoff.x, target.cx, source.cx) - trgoff.x;
}

int   Draw::DrawingPos::GetY(int y) const {
	return iscale(y + srcoff.y, target.cy, source.cy) - trgoff.y;
}

int   Draw::DrawingPos::GetCx(int cx) const {
	return iscale(cx, target.cx, source.cx);
}

int   Draw::DrawingPos::GetCy(int cy) const {
	return iscale(cy, target.cy, source.cy);
}

int   Draw::DrawingPos::GetW(int w) const {
	return iscale(w, target.cx + target.cy, source.cx + source.cy);
}

Point Draw::DrawingPos::Get(int x, int y) const {
	return Point(GetX(x), GetY(y));
}

Point Draw::DrawingPos::Get(Point p) const {
	return Get(p.x, p.y);
}

Rect  Draw::DrawingPos::Get(const Rect& r) const {
	return Rect(GetX(r.left), GetY(r.top), GetX(r.right), GetY(r.bottom));
}

Rect  Draw::DrawingPos::Get(int x, int y, int cx, int cy) const {
	return Get(RectC(x, y, cx, cy));
}

void Draw::DrawDrawingOp(const Rect& target, const Drawing& w) {
#ifdef _DEBUG
	int cl = GetCloffLevel();
#endif
	DrawingPos ps(w.data);
	ps.srcoff = ps.trgoff = Point(0, 0);
	ps.target = target.Size();
	ps.source = w.size;
	LLOG("DrawDrawingOp size: " << w.size);
	if(ps.target.cx == 0 || ps.target.cy == 0 || ps.source.cx == 0 || ps.source.cy == 0)
		return;
	Clipoff(target);
	ps.trgini = GetOffset();
	Rect r;
	int   x, y, cx, cy, width, vertex_count, count_count;
	Color color, pencolor, doxor;
	Image img;
	Drawing dw;
	Painting sw;
	Point p, p1;
	int vi = 0;
	while(!ps.IsEof()) {
		int code;
		ps / code;
		switch(code) {
		case BEGIN: 
			Begin();
			ps.stack.Add(ps.srcoff);
			break;
		case OFFSET:
			ps % p;
			Offset(ps(p));
			ps.stack.Add(ps.srcoff);
			ps.srcoff += p;
			ps.trgoff = GetOffset() - ps.trgini;
			break;
		case CLIP:
			Clip(ps.GetRect());
			ps.stack.Add(ps.srcoff);
			break;
		case CLIPOFF:
			ps % r;
			Clipoff(ps(r));
			ps.stack.Add(ps.srcoff);
			ps.srcoff += r.TopLeft();
			ps.trgoff = GetOffset() - ps.trgini;
			break;
		case EXCLUDECLIP:
			ExcludeClip(ps.GetRect());
			break;
		case INTERSECTCLIP:
			IntersectClip(ps.GetRect());
			break;
		case END:
			End();
			ps.srcoff = ps.stack.Pop();
			ps.trgoff = GetOffset() - ps.trgini;
			break;
		case DRAWRECT:
			ps % x % y % cx % cy % color;
			DrawRect(ps(x, y, cx, cy), color);
			break;
		case DRAWIMAGE:
			ps % x % y % cx % cy;
			if(w.val.GetCount())
				img = w.val[vi++];
			else
				ps % img;
			ps % r % color;
			DrawImageOp(ps.GetX(x), ps.GetY(y), ps.GetCx(cx), ps.GetCy(cy), img, r, color);
			break;
		case DRAWDATA:
			{
				String data, id;
				ps % x % y % cx % cy % id;
				if(w.val.GetCount())
					data = w.val[vi++];
				else
					ps % data;
				DrawData(ps(x, y, cx, cy), data, id);
			}
			break;
		case DRAWDRAWING:
			if(w.val.GetCount())
				dw = w.val[vi++];
			else
				ps % dw;
			DrawDrawing(ps.GetRect(), dw);
			break;
		case DRAWPAINTING:
			if(w.val.GetCount())
				sw = w.val[vi++];
			else
				ps % sw;
			DrawPainting(ps.GetRect(), sw);
			break;
		case DRAWLINE:
			ps % x % y % cx % cy % width % color;
			DrawLine(ps(x, y), ps(cx, cy), width > 0 ? ps.GetW(width) : width, color);
			break;
		case DRAWELLIPSE:
			r = ps.GetRect();
			ps % color / width % pencolor;
			DrawEllipse(r, color, width > 0 ? ps.GetW(width) : width, pencolor);
			break;
#ifndef PLATFORM_WINCE
		case DRAWARC:
			r = ps.GetRect();
			ps % p % p1 % color % width;
			DrawArc(r, ps(p), ps(p1), width > 0 ? ps.GetW(width) : width, color);
			break;
		case DRAWPOLYPOLYLINE:
			{
				int version;
				ps / version;
				ps % width % color % doxor;
				ps % vertex_count % count_count;
				Buffer<Point> vertices(vertex_count);
				Buffer<int> counts(count_count);
				StreamUnpackPoints(ps, vertices, vertex_count);
				StreamUnpackInts(ps, counts, count_count);
				if(!ps.Identity()) {
					for(Point *p = vertices, *e = p + vertex_count; p < e; p++)
						ps.Transform(*p);
					if(width > 0)
						ps.TransformW(width);
				}
				DrawPolyPolyline(vertices, vertex_count, counts, count_count, width, color, doxor);
			}
			break;
		case DRAWPOLYPOLYPOLYGON:
			{
				Color outline;
				uint64 pattern;
				int subpolygon_count_count, disjunct_polygon_count_count;
				int version = 2;
				ps / version;
				ps % color % width % outline % pattern % doxor;
				ps % vertex_count % subpolygon_count_count % disjunct_polygon_count_count;
				Buffer<Point> vertices(vertex_count);
				Buffer<int> subpolygon_counts(subpolygon_count_count);
				Buffer<int> disjunct_polygon_counts(disjunct_polygon_count_count);
				StreamUnpackPoints(ps, vertices, vertex_count);
				StreamUnpackInts(ps, subpolygon_counts, subpolygon_count_count);
				StreamUnpackInts(ps, disjunct_polygon_counts, disjunct_polygon_count_count);
				if(!ps.Identity()) {
					for(Point *p = vertices, *e = p + vertex_count; p < e; p++)
						ps.Transform(*p);
					ps.TransformW(width);
				}
				DrawPolyPolyPolygon(vertices, vertex_count,
				                    subpolygon_counts, subpolygon_count_count,
				                    disjunct_polygon_counts, disjunct_polygon_count_count,
				                    color, width, outline, pattern, doxor);
			}
			break;
#endif
		case DRAWTEXT:
			{
				int n, angle;
				Font font;
				Color ink;
				byte cs;
				ps % x % y % angle % font % ink / n % cs;
				if(font.GetHeight() == 0) {
					FontInfo fi = font.Info();
					font.Height(fi.GetHeight() - fi.GetInternal());
				}
				bool unicode = cs == CHARSET_UNICODE;
				WString text;
				if(unicode) {
					Buffer<wchar> txt(n);
					ps.Stream::GetW(txt, n);
					text = WString(txt, n);
				}
				else {
					Buffer<char> txt(n);
					ps.Stream::Get(txt, n);
					text = ToUnicode(txt, n, cs);
				}
				LLOG("wsDrawText \"" << WString(text, n)
				     << "\" at: (" << x << ", " << y << ", " << angle << ")");
				bool dxb;
				ps % dxb;
				Buffer<int> dx(n);
				int *wd = dx;
				int nn = n;
				angle %= 3600;
				if(ps.Identity()) {
					if(dxb) {
						while(nn--)
							ps / *wd++;
						DrawText(x, y, angle, text, font, ink, dx);
					}
					else
						DrawText(x, y, angle, text, font, ink);
				}
				else {
					FontInfo fi = font.Info();
					const wchar *wp = ~text;
					int odd = (angle / 900) & 1;
					if(angle % 900 == 0) {
						int error = 0;
						int a, b;
						if(odd) {
							a = ps.target.cy;
							b = ps.source.cy;
							int ht = ps.GetCx(fi.GetFontHeight());
							font.Width(ps.GetCy(fi.GetAveWidth())).Height(ht ? ht : 1);
							FontInfo nf = font.Info();
							x = angle == 2700 ? ps.GetX(x - fi.GetAscent()) + nf.GetAscent()
							                  : ps.GetX(x + fi.GetAscent()) - nf.GetAscent();
							y = ps.GetY(y);
						}
						else {
							a = ps.target.cx;
							b = ps.source.cx;
							int ht = ps.GetCy(fi.GetFontHeight());
							font.Width(ps.GetCx(fi.GetAveWidth())).Height(ht ? ht : 1);
							FontInfo nf = font.Info();
							x = ps.GetX(x);
							y = angle == 1800 ? ps.GetY(y - fi.GetAscent()) + nf.GetAscent()
							                  : ps.GetY(y + fi.GetAscent()) - nf.GetAscent();
						}
						while(nn--) {
							int c;
							if(dxb)
								ps / c;
							else
								c = fi[*wp++];
							*wd++ = (c * a + error) / b;
							error = (c * a + error) % b;
						}
						DrawText(x, y, angle, text, font, ink, dx);
					}
					else {
						double ang = (double) (angle % 900) * M_2PI / 3600;
						double sx = (double) ps.target.cx / ps.source.cx;
						double sy = (double) ps.target.cy / ps.source.cy;
						double ang2 = atan((odd ? sx / sy : sy / sx) * tan(ang));
						double q = (odd ? sx : sy) * sin(ang) / sin(ang2);
						double error = 0;
						while(nn--) {
							int cx;
							if(dxb)
								ps / cx;
							else
								cx = fi[*wp++];
							double ncx = q * cx + error;
							*wd++ = cx = (int) ncx;
							error = ncx - cx;
						}
						int ht = (int)(fi.GetFontHeight() * (sx * sin(ang) * sin(ang2) + sy * cos(ang) * cos(ang2)));
						font.Width(int(q * fi.GetAveWidth())).Height(ht ? ht : 1);
						DrawText(ps.GetX(x), ps.GetY(y), int(ang2 * 3600 / M_2PI) + (angle / 900) * 900,
						         text, font, ink, dx);
					}
				}
			}
		}
	}
//	LOGEND();
	End();
#ifdef _DEBUG
	ASSERT(GetCloffLevel() == cl);
#endif
}

void Draw::DrawDrawing(int x, int y, int cx, int cy, const Drawing& w) {
	DrawDrawing(RectC(x, y, cx, cy), w);
}

void  DrawingDraw::Create(int cx, int cy) {
	Create(Size(cx, cy));
}

void  DrawingDraw::Create(Size sz) {
	drawing.Create();
	size = sz;
#ifdef PLATFORM_WIN32
	cloff.Clear();
#endif
#ifdef PLATFORM_X11
	cloff.Clear();
	clip.Clear();
	Vector<Rect> cliplist;
	cliplist.Add(RectC(0, 0, sz.cx, sz.cy));
	Init(cliplist);
#endif
	drawingclip = Rect(0, 0, sz.cx, sz.cy);
	pixels = false;
	LLOG("DrawingDraw::Create, sz = " << sz << " -> clip = " << GetClip());
}

void DrawingDraw::DInit()
{
#ifdef PLATFORM_WIN32
	Attach(ScreenHDC());
#endif
#ifdef PLATFORM_X11
	gc = XCreateGC(Xdisplay, RootWindow(Xdisplay, Xscreenno), 0, 0);
	dw = RootWindow(Xdisplay, Xscreenno);
#endif
	pixels = false;
	backdraw = true;
	LLOG("DrawingDraw::DInit: pixels = " << Pixels());
}

DrawingDraw::DrawingDraw()
{
	DInit();
}

DrawingDraw::DrawingDraw(Size sz) {
	DInit();
	Create(sz);
}

DrawingDraw::DrawingDraw(int cx, int cy) {
	DInit();
	Create(cx, cy);
}

DrawingDraw::~DrawingDraw() {
#ifdef PLATFORM_X11
	XFreeGC(Xdisplay, gc);
#endif
}

Size Drawing::RatioSize(int cx, int cy) const {
	return GetRatioSize(GetSize(), cx, cy);
}

void Drawing::Append(Drawing& dw)
{
	if(IsNull(size))
		size = dw.size;
	data << dw.data;
	for(int i = 0; i < dw.val.GetCount(); i++)
		val.Add(dw.val[i]);
}

Drawing::Drawing(const Value& src)
{
	if(IsNull(src))
		size = Null;
	else
		*this = RichValue<Drawing>::Extract(src);
}

void Drawing::Serialize(Stream& s)
{
	if(val.GetCount())
		size.cy |= 0x80000000;
	s % size;
	s % data;
	if(size.cy & 0x80000000) {
		size.cy &= ~0x80000000;
		s % val;
	}
}

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE
Drawing Drawing::FromWMF(const WinMetaFile& wmf) {
	if(!wmf) return Drawing();
	Size sz = wmf.GetSize();
	DrawingDraw dd(sz);
	wmf.Paint(dd, 0, 0, sz.cx, sz.cy);
	return dd;
}

Drawing Drawing::LoadWMF(const char *file) {
	return FromWMF(WinMetaFile(file));
}

Drawing Drawing::ReadClipboardWMF() {
	WinMetaFile wmf;
	wmf.ReadClipboard();
	return FromWMF(wmf);
}

WinMetaFile Drawing::AsWMF() const {
	Size sz = GetSize();
	WinMetaFileDraw wd(sz.cx, sz.cy);
	wd.DrawDrawing(0, 0, sz.cx, sz.cy, *this);
	return wd.Close();
}

void Drawing::WriteClipboardWMF() const {
	WinMetaFile wmf = AsWMF();
	wmf.WriteClipboard();
}
#endif
#endif

END_UPP_NAMESPACE
