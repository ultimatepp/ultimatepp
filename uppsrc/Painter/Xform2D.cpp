#include "Painter.h"

namespace Upp {

Pointf Xform2D::GetScaleXY() const
{
	return Pointf(sqrt(x.x * x.x + y.x * y.x), sqrt(x.y * x.y + y.y * y.y));
}

double Xform2D::GetScale() const
{
	Pointf d = GetScaleXY();
	return Length(d) / M_SQRT2;
}

inline bool Epsqual(double x, double y)
{
	return fabs(x - y) < 1e-10 * max(fabs(x), fabs(y));
}

bool Xform2D::IsRegular() const
{
	Pointf d = GetScaleXY();
	return Epsqual(d.x, d.y);
}

Xform2D::Xform2D()
{
	x.x = y.y = 1;
	x.y = y.x = t.x = t.y = 0;
}

Xform2D operator*(const Xform2D& a, const Xform2D& b)
{
	Xform2D r;
	r.x.x = a.x.x * b.x.x + a.y.x * b.x.y;
	r.x.y = a.x.y * b.x.x + a.y.y * b.x.y;
	r.y.x = a.x.x * b.y.x + a.y.x * b.y.y;
	r.y.y = a.x.y * b.y.x + a.y.y * b.y.y;
	r.t.x = a.t.x * b.x.x + a.t.y * b.x.y + b.t.x;
	r.t.y = a.t.x * b.y.x + a.t.y * b.y.y + b.t.y;
	return r;
}

Xform2D Xform2D::Translation(double x, double y)
{
	Xform2D m;
	m.t = Pointf(x, y);
	return m;
}

Xform2D Xform2D::Scale(double sx, double sy)
{
	Xform2D m;
	m.x.x = sx;
	m.y.y = sy;
	return m;
}

Xform2D Xform2D::Scale(double scale)
{
	return Scale(scale, scale);
}

Xform2D Xform2D::Rotation(double fi)
{
	double cosf = cos(fi);
	double sinf = sin(fi);
	Xform2D m;
	m.x.x = cosf;
	m.x.y = -sinf;
	m.y.x = sinf;
	m.y.y = cosf;
	m.t.x = m.t.y = 0;
	return m;
}

Xform2D Xform2D::Sheer(double fi)
{
	Xform2D m;
	m.x.y = atan(fi);
	return m;
}

Xform2D Xform2D::SheerX(double fi)
{
	Xform2D m;
	m.y.x = atan(fi);
	return m;
}

Xform2D Xform2D::Identity()
{
	Xform2D m;
	return m;
}

Xform2D Xform2D::Map(Pointf s1, Pointf s2, Pointf s3)
{
	Xform2D s;
	s.x.x = s1.x - s3.x;
	s.y.x = s1.y - s3.y;
	s.x.y = s2.x - s3.x;
	s.y.y = s2.y - s3.y;
	s.t = s3;
	return s;
}

Xform2D Xform2D::Map(Pointf s1, Pointf s2, Pointf s3, Pointf t1, Pointf t2, Pointf t3)
{
	return Inverse(Xform2D::Map(s1, s2, s3)) * Map(t1, t2, t3);
}

double  Determinant(const Xform2D& m)
{
	return m.x.x * m.y.y - m.y.x * m.x.y;
}

Xform2D Inverse(const Xform2D& m)
{
	Xform2D r;
	double det = Determinant(m);
	r.x = Pointf(m.y.y, -m.x.y) / det;
	r.y = Pointf(-m.y.x, m.x.x) / det;
	r.t.x = -m.t.x * r.x.x - m.t.y * r.x.y;
	r.t.y = -m.t.x * r.y.x - m.t.y * r.y.y;
	return r;
}

}
