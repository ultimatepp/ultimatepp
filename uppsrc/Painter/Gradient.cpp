#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::ColorStop0(Attr& a, double pos, const RGBA& color)
{
	pos = minmax(pos, 0.0, 1.0);
	int i = FindLowerBound(a.stop, pos);
	a.stop.Insert(i, pos);
	a.stop_color.Insert(i, color);
}

void BufferPainter::ColorStopOp(double pos, const RGBA& color)
{
	ColorStop0(pathattr, pos, color);
	if(!inpath)
		ColorStop0(attr, pos, color);
}

void BufferPainter::ClearStopsOp()
{
	pathattr.stop.Clear();
	pathattr.stop_color.Clear();
	if(!inpath) {
		attr.stop.Clear();
		attr.stop_color.Clear();
	}
}

void BufferPainter::MakeGradient(RGBA *t, RGBA color1, RGBA color2, int cx)
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

void BufferPainter::FillOp(double x1, double y1, const RGBA& color1,
                           double x2, double y2, const RGBA& color2, int style)
{
	int n = max(2, int((fabs(pathrect.GetWidth()) + fabs(pathrect.GetHeight()))
	                   * pathattr.mtx.scale()));
	ImageBuffer ib(n, 1);
	MakeGradient(ib, color1, color2, n);
	Fill(ib, x1, y1, x2, y2, FILL_VPAD | FILL_FAST | 
	     (style == GRADIENT_PAD ? FILL_HPAD : style == GRADIENT_REPEAT
	                                        ? FILL_HREPEAT : FILL_HREFLECT));
}

END_UPP_NAMESPACE
