#include "SDraw.h"

NAMESPACE_UPP

SDraw& SDraw::ColorStop(double pos, const RGBA& color)
{
	Attr& a = Cttr();
	pos = minmax(pos, 0.0, 1.0);
	int i = FindLowerBound(a.stop, pos);
	a.stop.Insert(i, pos);
	a.stop_color.Insert(i, color);
	return *this;
}

SDraw& SDraw::ClearStops()
{
	Attr& a = Cttr();
	a.stop.Clear();
	a.stop_color.Clear();
	return *this;
}

void   SDraw::MakeGradient(RGBA *t, RGBA color1, RGBA color2, int cx)
{
	int l = 0;
	RGBA cl = color1;
	for(int i = 0; i <= pathattr.stop.GetCount(); i++) {
		int h;
		RGBA ch;
		if(i < pathattr.stop.GetCount()) {
			h = (int)(pathattr.stop[i] * (cx - 1));
			ch = pathattr.stop_color[i];
		}
		else {
			h = cx - 1;
			ch = color2;
		}
		int w = h - l;
		for(int j = 0; j < w; j++) {
			t->r = ((w - j) * cl.r + j * ch.r) / w;
			t->g = ((w - j) * cl.g + j * ch.g) / w;
			t->b = ((w - j) * cl.b + j * ch.b) / w;
			t->a = ((w - j) * cl.a + j * ch.a) / w;
			t++;
		}
		cl = ch;
		l = h;
	}
	*t = cl;
}

SDraw& SDraw::Fill(double x1, double y1, const RGBA& color1,
                   double x2, double y2, const RGBA& color2, int style)
{
	ImageBuffer ib(2048, 1); // adapt size according to bound rect
	MakeGradient(ib, color1, color2, 2048);
	Fill(ib, x1, y1, x2, y2, FILL_VPAD | FILL_FAST | 
	     (style == GRADIENT_PAD ? FILL_HPAD : style == GRADIENT_REPEAT
	                                        ? FILL_HREPEAT : FILL_HREFLECT));
	return *this;
}

END_UPP_NAMESPACE
