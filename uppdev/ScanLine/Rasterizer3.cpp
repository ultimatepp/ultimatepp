#include "ScanLine.h"

#ifdef RASTERIZER3

void Rasterizer::Init()
{
	x1 = y1 = 0;
	min_y = INT_MAX;
	max_y = INT_MIN;
}

void Rasterizer::Reset()
{
	PAINTER_TIMING("Rasterizer::Reset");
	Init();
	for(int i = 0; i < sz.cy; i++)
		cell[i].SetCount(0);
}

void Rasterizer::SetClip(const Rectf& rect)
{
	cliprect = rect & Sizef(sz);
}

Rasterizer::Rasterizer(int cx, int cy)
{
	sz.cx = cx;
	sz.cy = cy;
	cell.Alloc(sz.cy);
	ymax = (sz.cy << 8) - 1;
	xmax = (sz.cx << 8) - 1;
	cliprect = Sizef(sz);
	Reset();
}

inline dword *Rasterizer::AddCells(int y, int n)
{
	Vector<dword>& v = cell[y];
	if(v.GetCount() == 0) {
		v.Reserve(16);
		v.SetCount(n);
		return &v[0];
	}
	y = v.GetCount();
	v.SetCount(y + n);
	return &v[y];
}

inline void Rasterizer::RenderHLine(int ey, int x1, int y1, int x2, int y2)
{
	int ex1 = x1 >> 8;
	int ex2 = x2 >> 8;
	int fx1 = x1 & 255;
	int fx2 = x2 & 255;
	dword *c;
	if(y1 == y2)
		return;
	int dy = y2 - y1;
	if(ex1 == ex2) {
		*AddCells(ey, 1) = Cell(ex1, fx1 + fx2, dy);
		return;
	}
	int dx = x2 - x1;
	if(dx < 0) {
		int p = fx1 * dy;
		dx = -dx;
		int delta = p / dx;
		int mod = p % dx;
		if(mod < 0) {
			delta--;
			mod += dx;
		}
		c = AddCells(ey, abs(ex2 - ex1) + 1);
		*c++ = Cell(ex1, fx1, delta);
		ex1--;
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
				*c++ = Cell(ex1, 256, delta);
				y1 += delta;
				ex1--;
			}
		}
		*c++ = Cell(ex2, fx2 + 256, y2 - y1);
	}
	else {
		int p = (256 - fx1) * dy;
		int delta = p / dx;
		int mod = p % dx;
		if(mod < 0) {
			delta--;
			mod += dx;
		}
		c = AddCells(ey, abs(ex2 - ex1) + 1);
		*c++ = Cell(ex1, fx1 + 256, delta);
		ex1++;
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
				*c++ = Cell(ex1, 256, delta);
				y1 += delta;
				ex1++;
			}
		}
		*c++ = Cell(ex2, fx2, y2 - y1);
	}
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

	if(ey1 == ey2) {
		RenderHLine(ey1, x1, fy1, x2, fy2);
		return;
    }
	incr  = 1;
	if(dx == 0) {
		int ex = x1 >> 8;
		int two_fx = (x1 - (ex << 8)) << 1;
		first = 256;
		if(dy < 0) {
			first = 0;
			incr  = -1;
		}
		x_from = x1;
		delta = first - fy1;
		*AddCells(ey1, 1) = Cell(ex1, two_fx, delta);
		ey1 += incr;
		delta = first + first - 256;
		while(ey1 != ey2) {
			*AddCells(ey1, 1) = Cell(ex1, two_fx, delta);
			ey1 += incr;
		}
		delta = fy2 - 256 + first;
		*AddCells(ey1, 1) = Cell(ex1, two_fx, delta);
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
		}
    }
	RenderHLine(ey1, x_from, 256 - first, x2, fy2);
}

inline unsigned Alpha(int area)
{
	int cover = area >> 9;
	if(cover < 0) cover = -cover;
/*	if(evenodd) {
		cover &= 511;
		if(cover > 256)
			cover = 512 - cover;
    }*/
	return cover;
}

void Rasterizer::Render(int y, Target& g)
{
	PAINTER_TIMING("Render");
	const dword *c, *e;
	if(y < min_y || y > max_y) return;
	Vector<dword>& cl = cell[y];
	if(cl.GetCount() == 0) return;
	Sort(cl);
	c = cl;
	e = cl.End();
	g.Start(GetX(*c), GetX(*(e - 1)));
	int cover = 0;
	while(c < e) {
		int x = GetX(*c);
		int cv = GetC(*c);
		DDUMP(cv);
		int area = GetW(*c) * cv;
		cover += cv;
		c++;
		while(c < e && GetX(*c) == x) {
			int cv = GetC(*c);
			area += GetW(*c) * cv;
			cover += cv;
			c++;
		}
		if(area) {
			g.Render(Alpha((cover << (8 + 1)) - area));
			x++;
		}
		if(c < e && GetX(*c) > x)
			g.Render(Alpha(cover << (8 + 1)), GetX(*c) - x);
    }
}

#endif
