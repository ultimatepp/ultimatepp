#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::MakeGradient(RGBA color1, RGBA color2, int n)
{
	if(n == gradientn && color1 == gradient1 && color2 == gradient2)
		return;
	gradientn = n;
	gradient1 = color1;
	gradient2 = color2;
	ImageBuffer ib(n, 1);
	RGBA *t = ib[0];
	int l = 0;
	RGBA cl = color1;
	for(int i = 0; i <= pathattr.stop.GetCount(); i++) {
		int h;
		RGBA ch;
		if(i < pathattr.stop.GetCount()) {
			h = (int)(pathattr.stop[i] * (n - 1));
			ch = pathattr.stop_color[i];
		}
		else {
			h = n - 1;
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
	gradient = ib;
}

void BufferPainter::Gradient(const RGBA& color1, const RGBA& color2, const Pointf& p1, const Pointf& p2)
{
	MakeGradient(color1, color2, minmax(int(Distance(p1, p2) * pathattr.mtx.GetScale()), 2, 4096));
}

static dword sLinearStyle(int style)
{
	dword s = style & ~FILL_FAST;
	return FILL_VPAD | (style & FILL_FAST) |
	       decode(s, GRADIENT_PAD, FILL_HPAD, GRADIENT_REPEAT, FILL_HREPEAT, FILL_HREFLECT);
}

void BufferPainter::FillOp(const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	Gradient(color1, color2, p1, p2);
	Fill(gradient, p1, p2, sLinearStyle(style));
}

void BufferPainter::FillOp(const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	MakeGradient(color1, color2, 500);
	Fill(gradient, Xform2D::Scale(1.0 / 500) * transsrc, sLinearStyle(style));
}

void BufferPainter::StrokeOp(double width, const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	Gradient(color1, color2, p1, p2);
	Stroke(width, gradient, p1, p2, sLinearStyle(style));
}

void BufferPainter::StrokeOp(double width, const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	MakeGradient(color1, color2, 500);
	Stroke(width, gradient, Xform2D::Scale(1.0 / 500) * transsrc, sLinearStyle(style));
}

END_UPP_NAMESPACE
