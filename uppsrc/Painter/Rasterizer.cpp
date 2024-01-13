#include "Painter.h"

// This code is based in AGG rasterizer with this original information:

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
//
// The author gratefully acknowleges the support of David Turner, 
// Robert Wilhelm, and Werner Lemberg - the authors of the FreeType 
// libray - in producing this work. See http://www.freetype.org for details.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#define LLOG(x) // DLOG(x)

namespace Upp {

void Rasterizer::Create(int cx, int cy, bool subpixel)
{
	Free();

	mx = subpixel ? 3 * 256 : 256;
	sz.cx = cx;
	sz.cy = cy;

	cell.Alloc(sz.cy + 1); // one more for overrun

//	STATIC_ASSERT(sizeof(CellArray) == 256);

	cliprect = Sizef(sz);
	Init();
	Reset(); // TODO: Can be removed (cell is just constructed, Init called above)
}

void Rasterizer::Free()
{
	if(cell) {
		for(int i = min_y; i <= max_y; i++) {
			if(cell[i].alloc != SVO_ALLOC) {
				MemoryFree(cell[i].ptr);
				cell[i].alloc = SVO_ALLOC;
			}
			cell[i].count = 0;
		}
		if(cell[sz.cy].alloc != SVO_ALLOC) { // check overrun
			MemoryFree(cell[sz.cy].ptr);
			cell[sz.cy].alloc = SVO_ALLOC;
		}
		cell[sz.cy].count = 0;
	}
}

void Rasterizer::Init()
{
	p0 = Pointf(0, 0);
	min_y = INT_MAX;
	max_y = INT_MIN;
}

void Rasterizer::Reset()
{
	Free();
	Init();
}

void Rasterizer::SetClip(const Rectf& rect)
{
	cliprect = rect & Sizef(sz);
}

force_inline
Rasterizer::Cell *Rasterizer::AddCells(int y, int n)
{
	CellArray& a = cell[y];
	if(a.count + n <= SVO_ALLOC) {
		Cell *r = a.svo + a.count;
		a.count += n;
		return r;
	}
	if(a.count + n > a.alloc) {
		size_t sz = sizeof(Cell) * (a.alloc + (a.alloc >> 1) + n);
		Cell *cells = (Cell *)MemoryAllocSz(sz);
		if(a.alloc == SVO_ALLOC)
			memcpy_t(cells, a.svo, a.count);
		else {
			memcpy_t(cells, a.ptr, a.count);
			MemoryFree(a.ptr);
		}
		a.ptr = cells;
		a.alloc = int(sz / sizeof(Cell));
	}
	Cell *r = a.ptr + a.count;
	a.count += n;
	return r;
}

inline void Rasterizer::RenderHLine(int ey, int x1, int y1, int x2, int y2)
{
	int ex1 = x1 >> 8;
	int ex2 = x2 >> 8;
	int fx1 = x1 & 255;
	int fx2 = x2 & 255;
	Cell *c;
	if(y1 == y2)
		return;
	int dy = y2 - y1;
	if(ex1 == ex2) {
		c = AddCells(ey, 1);
		c->x = ex1;
		c->cover = dy;
		c->area = (fx1 + fx2) * dy;
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
		c->x = ex1;
		c->cover = delta;
		c->area = fx1 * delta;
		c++;
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
				c->x = ex1;
				c->cover = delta;
				c->area = delta * 256;
				c++;
				y1 += delta;
				ex1--;
			}
		}
		c->x = ex2;
		dy = y2 - y1;
		c->cover = dy;
		c->area = (fx2 + 256) * dy;
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
		c->x = ex1;
		c->cover = delta;
		c->area = (fx1 + 256) * delta;
		c++;
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
				c->x = ex1;
				c->cover = delta;
				c->area = delta * 256;
				c++;
				y1 += delta;
				ex1++;
			}
		}
		c->x = ex2;
		dy = y2 - y1;
		c->cover = dy;
		c->area = fx2 * dy;
	}
}

void Rasterizer::LineRaw(int x1, int y1, int x2, int y2)
{
	PAINTER_TIMING("LineRaw");
	LLOG("Rasterizer::LineRaw " << x1 / 256.0 << ':' << y1 / 256.0
	     << " - " << x2 / 256.0 << ':' << y2 / 256.0);
	int ex1 = x1 >> 8;
	int ey1 = y1 >> 8;
	int ey2 = y2 >> 8;

	ASSERT(ey1 >= 0 && ey1 <= sz.cy && ey2 >= 0 && ey2 <= sz.cy);

	if(ey1 < min_y) {
		if(ey1 < 0) return;
		min_y = ey1;
	}
	if(ey1 > max_y) {
		if(ey1 > sz.cy) return;
		max_y = min(ey1, sz.cy - 1);
	}
	if(ey2 < min_y) {
		if(ey2 < 0) return;
		min_y = ey2;
	}
	if(ey2 > max_y) {
		if(ey2 > sz.cy) return;
		max_y = min(ey2, sz.cy - 1);
	}

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
	int fy1 = y1 & 255;
	int fy2 = y2 & 255;


	Cell *c;
	int x_from, x_to;
	int p, rem, mod, lift, delta, first, incr;

	if(ey1 == ey2) {
		RenderHLine(ey1, x1, fy1, x2, fy2);
		return;
    }
	incr  = 1;
	if(dx == 0) {
		int two_fx = (x1 - (ex1 << 8)) << 1;
		int area;
		first = 256;
		if(dy < 0) {
			first = 0;
			incr  = -1;
		}
		x_from = x1;
		delta = first - fy1;
		c = AddCells(ey1, 1);
		c->x = ex1;
		c->cover = delta;
		c->area = two_fx * delta;
		ey1 += incr;
		delta = first + first - 256;
		area = two_fx * delta;
		while(ey1 != ey2) {
			c = AddCells(ey1, 1);
			c->x = ex1;
			c->cover = delta;
			c->area = area;
			ey1 += incr;
		}
		delta = fy2 - 256 + first;
		c = AddCells(ey1, 1);
		c->x = ex1;
		c->cover = delta;
		c->area = two_fx * delta;
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

void Rasterizer::Filler::End() {}

void Rasterizer::Render(int y, Rasterizer::Filler& g, bool evenodd)
{
	PAINTER_TIMING("Render");
	CellArray& a = cell[y];
	if(a.count == 0)
		return;
	Cell *c = a.Get();
	Cell *e = c + a.count;
	Sort(SubRange(c, e));
	g.Start(c->x, (e - 1)->x);
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
		if(evenodd) {
			if(area) {
				int h = abs(((cover << 9) - area) >> 9) & 511;
				g.Render(h > 256 ? 512 - h : h);
				x++;
			}
			if(c < e && c->x > x) {
				int h = abs(cover) & 511;
				g.Render(h > 256 ? 512 - h : h, c->x - x);
			}
		}
		else {
			if(area) {
				g.Render(min(abs(((cover << 9) - area) >> 9), 256));
				x++;
			}
			if(c < e && c->x > x)
				g.Render(min(abs(cover), 256), c->x - x);
		}
	}
	g.End();
}

}