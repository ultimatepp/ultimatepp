#include "Painter.h"

namespace Upp {

Image BufferPainter::MakeGradient(RGBA color1, Vector<ColorStop>& color_stop, RGBA color2, int n)
{
	ImageBuffer ib(n, 1);
	RGBA *t = ib[0];
	int l = 0;
	RGBA cl = color1;
	for(int i = 0; i <= color_stop.GetCount(); i++) {
		int h;
		RGBA ch;
		if(i < color_stop.GetCount()) {
			h = (int)(color_stop[i].stop * (n - 1));
			ch = color_stop[i].color;
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
	return Image(ib);
}

struct BufferPainter::GradientImageMaker : public ImageMaker {
	RGBA color1;
	Vector<ColorStop> *color_stop;
	RGBA color2;
	int n;

	virtual String Key() const;
	virtual Image  Make() const;
};

String BufferPainter::GradientImageMaker::Key() const
{
	StringStream ss;
	auto cc = const_cast<GradientImageMaker *>(this);
	ss % cc->color1 % *cc->color_stop % cc->color2 % cc->n;
	return ss.GetResult();
}

Image BufferPainter::GradientImageMaker::Make() const
{
	return MakeGradient(color1, *color_stop, color2, n);
}

Image BufferPainter::MakeGradientCached(RGBA color1, Vector<ColorStop>& color_stop, RGBA color2, int n)
{
	GradientImageMaker m;
	m.color1 = color1;
	m.color_stop = &color_stop;
	m.color2 = color2;
	m.n = n;
	return MakeImage(m);
}

Image BufferPainter::Gradient(const RGBA& color1, const RGBA& color2, int n)
{
	return (imagecache ? MakeGradientCached : MakeGradient)(color1, pathattr.color_stop, color2, n);
}

Image BufferPainter::Gradient(const RGBA& color1, const RGBA& color2, const Pointf& p1, const Pointf& p2)
{
	return Gradient(color1, color2, minmax(int(Distance(p1, p2) * pathattr.mtx.GetScale()), 2, 16384));
}

static dword sLinearStyle(int style)
{
	dword s = style & ~FILL_FAST;
	return FILL_VPAD | (style & FILL_FAST) |
	       decode(s, GRADIENT_PAD, FILL_HPAD, GRADIENT_REPEAT, FILL_HREPEAT, FILL_HREFLECT);
}

void BufferPainter::FillOp(const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	Fill(Gradient(color1, color2, p1, p2), p1, p2, sLinearStyle(style));
}

void BufferPainter::FillOp(const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	Fill(Gradient(color1, color2, 500), Xform2D::Scale(1.0 / 500) * transsrc, sLinearStyle(style));
}

void BufferPainter::StrokeOp(double width, const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	Stroke(width, Gradient(color1, color2, p1, p2), p1, p2, sLinearStyle(style));
}

void BufferPainter::StrokeOp(double width, const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	Stroke(width, Gradient(color1, color2, 500), Xform2D::Scale(1.0 / 500) * transsrc, sLinearStyle(style));
}

}
