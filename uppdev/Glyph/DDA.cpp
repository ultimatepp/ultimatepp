#include "glyph.h"

MiniRenderer::MiniRenderer(int cy)
:	cy(cy)
{
	p0 = p1 = Null;
	pseg = NULL;
}

void MiniRenderer::AHorz(int x, int y, int cx)
{
	if(cx < 0)
		PutHorz(x + cx + 1, y, -cx);
	else
		PutHorz(x, y, cx);
}

void MiniRenderer::AVert(int x, int y, int cy)
{
	if(cy < 0)
		PutVert(x, y + cy + 1, -cy);
	else
		PutVert(x, y, cy);
}

void MiniRenderer::Line(Point p1, Point p2)
{
	int dirx = sgn(p2.x - p1.x);
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
	int miny;
	int maxy;

	Vector< Vector< Tuple3<int16, int, int16> > > segment;
	
	virtual void PutHorz(int x, int y, int cx) {
		if(y >= 0 && y < cy) {
			segment.At(y).Add(MakeTuple((int16)x, cx, (int16)diry));
			if(y < miny)
				miny = y;
			if(y > maxy)
				maxy = y;
		}
	}
	virtual void PutVert(int x, int y, int cy) {
		for(int i = 0; i < cy; i++)
			PutHorz(x, y + i, 1);
	}
	
	Segments(int cy) : MiniRenderer(cy) { miny = INT_MAX; maxy = 0; }
};

void MiniRenderer::Line(Point p)
{
	if(!pseg)
		pseg = new Segments(cy);
	pseg->Line(p1, p);
	p1 = p;
}

void MiniRenderer::Close()
{
	if(p1 != p0)
		Line(p0);
}

void MiniRenderer::Render()
{
	Close();
	if(!pseg)
		return;
	for(int y = pseg->miny; y <= pseg->maxy; y++) {
		Vector< Tuple3<int16, int, int16> >& gg = pseg->segment[y];
		Sort(gg);
		int i = 0;
		if(gg.GetCount()) {
			int filldir = gg[i].c;
			while(i < gg.GetCount()) {
				int left = gg[i].a;
				int right = gg[i].a + gg[i].b;
				i++;
				while(i < gg.GetCount() && gg[i].c == filldir) {
					right = gg[i].a + gg[i].b;
					i++;
				}
				if(i < gg.GetCount())
					right = gg[i].a + gg[i].b;
				PutHorz(left, y, right - left);
				while(i < gg.GetCount() && gg[i].c != filldir)
					i++;
			}
		}
	}
	delete pseg;
	pseg = NULL;
}

void MiniRenderer::Ellipse(Point center, Size radius)
{
	Move(center + Size(0, radius.cy));
	int n = max(radius.cx, radius.cy);
	for(int i = 0; i <= n; i++)
		Line(center + Point(int(sin(2 * M_PI * i / n) * radius.cx), int(cos(2 * M_PI * i / n) * radius.cy)));
}

MiniRenderer::~MiniRenderer()
{
	if(pseg)
		delete pseg;
}
