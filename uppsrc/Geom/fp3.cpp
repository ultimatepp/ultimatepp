#include "Geom.h"

namespace Upp {

Pointf3 Length(Pointf3 p, double length)
{
	double l = max(Length(p), fpabsmax(p));
	if(l)
		return p * length / l;
	return p;
}

String AsString(const Pointf3& p)
{
	String out;
	if(IsNull(p))
		out = "Pointf3(Null)";
	else
		out << "Pointf3(" << DblStr(p.x) << ", " << DblStr(p.y) << ", " << DblStr(p.z) << ")";
	return out;
}

Pointf3 Bezier2(Pointf3 a, Pointf3 b, Pointf3 c, double t)
{
	double u = 1 - t;
	return a * (u * u) + b * (2 * u * t) + c * (t * t);
}

Pointf3 Orthogonal(Pointf3 v, Pointf3 against)
{
	return v - against * ((v ^ against) / Squared(against));
}

Pointf3 Orthonormal(Pointf3 v, Pointf3 against)
{
	return UnitV(Orthogonal(v, against));
}

Pointf3 FarthestAxis(Pointf3 v)
{
	double tx = fabs(v.x), ty = fabs(v.y), tz = fabs(v.z);
	return tx <= ty && tx <= tz ? Pointf3(1, 0, 0)
		:  ty <= tx && ty <= tz ? Pointf3(0, 1, 0)
		:                         Pointf3(0, 0, 1);
}

Pointf3 AtLine(Pointf3 a, double ta, Pointf3 b, double tb, double tx, double *ratio)
{
	double r = (ta == tb ? 0 : (tx - ta) / (tb - ta));
	if(ratio)
		*ratio = r;
	return a + (b - a) * r;
}

Pointf3 RotateX(Pointf3 v, double angle, Pointf3 centre)
{
	double c = cos(angle), s = sin(angle), y = v.y - centre.y, z = v.z - centre.z;
	return Pointf3(v.x, y * c - z * s + centre.y, y * s + z * c + centre.z);
}

Pointf3 RotateY(Pointf3 v, double angle, Pointf3 centre)
{
	double c = cos(angle), s = sin(angle), z = v.z - centre.z, x = v.x - centre.x;
	return Pointf3(z * s + x * c + centre.x, v.y, z * c - x * s + centre.z);
}

Pointf3 RotateZ(Pointf3 v, double angle, Pointf3 centre)
{
	double c = cos(angle), s = sin(angle), x = v.x - centre.x, y = v.y - centre.y;
	return Pointf3(x * c - y * s + centre.x, x * s + y * c + centre.y, v.z);
}

Pointf3 Rotate(Pointf3 v, Pointf3 axis, double angle, Pointf3 centre)
{
	Matrixf3 m = Matrixf3Rotate(axis, angle);
	m.Fix(centre);
	return v * m;
}

double Angle(Pointf3 a, Pointf3 b)
{
	double da = sqrt(Squared(a) * Squared(b));
	if(da == 0)
		return 0;
	da = (a ^ b) / da;
	if(da <= -1)
		return M_PI;
	else if(da >= 1)
		return 0;
	else
		return acos(da);
}

//////////////////////////////////////////////////////////////////////
// Plane3::

Plane3 Unit3(Plane3 p)
{
	double nt = max(Length(p.normal), fpabsmax(p.normal));
	if(nt)
		return Plane3(p.delta / nt, p.normal / nt);
	return p;
}

double Intersect(Plane3 p, Pointf3 la, Pointf3 lb)
{
	double an = la ^ p.normal, bn = lb ^ p.normal;
	if(an == bn)
		return 0;
	return (an + p.delta) / (an - bn);
}

//////////////////////////////////////////////////////////////////////
// Box3::

void Box3::Union(Pointf3 pt)
{
	double c0 = pt ^ normal[0], c1 = pt ^ normal[1], c2 = pt ^ normal[2];
	if(c0 < low[0]) low[0] = c0;
	if(c0 > high[0]) high[0] = c0;
	if(c1 < low[1]) low[1] = c0;
	if(c1 > high[1]) high[1] = c0;
	if(c2 < low[2]) low[2] = c0;
	if(c2 > high[2]) high[2] = c0;
}

//////////////////////////////////////////////////////////////////////
// Matrix3::

#define GLOBAL_VARP_INIT(a, b, c) a & b() { static a m c; return m; }

GLOBAL_VARP_INIT(const Matrixf3, Matrixf3_0, (0, 0, 0, 0, 0, 0, 0, 0, 0))
GLOBAL_VARP_INIT(const Matrixf3, Matrixf3_1, (1, 0, 0, 0, 1, 0, 0, 0, 1))
GLOBAL_VARP_INIT(const Matrixf3, Matrixf3_Null, (0, 0, 0, 0, 0, 0, 0, 0, 0, Null, Null, Null))
GLOBAL_VARP_INIT(const Matrixf3, Matrixf3_MirrorX, (-1, 0, 0, 0, 1, 0, 0, 0, 1))
GLOBAL_VARP_INIT(const Matrixf3, Matrixf3_MirrorY, (1, 0, 0, 0, -1, 0, 0, 0, 1))
GLOBAL_VARP_INIT(const Matrixf3, Matrixf3_MirrorZ, (1, 0, 0, 0, 1, 0, 0, 0, -1))

void Matrixf3::Serialize(Stream& stream)
{
	stream % x % y % z % a;
}

Matrixf3& Matrixf3::operator *= (const Matrixf3& m)
{
	Pointf3 nx(x ^ m.CX(), x ^ m.CY(), x ^ m.CZ());
	Pointf3 ny(y ^ m.CX(), y ^ m.CY(), y ^ m.CZ());
	Pointf3 nz(z ^ m.CX(), z ^ m.CY(), z ^ m.CZ());
	x = nx;
	y = ny;
	z = nz;
	a *= m;
	return *this;
}

String Matrixf3::ToString() const
{
	return String().Cat() << "[x = " << x << ", y = " << y << ", z = " << z << ", a = " << a << "]";
}

Matrixf3 Matrixf3Move(Pointf3 vector)
{
	return Matrixf3(Pointf3(1, 0, 0), Pointf3(0, 1, 0), Pointf3(0, 0, 1), vector);
}

Matrixf3 Matrixf3RotateX(double angle, Pointf3 fix)
{
	double c = cos(angle), s = sin(angle);
	return Matrixf3(1, 0, 0, 0, c, s, 0, -s, c).Fix(fix);
}

Matrixf3 Matrixf3RotateY(double angle, Pointf3 fix)
{
	double c = cos(angle), s = sin(angle);
	return Matrixf3(c, 0, s, 0, 1, 0, -s, 0, c).Fix(fix);
}

Matrixf3 Matrixf3RotateZ(double angle, Pointf3 fix)
{
	double c = cos(angle), s = sin(angle);
	return Matrixf3(c, s, 0, -s, c, 0, 0, 0, 1).Fix(fix);
}

Matrixf3 Matrixf3Scale(double scale, Pointf3 fix)
{
	return Matrixf3(Pointf3(scale, 0, 0), Pointf3(0, scale, 0),
		Pointf3(0, 0, scale), fix * (1 - scale));
}

Matrixf3 Matrixf3Scale(Pointf3 scale, Pointf3 fix)
{
	return Matrixf3(Pointf3(scale.x, 0, 0), Pointf3(0, scale.y, 0),
		Pointf3(0, 0, scale.z), fix - scale * fix);
}

Matrixf3 Matrixf3Inverse(const Matrixf3& mx)
{
	double det = Determinant(mx);
	Matrixf3 m(
		Pointf3(
			(mx.y.y * mx.z.z - mx.z.y * mx.y.z),
			(mx.z.y * mx.x.z - mx.x.y * mx.z.z),
			(mx.x.y * mx.y.z - mx.y.y * mx.x.z)) / det,
		Pointf3(
			(mx.y.z * mx.z.x - mx.z.z * mx.y.x),
			(mx.z.z * mx.x.x - mx.x.z * mx.z.x),
			(mx.x.z * mx.y.x - mx.y.z * mx.x.x)) / det,
		Pointf3(
			(mx.y.x * mx.z.y - mx.z.x * mx.y.y),
			(mx.z.x * mx.x.y - mx.x.x * mx.z.y),
			(mx.x.x * mx.y.y - mx.y.x * mx.x.y)) / det);
	m.a = -mx.a % m;
	return m;
}

Matrixf3 Matrixf3Affine(Pointf3 src1, Pointf3 dest1, Pointf3 src2, Pointf3 dest2)
{
	return Matrixf3Affine(src1, dest1, src2, dest2, src1 + FarthestAxis(src2 - src1),
		dest1 + FarthestAxis(dest2 - dest1));
}

Matrixf3 Matrixf3Affine(Pointf3 src1, Pointf3 dest1, Pointf3 src2, Pointf3 dest2,
	Pointf3 src3, Pointf3 dest3)
{
	return Matrixf3Affine(src1, dest1, src2, dest2, src3, dest3,
		src1 + (src2 - src1) % (src3 - src1),
		dest1 + (dest2 - dest1) % (dest3 - dest1));
}

Matrixf3 Matrixf3Affine(Pointf3 src1, Pointf3 dest1, Pointf3 src2, Pointf3 dest2,
	Pointf3 src3, Pointf3 dest3, Pointf3 src4, Pointf3 dest4)
{
	Matrixf3 rev(src2 - src1, src3 - src1, src4 - src1, src1);
	if(fabs(Determinant(rev)) <= 1e-100)
		return Matrixf3Move((dest1 - src1 + dest2 - src2 + dest3 - src3 + dest4 - src4) / 4.0);
	return Matrixf3Inverse(rev) * Matrixf3(dest2 - dest1, dest3 - dest1, dest4 - dest1, dest1);
}

double Determinant(const Matrixf3& mx)
{
	return mx.x.x * mx.y.y * mx.z.z
		+  mx.y.x * mx.z.y * mx.x.z
		+  mx.z.x * mx.x.y * mx.y.z
		-  mx.x.z * mx.y.y * mx.z.x
		-  mx.y.z * mx.z.y * mx.x.x
		-  mx.z.z * mx.x.y * mx.y.x;
}

double Matrixf3Measure(const Matrixf3& mx)
{
	double d = Determinant(mx);
	return (d >= 0 ? +1 : -1) * pow(fabs(d), 1.0 / 3.0);
}

Matrixf3 Matrixf3Rotate(Pointf3 axis, double angle)
{
	Matrixf3 to_space = Matrixf3_1();
	to_space.z = UnitV(axis);
	to_space.y = Orthonormal(FarthestAxis(to_space.z), to_space.z);
	to_space.x = to_space.y % to_space.z;
	double ca = cos(angle), sa = sin(angle);
	return Matrixf3Inverse(to_space) * Matrixf3(ca, -sa, 0, sa, ca, 0, 0, 0, 1) * to_space;
}

//////////////////////////////////////////////////////////////////////
// Camera::

Camera::Camera()
{
	ViewingAngle(120 * DEGRAD); // default viewing angle in radians
	Location(Pointf3(-100, 100, 0)); // 150 m from origin in the -x+y distance
	Target(Pointf3(0, 0, 0));
	Upwards(Pointf3(0, 1, 0));
	FarDistance(10000); // 10 km
	NearDistance(100); // 100 m
	Viewport(320, 240);
	ViewportOffset(0, 0);
	ViewportSize(1, 1);
	Projection(Matrixf3_1());
	Update();
}

void Camera::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "Camera");
	if(version >= 1)
	{
		stream % location % target % upwards
			% viewing_angle % width_mm % height_mm
			% shift_x % shift_y % near_distance % far_distance
			% projection_matrix;
	}
}

void Camera::Update()
{
	direction = UnitV(target - location);
	distance_delta = -(location ^ direction);
	viewing_distance = double(min(width_mm, height_mm) / (2e3 * tan(viewing_angle / 2.0)));
	Pointf3 up = Orthogonal(upwards, direction);
	if(Squared(up) <= 1e-10)
		up = Orthogonal(FarthestAxis(direction), direction);
	straight_up = UnitV(up);
	straight_right = direction % straight_up;
	camera_matrix = Matrixf3(straight_right, straight_up, direction, location);
	invcam_matrix = Matrixf3Inverse(camera_matrix);
	double dw = viewing_distance * view_px * 2e3 / width_mm;
	double dh = viewing_distance * view_py * 2e3 / height_mm;
	double z_times = far_distance - near_distance;
	z_delta = -near_distance / z_times;
	transform_matrix = invcam_matrix * Matrixf3(
		Pointf3(dw / z_times, 0, 0),
		Pointf3(0, dh / z_times, 0),
		Pointf3(shift_x, shift_y, 1) / z_times);
}

Pointf3 Camera::Transform(Pointf3 point) const
{
	Pointf3 axon = point * transform_matrix;
	static const double MIN_DISTANCE = 1e-3; // 1 mm should be good enough
	if(axon.z >= -MIN_DISTANCE && axon.z <= +MIN_DISTANCE)
		axon.z = MIN_DISTANCE;
	return Pointf3(axon.x / axon.z, axon.y / axon.z, axon.z + z_delta);
}

void Camera::SetPolar(Pointf3 dz, Pointf3 dx, double d, double a, double b, double r)
{
	Pointf3 dy = dz % dx;
	Pointf3 az = RotateX(Pointf3(0, 0, -1), -b);
	az = d * RotateY(az, a);
	Pointf3 loc = az * Matrixf3(dx, dy, dz, target);
	Pointf3 dir = UnitV(target - loc);
	Pointf3 su = Orthogonal(dy, dir);
	if(Length(su) <= 1e-10)
		su = Orthogonal(FarthestAxis(dir), dir);
	su = UnitV(su);
	Location(loc).Upwards(Rotate(su, dir, r));
}

void Camera::GetPolar(Pointf3 dz, Pointf3 dx,
double& distance, double& azimuth, double& bearing, double& rotation) const
{
	distance = Distance(location, target);
	Pointf3 dy = dz % dx;
	Pointf3 back = direction * Matrixf3Inverse(Matrixf3(dx, dy, dz));
	azimuth = GetZXBearing(back);
	bearing = asin(back.y);
	double r = Determinant(straight_up, dy, direction);
	rotation = asin(2 * r) * 0.5;
}

bool Camera::TransformClip(Pointf3 a, Pointf3 b, Pointf& outa, Pointf& outb) const
{
	Pointf3 a1 = a * transform_matrix, b1 = b * transform_matrix;
	double az = a1.z + z_delta, bz = b1.z + z_delta;
	bool na = (az <= 0), nb = (bz <= 0);
	bool fa = (az >= 1), fb = (bz >= 1);
	if(na && nb || fa && fb)
		return false;
	/**/ if(na) a1 += (b1 - a1) * (0 - az) / (b1.z - a1.z);
	else if(nb) b1 += (a1 - b1) * (0 - bz) / (a1.z - b1.z);
	/**/ if(fa) a1 += (b1 - a1) * (1 - az) / (b1.z - a1.z);
	else if(fb) b1 += (a1 - b1) * (1 - bz) / (a1.z - b1.z);
	outa.x = a1.x / a1.z;
	outa.y = a1.y / a1.z;
	outb.x = b1.x / b1.z;
	outb.y = b1.y / b1.z;
	return ClipLine(outa, outb, Rectf(-view_px, -view_py, +view_px, +view_py));
}

Pointf3 Camera::InverseXY(Pointf point, double z) const
{
	double dax = point.x * transform_matrix.x.z - transform_matrix.x.x;
	double day = point.x * transform_matrix.y.z - transform_matrix.y.x;
	double dar = z * transform_matrix.z.x + transform_matrix.a.x - point.x * (z * transform_matrix.z.z + transform_matrix.a.z);
	double dbx = point.y * transform_matrix.x.z - transform_matrix.x.y;
	double dby = point.y * transform_matrix.y.z - transform_matrix.y.y;
	double dbr = z * transform_matrix.z.y + transform_matrix.a.y - point.y * (z * transform_matrix.z.z + transform_matrix.a.z);
	double dx = dar * dby - dbr * day;
	double dy = dax * dbr - dar * dbx;
	double dr = dax * dby - dbx * day;
	double lim = fabs(dr) * 1e6;
	if(lim <= fabs(dx) || lim <= fabs(dy))
		return Null;
	return Pointf3(dx / dr, dy / dr, z);
}

}
