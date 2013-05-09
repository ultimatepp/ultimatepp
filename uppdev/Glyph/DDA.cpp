#include "glyph.h"

MiniRenderer::MiniRenderer(int cy)
:	cy(cy)
{
	p0 = p1 = Null;
	pseg = NULL;
}

void MiniRenderer::AHorz(int x, int y, int cx)
{
	if(cx)
		if(cx < 0)
			PutHorz(x + cx + 1, y, -cx);
		else
			PutHorz(x, y, cx);
}

void MiniRenderer::AVert(int x, int y, int cy)
{
	if(cy)
		if(cy < 0)
			PutVert(x, y + cy + 1, -cy);
		else
			PutVert(x, y, cy);
}

void MiniRenderer::Line(Point p1, Point p2)
{
	dirx = sgn(p2.x - p1.x);
	diry = sgn(p2.y - p1.y);
	int dx = abs(p2.x - p1.x);
	int dy = abs(p2.y - p1.y);
	int x = p1.x;
	int y = p1.y;
	int x0 = x;
	int y0 = y;
	if(dx < dy) {
		int dda = dy >> 1;
		int n = dy;
		for(;;) {
			if(x != x0) {
				AVert(x0, y0, y - y0);
				x0 = x;
				y0 = y;
			}
			if(n-- <= 0)
				break;
			y += diry;
			dda -= dx;
			if(dda < 0) {
				dda += dy;
				x += dirx;
			}
		}
		AVert(x0, y0, y - y0);
	}
	else {
		int dda = dx >> 1;
		int n = dx;
		for(;;) {
			if(y != y0) {
				AHorz(x0, y0, x - x0);
				x0 = x;
				y0 = y;
			}
			if(n-- <= 0)
				break;
			x += dirx;
			dda -= dy;
			if(dda < 0) {
				dda += dx;
				y += diry;
			}
		}
		AHorz(x0, y0, x - x0);
	}
}

struct MiniRenderer::Segments : MiniRenderer {
	int         miny;
	int         maxy;
	
	Point first;
	Point last, prev;

	Vector< Vector< Segment > > segment;
	
	static void Join(Segment& m, int x, int cx)
	{
		DLOG("-- JOIN");
		DDUMP(m.x);
		DDUMP(m.cx);
		int l = min(x, m.x);
		int r = max(x + cx, m.x + m.cx);
		m.x = l;
		m.cx = r - l;
		DDUMP(m.x);
		DDUMP(m.cx);
	}
	
	virtual void PutHorz(int x, int y, int cx) {
		DLOG("PutHorz x: " << x << ", y: " << y << ", cx: " << cx);
		if(y >= 0 && y < cy) {
			last.y = y;
			last.x = segment.At(y).GetCount();
			if(IsNull(first))
				first = last;
			if(prev.y == y) {
				Join(segment[y].Top(), x, cx);
				return;
			}
			prev = Null;
			Segment& m = segment[y].Add();
			m.x = x;
			m.cx = cx;
			if(y < miny)
				miny = y;
			if(y > maxy)
				maxy = y;
		}
	}
	virtual void PutVert(int x, int y, int cy) {
		DDUMP(diry);
		if(diry > 0)
			for(int i = 0; i < cy; i++)
				PutHorz(x, y + i, 1);
		else
			for(int i = 0; i < cy; i++)
				PutHorz(x, y + cy - 1 - i, 1);
	}
	
	Segments(int cy) : MiniRenderer(cy) { miny = INT_MAX; maxy = 0; first = prev = last = Null; }
};

void MiniRenderer::Move(Point p)
{
	DLOG("*** Move " << p);
	if(pseg)
		Close();
	else
		pseg = new Segments(cy);
	p0 = p1 = p;
	pseg->first = Null;
}

void MiniRenderer::Line(Point p)
{
	DLOG("*** Line " << p);
	if(!pseg)
		Move(Point(0, 0));
	pseg->Line(p1, p);
	pseg->prev = pseg->last;
	p1 = p;
}

void MiniRenderer::Close()
{
	if(!pseg)
		Move(Point(0, 0));
	if(p1 != p0)
		Line(p0);
	// Todo: JOIN
}

void MiniRenderer::Render()
{
	Close();
	if(!pseg)
		return;
	for(int y = pseg->miny; y <= pseg->maxy; y++) {
		Vector<Segment>& gg = pseg->segment[y];
		Sort(gg);
		int i = 0;
		while(i < gg.GetCount()) {
			if(i + 1 < gg.GetCount())
				PutHorz(gg[i].x, y, gg[i + 1].x + gg[i + 1].cx - gg[i].x);
			i += 2;
		}
	}
	delete pseg;
	pseg = NULL;
}

void MiniRenderer::Ellipse(Point center, Size radius)
{
	int n = max(abs(radius.cx), abs(radius.cy));
	Point p0;
	DLOG("--------------------");
	for(int i = 0; i < n; i++) {
		Point p = center + Point(int(sin(M_2PI * i / n) * radius.cx), int(cos(M_2PI * i / n) * radius.cy));
		if(i)
			Line(p);
		else
			Move(p);
	}
}

MiniRenderer::~MiniRenderer()
{
	if(pseg)
		delete pseg;
}
