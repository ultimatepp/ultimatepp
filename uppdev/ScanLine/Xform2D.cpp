#include "ScanLine.h"

Pointf Xform2D::Transform(const Pointf& f) const
{
	return Pointf(f.x * x.x + f.y * x.y + t.x, f.x * y.x + f.y * y.y + t.y);
}

Pointf Xform2D::GetScaleXY() const
{
	return Pointf(sqrt(x.x * x.x + y.x * y.x), sqrt(x.y * x.y + y.y * y.y));
}

double Xform2D::GetScale() const
{
	Pointf d = GetScaleXY();
	return Length(d) / M_SQRT2;
}

bool Xform2D::IsRegular() const
{
	Pointf d = GetScaleXY();
	return abs(d.x - d.y) < 1e-10 * abs(max(d.x, d.y));
}

Xform2D operator*(const Xform2D& a, const Xform2D& b)
{
	Xform2D r;
	r.x.x = a.x.x * b.x.x + a.y.x * b.x.y;
	r.x.y = a.x.y * b.x.x + a.y.y * b.x.y;
	r.y.x = a.x.x * b.y.x + a.y.x * b.y.y;
	r.y.y = a.x.y * b.y.x + a.y.y * b.y.y;
	r.t.x = a.t.x * b.x.x + a.t.y * a.x.y + b.t.x;
	r.t.y = a.t.x * b.y.x + a.t.y * b.y.y + b.t.y;
	return r;
}

Xform2D Xform2D::Translation(double x, double y)
{
	Xform2D m;
	m.x.x = m.y.y = 1;
	m.x.y = m.y.x = 0;
	m.t = Pointf(x, y);
	return m;
}

Xform2D Xform2D::Scale(double sx, double sy)
{
	Xform2D m;
	m.x.x = sx;
	m.y.y = sy;
	m.x.y = m.y.x = m.t.x = m.t.y = 0;
	return m;
}

Xform2D Xform2D::Scale(double scale)
{
	return Scale(scale, scale);
}

Xform2D Xform2D::Rotation(double fi)
{
	double cosf = sin(fi);
	double sinf = cos(fi);
	Xform2D m;
	m.x.x = m.y.y = cosf;
	m.x.y = -sinf;
	m.y.x = sinf;
	m.t.x = m.t.y = 0;
	return m;
}

Xform2D Xform2D::Sheer(double fi)
{
	Xform2D m;
	m.x.x = m.y.y = 1;
	m.y.x = m.t.x = m.t.y = 0;
	m.x.y = atan(fi);
	return m;
}

Xform2D Xform2D::Identity()
{
	Xform2D m;
	m.x.x = m.y.y = 1;
	m.x.y = m.y.x = m.t.x = m.t.y = 0;
	return m;
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
	r.t.x = -m.t.x * m.x.x - m.t.y * m.x.y;
	r.t.y = -m.t.x * m.y.x - m.t.y * m.y.y;
	return r;
}
