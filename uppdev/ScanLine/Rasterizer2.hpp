#include "ScanLine.h"

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

template <class Target>
void Rasterizer::Render(int y, Target& g)
{
	PAINTER_TIMING("Render");
	const Cell *c, *e;
	if(!BeginRender(y, c, e))
		return;
	g.Start(c->x);
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
			g.Render(Alpha((cover << (8 + 1)) - area));
			x++;
		}
		if(c < e && c->x > x)
			g.Render(Alpha(cover << (8 + 1)), c->x - x);
    }
}
