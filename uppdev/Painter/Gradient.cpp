#include "Painter.h"

NAMESPACE_UPP

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

Image BufferPainter::GetGradient(const RGBA& color1, const RGBA& color2, const Pointf& p1, const Pointf& p2)
{
	int n = minmax(int(Distance(p1, p2) * pathattr.mtx.GetScale()), 2, 4096);
	ImageBuffer ib(n, 1);
	MakeGradient(ib, color1, color2, n);
	return ib;
}

void BufferPainter::FillOp(const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	Fill(GetGradient(color1, color2, p1, p2), p1.x, p1.y, p2.x, p2.y,
	     FILL_VPAD | FILL_FAST | 
	     (style == GRADIENT_PAD ? FILL_HPAD : style == GRADIENT_REPEAT
	                            ? FILL_HREPEAT : FILL_HREFLECT));
}

void BufferPainter::StrokeOp(double width, const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	Stroke(width, GetGradient(color1, color2, p1, p2), p1.x, p1.y, p2.x, p2.y,
	       FILL_VPAD | FILL_FAST | 
	       (style == GRADIENT_PAD ? FILL_HPAD : style == GRADIENT_REPEAT
	                              ? FILL_HREPEAT : FILL_HREFLECT));
}

END_UPP_NAMESPACE
