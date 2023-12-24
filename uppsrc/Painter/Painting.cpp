#include "Painter.h"

namespace Upp {

void PaintingPainter::ClearOp(const RGBA& color)
{
	Put(PAINTING_CLEAR);
	Put(color);
}

void PaintingPainter::MoveOp(const Pointf& p, bool rel)
{
	Put(PAINTING_MOVE + rel);
	Putf(p);
}

void PaintingPainter::LineOp(const Pointf& p, bool rel)
{
	Put(PAINTING_LINE + rel);
	Putf(p);
}

void PaintingPainter::QuadraticOp(const Pointf& p1, const Pointf& p, bool rel)
{
	Put(PAINTING_QUADRATIC + rel);
	Putf(p1);
	Putf(p);
}

void PaintingPainter::QuadraticOp(const Pointf& p, bool rel)
{
	Put(PAINTING_QUADRATIC_S + rel);
	Putf(p);
}

void PaintingPainter::CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel)
{
	Put(PAINTING_CUBIC + rel);
	Putf(p1);
	Putf(p2);
	Putf(p);
}

void PaintingPainter::CubicOp(const Pointf& p2, const Pointf& p, bool rel)
{
	Put(PAINTING_CUBIC_S + rel);
	Putf(p2);
	Putf(p);
}

void PaintingPainter::ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel)
{
	Put(PAINTING_ARC + rel);
	Putf(c);
	Putf(r);
	Putf(angle);
	Putf(sweep);
}

void PaintingPainter::SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep, const Pointf& p, bool rel)
{
	Put(PAINTING_SVGARC + rel);
	Putf(r);
	Putf(xangle);
	Put(large);
	Put(sweep);
	Putf(p);
}

void PaintingPainter::CloseOp()
{
	Put(PAINTING_CLOSE);
}

void PaintingPainter::DivOp()
{
	Put(PAINTING_DIV);
}

void PaintingPainter::FillOp(const RGBA& color)
{
	Put(PAINTING_FILL_SOLID);
	Put(color);
}

void PaintingPainter::FillOp(const Image& image, const Xform2D& transsrc, dword flags)
{
	Put(PAINTING_FILL_IMAGE);
	Putf(transsrc);
	Put(flags);
	data.Add(image);
}

void PaintingPainter::FillOp(const Pointf& p1, const RGBA& color1, const Pointf& p2,
                             const RGBA& color2, int style)
{
	Put(PAINTING_FILL_GRADIENT);
	Putf(p1);
	Put(color1);
	Putf(p2);
	Put(color2);
	Put(style);
}

void PaintingPainter::FillOp(const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	Put(PAINTING_FILL_GRADIENT_X);
	Put(color1);
	Put(color2);
	Putf(transsrc);
	Put(style);
}

void PaintingPainter::FillOp(const Pointf& f, const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	Put(PAINTING_FILL_RADIAL_X);
	Putf(f);
	Put(color1);
	Put(color2);
	Putf(transsrc);
	Put(style);
}

void PaintingPainter::StrokeOp(double width, const Pointf& f, const RGBA& color1,
                               const RGBA& color2, const Xform2D& transsrc, int style)
{
	Put(PAINTING_FILL_RADIAL_X);
	Putf(width);
	Putf(f);
	Put(color1);
	Put(color2);
	Putf(transsrc);
	Put(style);
}


void PaintingPainter::FillOp(const Pointf& f, const RGBA& color1,
                             const Pointf& p, double r, const RGBA& color2, int style)
{
	Put(PAINTING_FILL_RADIAL);
	Putf(f);
	Put(color1);
	Putf(p);
	Putf(r);
	Put(color2);
	Put(style);
}

void PaintingPainter::StrokeOp(double width, const RGBA& color)
{
	Put(PAINTING_STROKE_SOLID);
	Putf(width);
	Put(color);
}

void PaintingPainter::StrokeOp(double width, const Image& image,
                               const Xform2D& transsrc, dword flags)
{
	Put(PAINTING_STROKE_IMAGE);
	Putf(width);
	Putf(transsrc);
	Put(flags);
	data.Add(image);
}

void PaintingPainter::StrokeOp(double width, const Pointf& p1, const RGBA& color1,
                               const Pointf& p2, const RGBA& color2, int style)
{
	Put(PAINTING_STROKE_GRADIENT);
	Putf(width);
	Putf(p1);
	Put(color1);
	Putf(p2);
	Put(color2);
	Put(style);
}

void PaintingPainter::StrokeOp(double width, const RGBA& color1, const RGBA& color2,
                               const Xform2D& transsrc, int style)
{
	Put(PAINTING_STROKE_GRADIENT_X);
	Putf(width);
	Put(color1);
	Put(color2);
	Putf(transsrc);
	Put(style);
}

void PaintingPainter::StrokeOp(double width, const Pointf& f,
                               const RGBA& color1, const Pointf& p, double r,
                               const RGBA& color2, int style)
{
	Put(PAINTING_STROKE_RADIAL);
	Putf(width);
	Putf(f);
	Put(color1);
	Putf(p);
	Putf(r);
	Put(color2);
	Put(style);
}

void PaintingPainter::ClipOp()
{
	Put(PAINTING_CLIP);
}

void PaintingPainter::CharacterOp(const Pointf& p, int ch, Font fnt)
{
	Put(PAINTING_CHARACTER);
	Putf(p);
	Put32(ch);
	Put(fnt);
}

void PaintingPainter::TextOp(const Pointf& p, const wchar *text, Font fnt, int n, const double *dx)
{
	Put(PAINTING_TEXT);
	Putf(p);
	Put32(n);
	Put((bool)dx);
	Put(fnt);
	for(int i = 0; i < n; i++) {
		Put32(text[i]);
		if(dx)
			Putf(dx[i]);
	}
}

void PaintingPainter::ColorStopOp(double pos, const RGBA& color)
{
	Put(PAINTING_COLORSTOP);
	Putf(pos);
	Put(color);
}

void PaintingPainter::ClearStopsOp()
{
	Put(PAINTING_CLEARSTOPS);
}

void PaintingPainter::OpacityOp(double o)
{
	Put(PAINTING_OPACITY);
	Putf(o);
}

void PaintingPainter::LineCapOp(int linecap)
{
	Put(PAINTING_LINECAP);
	Put(linecap);
}

void PaintingPainter::LineJoinOp(int linejoin)
{
	Put(PAINTING_LINEJOIN);
	Put(linejoin);
}

void PaintingPainter::MiterLimitOp(double l)
{
	Put(PAINTING_MITERLIMIT);
	Putf(l);
}

void PaintingPainter::EvenOddOp(bool evenodd)
{
	Put(PAINTING_EVENODD);
	Put(evenodd);
}

void PaintingPainter::InvertOp(bool invert)
{
	Put(PAINTING_INVERT);
	Put(invert);
}

void PaintingPainter::ImageFilterOp(int filter)
{
	Put(PAINTING_IMAGE_FILTER);
	Put(filter);
}

void PaintingPainter::DashOp(const Vector<double>& dash, double start)
{
	Put(PAINTING_DASH);
	Put32(dash.GetCount());
	for(int i = 0; i < dash.GetCount(); i++)
		Putf(dash[i]);
	Putf(start);
}

void PaintingPainter::TransformOp(const Xform2D& m)
{
	Put(PAINTING_TRANSFORM);
	Putf(m);
}

void PaintingPainter::BeginOp()
{
	Put(PAINTING_BEGIN);
}

void PaintingPainter::EndOp()
{
	Put(PAINTING_END);
}

void PaintingPainter::BeginMaskOp()
{
	Put(PAINTING_BEGINMASK);
}

void PaintingPainter::BeginOnPathOp(double q, bool abs)
{
	Put(PAINTING_BEGINONPATH);
	Putf(q);
	Put(abs);
}

Painting PaintingPainter::GetResult()
{
	Painting p;
	p.cmd = cmd.GetResult();
	p.data = data;
	p.size = size;
	return p;
}

void PaintingPainter::Create(double cx, double cy)
{
	cmd.Create();
	size.cx = cx;
	size.cy = cy;
}

void PaintingPainter::Create(Sizef sz)
{
	Create(sz.cx, sz.cy);
}

}
