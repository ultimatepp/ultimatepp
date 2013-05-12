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

void MiniRenderer::DoLine(Point p1, Point p2, bool last)
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

struct MiniRenderer::Segments : MiniRenderer {
	int         miny;
	int         maxy;
	bool        flag;
	Vector< Vector< Segment > > segment;
	
	virtual void PutHorz(int x, int y, int cx) {
		DLOG("PutHorz " << x << ", " << y << ", cx: " << cx);
		if(y >= 0 && y < cy) {
			Segment& m = segment.At(y).Add();
			m.l = x;
			m.h = x + cx;
			m.flag = flag;
			flag = true;
			miny = min(y, miny);
			maxy = max(y, maxy);
		}
	}
	virtual void PutVert(int x, int y, int cy) {
		for(int i = 0; i < cy; i++)
			PutHorz(x, y + i, 1);
	}
	
	Segments() { miny = INT_MAX; maxy = 0; }
};

void MiniRenderer::FatLine(Point p1, Point p2, int n)
{
	Point v = p2 - p1;
	Pointf shift = n * Orthogonal(Pointf(v) / Length((Pointf(v))));
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
	Close();
	p0 = p1 = p;
	return *this;
}

MiniRenderer& MiniRenderer::Line(Point p)
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
			FatLine(p1, p, width);
		p1 = p;
	}
	return *this;
}

MiniRenderer& MiniRenderer::Close()
{
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

MiniRenderer& MiniRenderer::Ellipse(const Rect& rect)
{
	Sizef size = rect.GetSize();
	size /= 2;
	Pointf center = Pointf(rect.TopLeft()) + size;
	int n = max(abs(rect.GetWidth()), abs(rect.GetHeight()));
	Move(center + Size(-size, size.cy));
	for(int i = 1; i < n; i++) {
		double angle = M_PI * i / n / 2;
		Line(center + Sizef(cos(angle) * size.cx, sin(angle) * size.cy));
	}
	Close();
	return *this;
}

MiniRenderer::~MiniRenderer() {}
