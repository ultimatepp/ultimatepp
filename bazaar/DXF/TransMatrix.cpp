#include "TransMatrix.h"

// constructor
TransMatrix::TransMatrix(void)
{
	SetIdentity();
}

TransMatrix::TransMatrix(TransMatrix const &m)
{
	c = m.c;
	s = m.s;
	tx = m.tx;
	ty = m.ty;
}

// sets to null transformation
TransMatrix &TransMatrix::SetIdentity(void)
{
	c = 1;
	s = 0;
	tx = ty = 0;
	return *this;
}

// multiply by another matrix
TransMatrix &TransMatrix::Multiply(TransMatrix const &m)
{
	double cx = c * m.c - s * m.s;
	double sx = s * m.c + c * m.s;
	double txx = c * m.tx - s * m.ty + tx;
	double tyx = s * m.tx + c * m.ty + ty;
	
	c = cx;
	s = sx;
	tx = txx;
	ty = tyx;
	return *this;
}

// invert
TransMatrix &TransMatrix::Invert(TransMatrix const &m)
{
	double txi = -tx * c - ty * s;
	double tyi = tx * s - ty * c;
	
	s = -s;
	tx = txi;
	ty = tyi;
	return *this;
}
		
// translate by given distance
TransMatrix &TransMatrix::Translate(double dx, double dy)
{
	tx += c * dx - s * dy;
	ty += s * dx + c * dy;
	return *this;
}

TransMatrix &TransMatrix::Translate(Pointf const &p)
{
	tx += c * p.x - s * p.y;
	ty += s * p.x + c * p.y;
	return *this;
}

// rotate
TransMatrix &TransMatrix::Rotate(double a)
{
	double s2 = sin(a);
	double c2 = cos(a);
	double cx = c * c2 - s * s2;
	double sx = s * c2 + c * s2;
	c = cx;
	s = sx;
	return *this;
}

TransMatrix &TransMatrix::Rotate(Pointf const &center, double a)
{
	Translate(center);
	Rotate(a);
	return Translate(-center);
}

// scale
TransMatrix &TransMatrix::Scale(double f)
{
	c *= f;
	s *= f;
	return *this;
}

// transform a point
Pointf TransMatrix::operator()(Pointf const &p) const
{
	return Pointf(c * p.x - s * p.y + tx, s * p.x + c * p.y + ty);
}

// rotate an angle by rotation part of matrix
double TransMatrix::R(double a) const
{
	double angle = acos(c / sqrt(c * c + s * s));
	return a + angle;
}

// scale a distance with scale part of matrix
double TransMatrix::S(double d) const
{
	double scale = sqrt(c * c + s * s);
	return d * scale;
}

TransMatrix &TransMatrix::operator=(TransMatrix const &m)
{
	c = m.c;
	s = m.s;
	tx = m.tx;
	ty = m.ty;
	return *this;
}

