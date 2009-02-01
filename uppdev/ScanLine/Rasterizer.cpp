#include "ScanLine.h"

#ifdef RASTERIZER1

void Rasterizer::Reset()
{
#ifdef YLINK
	cell.Clear();
	cell.Reserve(2000);
	ylink.Alloc(sz.cy, -1);
	line.Reserve(1000);
#else
	cell.Alloc(sz.cy);
#endif
	x1 = y1 = 0;
	min_y = INT_MAX;
	max_y = INT_MIN;
	current.area = current.cover = 0;
	current.x = -30000;
	current_y = 0;
	finish = true;
}

void Rasterizer::SetClip(const Rectf& rect)
{
	cliprect = rect & Sizef(sz);
}

Rasterizer::Rasterizer(int cx, int cy)
{
	sz.cx = cx;
	sz.cy = cy;
	ymax = (sz.cy << 8) - 1;
	xmax = (sz.cx << 8) - 1;
	cliprect = Sizef(sz);
	Reset();
}

inline void Rasterizer::AddCurrent()
{
	if(current.area | current.cover) {
		PAINTER_TIMING("AddCurrent");
#ifdef YLINK
		CellY& c = cell.Add();
		(Cell&)c = current;
		c.ylink = ylink[current_y];
		ylink[current_y] = cell.GetCount() - 1;
#else
		if(cell[current_y].GetCount() == 0)
			cell[current_y].Reserve(16);
		cell[current_y].Add(current);
#endif
//		DLOG(current.x << ", y=" << current_y);
	}
}

inline void Rasterizer::SetCurrent(int x, int y)
{
//	DLOG("cury=" << y);
	if((current.x - x) | (current_y - y)) {
		AddCurrent();
		current.x = x;
		current_y = y;
		current.cover = current.area = 0;
	}
}

inline void Rasterizer::RenderHLine(int ey, int x1, int y1, int x2, int y2)
{
	int ex1 = x1 >> 8;
	int ex2 = x2 >> 8;
	int fx1 = x1 & 255;
	int fx2 = x2 & 255;
	if(y1 == y2) {
		SetCurrent(ex2, ey);
		return;
    }
	if(ex1 == ex2) {
		int delta = y2 - y1;
		current.cover += delta;
		current.area += (fx1 + fx2) * delta;
		return;
	}
	int p = (256 - fx1) * (y2 - y1);
	int first = 256;
	int incr  = 1;
	int dx = x2 - x1;
	if(dx < 0) {
		p = fx1 * (y2 - y1);
		first = 0;
		incr = -1;
		dx = -dx;
	}
	int delta = p / dx;
	int mod = p % dx;
	if(mod < 0) {
		delta--;
		mod += dx;
	}
	current.cover += delta;
	current.area += (fx1 + first) * delta;
	ex1 += incr;
	SetCurrent(ex1, ey);
	y1 += delta;
	if(ex1 != ex2) {
		p = (y2 - y1 + delta) * 256;
		int lift = p / dx;
		int rem = p % dx;
		if (rem < 0) {
			lift--;
			rem += dx;
		}
		mod -= dx;
		while(ex1 != ex2) {
			delta = lift;
			mod += rem;
			if(mod >= 0) {
				mod -= dx;
				delta++;
			}
			current.cover += delta;
			current.area += delta * 256;
			y1 += delta;
			ex1 += incr;
			SetCurrent(ex1, ey);
		}
	}
	delta = y2 - y1;
	current.cover += delta;
	current.area  += (fx2 + 256 - first) * delta;
}

void Rasterizer::LineRaw(int x1, int y1, int x2, int y2)
{
//	PAINTER_TIMING("LineRaw");
	enum dx_limit_e { dx_limit = 16384 << 8 };
	int dx = x2 - x1;
	if(dx >= dx_limit || dx <= -dx_limit) {
		int cx = (x1 + x2) >> 1;
		int cy = (y1 + y2) >> 1;
		LineRaw(x1, y1, cx, cy);
		LineRaw(cx, cy, x2, y2);
		return;
    }
	int dy = y2 - y1;
	int ex1 = x1 >> 8;
	int ex2 = x2 >> 8;
	int ey1 = y1 >> 8;
	int ey2 = y2 >> 8;
	int fy1 = y1 & 255;
	int fy2 = y2 & 255;
	
	ASSERT(ey1 >= 0 && ey1 < sz.cy && ey2 >= 0 && ey2 < sz.cy);

	int x_from, x_to;
	int p, rem, mod, lift, delta, first, incr;

	if(ey1 < min_y) min_y = ey1;
	if(ey1 > max_y && ey1 < sz.cy) max_y = ey1;
	if(ey2 < min_y) min_y = ey2;
	if(ey2 > max_y && ey2 < sz.cy) max_y = ey2;

	SetCurrent(ex1, ey1);

	if(ey1 == ey2) {
		RenderHLine(ey1, x1, fy1, x2, fy2);
		return;
    }
	incr  = 1;
	if(dx == 0) {
		int ex = x1 >> 8;
		int two_fx = (x1 - (ex << 8)) << 1;
		int area;
		first = 256;
		if(dy < 0) {
			first = 0;
			incr  = -1;
		}
		x_from = x1;
		delta = first - fy1;
		current.cover += delta;
		current.area += two_fx * delta;
		ey1 += incr;
		SetCurrent(ex, ey1);
		delta = first + first - 256;
		area = two_fx * delta;
		while(ey1 != ey2) {
			current.cover = delta;
			current.area = area;
			ey1 += incr;
			SetCurrent(ex, ey1);
		}
		delta = fy2 - 256 + first;
		current.cover += delta;
		current.area += two_fx * delta;
		return;
    }
	p = (256 - fy1) * dx;
	first = 256;
	if(dy < 0) {
		p = fy1 * dx;
		first = 0;
		incr = -1;
		dy = -dy;
    }
	delta = p / dy;
	mod = p % dy;
	if(mod < 0) {
		delta--;
		mod += dy;
	}
	x_from = x1 + delta;
	RenderHLine(ey1, x1, fy1, x_from, first);
	ey1 += incr;
	SetCurrent(x_from >> 8, ey1);
	if(ey1 != ey2) {
		p = dx << 8;
		lift = p / dy;
		rem = p % dy;
		if(rem < 0) {
			lift--;
			rem += dy;
		}
		mod -= dy;
		while(ey1 != ey2) {
			delta = lift;
			mod  += rem;
			if(mod >= 0) {
				mod -= dy;
				delta++;
			}
			x_to = x_from + delta;
			RenderHLine(ey1, x_from, 256 - first, x_to, first);
			x_from = x_to;
			ey1 += incr;
			SetCurrent(x_from >> 8, ey1);
		}
    }
	RenderHLine(ey1, x_from, 256 - first, x2, fy2);
}

bool Rasterizer::BeginRender(int y, const Cell *&c, const Cell *&e)
{
	if(finish) {
		AddCurrent();
		finish = false;
//		DDUMP(cell.GetCount());
	}
	if(y < min_y || y > max_y) return false;
#ifdef YLINK
	int link = ylink[y];
	if(link < 0) return false;
	line.SetCount(0);
	Cell *t = line;
	do {
		line.Add(cell[link]);
		link = cell[link].ylink;
	}
	while(link >= 0);
	Sort(line);
	c = line;
	e = line.End();
	return true;
#else
	Vector<Cell>& cl = cell[y];
	if(cl.GetCount() == 0) return false;
	Sort(cl);
	c = cl;
	e = cl.End();
	return true;
#endif
}

#if 0
inline unsigned Alpha(int area)
{
	int cover = area >> 9;
	if(cover < 0) cover = -cover;
/*	if(evenodd) {
		cover &= 511;
		if(cover > 256)
			cover = 512 - cover;
    }*/
	if(cover > 255) cover = 255;
	return (cover + 1) / 2;
}

int sl_hist[100];
int d_hist[100];

ScanLine Rasterizer::Get(int y)
{
	PAINTER_TIMING("Get");
	if(finish) {
		AddCurrent();
		finish = false;
	}
	ScanLine r;
	StringBuffer b;
	r.len = 0;
	r.x = 0;
	Vector<Cell>& cl = cell[y];
	if(y < min_y || y > max_y || cl.GetCount() == 0) return r;
	{
		PAINTER_TIMING("Sort");
		Sort(cl);
	}
	const Cell *c = cl;
	const Cell *e = cl.End();
	r.x = c->x;
	int cover = 0;
	while(c < e) {
		int x = c->x;
		int area = c->area;
		cover += c->cover;
		c++;
		while(c < e && c->x == x) {
			area += c->area;
			cover += c->cover;
			c++;
		}
		if(area) {
			b.Cat(Alpha((cover << (8 + 1)) - area));
			r.len++;
			x++;
		}
		if(c < e && c->x > x) {
			byte val = Alpha(cover << (8 + 1));
			int n = c->x - x;
			r.len += n;
			while(n > 0) {
				int q = min(n, 127);
				b.Cat(q + 128);
				b.Cat(val);
				n -= q;
			}
		}
    }
	r.data = b;
	sl_hist[min(cl.GetCount(), 99)]++;
	d_hist[min(r.data.GetCount(), 99)]++;
	return r;
}

EXITBLOCK {
	LOG("Cells");
	int sum = 0;
	for(int i = 0; i < 100; i++) {
		sum += sl_hist[i];
		LOG(i << " " << sl_hist[i] << " " << sum);
	}
	LOG("Scanline Data");
	for(int i = 0; i < 100; i++) {
		sum += d_hist[i];
		LOG(i << " " << d_hist[i] << " " << sum);
	}
}
#endif

#endif
