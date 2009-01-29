#include "ScanLine.h"

Rasterizer::Rasterizer(Size _sz)
{
	sz = _sz;
	x1 = y1 = 0;
	min_y = INT_MAX;
	max_y = INT_MIN;
	finish = true;
	current.Init();
	cell.Alloc(sz.cy);
	cliprect = Sizef(sz);
}

inline void Rasterizer::AddCurrent()
{
	if((current.area | current.cover) && current_y >= 0 && current_y < sz.cy) {
		cell[current_y].Add(current);
//		DLOG(current.x << ", y=" << current_y);
	}
}

inline void Rasterizer::SetCurrent(int x, int y)
{
	DLOG("cury=" << y);
	if(current.x != x || current_y != y) {
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
		current.area  += (fx1 + fx2) * delta;
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
		p = (y2 - y1 + delta) << 8;
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
			current.area += delta << 8;
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

	DLOG(ex1 << ", " << ey1 << " - " << ex2 << ", " << ey2);

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
	mod   = p % dy;
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

inline unsigned Alpha(int area)
{
	int cover = area >> (2 * 8 + 1 - 8);
	if(cover < 0) cover = -cover;
/*	if(evenodd) {
		cover &= 511;
		if(cover > 256)
			cover = 512 - cover;
    }*/
	if(cover > 255) cover = 255;
	return (cover + 1) / 2;
}

ScanLine Rasterizer::Get(int y)
{
	if(finish) {
		AddCurrent();
		finish = false;
	}
	ScanLine r;
	r.len = 0;
	r.x = 0;
	Vector<Cell>& cl = cell[y];
	Sort(cl);
	if(y < min_y || y > max_y || cl.GetCount() == 0) return r;
	const Cell *c = cl;
	const Cell *e = cl.End();
	r.x = c->x;
	int cover = 0;
	DDUMP(y);
	while(c < e) {
		DDUMP(c->x);
		int x = c->x;
		int area = c->area;
		cover += c->cover;
		c++;
		while(c < e && c->x == x) {
			area += c->area;
			cover += c->cover;
			c++;
		}
		if(area)
			r.data.Cat(Alpha((cover << (8 + 1)) - area));
		else
			r.data.Cat(0);
		r.len++;
		x++;		
		if(c < e && c->x > x) {
			byte val = Alpha(cover << (8 + 1));
			int n = c->x - x;
			r.len += n;
			while(n > 0) {
				int q = min(n, 127);
				r.data.Cat(q + 128);
				r.data.Cat(val);
				n -= q;
			}
		}
    }
	return r;
}
