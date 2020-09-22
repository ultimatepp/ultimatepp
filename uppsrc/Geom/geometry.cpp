#include "Geom.h"

namespace Upp {

Pointf Length(const Pointf& p, double length)
{
	double l = max(Length(p), AbsMax(p));
	Pointf out = p;
	if(l)
		out *= length / l;
	return out;
}

Sizef Length(const Sizef& p, double length)
{
	double l = max(Length(p), AbsMax(p));
	Sizef out = p;
	if(l)
		out *= length / l;
	return out;
}

Pointf Project(const Pointf& p, const Pointf& A, const Pointf& B)
{
	Pointf AB = B - A;
	double length = Squared(AB);
	if(length == 0)
		return A;
	return A + AB * ((AB ^ (p - A)) / length);
}

/* TRC 09/04/22: moved to Core by CXL
double Bearing(const Pointf& p)
{
	if(p.y == 0)
		return (p.x >= 0 ? 0 : M_PI);
	if(p.x == 0)
		return (p.y >= 0 ? M_PI_2 : 3 * M_PI_2);
	double b = atan2(p.y, p.x);
	if(b < 0)
		b += 2 * M_PI;
	return b;
}
*/

Pointf Bezier2(const Pointf& a, const Pointf& b, const Pointf& c, double t)
{
	Pointf ab = b - a, bc = c - b;
	return a + (ab * (2 * t) + (bc - ab) * (t * t));
}

double Bezier2Length(const Pointf& a, const Pointf& b, const Pointf& c, double t)
{
	Pointf v = b - a, w = (c - b) - v;
	v *= 2;
	double w2 = Squared(w);
	if(fabs(w2) <= 1e-100)
		return t * Distance(c, a);
	double k = ScalarProduct(Sizef(v), Sizef(w)) / (2 * w2);
	double d = fabs(VectorProduct(Sizef(v), Sizef(w))) / (2 * w2);
	double wl = sqrt(w2);
	double x = t + k;
	if(fabs(d) <= 1e-100)
		return wl * x * fabs(x) / 2;
	return wl * (x * hypot(x, d) + (d * d) * argsinh(x / d));
}

Sizef Bezier2Tangent(const Pointf& a, const Pointf& b, const Pointf& c, double t)
{
	Sizef ab = b - a, bc = c - b;
	return 2.0 * (ab + t * (bc - ab));
}

Sizef Orthogonal(const Sizef& v, const Sizef& against)
{
	return v - against * ((v ^ against) / Squared(against));
}

Sizef Orthonormal(const Sizef& v, const Sizef& against)
{
	return UnitV(Orthogonal(v, against));
}

Sizef GetFarthestAxis(const Sizef& v)
{
	return fabs(v.cx) <= fabs(v.cy) ? Sizef(1, 0) : Sizef(0, 1);
}

double Distance(const Rectf& rc, const Pointf& pt)
{
	double dx = (pt.x >= rc.right  ? pt.x - rc.right  : pt.x <= rc.left ? pt.x - rc.left : 0);
	double dy = (pt.y >= rc.bottom ? pt.y - rc.bottom : pt.y <= rc.top  ? pt.y - rc.top  : 0);
	if(dx == 0)
		return fabs(dy);
	if(dy == 0)
		return fabs(dx);
	return hypot(dx, dy);
}

Rectf& SetUnion(Rectf& rc, const Pointf& pt)
{
	if(IsNull(pt))
		return rc;
	if(IsNull(rc))
	{
		rc.left = rc.right = pt.x;
		rc.top = rc.bottom = pt.y;
		return rc;
	}
	if(pt.x < rc.left)   rc.left   = pt.x;
	if(pt.x > rc.right)  rc.right  = pt.x;
	if(pt.y < rc.top)    rc.top    = pt.y;
	if(pt.y > rc.bottom) rc.bottom = pt.y;
	return rc;
}

Rectf GetUnion(const Array<Pointf>& pt)
{
	if(pt.IsEmpty())
		return Null;
	Rectf rc = PointfRectf(pt[0]);
	for(int i = 1; i < pt.GetCount(); i++)
		rc |= pt[i];
	return rc;
}

Rectf& SetMinMaxMove(Rectf& rc, const Rectf& outer_rc)
{
	if(IsNull(rc))
		return rc;
	if(IsNull(outer_rc))
		return rc = Null;
	if(rc.right  > outer_rc.right)  rc.OffsetHorz(outer_rc.right  - rc.right);
	if(rc.bottom > outer_rc.bottom) rc.OffsetVert(outer_rc.bottom - rc.bottom);
	if(rc.left   < outer_rc.left)   rc.OffsetHorz(outer_rc.left   - rc.left);
	if(rc.top    < outer_rc.top)    rc.OffsetVert(outer_rc.top    - rc.top);
	rc &= outer_rc;
	return rc;
}

const Matrixf& Matrixf_0()       { static Matrixf m(0, 0, 0, 0, 0, 0);                   return m; }
const Matrixf& Matrixf_1()       { static Matrixf m(1, 0, 0, 1, 0, 0);                   return m; }
const Matrixf& Matrixf_Null()    { static Matrixf m(Null, Null, Null, Null, Null, Null); return m; }
const Matrixf& Matrixf_MirrorX() { static Matrixf m(1, 0, 0, -1, 0, 0);                  return m; }
const Matrixf& Matrixf_MirrorY() { static Matrixf m(-1, 0, 0, 1, 0, 0);                  return m; }

Matrixf MatrixfMove(Pointf vector)
{
	return Matrixf(1, 0, 0, 1, vector.x, vector.y);
}

Matrixf MatrixfRotate(double angle, Pointf f)
{
	Pointf temp = PolarPointf(angle);
	Matrixf out(temp, Left(temp));
	out.Fix(f);
	return out;
}

Matrixf MatrixfScale(double scale, Pointf f)
{
	Matrixf out(Pointf(scale, 0), Pointf(0, scale));
	out.Fix(f);
	return out;
}

Matrixf MatrixfScale(Pointf scale, Pointf f)
{
	Matrixf out(Pointf(scale.x, 0), Pointf(0, scale.y));
	out.Fix(f);
	return out;
}

Matrixf MatrixfScale(const Rectf& src, const Rectf& dest)
{
	Matrixf result(Pointf(dest.Width() / src.Width(), 0), Pointf(0, dest.Height() / src.Height()));
	result.a = dest.TopLeft() - src.TopLeft() * result;
	return result;
}

Matrixf MatrixfMirror(Pointf A, Pointf B)
{
	Pointf V = B - A;
	double vx2 = sqr(V.x);
	double vy2 = sqr(V.y);
	double det = vx2 + vy2;
//	double dif = (vx2 - vy2) / det;
	double mul = 2 * V.x * V.y / det;
	Matrixf out(Pointf(vx2 - vy2, mul), Pointf(mul, vy2 - vx2));
	out.Fix(A);
	return out;
}

Matrixf MatrixfAffine (Pointf src1, Pointf dest1, Pointf src2, Pointf dest2)
{
	return MatrixfAffine(src1, dest1, src2, dest2, src1 + Right(src2 - src1), dest1 + Right(dest2 - dest1));
}

Matrixf MatrixfAffine (Pointf src1, Pointf dest1, Pointf src2, Pointf dest2, Pointf src3, Pointf dest3)
{
	Matrixf rev(src2 - src1, src3 - src1, src1);
	if(fabs(Determinant(rev)) <= 1e-100)
		return MatrixfMove((dest1 - src1 + dest2 - src2 + dest3 - src3) / 3.0);
	return MatrixfInverse(rev) * Matrixf(dest2 - dest1, dest3 - dest1, dest1);
}

Matrixf& Matrixf::operator *= (const Matrixf& m)
{
	double xx = m.x.x * x.x + m.y.x * x.y;
	double xy = m.x.x * y.x + m.y.x * y.y;
	double yx = m.x.y * x.x + m.y.y * x.y;
	double yy = m.x.y * y.x + m.y.y * y.y;
	x = Pointf(xx, yx);
	y = Pointf(xy, yy);
	a *= m;
	return *this;
}

Pointf operator * (Pointf point, const Matrixf& matrix)
{
	return Pointf(
		point.x * matrix.x.x + point.y * matrix.y.x + matrix.a.x,
		point.x * matrix.x.y + point.y * matrix.y.y + matrix.a.y);
}

Pointf operator / (Pointf point, const Matrixf& matrix)
{
	return point * MatrixfInverse(matrix);
}

Rectf operator * (const Rectf& rect, const Matrixf& matrix)
{
	if(IsNull(rect))
		return Null;
	double xl = min(rect.left, rect.right),  xh = max(rect.left, rect.right);
	double yl = min(rect.top,  rect.bottom), yh = max(rect.top,  rect.bottom);
	Rectf out;
	out.left   = matrix.x.x * (matrix.x.x >= 0 ? xl : xh) + matrix.y.x * (matrix.y.x >= 0 ? yl : yh) + matrix.a.x;
	out.top    = matrix.x.y * (matrix.x.y >= 0 ? xl : xh) + matrix.y.y * (matrix.y.y >= 0 ? yl : yh) + matrix.a.y;
	out.right  = matrix.x.x * (matrix.x.x >= 0 ? xh : xl) + matrix.y.x * (matrix.y.x >= 0 ? yh : yl) + matrix.a.x;
	out.bottom = matrix.x.y * (matrix.x.y >= 0 ? xh : xl) + matrix.y.y * (matrix.y.y >= 0 ? yh : yl) + matrix.a.y;
	return out;
}

Rectf operator / (const Rectf& rect, const Matrixf& matrix)
{
	return rect * MatrixfInverse(matrix);
}

double MatrixfMeasure(const Matrixf& mx)
{
	double det = Determinant(mx);
	return (det >= 0 ? +1 : -1) * sqrt(fabs(det));
}

Matrixf MatrixfInverse(const Matrixf& mx)
{
	double det = Determinant(mx);
	Matrixf m(Pointf(mx.y.y, -mx.x.y) / det, Pointf(-mx.y.x, mx.x.x) / det);
	m.a = -mx.a % m;
	return m;
}

String Matrixf::ToString() const
{
	return String().Cat() << "[x = " << x << ", y = " << y << ", a = " << a << "]";
}

}
