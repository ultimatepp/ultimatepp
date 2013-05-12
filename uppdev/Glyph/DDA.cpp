#include "glyph.h"

MiniRenderer::MiniRenderer()
{
	cy = 0;
	p0 = p1 = Point(0, 0);
	width = 1;
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

void MiniRenderer::DoLine(Point p1, Point p2)
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
	
	static void Join(Segment& m, int l, int h) {
		m.l = min(m.l, l);
		m.h = max(m.h, h);
	}

	void JoinLast() {
		DLOG("JoinLast");
		if(!IsNull(first) && prev.y == first.y) {
			DLOG("JOIN LAST");
			ASSERT(first.y < segment.GetCount());
			ASSERT(first.x < segment[first.y].GetCount());
			Join(segment[first.y][first.x], segment[prev.y].Top().l, segment[prev.y].Top().h);
			segment[prev.y].Drop();
			first = Null;
		}
	}
	
	virtual void PutHorz(int x, int y, int cx) {
		DLOG("PutHorz " << x << ", " << y << ", cx: " << cx);
		if(y >= 0 && y < cy) {
			if(prev.y == y) {
				DLOG("JOIN!");
				Join(segment[y].Top(), x, x + cx);
				last = prev;
			}
			else {
				last.y = y;
				last.x = segment.At(y).GetCount();
				Segment& m = segment[y].Add();
				m.l = x;
				m.h = x + cx;
				if(y < miny)
					miny = y;
				if(y > maxy)
					maxy = y;
			}
			if(IsNull(first)) {
				first = last;
				DLOG("SET FIRST");
				DDUMP(first);
				DDUMP(segment[first.y].GetCount());
			}
			prev = Null;
		}
	}
	virtual void PutVert(int x, int y, int cy) {
		if(diry > 0)
			for(int i = 0; i < cy; i++)
				PutHorz(x, y + i, 1);
		else
			for(int i = 0; i < cy; i++)
				PutHorz(x, y + cy - 1 - i, 1);
	}
	
	Segments() { miny = INT_MAX; maxy = 0; first = prev = last = Null; }
};

void MiniRenderer::FatLine(Point p1, Point p2, int n)
{
	Point v = p2 - p1;
	Pointf shift = n * Orthogonal(Pointf(v) / Length(v));
	Point p = Pointf(p1) + shift / 2;
	Polygon();
	Move(p);
	Line(p += v);
	Line(p -= shift);
	Line(p - v);
	Fill();
}

MiniRenderer& MiniRenderer::Move(Point p)
{
	if(pseg) {
		pseg->Close();
		pseg->first = Null;
		pseg->p0 = pseg->p1 = p;
	}
	else {
		DLOG("Move " << p);
		Close();
		p0 = p1 = p;
	}
	return *this;
}

MiniRenderer& MiniRenderer::Line(Point p)
{
	if(pseg) {
		pseg->Line(p);
		pseg->prev = pseg->last;
	}
	else {
		DLOG("Line " << p);
		if(width <= 1)
			DoLine(p1, p);
		else
			FatLine(p1, p, width);
		p1 = p;
	}
	return *this;
}

MiniRenderer& MiniRenderer::Close()
{
	if(pseg) {
		if(pseg->p1 != pseg->p0)
			Line(pseg->p0);
		pseg->JoinLast();
	}
	else
		if(p1 != p0)
			Line(p0);
	return *this;
}

MiniRenderer& MiniRenderer::Polygon()
{
	pseg.Create();
	pseg->Cy(cy);
	return *this;
}

MiniRenderer& MiniRenderer::Fill()
{
	ASSERT(pseg);
	Close();
	for(int y = pseg->miny; y <= pseg->maxy; y++) {
		Vector<Segment>& gg = pseg->segment[y];
		Sort(gg);
		for(int i = 0; i < gg.GetCount(); i += 2)
			PutHorz(gg[i].l, y, gg[i + (i + 1 < gg.GetCount())].h - gg[i].l);
	}
	pseg.Clear();
	return *this;
}

MiniRenderer& MiniRenderer::Ellipse(Point center, Size radius)
{
	DLOG("Ellipse");
	int n = max(abs(radius.cx), abs(radius.cy));
	Point p0;
	for(int i = 0; i < n; i++) {
		Point p = center + Point(int(sin(M_2PI * i / n) * radius.cx), int(cos(M_2PI * i / n) * radius.cy));
		if(i)
			Line(p);
		else
			Move(p);
	}
	Close();
	return *this;
}

MiniRenderer::~MiniRenderer()
{}
