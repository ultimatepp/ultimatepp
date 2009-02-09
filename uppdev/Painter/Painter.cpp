#include "Painter.h"

NAMESPACE_UPP

Painter& Painter::Move(const Pointf& p)
{
	MoveOp(p, false);
	return *this;
}

Painter& Painter::Line(const Pointf& p)
{
	LineOp(p, false);
	return *this;
}

Painter& Painter::Quadratic(const Pointf& p1, const Pointf& p)
{
	QuadraticOp(p1, p, false);
	return *this;
}

Painter& Painter::Quadratic(const Pointf& p)
{
	QuadraticOp(p, false);
	return *this;
}

Painter& Painter::Cubic(const Pointf& p1, const Pointf& p2, const Pointf& p)
{
	CubicOp(p1, p2, p, false);
	return *this;
}

Painter& Painter::Cubic(const Pointf& p2, const Pointf& p)
{
	CubicOp(p2, p, false);
	return *this;
}

Painter& Painter::RelMove(const Pointf& p)
{
	MoveOp(p, true);
	return *this;
}

Painter& Painter::RelLine(const Pointf& p)
{
	LineOp(p, true);
	return *this;
}

Painter& Painter::RelQuadratic(const Pointf& p1, const Pointf& p)
{
	QuadraticOp(p1, p, true);
	return *this;
}

Painter& Painter::RelQuadratic(const Pointf& p)
{
	QuadraticOp(p, true);
	return *this;
}

Painter& Painter::RelCubic(const Pointf& p1, const Pointf& p2, const Pointf& p)
{
	CubicOp(p1, p2, p, true);
	return *this;
}

Painter& Painter::RelCubic(const Pointf& p2, const Pointf& p)
{
	CubicOp(p2, p, true);
	return *this;
}

Painter& Painter::Move(double x, double y, bool rel)
{
	MoveOp(Pointf(x, y), rel);
	return *this;
}

Painter& Painter::Line(double x, double y, bool rel)
{
	LineOp(Pointf(x, y), rel);
	return *this;
}

Painter& Painter::Quadratic(double x1, double y1, double x, double y, bool rel)
{
	QuadraticOp(Pointf(x1, y1), Pointf(x, y), rel);
	return *this;
}

Painter& Painter::Quadratic(double x, double y, bool rel)
{
	QuadraticOp(Pointf(x, y), rel);
	return *this;
}

Painter& Painter::Cubic(double x1, double y1, double x2, double y2, double x, double y, bool rel)
{
	CubicOp(Pointf(x1, y1), Pointf(x2, y2), Pointf(x, y), rel);
	return *this;
}

Painter& Painter::Cubic(double x2, double y2, double x, double y, bool rel)
{
	CubicOp(Pointf(x2, y2), Pointf(x, y), rel);
	return *this;
}

Painter& Painter::Move(double x, double y)
{
	Move(x, y, false);
	return *this;
}

Painter& Painter::Line(double x, double y)
{
	Line(x, y, false);
	return *this;
}

Painter& Painter::Quadratic(double x1, double y1, double x, double y)
{
	Quadratic(x1, y1, x, y, false);
	return *this;
}

Painter& Painter::Quadratic(double x, double y)
{
	Quadratic(x, y, false);
	return *this;
}

Painter& Painter::Cubic(double x1, double y1, double x2, double y2, double x, double y)
{
	Cubic(x1, y1, x2, y2, x, y, false);
	return *this;
}

Painter& Painter::Cubic(double x2, double y2, double x, double y)
{
	Cubic(x2, y2, x, y, false);
	return *this;
}

Painter& Painter::RelMove(double x, double y)
{
	Move(x, y, true);
	return *this;
}

Painter& Painter::RelLine(double x, double y)
{
	Line(x, y, true);
	return *this;
}

Painter& Painter::RelQuadratic(double x1, double y1, double x, double y)
{
	Quadratic(x1, y1, x, y, true);
	return *this;
}

Painter& Painter::RelQuadratic(double x, double y)
{
	Quadratic(x, y, true);
	return *this;
}

Painter& Painter::RelCubic(double x1, double y1, double x2, double y2, double x, double y)
{
	Cubic(x1, y1, x2, y2, x, y, true);
	return *this;
}

Painter& Painter::RelCubic(double x2, double y2, double x, double y)
{
	Cubic(x2, y2, x, y, true);
	return *this;
}

Painter& Painter::Translate(double x, double y)
{
	Transform(Xform2D::Translation(x, y));
	return *this;
}

Painter& Painter::Rotate(double a)
{
	Transform(Xform2D::Rotation(a));
	return *this;
}

Painter& Painter::Scale(double scalex, double scaley)
{
	Transform(Xform2D::Scale(scalex, scaley));
	return *this;
}

Painter& Painter::Scale(double scale)
{
	Scale(scale, scale);
	return *this;
}

Painter& Painter::Dash(const char *dash, double start)
{
	Vector<double> d;
	CParser p(dash);
	try {
		while(!p.IsEof())
			if(p.Char(':'))
				start = p.ReadDouble();
			else
				d.Add(p.ReadDouble());
	}
	catch(CParser::Error) {}
	Dash(d, start);
	return *this;
}

END_UPP_NAMESPACE
