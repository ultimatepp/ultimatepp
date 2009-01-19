#include "Painter.h"

NAMESPACE_UPP

void PaintingPainter::ClearOp(const RGBA& color)
{
	Put(PAINTING_CLEAR);
	Put(color);
}

void PaintingPainter::MoveOp(double x, double y, bool rel)
{
	Put(PAINTING_MOVE + rel);
	Putf(x, y);
}

void PaintingPainter::LineOp(double x, double y, bool rel)
{
	Put(PAINTING_LINE + rel);
	Putf(x, y);
}

void PaintingPainter::QuadraticOp(double x1, double y1, double x, double y, bool rel)
{
	Put(PAINTING_QUADRATIC + rel);
	Putf(x1, y1);
	Putf(x, y);
}

void PaintingPainter::QuadraticOp(double x, double y, bool rel)
{
	Put(PAINTING_QUADRATIC_S + rel);
	Putf(x, y);
}

void PaintingPainter::CubicOp(double x1, double y1, double x2, double y2, double x, double y, bool rel)
{
	Put(PAINTING_CUBIC + rel);
	Putf(x1, y1);
	Putf(x2, y2);
	Putf(x, y);
}

void PaintingPainter::CubicOp(double x2, double y2, double x, double y, bool rel)
{
	Put(PAINTING_CUBIC_S + rel);
	Putf(x2, y2);
	Putf(x, y);
}

void PaintingPainter::CloseOp()
{
	Put(PAINTING_CLOSE);
}

void PaintingPainter::FillOp(const RGBA& color)
{
	Put(PAINTING_FILL_SOLID);
	Put(color);
}

void PaintingPainter::FillOp(const Image& image, const Matrix2D& transsrc, dword flags)
{
	Put(PAINTING_FILL_IMAGE);
	Putf(transsrc);
	Put(flags);
	data.Add(image);
}

void PaintingPainter::FillOp(double x1, double y1, const RGBA& color1, double x2, double y2, const RGBA& color2, int style)
{
	Put(PAINTING_FILL_GRADIENT);
	Putf(x1, y1);
	Put(color1);
	Putf(x2, y2);
	Put(color2);
	Put(style);
}

void PaintingPainter::FillOp(double fx, double fy, const RGBA& color1, double x, double y, double r, const RGBA& color2, int style)
{
	Put(PAINTING_FILL_RADIAL);
	Putf(fx, fy);
	Put(color1);
	Putf(x, y);
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

void PaintingPainter::StrokeOp(double width, const Image& image, const Matrix2D& transsrc, dword flags)
{
	Put(PAINTING_STROKE_IMAGE);
	Putf(width);
	Putf(transsrc);
	Put(flags);
	data.Add(image);
}

void PaintingPainter::StrokeOp(double width, double x1, double y1, const RGBA& color1, double x2, double y2, const RGBA& color2, int style)
{
	Put(PAINTING_STROKE_GRADIENT);
	Putf(width);
	Putf(x1, y1);
	Put(color1);
	Putf(x2, y2);
	Put(color2);
	Put(style);
}

void PaintingPainter::StrokeOp(double width, double fx, double fy, const RGBA& color1, double x, double y, double r, const RGBA& color2, int style)
{
	Put(PAINTING_STROKE_RADIAL);
	Putf(width);
	Putf(fx, fy);
	Put(color1);
	Putf(x, y);
	Putf(r);
	Put(color2);
	Put(style);
}

void PaintingPainter::ClipOp()
{
	Put(PAINTING_CLIP);
}

void PaintingPainter::CharacterOp(double x, double y, int ch, Font fnt)
{
	Put(PAINTING_CHARACTER);
	Putf(x, y);
	Put32(ch);
	Put(fnt);
}

void PaintingPainter::TextOp(double x, double y, const wchar *text, Font fnt, int n, double *dx)
{
	Put(PAINTING_TEXT);
	Putf(x, y);
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

void PaintingPainter::DashOp(const Vector<double>& dash, double start)
{
	Put(PAINTING_DASH);
	Put32(dash.GetCount());
	for(int i = 0; i < dash.GetCount(); i++)
		Putf(dash[i]);
	Putf(start);
}

void PaintingPainter::NoAAOp(bool noaa)
{
	Put(PAINTING_NOAA);
	Put(noaa);
}

void PaintingPainter::TransformOp(const Matrix2D& m)
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

END_UPP_NAMESPACE
