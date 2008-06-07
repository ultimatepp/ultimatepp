#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

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
	DrawingOp(DRAWIMAGE) % x % y % cx % cy % const_cast<Image&>(img) % s % color;
}

void DrawingDraw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
	String h = id;
	DrawingOp(DRAWDATA) % x % y % cx % cy % const_cast<String&>(data) % h;
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
	return out;
}

// ------------------------------

int   DrawingPos::GetX(int x) const {
	return iscale(x + srcoff.x, target.cx, source.cx) - trgoff.x;
}

int   DrawingPos::GetY(int y) const {
	return iscale(y + srcoff.y, target.cy, source.cy) - trgoff.y;
}

int   DrawingPos::GetCx(int cx) const {
	return iscale(cx, target.cx, source.cx);
}

int   DrawingPos::GetCy(int cy) const {
	return iscale(cy, target.cy, source.cy);
}

int   DrawingPos::GetW(int w) const {
	return iscale(w, target.cx + target.cy, source.cx + source.cy);
}

Point DrawingPos::Get(int x, int y) const {
	return Point(GetX(x), GetY(y));
}

Point DrawingPos::Get(Point p) const {
	return Get(p.x, p.y);
}

Size  DrawingPos::Get(Size sz) const {
	return Size(GetCx(sz.cx), GetCy(sz.cy));
}

Rect  DrawingPos::Get(const Rect& r) const {
	return Rect(GetX(r.left), GetY(r.top), GetX(r.right), GetY(r.bottom));
}

Rect  DrawingPos::Get(int x, int y, int cx, int cy) const {
	return Get(RectC(x, y, cx, cy));
}

Rect GetRect16(Stream& s) {
	Rect r;
	Pack16(s, r);
	return r;
}

Point GetPoint16(Stream& s) {
	Point p;
	Pack16(s, p);
	return p;
}

static void wsBegin(Draw& w, Stream&, const DrawingPos& ps) {
	w.Begin();
	DrawingPos& cps = const_cast<DrawingPos&>(ps);
	cps.stack.Add(cps.srcoff);
}

static void wsOffset(Draw& w, Stream& s, const DrawingPos& ps) {
	Point off;
	s % off;
	w.Offset(ps(off));
	DrawingPos& cps = const_cast<DrawingPos&>(ps);
	cps.stack.Add(cps.srcoff);
	cps.srcoff += off;
	cps.trgoff = w.GetOffset() - cps.trgini;
}

static void wsClip(Draw& w, Stream& s, const DrawingPos& ps) {
	Rect rc;
	s % rc;
	w.Clip(ps(rc));
	DrawingPos& cps = const_cast<DrawingPos&>(ps);
	cps.stack.Add(cps.srcoff);
}

static void wsClipoff(Draw& w, Stream& s, const DrawingPos& ps) {
	Rect rc;
	s % rc;
	w.Clipoff(ps(rc));
	DrawingPos& cps = const_cast<DrawingPos&>(ps);
	cps.stack.Add(cps.srcoff);
	cps.srcoff += rc.TopLeft();
	cps.trgoff = w.GetOffset() - cps.trgini;
}

static void wsExcludeClip(Draw& w, Stream& s, const DrawingPos& ps) {
	Rect rc;
	s % rc;
	w.ExcludeClip(ps(rc));
}

static void wsIntersectClip(Draw& w, Stream& s, const DrawingPos& ps) {
	Rect rc;
	s % rc;
	w.IntersectClip(ps(rc));
}

static void wsEnd(Draw& w, Stream& s, const DrawingPos& ps) {
	w.End();
	DrawingPos& cps = const_cast<DrawingPos&>(ps);
	cps.srcoff = cps.stack.Pop();
	cps.trgoff = w.GetOffset() - cps.trgini;
}

static void wsDrawRect(Draw& w, Stream& s, const DrawingPos& ps) {
	int   x, y, cx, cy;
	Color color;
	s % x % y % cx % cy % color;
	LLOG("wsDrawRect " << RectC(x, y, cx, cy) << " ps:" << ps(x, y, cx, cy) << " color:" << color);
	w.DrawRect(ps(x, y, cx, cy), color);
}

static void wsDrawImage(Draw& w, Stream& s, const DrawingPos& ps) {
	int x, y, cx, cy;
	Image img;
	Rect src;
	Color color;
	s % x % y % cx % cy % img % src % color;
	Rect r = ps(x, y, cx, cy);
	w.DrawImageOp(r.left, r.top, r.Width(), r.Height(), img, src, color);
}

static void wsDrawData(Draw& w, Stream& s, const DrawingPos& ps)
{
	String data, id;
	int x, y, cx, cy;
	s % x % y % cx % cy % const_cast<String&>(data) % id;
	Rect r = ps(x, y, cx, cy);
	w.DrawData(r, data, id);
}

static void wsDrawDrawing(Draw& w, Stream& s, const DrawingPos& ps) {
	Drawing dw;
	Rect rc;
	s % dw % rc;
	w.DrawDrawing(ps(rc), dw);
}

static void wsDrawLine(Draw& w, Stream& s, const DrawingPos& ps) {
	int x1, y1, x2, y2, width;
	Color color;
	s % x1 % y1 % x2 % y2 % width % color;
	w.DrawLine(ps(x1, y1), ps(x2, y2), width > 0 ? ps.GetW(width) : width, color);
}

#ifndef PLATFORM_WINCE

static void wsDrawPolyPolyline(Draw& w, Stream& stream, const DrawingPos& dp)
{
	int width, vertex_count, count_count;
	Color color, doxor;
	int version;
	stream / version;
	stream % width % color % doxor;
	stream % vertex_count % count_count;
	Buffer<Point> vertices(vertex_count);
	Buffer<int> counts(count_count);
	StreamUnpackPoints(stream, vertices, vertex_count);
	StreamUnpackInts(stream, counts, count_count);
	if(!dp.Identity()) {
		for(Point *p = vertices, *e = p + vertex_count; p < e; p++)
			dp.Transform(*p);
		if(width > 0)
			dp.TransformW(width);
	}
	w.DrawPolyPolyline(vertices, vertex_count, counts, count_count, width, color, doxor);
}

static void wsDrawPolyPolyPolygon(Draw& w, Stream& stream, const DrawingPos& dp)
{
	Color color, outline, doxor;
	uint64 pattern;
	int width, vertex_count, subpolygon_count_count, disjunct_polygon_count_count;
	int version = 2;
	stream / version;
	stream % color % width % outline % pattern % doxor;
	stream % vertex_count % subpolygon_count_count % disjunct_polygon_count_count;
	Buffer<Point> vertices(vertex_count);
	Buffer<int> subpolygon_counts(subpolygon_count_count);
	Buffer<int> disjunct_polygon_counts(disjunct_polygon_count_count);
	StreamUnpackPoints(stream, vertices, vertex_count);
	StreamUnpackInts(stream, subpolygon_counts, subpolygon_count_count);
	StreamUnpackInts(stream, disjunct_polygon_counts, disjunct_polygon_count_count);
	if(!dp.Identity()) {
		for(Point *p = vertices, *e = p + vertex_count; p < e; p++)
			dp.Transform(*p);
		dp.TransformW(width);
	}
	w.DrawPolyPolyPolygon(vertices, vertex_count,
		                  subpolygon_counts, subpolygon_count_count,
		                  disjunct_polygon_counts, disjunct_polygon_count_count,
		                  color, width, outline, pattern, doxor);
}

static void wsDrawArc(Draw& w, Stream& s, const DrawingPos& ps)
{
	Rect r;
	Point start, end;
	Color color;
	int width;
	s % r % start % end % color % width;
	w.DrawArc(r, start, end, width, color);
}

#endif

static void wsDrawEllipse(Draw& w, Stream& s, const DrawingPos& ps) {
	Rect r;
	s % r;
	Color color, pencolor;
	int pen;
	s % color / pen % pencolor;
	w.DrawEllipse(ps(r), color, pen > 0 ? ps.GetW(pen) : pen, pencolor);
}

static void wsDrawText(Draw& w, Stream& s, const DrawingPos& ps) {
	int x, y, n, angle;
	Font font;
	Color ink;
	byte cs;
	s % x % y % angle % font % ink / n % cs;
	if(font.GetHeight() == 0) {
		FontInfo fi = font.Info();
		font.Height(fi.GetHeight() - fi.GetInternal());
	}
	bool unicode = cs == CHARSET_UNICODE;
	WString text;
	if(unicode) {
		Buffer<wchar> txt(n);
		s.GetW(txt, n);
		text = WString(txt, n);
	}
	else {
		Buffer<char> txt(n);
		s.Get(txt, n);
		text = ToUnicode(txt, n, cs);
	}
	LLOG("wsDrawText \"" << WString(text, n)
	     << "\" at: (" << x << ", " << y << ", " << angle << ")");
	bool dxb;
	s % dxb;
	Buffer<int> dx(n);
	int *wd = dx;
	int nn = n;
	angle %= 3600;
	if(ps.Identity()) {
		if(dxb) {
			while(nn--)
				s / *wd++;
			w.DrawText(x, y, angle, text, font, ink, dx);
		}
		else
			w.DrawText(x, y, angle, text, font, ink);
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
					s / c;
				else
					c = fi[*wp++];
				*wd++ = (c * a + error) / b;
				error = (c * a + error) % b;
			}
			w.DrawText(x, y, angle, text, font, ink, dx);
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
					s / cx;
				else
					cx = fi[*wp++];
				double ncx = q * cx + error;
				*wd++ = cx = (int) ncx;
				error = ncx - cx;
			}
			int ht = (int)(fi.GetFontHeight() * (sx * sin(ang) * sin(ang2) + sy * cos(ang) * cos(ang2)));
			font.Width(int(q * fi.GetAveWidth())).Height(ht ? ht : 1);
			w.DrawText(ps.GetX(x), ps.GetY(y), int(ang2 * 3600 / M_2PI) + (angle / 900) * 900,
					   text, font, ink, dx);
		}
	}
}

static VectorMap<int, Draw::Drawer>& sDrawerMap()
{
	return Single< VectorMap<int, Draw::Drawer> > ();
}

void Draw::Register(int code, Drawer drawer)
{
	static StaticCriticalSection lock;
	CriticalSection::Lock __(lock);
	VectorMap<int, Draw::Drawer>& map = sDrawerMap();
	int i = map.Find(code);
	if(i >= 0) {
		ASSERT(map[i] == drawer);
		return;
	}
	map.Add(code, drawer);
}

void Draw::DrawDrawingOp(const Rect& target, const Drawing& w) {
	{
		ONCELOCK {
			Register(BEGIN, wsBegin);
			Register(OFFSET, wsOffset);
			Register(CLIP, wsClip);
			Register(CLIPOFF, wsClipoff);
			Register(EXCLUDECLIP, wsExcludeClip);
			Register(INTERSECTCLIP, wsIntersectClip);
			Register(END, wsEnd);
			Register(DRAWRECT, wsDrawRect);
			Register(DRAWIMAGE, wsDrawImage);
			Register(DRAWDRAWING, wsDrawDrawing);
			Register(DRAWLINE, wsDrawLine);
			Register(DRAWELLIPSE, wsDrawEllipse);
			Register(DRAWTEXT, wsDrawText);
#ifndef PLATFORM_WINCE
			Register(DRAWARC, wsDrawArc);
			Register(DRAWPOLYPOLYLINE, wsDrawPolyPolyline);
			Register(DRAWPOLYPOLYPOLYGON, wsDrawPolyPolyPolygon);
#endif
			Register(DRAWDATA, wsDrawData);
		}
	}

#ifdef _DEBUG
	int cl = GetCloffLevel();
#endif
	DrawingPos pos;
	pos.srcoff = pos.trgoff = Point(0, 0);
	pos.target = target.Size();
	pos.source = w.size;
	LLOG("DrawDrawingOp size: " << w.size);
	if(pos.target.cx == 0 || pos.target.cy == 0 || pos.source.cx == 0 || pos.source.cy == 0)
		return;
	Clipoff(target);
	pos.trgini = GetOffset();
	VectorMap<int, Draw::Drawer>& map = sDrawerMap();
	StringStream s(w.data);
	LOGBEGIN();
	while(!s.IsEof()) {
		int code;
		s / code;
		int i = map.Find(code);
		if(i < 0)
			break;
		(*map[i])(*this, s, pos);
	}
	LOGEND();
	End();
#ifdef _DEBUG
	ASSERT(GetCloffLevel() == cl);
#endif
}

void DrawingDraw::DrawDrawingOp(const Rect& target, const Drawing& w)
{
	DrawingOp(DRAWDRAWING) % const_cast<Drawing&>(w) % const_cast<Rect&>(target);
	return;
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
}

void Drawing::Serialize(Stream& s)
{
	s % size;
	s % data;
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
