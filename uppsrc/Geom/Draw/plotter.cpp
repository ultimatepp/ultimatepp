#include "GeomDraw.h"

namespace Upp {

#define LLOG(x) // LOG(x)

#define DEBUG_AREA 0
#define CHECK_DIST 0 // 0 = off, 1 = on (purging very short line segments)

#define STAT_LINES 0 // 0 = off, 1 = on, 2 = log individual vertices

static void DrawFatPolyPolygonOutline(Draw& draw,
	const Point *vertices, int vertex_counts,
	const int *polygon_counts, int polygon_count_counts,
	Color color, int width)
{
	if(IsNull(color))
		return;
	if(width <= 1)
	{
		DrawPolyPolygon(draw, vertices, vertex_counts, polygon_counts, polygon_count_counts, Null, 0, color);
		return;
	}
	enum { BATCH = 4000 };
	int nsegs = min<int>(BATCH, (vertex_counts + polygon_count_counts + 1) * width);
	int batch = 2 * nsegs;
	Buffer<Point> buffer(batch);
	Buffer<int> counts(nsegs);
	Fill(counts + 0, counts + nsegs, 2);
	int half = width >> 1;
	Point *gen = buffer, *genend = buffer + batch - 2 * width;

	for(; --polygon_count_counts >= 0; vertices += *polygon_counts++)
	{
		int vert = *polygon_counts;
		int vw = vert * width;
		int pc = 2 * vw;
		Point prev = *vertices;
		for(const Point *next = vertices + vert; next-- > vertices; prev = *next)
		{
			int dx = next->x - prev.x, dy = next->y - prev.y;
			bool vert = (tabs(dx) < tabs(dy));
			if(vert)
			{
//				if(dy >= 0) { dy += width; prev.y -= half; }
//				else        { dy -= width; prev.y += half; }

				prev.x -= half;
				for(int i = 0; i < width; i++, gen += 2)
				{
					gen[1].x = dx + (gen[0].x = prev.x + i);
					gen[1].y = dy + (gen[0].y = prev.y);
				}
			}
			else
			{
//				if(dx >= 0) { dx += width; prev.x -= half; }
//				else        { dx -= width; prev.x += half; }

				prev.y -= half;
				for(int i = 0; i < width; i++, gen += 2)
				{
					gen[1].x = dx + (gen[0].x = prev.x);
					gen[1].y = dy + (gen[0].y = prev.y + i);
				}
			}
			if(gen >= genend)
			{
				int pc = gen - buffer;
				ASSERT(pc <= batch);
				ASSERT(!(pc & 1));
				draw.DrawPolyPolyline(buffer, pc, counts, pc >> 1, 0, color);
				gen = buffer;
			}
		}
	}
	if(int pc = gen - buffer)
	{
		ASSERT(pc <= batch);
		ASSERT(!(pc & 1));
		draw.DrawPolyPolyline(buffer, pc, counts, pc >> 1, 0, color);
		gen = buffer;
	}
}

static void DrawFatPolyPolygonOutline(Draw& draw,
	const Vector<Point>& vertices, const Vector<int>& counts,
	Color color, int width)
{
	DrawFatPolyPolygonOutline(draw, vertices.Begin(), vertices.GetCount(),
		counts.Begin(), counts.GetCount(), color, width);
}

LineStyle SolidLine      = { 0 };
LineStyle EmptyLine      = { -1 };
LineStyle DashLine       = { 8, -4, 0 };
LineStyle DotLine        = { 2, -4, 0 };
LineStyle DashDotLine    = { 8, -4, 2, -4, 0 };
LineStyle DashDotDotLine = { 8, -4, 2, -4, 2, -4, 0 };

enum { MAX_COUNT = 2048, MAX_MANUAL = 50 };

LineDraw::LineDraw()
: draw(0)
, MoveToRaw(0)
, LineToRaw(0)
, AddVector(0)
{
}

LineDraw::LineDraw(Draw& draw, LineStyle pattern, Color color, int width, double dash)
{
	Set(draw, pattern, color, width, dash);
}

void LineDraw::Set(Draw& _draw, LineStyle pattern, Color color, int width, double dash)
{
	draw = &_draw;
	empty = (*pattern < 0 || IsNull(color));
	if(empty)
	{
		active = 0;
		ClearExtent();
		return;
	}
	pen_width = (width >= 0 ? width : DotsToPixels(*draw, -width));
	if(dash < 0)
		dash *= -GetAvgPixelsPerDot(*draw);
	else if(dash == 0)
		dash = max(pen_width, 1);
	dash = max<double>(dash, 1);
	pen_color = color;
	if(pen_width <= 1)
		AddVector = &LineDraw::AddVectorThin;
	else if(draw->Pixels() && pen_width <= 5)
	{
		AddVector = &LineDraw::AddVectorThick;
		half_width = pen_width >> 1;
	}
	else
	{
		AddVector = &LineDraw::AddVectorArea;
		factor = pen_width / 2.0;
	}
	if(*pattern > 0)
	{
		if(dash == 0)
			dash = width;
		const signed char *p = pattern;
		while(*p++)
			;
		p--;
		while(p > pattern)
			segments.Add(*--p * dash);
		active = segments.End();
		remains = *--active;
	}
	else
	{
		remains = 1e100;
		active = NULL;
	}
	first = last = Null;
	Size size = draw->GetPageSize();
	clip = draw->GetPaintRect();
	max_rad = max(size.cx, size.cy);
	vertices.Clear();
	indices.Clear();
	ClearExtent();
}

bool LineDraw::SetExtent(const Rect& rc)
{
	if(!(rc && clip) || empty)
		return false;
	bool in = clip.Contains(rc);
	MoveToRaw = active ? &LineDraw::MoveToDashed : &LineDraw::MoveToSolid;
	if(active)
		LineToRaw = in ? &LineDraw::LineToDashedSimple : &LineDraw::LineToDashedClip;
	else
		LineToRaw = in ? &LineDraw::LineToSolidSimple : &LineDraw::LineToSolidClip;
	return true;
}

void LineDraw::ClearExtent()
{
	MoveToRaw = empty ? &LineDraw::MoveToEmpty : active ? &LineDraw::MoveToDashed : &LineDraw::MoveToSolid;
	LineToRaw = empty ? &LineDraw::LineToEmpty : active ? &LineDraw::LineToDashedClip : &LineDraw::LineToSolidClip;
}

void LineDraw::Clear()
{
	indices.Clear();
	vertices.Clear();
}

void LineDraw::Flush()
{
	if(indices.IsEmpty())
		return;
	draw->DrawPolyPolyline(vertices, indices, 0, pen_color);
	vertices.Clear();
	indices.Clear();
}

void LineDraw::AddVectorThin(Point a, Point b)
{
	if(a == b)
		return;
	if(!vertices.IsEmpty())
		if(vertices.Top() == a)
		{
			vertices.Add(b);
			indices.Top()++;
			return;
		}
		else if(vertices.Top() == b)
		{
			vertices.Add(a);
			indices.Top()++;
			return;
		}
	indices.Add(2);
	vertices.Add(a);
	vertices.Add(b);
	if(vertices.GetCount() >= MAX_COUNT)
		Flush();
}

void LineDraw::AddVectorThick(Point a, Point b)
{
	Size vector = b - a;
	if((vector.cx | vector.cy) == 0)
		return;
	Size sh(sgn(vector.cx), sgn(vector.cy));
	Size mv(0, 0);
	if(tabs(vector.cx) > tabs(vector.cy))
	{
		mv.cy = 1;
		a.y += half_width;
		b.y += half_width;
	}
	else
	{
		mv.cx = 1;
		a.x += half_width;
		b.x += half_width;
	}
	indices.Add(2 * pen_width);
	int i = pen_width;
	while(i >= 2)
	{
		vertices.Add(a);
		vertices.Add(b);
		a -= mv;
		b -= mv;
		vertices.Add(b);
		vertices.Add(a);
		a -= mv;
		b -= mv;
		i -= 2;
	}
	if(i)
	{
		vertices.Add(a);
		vertices.Add(b);
	}
	if(vertices.GetCount() >= MAX_COUNT)
		Flush();
}

void LineDraw::AddVectorArea(Point a, Point b)
{
//	static Point last(0, 0);
//	LOG(a << " - " << b << ": " << abs(a - b) << " (" << abs(a - last) << ")");
//	last = b;

	Size vector = b - a;
	if((vector.cx | vector.cy) == 0)
		vector.cx = 1;
	double f = factor / sqrt(double(vector.cx * vector.cx + vector.cy * vector.cy));
	vector.cx = fround(f * vector.cx);
	vector.cy = fround(f * vector.cy);
	Size larger((vector.cy - vector.cx) * 3 >> 2, (vector.cy + vector.cx) * 3 >> 2);
	Point polygon[10];
	polygon[0].x = a.x + vector.cy; polygon[0].y = a.y - vector.cx;
	polygon[1].x = a.x + larger.cx; polygon[1].y = a.y - larger.cy;
	polygon[2].x = a.x - vector.cx; polygon[2].y = a.y - vector.cy;
	polygon[3].x = a.x - larger.cy; polygon[3].y = a.y - larger.cx;
	polygon[4].x = a.x - vector.cy; polygon[4].y = a.y + vector.cx;
	polygon[5].x = b.x - vector.cy; polygon[5].y = b.y + vector.cx;
	polygon[6].x = b.x - larger.cx; polygon[6].y = b.y + larger.cy;
	polygon[7].x = b.x + vector.cx; polygon[7].y = b.y + vector.cy;
	polygon[8].x = b.x + larger.cy; polygon[8].y = b.y + larger.cx;
	polygon[9].x = b.x + vector.cy; polygon[9].y = b.y - vector.cx;
	DrawPolygon(*draw, polygon, 10, pen_color, PEN_NULL, Black, 0);
//	draw->SetColor(pen_color);
//	draw->SetDrawPen(PEN_NULL, Black);
//	Polygon(*draw, polygon, 10);
}

void LineDraw::MoveToEmpty(Point pt) {}

void LineDraw::MoveToSolid(Point pt)
{
	first = last = pt;
}

void LineDraw::MoveToDashed(Point pt)
{
	first = last = pt;
	active = segments.End();
	remains = *--active;
}

void LineDraw::LineToEmpty(Point pt)
{
}

void LineDraw::LineToSolidSimple(Point pt)
{
	if(!IsNull(last) && pt != last)
		(this ->* AddVector)(last, pt);
	last = pt;
}

void LineDraw::LineToSolidClip(Point pt)
{
	if(!IsNull(last))
	{
		if(pt == last)
			return;
		Point next = pt;
		if(!clip.Contains(next) || !clip.Contains(last))
		{
			Pointf A = last, B = next;
			if(!ClipLine(A, B, clip))
			{
				last = pt;
				return;
			}
			last = PointfToPoint(A);
			next = PointfToPoint(B);
		}
		(this ->* AddVector)(last, next);
	}
	last = pt;
}

void LineDraw::LineToDashedClip(Point pt)
{
	if(pt == last)
		return;
	if(!IsNull(last))
	{
		Point next = pt;
		if(!clip.Contains(next) || !clip.Contains(last))
		{
			Pointf A = last, B = next;
			if(!ClipLine(A, B, clip))
			{
				last = pt;
				return;
			}
			last = PointfToPoint(A);
			next = PointfToPoint(B);
		}
		LineToDashedSimple(next);
	}
	last = pt;
}

void LineDraw::LineToDashedSimple(Point pt)
{
	if(IsNull(last))
	{
		last = pt;
		return;
	}
	Size vector = pt - last;
	if((vector.cx | vector.cy) == 0)
		return;
	double segment = hypot(vector.cx, vector.cy), left = segment;
	for(;;)
	{
		if(remains > 0)
		{
			if(left < segment)
			{
				double r = left / segment;
				last.x = pt.x - fround(vector.cx * r);
				last.y = pt.y - fround(vector.cy * r);
			}
			if(remains >= left)
			{
				remains -= left;
				(this ->* AddVector)(last, pt);
				break;
			}
			else
			{
				double r = (left -= remains) / segment;
				Point end(pt.x - fround(vector.cx * r), pt.y - fround(vector.cy * r));
				(this ->* AddVector)(last, end);
				last = end;
			}
		}
		else
		{
			if(remains + left <= 0)
			{
				remains += left;
				break;
			}
			else
				left += remains;
		}
		if(active == segments.Begin())
			active = segments.End();
		remains = *--active;
	}
	last = pt;
}

void LineDraw::Close()
{
	if(empty)
		return;
	if(!IsNull(first.x) && first != last)
		LineTo(first);
	first = Null;
}

void LineDraw::Rectangle(const Rect& rc)
{
	if(empty)
		return;
	MoveTo(rc.TopLeft());
	LineTo(Point(rc.right, rc.top));
	LineTo(rc.BottomRight());
	LineTo(Point(rc.left, rc.bottom));
	Close();
}

void LineDraw::RecurseArc(Point next, int length, int bulge, int levels)
{
	if(--levels < 0 || tabs(bulge) <= 1 || length <= 2)
		LineTo(next); // degenerate next line
	else
	{ // bisect arc
		Size normal = iscale(Size(next.y - last.y, last.x - next.x), bulge, length);
		Point centre = ((last + next) >> 1) + normal;
		int ll = (int)Length(centre - next);
		int hh = iscale(bulge, ll, 2 * ll + length);
		RecurseArc(centre, ll, hh, levels);
		RecurseArc(next, ll, hh, levels);
	}
}

void LineDraw::ArcTo(Point p, int bulge, int levels)
{
	if(empty)
		return;
	if(p == last)
		return;
	if(bulge >= -1 && bulge <= 1)
	{
		LineTo(p);
		return;
	}
	VecArcInfo info(last, Pointf(p), -bulge);
	if(!info.IsCurved())
	{
		LineTo(p);
		return;
	}
	if(info.radius >= max_rad)
	{
		RecurseArc(p, (int)Length(p - last), bulge, levels);
		return;
	}
	Point c = PointfToPoint(info.C);
	int rad = fround(info.radius);
	Rect rc(c.x - rad, c.y - rad, c.x + rad, c.y + rad);
	if(!active)
	{ // draw simple arc
		if(bulge >= 0)
			draw->DrawArc(rc, last, p, pen_width, pen_color);
		else
			draw->DrawArc(rc, p, last, pen_width, pen_color);
		last = p;
		return;
	}

	double left = info.Length();
	double end_angle = info.reversed ? info.alpha : info.beta;
	double len_ang = (info.reversed ? info.beta - info.alpha : info.alpha - info.beta) / left;

	int part = 1000;
	for(;;)
	{
		if(--part < 0)
			break;

		if(remains > 0)
		{
			Point start = PointfToPoint(PolarPointf(info.C, info.radius, end_angle + len_ang * left));
			Point end;
			if(remains >= left)
			{
				remains -= left;
				left = 0;
				end = p;
			}
			else
				end = PointfToPoint(PolarPointf(info.C, info.radius, end_angle + len_ang * (left -= remains)));
			if(bulge >= 0)
				Swap(start, end);
			if(max(start.x - end.x, start.y - end.y) <= 2)
				draw->DrawLine(start, end, pen_width, pen_color);
			else
			{
				if(bulge >= 0)
					draw->DrawArc(rc, start, end, pen_width, pen_color);
				else
					draw->DrawArc(rc, end, start, pen_width, pen_color);
			}
			if(left == 0)
				break;
		}
		else if(remains < 0)
		{
			if(remains + left <= 0)
			{
				remains += left;
				break;
			}
			else
				left += remains;
		}
		if(active == segments.Begin())
			active = segments.End();
		remains = *--active;
	}
	last = p;
}

void LineDraw::Circle(Point centre, int radius)
{
	if(empty)
		return;
	Point east(centre.x + radius, centre.y), west(centre.x - radius, centre.y);
	MoveTo(east);
	ArcTo(west, radius);
	ArcTo(east, radius);
}

void LineDraw::Ellipse(const Rect& rc)
{
	enum { SEGMENTS = 16 };
	Point centre = rc.CenterPoint();
	Size radius = rc.Size() >> 1;
	Point beg = Point(centre.x + radius.cx, centre.y);
	MoveTo(beg);
	for(int i = 1; i <= SEGMENTS; i++)
	{
		double a = i * (2 * M_PI / SEGMENTS) - M_PI / SEGMENTS;
		Point mid(centre.x + fround(radius.cx * cos(a)), centre.y + fround(radius.cy * sin(a)));
		a += M_PI / SEGMENTS;
		Point end(centre.x + fround(radius.cx * cos(a)), centre.y + fround(radius.cy * sin(a)));
		int bulge = 0;
		int dist = Squared(end - beg);
		if(dist)
			bulge = fround(tabs(VectorProduct(mid - beg, end - beg)) / sqrt(double(dist)));
		ArcTo(end, bulge);
		beg = end;
	}
}

void Plotter::Set(Draw& _draw, const Rectf& _src, const Rectf& _dest, int flags, int reserve, double meter)
{
	Rectf src = _src;
	if(src.right  <= src.left) src.right  = src.left + 1;
	if(src.bottom <= src.top)  src.bottom = src.top  + 1;
	Rectf dest = _dest;
	if(flags & MIRROR_X) Swap(dest.left, dest.right);
	if(flags & MIRROR_Y) Swap(dest.top,  dest.bottom);
	Pointf scale = dest.Size() / src.Size();
	if(flags & ISOTROPIC)
	{
		scale.x = sgn(scale.x) * AbsMin(scale);
		scale.y = sgn(scale.y) * fabs(scale.x);
	}
	Pointf delta = dest.CenterPoint() - src.CenterPoint() * scale;
	Set(_draw, scale, delta, reserve, meter);
}

void Plotter::Set(Draw& _draw, Sizef _scale, Pointf _delta, int reserve, double meter)
{
	Set(_draw, Matrixf(Pointf(_scale.cx, 0), Pointf(0, _scale.cy), _delta), reserve, meter);
}

void Plotter::Set(Draw& _draw, const Matrixf& matrix, int reserve, double meter)
{
	Rect rc = _draw.GetPaintRect();
	if(reserve < 0)
		reserve = DotsToPixels(_draw, -reserve);
	rc.Inflate(reserve);
	Set(_draw, matrix, rc, meter);
}

void Plotter::Set(Draw& _draw, const Matrixf& _transform, const Rect& _clip, double meter)
{
	physical = _transform;
	logical = MatrixfInverse(_transform);
	signed_measure = MatrixfMeasure(physical);
	measure = fabs(signed_measure);
	logprec = 1.0 / measure;
	ortho = (physical.x.y == 0 && physical.y.x == 0);
	ltop = (ortho ? &Plotter::LtoPOrtho : &Plotter::LtoPFull);
	ltopoint = (ortho ? &Plotter::LtoPointOrtho : &Plotter::LtoPointFull);
	SetDraw(_draw, _clip, meter);
}

void Plotter::SetDraw(Draw& _draw, const Rect& _clip, double meter)
{
	draw = &_draw;
	SetPixelsPerDot(GetAvgPixelsPerDot(*draw));
	clip = _clip;
	logclip = PtoL(clip);
	logdiag = Diagonal(logclip);
	path_map = PATH_MAP_NULL;
	SetMapMeters(meter);
}

void Plotter::Set(const Plotter& info, const Rect& clip)
{
	ASSERT(info.draw);
	Set(*info.draw, info.physical, clip, info.map_meters);
	path_map = info.path_map;
	pixels_per_dot = info.pixels_per_dot;
	SetMapMeters(info.map_meters);
}

void Plotter::Set(const Plotter& info, int reserve)
{
	ASSERT(info.draw);
	Set(*info.draw, info.physical, reserve);
	path_map = info.path_map;
	pixels_per_dot = info.pixels_per_dot;
	SetMapMeters(info.map_meters);
}

void Plotter::SetMapMeters(double mm)
{
	map_meters = mm;
	physical_scale = fabs(measure) / (map_meters * GetPixelsPerMeter());
	LLOG("Plotter::SetMapMeters(" << mm << "): pixel per dot = " << pixels_per_dot << ", physical scale = " << physical_scale);
}

void Plotter::SetXorMode(bool xm)
{
	if(SystemDraw *sdraw = dynamic_cast<SystemDraw *>(draw)) {
#ifdef PLATFORM_WIN32
		SetROP2(*sdraw, xm ? R2_NOTXORPEN : R2_COPYPEN);
#endif
#ifdef GUI_X11
		XSetFunction(Xdisplay, sdraw->GetGC(), xm ? X11_ROP2_NOT_XOR : X11_ROP2_COPY);
#endif
	}
}

static inline void AddRectMatrix(Rectf& out, const Rectf& rc, const Matrixf& mx)
{
	out.left   += mx.x.x * (mx.x.x >= 0 ? rc.left : rc.right) + mx.y.x * (mx.y.x >= 0 ? rc.top : rc.bottom);
	out.top    += mx.x.y * (mx.x.y >= 0 ? rc.left : rc.right) + mx.y.y * (mx.y.y >= 0 ? rc.top : rc.bottom);
	out.right  += mx.x.x * (mx.x.x <= 0 ? rc.left : rc.right) + mx.y.x * (mx.y.x <= 0 ? rc.top : rc.bottom);
	out.bottom += mx.x.y * (mx.x.y <= 0 ? rc.left : rc.right) + mx.y.y * (mx.y.y <= 0 ? rc.top : rc.bottom);
}

Rectf Plotter::LtoPrel(const Rectf& rc) const
{
	Rectf result(0, 0, 0, 0);
	AddRectMatrix(result, rc, physical);
	return result;
}

Rectf Plotter::LtoP(const Rectf& rc) const
{
	Rectf result = PointfRectf(physical.a);
	AddRectMatrix(result, rc, physical);
	return result;
}

Rectf Plotter::PtoLrel(const Rectf& rc) const
{
	Rectf result = Rectf(0, 0, 0, 0);
	AddRectMatrix(result, rc, logical);
	return result;
}

Rectf Plotter::PtoL(const Rectf& rc) const
{
	Rectf result = PointfRectf(logical.a);
	AddRectMatrix(result, rc, logical);
	return result;
}

Point Plotter::LtoPointFull(Pointf pt) const
{
	return PointfToPoint(pt * physical);
}

Point Plotter::LtoPointOrtho(Pointf pt) const
{
	return Point((int)(pt.x * physical.x.x + physical.a.x), (int)(pt.y * physical.y.y + physical.a.y));
}

static void PaintRectPart(Draw& draw, int x, int y, int w, int h)
{
	if(SystemDraw *sdraw = dynamic_cast<SystemDraw *>(&draw)) {
#if defined(PLATFORM_WIN32)
		::PatBlt(*sdraw, x, y, w, h, PATINVERT);
#elif defined(GUI_X11)
		Point offset = sdraw->GetOffset();
		XFillRectangle(Xdisplay, sdraw->GetDrawable(), sdraw->GetGC(), x + offset.x, y + offset.y, w, h);
#endif
	}
}

static void PaintRectPart(Draw& draw, const Rect& rc) { PaintRectPart(draw, rc.left, rc.top, rc.Width(), rc.Height()); }

enum { DRAG_STEP = 4 };

void PaintDragHorzLine(Draw& draw, const Rect& rc, Color c1, Color c2, Color bgnd, int mingap)
{
	Size sz = rc.Size();
	if(SystemDraw *sdraw = dynamic_cast<SystemDraw *>(&draw)) {
#if defined(PLATFORM_WIN32)
		sdraw->BeginGdi();
		static word bmp_bits[8];
		HBITMAP hbmp = CreateBitmap(8, 8, 1, 1, bmp_bits);
		HBRUSH brush = CreatePatternBrush(hbmp);
		DeleteObject(hbmp);
		COLORREF cc1 = (COLORREF)c1 ^ (COLORREF)bgnd, cc2 = (COLORREF)c2 ^ (COLORREF)bgnd;
		COLORREF old_bk = SetTextColor(*sdraw, cc1);
		HGDIOBJ old_brush = SelectObject(*sdraw, brush);
#elif defined(GUI_X11)
		XGCValues gcv_old, gcv_new;
		XGetGCValues(Xdisplay, sdraw->GetGC(), GCForeground | GCFunction, &gcv_old);
		gcv_new.function = X11_ROP2_XOR;
		gcv_new.foreground = GetXPixel(c1) ^ GetXPixel(bgnd);
		XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground | GCFunction, &gcv_new);
#endif
		int whites = (sz.cx - 2 * mingap + DRAG_STEP) / (2 * DRAG_STEP);
		if(rc.Width() <= 2 * mingap)
			PaintRectPart(*sdraw, rc);
		else if(whites <= 0) {
			PaintRectPart(*sdraw, rc.left, rc.top, mingap, sz.cy);
			PaintRectPart(*sdraw, rc.right - mingap, rc.top, mingap, sz.cy);
#if defined(PLATFORM_WIN32)
			SetTextColor(*sdraw, cc2);
#elif defined(GUI_X11)
			gcv_new.foreground = GetXPixel(c2) ^ GetXPixel(bgnd);
			XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground, &gcv_new);
#endif
			PaintRectPart(*sdraw, rc.left + mingap, rc.top, sz.cx - 2 * mingap, sz.cy);
		}
		else
		{
			int rem = sz.cx - whites * 2 * DRAG_STEP + DRAG_STEP;
			int lrem = rem >> 1, rrem = (rem + 1) >> 1;
			PaintRectPart(*sdraw, rc.left, rc.top, lrem, sz.cy);
			PaintRectPart(*sdraw, rc.right - rrem, rc.top, rrem, sz.cy);
			int i;
			int start = rc.left + lrem - DRAG_STEP;
			for(i = 1; i < whites; i++)
				PaintRectPart(*sdraw, start + 2 * DRAG_STEP * i, rc.top, DRAG_STEP, sz.cy);
#if defined(PLATFORM_WIN32)
			SetTextColor(*sdraw, cc2);
#elif defined(GUI_X11)
			gcv_new.foreground = GetXPixel(c2) ^ GetXPixel(bgnd);
			XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground, &gcv_new);
#endif
			start = rc.left + lrem;
			for(i = 0; i < whites; i++)
				PaintRectPart(*sdraw, start + 2 * DRAG_STEP * i, rc.top, DRAG_STEP, sz.cy);
		}
#if defined(PLATFORM_WIN32)
		SelectObject(*sdraw, old_brush);
		DeleteObject(brush);
		SetTextColor(*sdraw, old_bk);
		sdraw->EndGdi();
#elif defined(GUI_X11)
		XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground | GCFunction, &gcv_old);
#endif
	}
}

void PaintDragVertLine(Draw& draw, const Rect& rc, Color c1, Color c2, Color bgnd, int mingap)
{
	Size sz = rc.Size();
	if(SystemDraw *sdraw = dynamic_cast<SystemDraw *>(&draw)) {
#if defined(PLATFORM_WIN32)
		sdraw->BeginGdi();
		static word bmp_bits[8];
		HBITMAP hbmp = CreateBitmap(8, 8, 1, 1, bmp_bits);
		HBRUSH brush = CreatePatternBrush(hbmp);
		DeleteObject(hbmp);
		COLORREF cc1 = (COLORREF)c1 ^ (COLORREF)bgnd, cc2 = (COLORREF)c2 ^ (COLORREF)bgnd;
		COLORREF old_bk = SetTextColor(*sdraw, cc1);
		HGDIOBJ old_brush = SelectObject(*sdraw, brush);
#elif defined(GUI_X11)
		XGCValues gcv_old, gcv_new;
		XGetGCValues(Xdisplay, sdraw->GetGC(), GCForeground | GCFunction, &gcv_old);
		gcv_new.function = X11_ROP2_XOR;
		gcv_new.foreground = GetXPixel(c1) ^ GetXPixel(bgnd);
		XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground | GCFunction, &gcv_new);
#endif
		int whites = (sz.cy - 2 * mingap + DRAG_STEP) / (2 * DRAG_STEP);
		if(rc.Height() <= 2 * mingap)
			PaintRectPart(*sdraw, rc);
		else if(whites <= 0)
		{
			PaintRectPart(*sdraw, rc.left, rc.top, sz.cx, mingap);
			PaintRectPart(*sdraw, rc.left, rc.bottom - mingap, sz.cx, mingap);
#if defined(PLATFORM_WIN32)
			SetTextColor(*sdraw, cc2);
#elif defined(GUI_X11)
			gcv_new.foreground = GetXPixel(c2) ^ GetXPixel(bgnd);
			XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground, &gcv_new);
#endif
			PaintRectPart(*sdraw, rc.left, rc.top + mingap, sz.cx, sz.cy - 2 * mingap);
		}
		else
		{
			int rem = sz.cy - whites * 2 * DRAG_STEP + DRAG_STEP;
			int lrem = rem >> 1, rrem = (rem + 1) >> 1;
			PaintRectPart(*sdraw, rc.left, rc.top, sz.cx, lrem);
			PaintRectPart(*sdraw, rc.left, rc.bottom - rrem, sz.cx, rrem);
			int i;
			int start = rc.top + lrem - DRAG_STEP;
			for(i = 1; i < whites; i++)
				PaintRectPart(*sdraw, rc.left, start + 2 * DRAG_STEP * i, sz.cx, DRAG_STEP);
#if defined(PLATFORM_WIN32)
			SetTextColor(*sdraw, cc2);
#elif defined(GUI_X11)
			gcv_new.foreground = GetXPixel(c2) ^ GetXPixel(bgnd);
			XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground, &gcv_new);
#endif
			start = rc.left + lrem;
			for(i = 0; i < whites; i++)
				PaintRectPart(*sdraw, rc.left, start + 2 * DRAG_STEP * i, sz.cx, DRAG_STEP);
		}
#if defined(PLATFORM_WIN32)
		SelectObject(*sdraw, old_brush);
		DeleteObject(brush);
		SetTextColor(*sdraw, old_bk);
		sdraw->EndGdi();
#elif defined(GUI_X11)
		XChangeGC(Xdisplay, sdraw->GetGC(), GCForeground | GCFunction, &gcv_old);
#endif
	}
}

void PaintDragRect(Draw& draw, const Rect& rc, Color c1, Color c2, Color bgnd, int width)
{
	if(rc.IsEmpty())
		return;
	if(rc.Height() <= 2 * width)
		PaintDragHorzLine(draw, rc, LtRed, White, SWhite, width);
	else if(rc.Width() <= 2 * width)
		PaintDragVertLine(draw, rc, LtRed, White, SWhite, width);
	else {
		PaintDragHorzLine(draw, Rect(rc.left, rc.top, rc.right, rc.top + width), LtRed, White, SWhite, width);
		PaintDragHorzLine(draw, Rect(rc.left, rc.bottom - width, rc.right, rc.bottom), LtRed, White, SWhite, width);
		PaintDragVertLine(draw, Rect(rc.left, rc.top + width, rc.left + width, rc.bottom - width), LtRed, White, SWhite, 0);
		PaintDragVertLine(draw, Rect(rc.right - width, rc.top + width, rc.right, rc.bottom - width), LtRed, White, SWhite, 0);
	}
}

void PlotDragRect(Plotter& plotter, const Rectf& r)
{
	if(!IsNull(r))
	{
		Rect rc = RectfToRect(plotter.LtoP(r).Inflated(1, 1, 2, 2)) & plotter.clip.Inflated(10);
		PaintDragRect(plotter.GetDraw(), rc, LtRed, White, SWhite, 3);
	}
}

PlotterTool::PlotterTool()
: last_point(0, 0)
, clip_arcs(true)
, LineToRaw(0) // acts like a pure virtual function: throws exception when called
, MoveToRaw(0)
{
}

PlotterTool::~PlotterTool() {}

void PlotterTool::Paint()
{
}

void PlotterTool::Clear()
{
}

void PlotterTool::Line(const Array<Pointf>& points)
{
	if(!points.IsEmpty()) {
		bool line = false;
		for(int i = 0; i < points.GetCount(); i++)
			if(IsNull(points[i]))
				line = false;
			else {
				DrawTo(points[i], line);
				line = true;
			}
	}
}

bool PlotterTool::ArcToRaw(Pointf pt, double bulge)
{
	return false;
}

void PlotterTool::ArcTo(Pointf pt, double bulge, int levels)
{
	if(fabs(bulge) <= plotter.logprec)
		LineTo(pt);
	else
	{
		VecArcIterator arc(last_point, pt, bulge, LineToRaw);
		arc.Precision(plotter.logprec).ArcTo(THISBACK(ArcToRaw));
		if(clip_arcs)
			arc.Clip(plotter.logclip, MoveToRaw);
		arc.Go();
	}
}

void PlotterTool::Rectangle(double x1, double y1, double x2, double y2, bool r)
{
	MoveTo(x1, y1);
	LineTo(x2, y1);
	LineTo(x2, y2);
	if(!r || min(fabs(x2 - x1), fabs(y2 - y1)) * plotter.measure > 1)
	{
		LineTo(x1, y2);
		LineTo(x1, y1);
	}
}

void PlotterTool::Ellipse(const Rectf& rc)
{
	enum { SEGMENTS = 16 };
	Pointf centre = rc.CenterPoint();
	Pointf radius = Sizef(rc.Size()) / 2.0;
	Pointf beg(centre.x + radius.x, centre.y);
	MoveTo(beg);
//	int sign = sgn(plotter.measure);
	for(int i = 1; i <= SEGMENTS; i++)
	{
		double a = i * (2 * M_PI / SEGMENTS) - M_PI / SEGMENTS;
		Pointf mid = centre + radius * PolarPointf(a);
		Pointf end = centre + radius * PolarPointf(a + M_PI / SEGMENTS);
		double bulge = 0, dist = end | beg;
		if(dist)
			bulge = fabs((mid - beg) % (end - beg)) / dist;
		ArcTo(end, bulge);
		beg = end;
	}
}

void PlotterTool::ArrowHead(Pointf P, Pointf Q, double length, double angle)
{
	double base = Bearing(P, Q);
	Line(PolarPointf(Q, length, base + angle), Q);
	Line(PolarPointf(Q, length, base - angle), Q);
}

void PlotterTool::Arrow(Pointf P, Pointf Q, double length, double angle)
{
	Line(P, Q);
	ArrowHead(P, Q, min(length, P | Q), angle);
}

void PlotterTool::BiArrow(Pointf P, Pointf Q, double length, double angle)
{
	Line(P, Q);
	length = min(length, (P | Q) / 2);
	ArrowHead(P, Q, length, angle);
	ArrowHead(Q, P, length, angle);
}

void PlotterTool::Circle(double x, double y, double radius)
{
	Pointf right(x + radius, y);
	Pointf left(x - radius, y);
	MoveTo(right);
	ArcTo(left, radius);
	ArcTo(right, radius);
}

#ifdef PLOTTER_TIMING
int  PathTool::segments = 0;
int  PathTool::drawn = 0;
long PathTool::ticks = 0;
#endif

PathTool::PathTool()
: use_last(false)
, dummy(true)
{
	ClearExtent();
}

PathTool::~PathTool() {}

void PathTool::Reset()
{
	dummy = true;
	ClearExtent();
}

bool PathTool::Set(const Plotter& _info, const String& pattern, Color color, int width, double dash)
{
	if(dummy = IsNull(color)) {
		Reset();
		return false;
	}
	Clear();
	plotter = _info;
	pathdraw.Set(*plotter.draw, plotter.GetPath(pattern), color, width, dash, false);
	use_last = false;
	return true;
}

bool PathTool::SetExtent(const Rectf& rc)
{
	if(!dummy && rc.Intersects(plotter.logclip)) {
		Rectf rcc = plotter.LtoP(rc);
		simple = rcc.left  >= plotter.clip.left  && rcc.top    >= plotter.clip.top
			&&   rcc.right <= plotter.clip.right && rcc.bottom <= plotter.clip.bottom;
		bool within = true;
		if(simple)
			within = pathdraw.SetExtent(RectfToRect(rcc));
		else
			pathdraw.ClearExtent();
		MoveToRaw = (simple ? THISBACK(MoveToRawSimple) : THISBACK(MoveToRawClip));
		LineToRaw = (simple ? THISBACK(LineToRawSimple) : THISBACK(LineToRawClip));
		return within;
	}
	else {
		MoveToRaw = THISBACK(MoveToRawDummy);
		LineToRaw = THISBACK(LineToRawDummy);
		return false;
	}
}

void PathTool::ClearExtent()
{
	if(dummy) {
		MoveToRaw = THISBACK(MoveToRawDummy);
		LineToRaw = THISBACK(LineToRawDummy);
	}
	else {
		MoveToRaw = THISBACK(MoveToRawClip);
		LineToRaw = THISBACK(LineToRawClip);
		simple = false;
		pathdraw.ClearExtent();
	}
}

void PathTool::Paint()
{
	if(!dummy && plotter.draw)
		pathdraw.Paint();
	ClearExtent();
}

void PathTool::Clear()
{
	pathdraw.Clear();
	ClearExtent();
}

void PathTool::MoveToRawDummy(Pointf pt) {}
void PathTool::LineToRawDummy(Pointf pt) {}

void PathTool::MoveToRawSimple(Pointf pt)
{
	pathdraw.MoveTo(plotter.LtoPoint(last_point = pt));
}

void PathTool::MoveToRawClip(Pointf pt)
{
	last_phys = plotter.LtoP(last_point = pt);
	use_last = false;
}

void PathTool::LineToRawSimple(Pointf pt)
{
	if(IsNull(pt.x) || IsNull(pt.y))
	{
		use_last = false;
		return;
	}
#ifdef PLOTTER_TIMING
	ticks -= GetTickCount();
	segments++;
	drawn++;
#endif
	pathdraw.LineTo(plotter.LtoPoint(pt));
}

void PathTool::LineToRawClip(Pointf pt)
{
	if(IsNull(pt.x) || IsNull(pt.y))
	{
		use_last = false;
		return;
	}
	ASSERT(pt.x <= 1e100);
#ifdef PLOTTER_TIMING
	ticks -= GetTickCount();
	segments++;
#endif
	Pointf B = plotter.LtoP(pt);
#if CHECK_DIST
	if(use_last && fabs(B.x - last_phys.x) < 1 && fabs(B.y - last_phys.y) < 1)
	{
#ifdef PLOTTER_TIMING
		ticks += GetTickCount();
#endif
		return;
	}
#endif//CHECK_DIST
	Pointf A = last_phys, end = B;
	last_point = pt;
	last_phys = B;
	if(ClipLine(A, B, plotter.clip))
	{ // draw line
#ifdef PLOTTER_TIMING
		drawn++;
#endif
		if(!use_last)
			pathdraw.MoveTo(PointfToPoint(A));
		pathdraw.LineTo(PointfToPoint(B));
		use_last = (B | end) <= 1;
	}
	else
		use_last = false;
#ifdef PLOTTER_TIMING
	ticks += GetTickCount();
#endif
}

bool PathTool::ArcToRaw(Pointf pt, double bulge)
{
	if(bulge >= plotter.logdiag)
		return false;
	if(bulge <= plotter.logprec)
	{
		LineTo(pt);
		return true;
	}
	Pointf pt_phys = plotter.LtoP(pt);
	pathdraw.Arc(PointfToPoint(last_phys), PointfToPoint(pt_phys),
		fround(bulge * plotter.signed_measure));
	last_point = pt;
	last_phys = pt_phys;
	return true;
}

AreaTool::AreaTool()
{
	prev_ghost = false;
	clip_arcs = false;
	fill_color = Black;
	dummy = true;
	Clear();
}

AreaTool::~AreaTool() {}

bool AreaTool::SetExtent(const Rectf& rc)
{
	if(!dummy && rc.Intersects(plotter.logclip)) {
		Rectf rcc = plotter.LtoP(rc);
		simple = rcc.left  >= plotter.clip.left  && rcc.top    >= plotter.clip.top
			&&   rcc.right <= plotter.clip.right && rcc.bottom <= plotter.clip.bottom;
		MoveToRaw = (simple ? THISBACK(MoveToRawSimple) : THISBACK(MoveToRawClip));
		LineToRaw = (simple ? THISBACK(LineToRawSimple) : THISBACK(LineToRawClip));
		return true;
	}
	else {
		MoveToRaw = THISBACK(MoveToRawDummy);
		LineToRaw = THISBACK(LineToRawDummy);
		return false;
	}
}

void AreaTool::ClearExtent()
{
	if(dummy) {
		MoveToRaw = THISBACK(MoveToRawDummy);
		LineToRaw = THISBACK(LineToRawDummy);
	}
	else {
		MoveToRaw = THISBACK(MoveToRawClip);
		LineToRaw = THISBACK(LineToRawClip);
	}
	simple = false;
}

void AreaTool::Clear()
{
	begin_index = 0;
	disjunct_begin_index = 0;
	last_start = Null;
	last_in = true;
	vertices.Clear();
	counts.Clear();
	disjunct_counts.Clear();
	ClearExtent();
}

void AreaTool::MoveToRawDummy(Pointf pt) {}
void AreaTool::LineToRawDummy(Pointf pt) {}

void AreaTool::MoveToRawSimple(Pointf pt)
{
	Flush();
	if(prev_ghost)
		ghost_lines.Add(vertices.GetCount());
	vertices.Add(plotter.LtoPoint(last_point = pt));
}

void AreaTool::MoveToRawClip(Pointf pt)
{
	Flush();
	if(prev_ghost)
		ghost_lines.Add(vertices.GetCount());
	vertices.Add(ClipBind(last_phys = plotter.LtoP(last_point = last_start = pt)));
	last_in = (last_phys.x >= plotter.clip.left && last_phys.x <= plotter.clip.right
		&&     last_phys.y >= plotter.clip.top  && last_phys.y <= plotter.clip.bottom);
}

void AreaTool::LineToRawSimple(Pointf pt)
{
//	RTIMING("AreaTool::LineToRawSimple");
	Point ipt = plotter.LtoPoint(last_point = pt);
	if(ipt != vertices.Top())
	{
		if(prev_ghost)
			ghost_lines.Add(vertices.GetCount());
		vertices.Add(ipt);
	}
}

void AreaTool::LineToRawClip(Pointf pt)
{
	if(IsNull(pt.x) || IsNull(pt.y))
		return;
//	RTIMING("AreaTool::LineToRawClip");
#if DEBUG_AREA
	Pointf anchor(-747102, -1043537);
	bool show = (pt | anchor) <= 500 || (last_point | anchor) <= 500;
#endif
	Pointf next_phys = plotter.LtoP(pt);
#if CHECK_DIST
	if(fabs(next_phys.x - last_phys.x) < 1 && fabs(next_phys.y - last_phys.y) < 1)
		return;
#endif//CHECK_DIST
	last_point = pt;
	bool next_in = (next_phys.x >= plotter.clip.left && next_phys.x <= plotter.clip.right
		&&          next_phys.y >= plotter.clip.top  && next_phys.y <= plotter.clip.bottom);
#if DEBUG_AREA
	int vc = vertices.GetCount();
	if(show) {
		LOG("[" << vc << "/" << begin_index << "] AreaTool::LineToRawClip " << pt << ", next_phys " << next_phys
			<< ", top = " << vertices.Top() << ", last_in " << last_in << ", next_in " << next_in);
	}
#endif
	if(next_in && last_in) {
		Point ppt = PointfToPoint(next_phys);
		if(ppt != vertices.Top())
		{
			if(prev_ghost)
				ghost_lines.Add(vertices.GetCount());
			vertices.Add(ppt);
		}
		last_phys = next_phys;
		last_in = next_in;
	}
	else {
		Pointf A = last_phys, B = next_phys;
		bool clipped = ClipLine(A, B, plotter.clip);
		Point cb = ClipBind(clipped ? B : next_phys);
		Point ca = clipped ? ClipBind(A) : cb;
		if(vertices.GetCount() > begin_index || clipped)
			SkipTo(ca, (next_phys - last_phys) % (clip_center - last_phys) >= 0);
		if(clipped && cb != vertices.Top()) {
			if(prev_ghost)
				ghost_lines.Add(vertices.GetCount());
			vertices.Add(cb);
		}
		last_phys = next_phys;
		last_in = (clipped && B == next_phys);
	}
#if DEBUG_AREA
	Point check = plotter.LtoP(anchor);
	for(int t = vc; t < vertices.GetCount(); t++)
		if(tabs(vertices[t].x - check.x) <= 50 && tabs(vertices[t].y - check.y) <= 50)
		{
			LOG("AreaTool::LineToRawClip / orphan [" << t << "] = " << vertices[t] << " for " << pt);
		}

	if(show)
	{
		String plist;
		while(vc < vertices.GetCount())
			plist << " " << vertices[vc++];
		LOG("//AreaTool::LineToRawClip " << plist);
	}
#endif
}

void AreaTool::SkipTo(Point pt, bool clockwise)
{
/*
	if(vertices.GetCount() == begin_index)
	{
		vertices.Add(pt);
		return;
	}
*/
	Point la = vertices.Top();
	if(la == pt)
		return;

//	Size diff(pt.x + la.x - plotter.clip.left - plotter.clip.right, pt.y + la.y - plotter.clip.top - plotter.clip.bottom);
	if(la.y <= plotter.clip.top) {
		if(pt.y <= plotter.clip.top)
			Horz(pt.x);
		else if(pt.y >= plotter.clip.bottom)
			Horz(clockwise ? plotter.clip.right : plotter.clip.left).Vert(plotter.clip.bottom).Horz(pt.x);
		else if(pt.x <= plotter.clip.left)
			Horz(plotter.clip.left).Vert(pt.y);
		else if(pt.x >= plotter.clip.right)
			Horz(plotter.clip.right).Vert(pt.y);
		else
			NEVER();
	}
	else if(la.y >= plotter.clip.bottom) {
		if(pt.y <= plotter.clip.top)
			Horz(clockwise ? plotter.clip.left : plotter.clip.right).Vert(plotter.clip.top).Horz(pt.x);
		else if(pt.y >= plotter.clip.bottom)
			Horz(pt.x);
		else if(pt.x <= plotter.clip.left)
			Horz(plotter.clip.left).Vert(pt.y);
		else if(pt.x >= plotter.clip.right)
			Horz(plotter.clip.right).Vert(pt.y);
		else
			NEVER();
	}
	else if(la.x <= plotter.clip.left) {
		if(pt.y <= plotter.clip.top)
			Vert(plotter.clip.top).Horz(pt.x);
		else if(pt.y >= plotter.clip.bottom)
			Vert(plotter.clip.bottom).Horz(pt.x);
		else if(pt.x <= plotter.clip.left)
			Vert(pt.y);
		else if(pt.x >= plotter.clip.right)
			Vert(clockwise ? plotter.clip.top : plotter.clip.bottom).Horz(plotter.clip.right).Vert(pt.y);
		else
			NEVER();
	}
	else if(la.x >= plotter.clip.right) {
		if(pt.y <= plotter.clip.top)
			Vert(plotter.clip.top).Horz(pt.x);
		else if(pt.y >= plotter.clip.bottom)
			Vert(plotter.clip.bottom).Horz(pt.x);
		else if(pt.x <= plotter.clip.left)
			Vert(clockwise ? plotter.clip.bottom : plotter.clip.top).Horz(plotter.clip.left).Vert(pt.y);
		else if(pt.x >= plotter.clip.right)
			Vert(pt.y);
		else
			NEVER();
	}
	else
		NEVER(); // invalid case
}

void AreaTool::Flush()
{
	if((vertices.GetCount() - begin_index >= 3) && !simple) {
		ASSERT(!IsNull(last_start));
		LineTo(last_start);
	}
	int add = vertices.GetCount() - begin_index;
	ASSERT(add >= 0);
	if(add >= 2 && vertices.Top() == vertices[begin_index]) {
		vertices.Drop(); // remove superfluous end point
		if(!ghost_lines.IsEmpty() && ghost_lines.Top() == vertices.GetCount())
			ghost_lines.Drop();
		add--;
	}
	if(add <= 2) {
		vertices.SetCountR(begin_index);
		while(!ghost_lines.IsEmpty() && ghost_lines.Top() >= begin_index)
			ghost_lines.Drop();
	}
	else {
		counts.Add(add);
		begin_index = vertices.GetCount();
	}
}

AreaTool& AreaTool::Horz(int x)
{
	int count = vertices.GetCount() - begin_index;
	ASSERT(count > 0);
	Point *p = vertices.End();
	int y = p[-1].y;
	if(count >= 2 && y == p[-2].y) {
		while(count > 2 && y == p[-3].y) {
			count--;
			p--;
		}
		p[-1].x = x;
		int end = begin_index + count;
		vertices.SetCountR(end);
		while(!ghost_lines.IsEmpty() && ghost_lines.Top() >= end)
			ghost_lines.Drop();
	}
	else {
		if(prev_ghost)
			ghost_lines.Add(vertices.GetCount());
		vertices.Add(Point(x, y));
	}
	return *this;
}

AreaTool& AreaTool::Vert(int y)
{
	int count = vertices.GetCount() - begin_index;
	ASSERT(count > 0);
	Point *p = vertices.End();
	int x = p[-1].x;
	if(count >= 2 && x == p[-2].x) {
		while(count > 2 && x == p[-3].x) {
			count--;
			p--;
		}
		p[-1].y = y;
		int end = begin_index + count;
		vertices.SetCountR(end);
		while(!ghost_lines.IsEmpty() && ghost_lines.Top() >= end)
			ghost_lines.Drop();
	}
	else {
		if(prev_ghost)
			ghost_lines.Add(vertices.GetCount());
		vertices.Add(Point(x, y));
	}
	return *this;
}

void AreaTool::Reset()
{
	dummy = true;
	Clear();
}

bool AreaTool::Set(const Plotter& _info, Color _fill_color, uint64 _fill_pattern,
	const String& outline_pattern, Color outline_color, int outline_width, double outline_dash)
{
	dummy = IsNull(_fill_color) && IsNull(outline_color);
	if(dummy) {
		Reset();
		return false;
	}
	if(outline_width < 0)
		outline_width = DotsToPixels(*_info.draw, -outline_width);
	const PathStyle& path = _info.GetPath(outline_pattern);
	plotter = Plotter(_info, int(outline_width * path.width) + 2);
	clip_center = Pointf(plotter.clip.CenterPoint());
	fill_color = _fill_color;
	fill_pattern = _fill_pattern;
	double pw;
	Color pc;
	bool is_solid = path.IsSolid(pw, pc);
	outline_pixels = (outline_width >= 0 ? outline_width : fround(_info.GetPixelsPerDot() * -outline_width));
	pw *= outline_width;
	is_line = !path.IsEmpty();
	std_pen = !is_line || IsNull(outline_color)
	|| (ghost_lines.IsEmpty() && is_solid && pw < (_info.draw->Dots() ? 20 : 2.5) /*&& !IsNull(fill_color)*/ && !fill_pattern);
	if(std_pen) {
		fill_outline_color = is_solid ? Nvl(pc, outline_color) : Color(Null);
		fill_outline_style = is_solid ? fround(pw) : 0;
		if(_info.draw->Pixels() && outline_width > 1)
		{
			thick_outline_color = fill_outline_color;
			fill_outline_color = Null;
			fill_outline_style = 0;
		}
	}
	else {
		raw_outline.Set(*plotter.draw, path, outline_color, outline_width, outline_dash, true);
		fill_outline_color = thick_outline_color = Null;
		fill_outline_style = 0;
	}
	point_pixels = outline_pixels;
	point_color = Nvl(Nvl(pc, outline_color), fill_color);
	Clear();
#if DEBUG_AREA
	LOG("AreaTool::Set, phys clip = " << plotter.clip << ", log clip = " << plotter.logclip
		<< ", a = " << plotter.physical.a);
#endif
	return true;
}

void AreaTool::FlushFill()
{
	Flush();
	if(IsFill() && disjunct_begin_index < vertices.GetCount())
	{
		disjunct_counts.Add(vertices.GetCount() - disjunct_begin_index);
		disjunct_begin_index = vertices.GetCount();
	}
	if(vertices.GetCount() >= FLUSH_BATCH)
		Paint();
}

void AreaTool::Paint()
{
	if(!plotter.draw)
		return;
	if(!(plotter.clip.Intersects(plotter.draw->GetPaintRect()))) {
		Clear();
		return;
	}
	Flush();
	if(vertices.IsEmpty() || counts.IsEmpty())
	{
		Clear();
		return;
	}
	if(disjunct_begin_index < vertices.GetCount())
		disjunct_counts.Add(vertices.GetCount() - disjunct_begin_index);
	if(!IsNull(fill_color) || !IsNull(fill_outline_color))
		DrawPolyPolyPolygon(*plotter.draw, vertices, counts, disjunct_counts,
			fill_color, fill_outline_style, fill_outline_color, fill_pattern);
	if(!IsNull(thick_outline_color))
		DrawFatPolyPolygonOutline(*plotter.draw, vertices, counts,
			thick_outline_color, outline_pixels);
/*
	else if(fill_pattern)
	{
//		RTIMING("AreaTool::Fill(pattern)");
		HDC hdc = *plotter.draw;
		int old_rop = SetROP2(hdc, R2_MASKPEN);
		HGDIOBJ old_brush = SelectObject(hdc, fill_pattern.GetBrush(Null, Null));
		int old_color = SetTextColor(hdc, Black);
		int old_bk = SetBkColor(hdc, White);
		plotter.draw->SetDrawPen(PEN_NULL, Black);
		if(counts.GetCount() == 1)
			Polygon(*plotter.draw, (const POINT *)vertices.Begin(), vertices.GetCount());
		else
			PolyPolygon(*plotter.draw, (const POINT *)vertices.Begin(), counts.Begin(), counts.GetCount());
		SetROP2(hdc, R2_MERGEPEN);
		if(IsNull(fill_color)) // use color fill brush
			SelectObject(hdc, fill_pattern.GetBrush(Null, Black));
		else
		{ // just change text color
			SetTextColor(hdc, fill_color);
			SetBkColor(hdc, Black);
		}
		if(counts.GetCount() == 1)
			Polygon(*plotter.draw, (const POINT *)vertices.Begin(), vertices.GetCount());
		else
			PolyPolygon(*plotter.draw, (const POINT *)vertices.Begin(), counts.Begin(), counts.GetCount());
		SetTextColor(hdc, old_color);
		SetBkColor(hdc, old_bk);
		SetROP2(hdc, old_rop);
		SelectObject(hdc, old_brush);
	}
	else if(!IsNull(fill_color) || (std_pen && raw_outline_style != PEN_NULL))
	{ // simple fill
//		RTIMING("AreaTool::Fill(solid color)");
		plotter.draw->SetDrawPen(raw_outline_style, raw_outline_color);
		HGDIOBJ old_brush = 0;
		if(IsNull(fill_color))
		{
			static HGDIOBJ null_brush = GetStockObject(NULL_BRUSH);
			old_brush = SelectObject(*plotter.draw, null_brush);
		}
		else
			plotter.draw->SetColor(fill_color);
		if(counts.GetCount() == 1)
			Polygon(*plotter.draw, (const POINT *)vertices.Begin(), vertices.GetCount());
		else
			PolyPolygon(*plotter.draw, (const POINT *)vertices.Begin(), counts.Begin(), counts.GetCount());
		if(old_brush)
			SelectObject(*plotter.draw, old_brush);
	}
*/
	if(!std_pen)
	{ // manual outline
//		RTIMING("AreaTool::Outline()");
		if(ghost_lines.IsEmpty())
		{
			const Point *p = vertices.Begin();
			for(int i = 0; i < counts.GetCount(); i++)
			{
				const Point *b = p, *n = p + counts[i];
				raw_outline.MoveTo(*p);
				while(++p < n)
					raw_outline.LineTo(*p);
/*
				if((p[0].x >= plotter.clip.right || p[0].x <= plotter.clip.left) && p[-1].x == p[0].x
				|| (p[0].y >= plotter.clip.bottom || p[0].y <= plotter.clip.top) && p[-1].y == p[0].y)
					outline.MoveTo(*p);
				else
					outline.LineTo(*p);
*/
//			if((b[0].x >= plotter.clip.right || b[0].x <= plotter.clip.left) && p[-1].x == b[0].x
//			|| (b[0].y >= plotter.clip.bottom || b[0].y <= plotter.clip.top) && p[-1].y == b[0].y)
//				;
//			else
//				outline.LineTo(*b);
			}
		}
		else
		{
			int gl = 0;
			int ngl = (gl >= ghost_lines.GetCount() ? vertices.GetCount() : ghost_lines[gl]);
			int v = 0;
			for(int i = 0; i < counts.GetCount(); i++)
			{
				int nv = v + counts[i];
				if(nv >= v + 2)
				{
					for(raw_outline.MoveTo(vertices[nv - 1]); v < nv; v++)
					{
						if(ngl == v)
						{
							ngl = (++gl >= ghost_lines.GetCount() ? vertices.GetCount() : ghost_lines[gl]);
							raw_outline.MoveTo(vertices[v]);
						}
						else
							raw_outline.LineTo(vertices[v]);
					}
				}
				else if(ngl < nv)
					ngl = (++gl >= ghost_lines.GetCount() ? vertices.GetCount() : ghost_lines[gl]);
				v = nv;
			}
		}
		raw_outline.Paint();
	}
	Clear();
}

class EmptyMarker : public MarkTool::Marker
{
public:
	virtual void Paint(Draw& draw, const Vector<Point>& pt) {}
	virtual int  GetSize() const                            { return 0; }
};


One<MarkTool::Marker> MarkTool::Empty()
{
	return new EmptyMarker;
}

class ImageMarker : public MarkTool::Marker
{
public:
	ImageMarker(const Image& img, int size, Color color, bool antialias, bool ignore_hotspot);

	virtual void  Paint(Draw& draw, const Vector<Point>& pt);
	virtual int   GetSize() const { return linear_size; }

private:
	Image         img;
	Size          imgsize;
	Size          outsize;
	Point         hotspot;
	int           linear_size;
	Color         color;
//	bool          body;
	bool          mask;
	bool          antialias;
	bool          ignore_hotspot;
};

ImageMarker::ImageMarker(const Image& srcimg, int size_, Color color_, bool antialias_, bool ignore_hotspot_)
: imgsize(srcimg.GetSize())
, color(color_)
, antialias(antialias_)
, ignore_hotspot(ignore_hotspot_)
, outsize(0, 0)
{
	hotspot = (ignore_hotspot ? (Point)(srcimg.GetSize() >> 1) : srcimg.GetHotSpot());
	if(imgsize.cx > 0 && imgsize.cy > 0) {
		hotspot = iscale(hotspot, size_, max(imgsize.cx, imgsize.cy));
		if(imgsize.cx > imgsize.cy)
			outsize = Size(size_, iscale(size_, imgsize.cy, imgsize.cx));
		else
			outsize = Size(iscale(size_, imgsize.cx, imgsize.cy), size_);
		if(imgsize == outsize || outsize.cx >= imgsize.cx && outsize.cy >= imgsize.cy && !antialias)
			img = srcimg;
		else {
			Size xsize = 2 * imgsize;
			while(2 * xsize.cx <= outsize.cx && 2 * xsize.cy <= outsize.cy && xsize.cx <= 256 && xsize.cy <= 256)
				xsize <<= 2;
			Size tmpsize = min(outsize, xsize);
//			if(antialias)
			img = Rescale(srcimg, tmpsize);
			// else RescaleNoAA !! todo
		}
		imgsize = img.GetSize();
	}
	linear_size = max(
		max(hotspot.x, outsize.cx - hotspot.x),
		max(hotspot.y, outsize.cy - hotspot.y));
	mask = (srcimg.GetKind() == IMAGE_OPAQUE);
}

void ImageMarker::Paint(Draw& draw, const Vector<Point>& pts)
{
	if(img.IsEmpty())
		return;
	for(int t = 0; t < pts.GetCount(); t++) {
		Point pt = pts[t] - hotspot;
		draw.DrawImage(Rect(pt, outsize), img);
	}
}

One<MarkTool::Marker> MarkTool::Picture(Image pic, int size, Color color, bool antialias, bool ignore_hotspot)
{
	return new ImageMarker(pic, size, color, antialias, ignore_hotspot);
}

class AreaMarker : public MarkTool::Marker
{
public:
	AreaMarker(int size, Color color, Color outline, int outline_width)
	: size(size), color(color), outline(outline), outline_width(outline_width)
	{ out_size = (size + outline_width) >> 1; }

	virtual int  GetSize() const { return out_size; }

public:
	int          size;
	Color        color;
	Color        outline;
	int          outline_width;
	int          out_size;
};

class SquareMarker : public AreaMarker
{
public:
	SquareMarker(int size, Color color, Color outline, int outline_width)
	: AreaMarker(size, color, outline, outline_width) {}

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
};

void SquareMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	Vector<Point> outpt;
	int obj = pt.GetCount();
	outpt.SetCount(4 * obj);
	Point *op = outpt.Begin();
	int half = size >> 1;
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 4)
	{
		op[0].x = op[3].x = ip->x - half;
		op[0].y = op[1].y = ip->y - half;
		op[1].x = op[2].x = ip->x + half;
		op[2].y = op[3].y = ip->y + half;
	}
	Vector<int> outix;
	outix.SetCount(obj, 4);
	draw.DrawPolygons(outpt, outix, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Square(int size, Color color, Color outline, int outline_width)
{
	return new SquareMarker(size, color, outline, outline_width);
}

class Square45Marker : public AreaMarker
{
public:
	Square45Marker(int size, Color color, Color outline, int outline_width)
	: AreaMarker(size, color, outline, outline_width) {}

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
};

void Square45Marker::Paint(Draw& draw, const Vector<Point>& pt)
{
	Vector<Point> outpt;
	int obj = pt.GetCount();
	outpt.SetCount(4 * obj);
	Point *op = outpt.Begin();
	int half = size >> 1;
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 4)
	{
		op[0].x = ip->x - half;
		op[0].y = op[2].y = ip->y;
		op[1].x = op[3].x = ip->x;
		op[1].y = ip->y + half;
		op[2].x = ip->x + half;
		op[3].y = ip->y - half;
	}
	Vector<int> outix;
	outix.SetCount(obj, 4);
	draw.DrawPolygons(outpt, outix, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Square45(int size, Color color, Color outline, int outline_width)
{
	return new Square45Marker(size, color, outline, outline_width);
}

class TriangleMarker : public AreaMarker
{
public:
	TriangleMarker(int size, Color color, Color outline, int outline_width)
	: AreaMarker(size, color, outline, outline_width) {}

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
};

void TriangleMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	int half = size >> 1;
	Vector<Point> outpt;
	int obj = pt.GetCount();
	outpt.SetCount(3 * obj);
	Point *op = outpt.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 3)
	{
		op[0].x = ip->x - half;
		op[1].x = ip->x + half;
		op[0].y = op[1].y = ip->y + half + 1;
		op[2].x = ip->x;
		op[2].y = ip->y - half;
	}
	Vector<int> outix;
	outix.SetCount(obj, 3);
	draw.DrawPolygons(outpt, outix, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Triangle(int size, Color color, Color outline, int outline_width)
{
	return new TriangleMarker(size, color, outline, outline_width);
}

class NablaMarker : public AreaMarker
{
public:
	NablaMarker(int size, Color color, Color outline, int outline_width)
	: AreaMarker(size, color, outline, outline_width) {}

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
};

void NablaMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	int half = size >> 1;
	Vector<Point> outpt;
	int obj = pt.GetCount();
	outpt.SetCount(3 * obj);
	Point *op = outpt.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 3)
	{
		op[0].x = ip->x - half;
		op[1].x = ip->x + half;
		op[0].y = op[1].y = ip->y - half;
		op[2].x = ip->x;
		op[2].y = ip->y + half + 1;
	}
	Vector<int> outix;
	outix.SetCount(obj, 3);
	draw.DrawPolygons(outpt, outix, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Nabla(int size, Color color, Color outline, int outline_width)
{
	return new NablaMarker(size, color, outline, outline_width);
}

class CircleMarker : public AreaMarker
{
public:
	CircleMarker(int size, Color color, Color outline, int outline_width);

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
};

CircleMarker::CircleMarker(int size, Color color, Color outline, int outline_width)
: AreaMarker(size, color, Nvl(outline, color), IsNull(outline) ? 0 : outline_width)
{
}

void CircleMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	int half = size >> 1;
	for(int t = 0; t < pt.GetCount(); t++)
		draw.DrawEllipse(RectC(pt[t].x - half, pt[t].y - half, size, size),
			color, outline_width, Nvl(outline, color));
}

One<MarkTool::Marker> MarkTool::Circle(int size, Color color, Color outline, int outline_width)
{
	return new CircleMarker(size, color, outline, outline_width);
}

class CrossMarker : public AreaMarker
{
public:
	CrossMarker(int size, Color outline, int outline_width)
	: AreaMarker(size, Null, outline, outline_width) {}

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
};

void CrossMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	int half = size >> 1;
	Vector<Point> out;
	out.SetCount(pt.GetCount() * 4);
	Point *op = out.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 4)
	{
		op[0].x = ip->x - half;
		op[1].x = ip->x + half;
		op[0].y = op[1].y = ip->y;
		op[2].x = op[3].x = ip->x;
		op[2].y = ip->y - half;
		op[3].y = ip->y + half;
	}
	Vector<int> seg;
	seg.SetCount(2 * pt.GetCount(), 2);
	draw.DrawPolyPolyline(out, seg, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Cross(int size, Color outline, int outline_width)
{
	return new CrossMarker(size, outline, outline_width);
}

class DiamondMarker : public AreaMarker
{
public:
	DiamondMarker(int size, Color color, Color outline, int outline_width);

	virtual void Paint(Draw& draw, const Vector<Point>& pt);

private:
	int x2, y2, x4;
};

DiamondMarker::DiamondMarker(int size, Color color, Color outline, int outline_width)
: AreaMarker(size, color, outline, outline_width)
{
	y2 = size >> 1;
	x2 = size * 17 >> 5;
	x4 = x2 >> 1;
}

void DiamondMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	Vector<Point> out;
	out.SetCount(pt.GetCount() * 5);
	Point *op = out.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 5)
	{
		op[0].x = ip->x - x4; op[0].y = ip->y + y2;
		op[1].x = ip->x + x4; op[1].y = ip->y + y2;
		op[2].x = ip->x + x2; op[2].y = ip->y;
		op[3].x = ip->x;      op[3].y = ip->y - y2;
		op[4].x = ip->x - x2; op[4].y = ip->y;
	}
	Vector<int> seg;
	seg.SetCount(pt.GetCount(), 5);
	draw.DrawPolygons(out, seg, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Diamond(int size, Color color, Color outline, int outline_width)
{
	return new DiamondMarker(size, color, outline, outline_width);
}

class HexagonMarker : public AreaMarker
{
public:
	HexagonMarker(int size, Color color, Color outline, int outline_width);

	virtual void Paint(Draw& draw, const Vector<Point>& pt);

private:
	int x2, y2, x4;
};

HexagonMarker::HexagonMarker(int size, Color color, Color outline, int outline_width)
: AreaMarker(size, color, outline, outline_width)
{
	y2 = size >> 1;
	x2 = size * 19 >> 5;
	x4 = x2 >> 1;
}

void HexagonMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	Vector<Point> out;
	out.SetCount(pt.GetCount() * 6);
	Point *op = out.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 6)
	{
		op[0].x = ip->x - x4; op[0].y = ip->y + y2;
		op[1].x = ip->x + x4; op[1].y = ip->y + y2;
		op[2].x = ip->x + x2; op[2].y = ip->y;
		op[3].x = ip->x + x4; op[3].y = ip->y - y2;
		op[4].x = ip->x - x4; op[4].y = ip->y - y2;
		op[5].x = ip->x - x2; op[5].y = ip->y;
	}
	Vector<int> seg;
	seg.SetCount(pt.GetCount(), 6);
	draw.DrawPolygons(out, seg, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Hexagon(int size, Color color, Color outline, int outline_width)
{
	return new HexagonMarker(size, color, outline, outline_width);
}

class StarMarker : public AreaMarker
{
public:
	StarMarker(int size, Color color, Color outline, int outline_width);

	virtual void Paint(Draw& draw, const Vector<Point>& pt);

private:
	int x2, x4, X2, X4, y2, y4, Y2, Y4, dy;
};

StarMarker::StarMarker(int size, Color color, Color outline, int outline_width)
: AreaMarker(size, color, outline, outline_width)
{
	y2 = size >> 1;
	x2 = size * 18 >> 5;
	x4 = size * 11 >> 5;
	Y2 = y2 * 5 >> 4;
	Y4 = Y2 * 5 >> 4;
	X2 = x2 * 5 >> 4;
	X4 = x4 * 5 >> 4;
	dy = size / 18;
}

void StarMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	Vector<Point> out;
	out.SetCount(pt.GetCount() * 10);
	Point *op = out.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 10)
	{
		int sy = ip->y + dy;
		op[0].x = ip->x - x4; op[0].y = ip->y + y2;
		op[1].x = ip->x;      op[1].y = sy      + Y2;
		op[2].x = ip->x + x4; op[2].y = ip->y + y2;
		op[3].x = ip->x + X2; op[3].y = sy      + Y4;
		op[4].x = ip->x + x2; op[4].y = ip->y - Y4;
		op[5].x = ip->x + X4; op[5].y = sy      - Y2;
		op[6].x = ip->x;      op[6].y = ip->y - y2;
		op[7].x = ip->x - X4; op[7].y = sy      - Y2;
		op[8].x = ip->x - x2; op[8].y = ip->y - Y4;
		op[9].x = ip->x - X2; op[9].y = sy      + Y4;
	}
	Vector<int> seg;
	seg.SetCount(pt.GetCount(), 10);
	draw.DrawPolygons(out, seg, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::Star(int size, Color color, Color outline, int outline_width)
{
	return new StarMarker(size, color, outline, outline_width);
}

class ThickCrossMarker : public AreaMarker
{
public:
	ThickCrossMarker(int size, Color color, Color outline, int outline_width);

	virtual void Paint(Draw& draw, const Vector<Point>& pt);

private:
	int s2, s6;
};

ThickCrossMarker::ThickCrossMarker(int size, Color color, Color outline, int outline_width)
: AreaMarker(size, color, outline, outline_width)
{
	s2 = size >> 1;
	s6 = size / 6;
}

void ThickCrossMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	Vector<Point> out;
	out.SetCount(pt.GetCount() * 12);
	Point *op = out.Begin();
	for(const Point *ip = pt.Begin(), *ie = pt.End(); ip < ie; ip++, op += 12) {
		int x = ip->x, y = ip->y;
		op[0].x = op[11].x = x + s2;
		op[1].x = op[2].x = op[9].x = op[10].x = x + s6;
		op[3].x = op[4].x = op[7].x = op[8].x = x - s6;
		op[5].x = op[6].x = x - s2;

		op[2].y = op[3].y = y + s2;
		op[0].y = op[1].y = op[4].y = op[5].y = y + s6;
		op[6].y = op[7].y = op[10].y = op[11].y = y - s6;
		op[8].y = op[9].y = y - s2;
	}
	Vector<int> seg;
	seg.SetCount(pt.GetCount(), 12);
	draw.DrawPolygons(out, seg, color, outline_width, outline);
}

One<MarkTool::Marker> MarkTool::ThickCross(int size, Color color, Color outline, int outline_width)
{
	return new ThickCrossMarker(size, color, outline, outline_width);
}

class LetterMarker : public MarkTool::Marker
{
public:
	LetterMarker(char ascii, int angle, Font font, Color color, Color outline);

	virtual void Paint(Draw& draw, const Vector<Point>& pt);
	virtual int  GetSize() const;

private:
	void         PaintSimple(Point pt);
	void         PaintOutline(Point pt);
	void         PaintImage(Point pt);
	void         PaintEmpty(Point pt);

private:
	Draw         *draw;
	char          ascii;
	Font          font;
	Font          raw_font;
	Color         color;
	Color         outline;
//	Image         image;
//	ImageDraw     data;
//	ImageMaskDraw mask;
	int           angle;
	int           img_size;
	int           raw_angle;
	Size          offset;

//	HFONT        new_font, old_font;
//	int          old_align;
};

LetterMarker::LetterMarker(char ascii, int angle, Font font, Color color, Color outline)
: ascii(ascii)
, angle(angle)
, font(font)
, color(color)
, outline(outline)
{
}

int LetterMarker::GetSize() const
{
	Font raw = font;
	if(raw.GetHeight() >= 0)
		raw.Height(-raw.GetHeight());
	else
		raw.Height(-DotsToPixels(ScreenInfo(), -raw.GetHeight()));
	FontInfo info = raw.Info();
	return fceil(hypot(info[(byte)ascii], info.GetHeight())) >> 1;
}

static void DrawTextWithMask(Draw& data, Draw& mask, int x, int y, char letter, int angle, Font font, Color cdata, Color cmask)
{
	data.DrawText(x, y, angle, &letter, font, cdata, 1);
	mask.DrawText(x, y, angle, &letter, font, cmask, 1);
}

void LetterMarker::Paint(Draw& draw, const Vector<Point>& pt)
{
	double ang = angle * (M_PI / 180), c = cos(ang), s = sin(ang);

	raw_font = font;
	if(raw_font.GetHeight() >= 0)
		raw_font.Height(-raw_font.GetHeight());
	else
		raw_font.Height(-DotsToPixels(draw, -raw_font.GetHeight()));
	Size size = GetTextSize(&ascii, raw_font, 1);
//	offset = Pointf(size).Rotated(-angle * M_PI / 1800.0).AsSize() >> 1;

/*
	new_font = CreateFont(raw_font.GetHeight(),
		                  0, 10 * angle, 10 * angle, raw_font.IsBold() ? FW_BOLD : FW_NORMAL,
		                  raw_font.IsItalic(), raw_font.IsUnderline(), raw_font.IsStrikeout(),
						  raw_font.GetCharSet() ? raw_font.GetCharSet() : DEFAULT_CHARSET,
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE,
						  raw_font.GetFaceName());
	draw->BeginGdi();
	old_font = (HFONT)SelectObject(*draw, new_font);
*/
//	Size size;
//	GetTextExtentPoint32(*draw, &ascii, 1, &size);

	raw_angle = 10 * angle;
	offset.cx = fround((size.cx * c + size.cy * s) * 0.5);
	offset.cy = fround((size.cx * s - size.cy * c) * -0.5);

	if(IsNull(color))
		;
	else if(IsNull(outline))
	{
		for(int i = 0; i < pt.GetCount(); i++)
			draw.DrawText(pt[i].x - offset.cx, pt[i].y - offset.cy, raw_angle, &ascii, raw_font, color, 1);
	}
	else
	{
		for(int i = 0; i < pt.GetCount(); i++)
		{
			Point pos = pt[i] - offset;
			draw.DrawText(pos.x + 1, pos.y + 0, raw_angle, &ascii, raw_font, outline, 1);
			draw.DrawText(pos.x + 0, pos.y - 1, raw_angle, &ascii, raw_font, outline, 1);
			draw.DrawText(pos.x - 1, pos.y + 0, raw_angle, &ascii, raw_font, outline, 1);
			draw.DrawText(pos.x + 0, pos.y + 1, raw_angle, &ascii, raw_font, outline, 1);
			draw.DrawText(pos.x + 0, pos.y + 0, raw_angle, &ascii, raw_font, color, 1);
		}
	}

/*
	if(!IsNull(outline))
	{
		if(IsNull(color))
		{ // generate image with outline
			img_size = abs(size);
			image = Image(*draw, img_size, img_size);
			data.Open(*draw, image);
			mask.Open(image);
			Point pos((img_size >> 1) - offset.cx, (img_size >> 1) - offset.cy);
			data.DrawRect(0, 0, img_size, img_size, Black);
			mask.DrawRect(0, 0, img_size, img_size, White);
			DrawTextWithMask(data, mask, pos.x + 1, pos.y + 0, ascii, raw_angle, raw_font, outline, Black);
			DrawTextWithMask(data, mask, pos.x + 0, pos.y - 1, ascii, raw_angle, raw_font, outline, Black);
			DrawTextWithMask(data, mask, pos.x - 1, pos.y + 0, ascii, raw_angle, raw_font, outline, Black);
			DrawTextWithMask(data, mask, pos.x + 0, pos.y + 1, ascii, raw_angle, raw_font, outline, Black);
			DrawTextWithMask(data, mask, pos.x + 0, pos.y + 0, ascii, raw_angle, raw_font, Black, White);
			offset.cx = offset.cy = img_size >> 1;
			PaintRaw = &LetterMarker::PaintImage;
		}
		else
			PaintRaw = &LetterMarker::PaintOutline;
	}
*/

//	old_align = SetTextAlign(*draw, TA_LEFT | TA_TOP);
//	SetTextColor(*draw, color);
}

One<MarkTool::Marker> MarkTool::Letter(char ascii, int angle, Font font, Color color, Color outline)
{
	return new LetterMarker(ascii, angle, font, color, outline);
}

One<MarkTool::Marker> MarkTool::StandardMarker(int type, int size, Color color, Color outline, int outline_width)
{
	switch(type)
	{
	case CIRCLE:   return Circle(size, color, outline, outline_width);
	case SQUARE:   return Square(size, color, outline, outline_width);
	case TRIANGLE: return Triangle(size, color, outline, outline_width);
	case CROSS:    return Cross(size, outline, outline_width);
	case DIAMOND:  return Diamond(size, color, outline, outline_width);
	case HEXAGON:  return Hexagon(size, color, outline, outline_width);
	case STAR:     return Star(size, color, outline, outline_width);
	case SQUARE45: return Square45(size, color, outline, outline_width);
	case NABLA:    return Nabla(size, color, outline, outline_width);
	default:       return Empty();
	}
}

MarkTool::MarkTool()
{
	PutRaw = &MarkTool::PutDummy;
}

MarkTool::~MarkTool() {}

void MarkTool::Reset()
{
	marker.Clear();
	Clear();
}

bool MarkTool::Set(const Plotter& info, One<Marker> _marker)
{
	if(!_marker) {
		Reset();
		return false;
	}
	marker = _marker;
	size = marker->GetSize();
	plotter.Set(info, size);
	Clear();
	return true;
}

bool MarkTool::SetExtent(const Rectf& rc)
{
	if(marker && plotter.IntersectsLClip(rc)) {
		Rectf crc = plotter.LtoP(rc);
		if(plotter.InPClip(crc))
			PutRaw = &MarkTool::PutSimple;
		else
			PutRaw = &MarkTool::PutClip;
		return true;
	}
	else {
		PutRaw = &MarkTool::PutDummy;
		return false;
	}
}

void MarkTool::ClearExtent()
{
	if(marker)
		PutRaw = &MarkTool::PutClip;
	else
		PutRaw = &MarkTool::PutDummy;
}

void MarkTool::Put(const Array<Pointf>& pt)
{
	for(int i = 0; i < pt.GetCount(); i++)
		Put(pt[i]);
}

void MarkTool::Paint()
{
	if(!plotter.draw)
		return;
	Flush();
	ClearExtent();
}

void MarkTool::Flush()
{
	if(!vertices.IsEmpty()) {
		marker->Paint(*plotter.draw, vertices);
		vertices.Clear();
	}
}

void MarkTool::Clear()
{
	vertices.Clear();
	ClearExtent();
}

void MarkTool::PutDummy(Pointf pt) {}

void MarkTool::PutSimple(Pointf pt)
{
	if(IsNull(pt.x) || IsNull(pt.y))
		return;
	vertices.Add(plotter.LtoPoint(pt));
	if(vertices.GetCount() >= BUF_COUNT)
		Flush();
}

void MarkTool::PutClip(Pointf pt)
{
	if(IsNull(pt.x) || IsNull(pt.y))
		return;
	Pointf cpt = plotter.LtoP(pt);
	if(plotter.InPClip(cpt))
	{
		vertices.Add(PointfToPoint(cpt));
		if(vertices.GetCount() >= BUF_COUNT)
			Flush();
	}
}

Size TextTool::Type::GetTextSize(const String& s) const
{
	int wd = 0;
	const byte *b = s;
	unsigned len = s.GetLength();
	for(unsigned rep = len >> 2; rep; rep--)
	{
		wd += widths[b[0]] + widths[b[1]] + widths[b[2]] + widths[b[3]];
		b += 4;
	}
	switch(len & 3)
	{
	case 3: wd += widths[b[0]] + widths[b[1]] + widths[b[2]]; break;
	case 2: wd += widths[b[0]] + widths[b[1]]; break;
	case 1: wd += widths[b[0]]; break;
	}
	return Size(wd, height);
}

TextTool::TextTool()
{
}

TextTool::~TextTool() {}

void TextTool::Set(const Plotter& info, Font _f, Color _c, Color _o, double _a, Alignment _x, Alignment _y, bool _flip, int _aprec)
{
	cache_size = 0;
	font_index = 0;
	plotter    = info;
	color      = _c;
	outline    = _o;
	angle      = _a;
	x_align    = _x;
	y_align    = _y;
	flip       = _flip;
	angle_prec = _aprec;
	ASSERT(angle_prec > 0 && angle_prec <= 36000);
	SetFont(_f);
}

void TextTool::SetFont(Font _font)
{
	if((font_index = font_map.Find(font = _font)) < 0)
	{
		if(font_map.GetCount() >= FONT_LIMIT)
			Paint();
		font_index = font_map.GetCount();
		Type& type = font_map.Add(_font);
		FontInfo fi = _font.Info();
		short *out = type.widths;
		for(int c = 256; --c >= 0; out[c] = fi.GetWidth(c))
			;
		type.height = fi.GetHeight();
	}
}

void TextTool::PutFlip(const String& text, Pointf pt, double angle)
{
	if(flip && (angle >= 0.5 * M_PI && angle <= 1.5 * M_PI || angle <= -0.5 * M_PI))
		angle -= M_PI;
	Put(text, pt, angle);
}

void TextTool::Put(const String& text, Pointf pt, double angle)
{
	if(IsNull(pt.x) || IsNull(pt.y))
		return;
	Size size = font_map[font_index].GetTextSize(text);
	int delta = size.cx + size.cy;
	Pointf pos = plotter.LtoP(pt);
	if(pos.x >= plotter.clip.right + delta  || pos.x <= plotter.clip.left - delta
	|| pos.y >= plotter.clip.bottom + delta || pos.y <= plotter.clip.top - delta)
		return;
	Pointf offset(0, 0);
	switch(x_align)
	{
	case ALIGN_LEFT:   break;
	case ALIGN_RIGHT:  offset.x -= size.cx; break;
	default:           offset.x -= size.cx >> 1; break;
	}
	switch(y_align)
	{
	case ALIGN_TOP:    break;
	case ALIGN_BOTTOM: offset.y -= size.cy; break;
	default:           offset.y -= size.cy >> 1; break;
	}
	int apart = fround(angle * angle_prec / (2 * M_PI)) % angle_prec;
	if(apart < 0) apart += angle_prec;
	if(apart)
		offset = Rotated(offset, (-2 * M_PI * apart) / angle_prec);
	One<Item> item = new Item(PointfToPoint(pos + offset), text, color, outline);
	int sz = item->GetSize();
	if(cache_size + sz > CACHE_LIMIT) // flush
		Paint();
	cache_size += sz;
	cache.GetAdd(font_index * angle_prec + apart).Add(item.Detach());
}

void TextTool::Paint()
{
	if(plotter.draw)
		Flush();
	Clear();
}

void TextTool::Clear()
{
	ClearCache();
	SetFont(font);
}

void TextTool::Flush()
{
	Draw& draw = *plotter.draw;
	for(int i = 0; i < cache.GetCount(); i++)
	{
		int angle = cache.GetKey(i);
		Font font = font_map.GetKey(angle / angle_prec);
		Font font_naa = Font(font).NonAntiAliased();
		angle = (angle % angle_prec) * 3600 / angle_prec;
		const Array<Item>& items = cache[i];
		for(int t = 0; t < items.GetCount(); t++)
		{
			const Item& ii = items[t];
			if(!IsNull(ii.outline))
			{
				draw.DrawText(ii.point.x - 1, ii.point.y, angle, ii.text, font_naa, ii.outline);
				draw.DrawText(ii.point.x + 1, ii.point.y, angle, ii.text, font_naa, ii.outline);
				draw.DrawText(ii.point.x, ii.point.y - 1, angle, ii.text, font_naa, ii.outline);
				draw.DrawText(ii.point.x, ii.point.y + 1, angle, ii.text, font_naa, ii.outline);
				draw.DrawText(ii.point.x, ii.point.y, angle, ii.text, font_naa, ii.color);
			}
			else
				draw.DrawText(ii.point.x, ii.point.y, angle, ii.text, font, ii.color);
		}
	}
	ClearCache();
}

void TextTool::ClearCache()
{
	font_map.Clear();
	cache.Clear();
	cache_size = 0;
}

#if 0
class TestWindow : public TopWindow
{
public:
	virtual void Paint(Draw& draw);
};

void TestWindow::Paint(Draw& draw)
{
	draw.DrawRect(draw.GetPaintRect(), LtCyan);
	Plotter plotter(draw);
	PathTool path;
	path.Set(plotter, SolidLine);
	for(int i = 0; i < 320; i += 20)
	{
		Rectf rc = Pointf(300, 200);
		rc.Inflate(i);
		path.Rectangle(rc);
		path.Circle(Pointf(300, 200), i);
	}
	Rect rc = RectC(GetSize());
	rc.DeflateRect(20, 20);
	AreaTool area;
	area.Set(Plotter(plotter, rc), LtBlue, Image(), DashDotDotLine, Black, -50, 0);
	area.Rectangle(100, 100, 400, 400);
	area.Circle(400, 250, 100);
//	area.MoveTo(100, 0);
//	area.LineTo(0, 400);
//	area.LineTo(300, 500);
//	area.Circle(300, 200, 200);
	area.Paint();
}

void PlotterTest()
{
	Size size(600, 400);
	TestWindow window;
	window.SetRect(CalcWindowRect(window, size));
	window.Sizeable().Zoomable();

	window.Run();
}
#endif

}
