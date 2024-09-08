#include "Draw.h"

namespace Upp {

DDARasterizer::DDARasterizer()
{
	cy = 0;
	p0 = p1 = Point(0, 0);
	j1 = j2 = Null;
	b1 = b2 = Null;
	close = false;
	width = 1;
}

void DDARasterizer::AHorz(int x, int y, int cx)
{
	if(cx) {
		if(cx < 0)
			PutHorz(x + cx + 1, y, -cx);
		else
			PutHorz(x, y, cx);
	}
}

void DDARasterizer::AVert(int x, int y, int cy)
{
	if(cy) {
		if(cy < 0)
			PutVert(x, y + cy + 1, -cy);
		else
			PutVert(x, y, cy);
	}
}

void DDARasterizer::DoLine(Point p1, Point p2, bool last)
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
		int n = dy + last;
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
		int n = dx + last;
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

struct DDARasterizer::Segments : DDARasterizer {
	int         miny;
	int         maxy;
	bool        flag;
	Vector< Vector< Segment > > segment;
	
	virtual void PutHorz(int x, int y, int cx) {
		if(y >= 0 && y < cy) {
			Segment& m = segment.At(y).Add();
			m.l = x;
			m.h = x + cx;
			m.flag = flag;
			miny = min(y, miny);
			maxy = max(y, maxy);
		}
		flag = true;
	}
	virtual void PutVert(int x, int y, int cy) {
		for(int i = 0; i < cy; i++)
			PutHorz(x, y + i, 1);
	}
	
	Segments() { miny = INT_MAX; maxy = 0; }
};

void DDARasterizer::FatLine(Point p2)
{
	if(p1 == p2)
		return;
	Point pp = p0;
	p0 = p1; // avoid Close
	Point v = p2 - p1;
	Point shift = (width - 0.5) * Orthogonal(Pointf(v) / Length((Pointf(v))));
	Point p = p1 + shift / 2;
	Point bj2 = j2;
	Polygon();
	if(!IsNull(j1)) {
		Move(j1);
		Line(p);
	}
	else
		Move(p);
	if(IsNull(b1))
		b1 = p;
	Line(p += v);
	j1 = p;
	if(close && !IsNull(b1)) {
		Line(b1);
		Line(b2);
	}
	Line(p -= shift);
	j2 = p;
	Line(p -= v);
	if(!IsNull(bj2))
		Line(bj2);
	if(IsNull(b2))
		b2 = p;
	Fill();
	p0 = pp;
}

DDARasterizer& DDARasterizer::Move(Point p)
{
	if(pseg)
		Close();
	p0 = p1 = p;
	j1 = j2 = Null;
	return *this;
}

DDARasterizer& DDARasterizer::Line(Point p)
{
	if(pseg) {
		Point a = p1;
		Point b = p;
		if(a.y > b.y)
			Swap(a, b);
		pseg->flag = false;
		pseg->DoLine(a, b, true);
		p1 = p;
	}
	else {
		if(width <= 1)
			DoLine(p1, p, false);
		else
			FatLine(p);
		p1 = p;
	}
	return *this;
}

DDARasterizer& DDARasterizer::Close()
{
	if(p1 != p0) {
		close = true;
		Line(p0);
		close = false;
		if(!pseg)
			b1 = b2 = Null;
	}
	return *this;
}

DDARasterizer& DDARasterizer::Polygon()
{
	pseg.Create();
	pseg->Cy(cy);
	return *this;
}

DDARasterizer& DDARasterizer::Fill()
{
	ASSERT(pseg);
	Close();
	for(int y = pseg->miny; y <= pseg->maxy; y++) {
		Vector<Segment>& gg = pseg->segment[y];
		Sort(gg);
		int i = 0;
		bool flag = false;
		while(i < gg.GetCount()) {
			int l = gg[i].l;
			int h = gg[i].h;
			flag ^= gg[i++].flag;
			while(i < gg.GetCount() && flag) {
				h = gg[i].h;
				flag ^= gg[i++].flag;
			}
			PutHorz(l, y, h - l);
		}
	}
	pseg.Clear();
	return *this;
}

DDARasterizer& DDARasterizer::Ellipse(const Rect& r, int width)
{
	double rx = r.GetWidth() * 0.5;
	double ry = r.GetHeight() * 0.5;
	double r2 = sqr(ry);
	double rr = rx / ry;
	
	if(rx <= 0 || ry <= 0)
		return *this;
	
	double wy, w2, wr;
	if(width > 1) {
		wy = ry - width;
		w2 = sqr(wy);
		wr = (rx - width) / wy;
	}
	

	int y = 0;
	int px = (int)rx;
	for(;;) {
		int x = int(rr * (ry - sqrt(r2 - sqr(ry - y - 0.5))) + 0.5);
		int y1 = y + r.top;
		int y2 = r.bottom - y - 1;
		int n = max(px - x, 1);
		int n2 = n;
		int x1 = x + r.left;
		int x2 = r.right - x - n;
		if(width < 0 || y == 0 || width > 1 && y < width || width >= ry) {
			n = x2 - x1 + n;
			n2 = 0;
		}
		else
		if(width > 1) {
			int wx = int(wr * (wy - sqrt(max(w2 - sqr(wy - (y - width) - 0.5), 0.0))) + 0.5) + int(ry - wy);
			n = n2 = wx - x;
			x2 = r.right - x - n;
		}
		PutHorz(x1, y1, n);
		PutHorz(x2, y1, n2);
		if(y2 < y1) break;
		PutHorz(x1, y2, n);
		PutHorz(x2, y2, n2);
		px = x;
		y++;
	}
	return *this;
}

DDARasterizer::~DDARasterizer() {}

}
