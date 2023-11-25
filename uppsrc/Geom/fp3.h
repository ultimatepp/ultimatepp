//////////////////////////////////////////////////////////////////////
// fp3: 3D point class.

struct Pointf3 : Moveable<Pointf3>
{
public:
	Pointf3() {}
	Pointf3(const Nuller&) : x(Null), y(Null), z(Null) {}
	Pointf3(double x, double y, double z) : x(x), y(y), z(z) {}
	Pointf3(Pointf p, double z = 0) : x(p.x), y(p.y), z(z) {}
	explicit Pointf3(double t) : x(t), y(t), z(t) {}

	void              Serialize(Stream& stream)                    { stream % x % y % z; }

	double            operator [] (int i) const                    { return i == 0 ? x : i == 1 ? y : z; }
	double&           operator [] (int i)                          { return i == 0 ? x : i == 1 ? y : z; }

	Pointf            XY() const                                   { return Pointf(x, y); }
	Pointf            YX() const                                   { return Pointf(y, x); }

	Pointf            YZ() const                                   { return Pointf(y, z); }
	Pointf            ZY() const                                   { return Pointf(y, z); }

	Pointf            XZ() const                                   { return Pointf(x, z); }
	Pointf            ZX() const                                   { return Pointf(z, x); }

	Pointf3&          operator += (Pointf3 p)               { x += p.x; y += p.y; z += p.z; return *this; }
	Pointf3&          operator -= (Pointf3 p)               { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pointf3&          operator *= (double val)                     { x *= val; y *= val; z *= val; return *this; }
	Pointf3&          operator *= (Pointf3 p)               { x *= p.x; y *= p.y; z *= p.z; return *this; }
	Pointf3&          operator /= (double val)                     { x /= val; y /= val; z /= val; return *this; }
	Pointf3&          operator /= (Pointf3 p)               { x /= p.x; y /= p.y; z /= p.z; return *this; }

public:
	double            x, y, z;
};

//inline Pointf3        Pointf3_0()                             { return Pointf3(0, 0, 0); }
//inline Pointf3        Pointf3_X()                             { return Pointf3(1, 0, 0); }
//inline Pointf3        Pointf3_Y()                             { return Pointf3(0, 1, 0); }
//inline Pointf3        Pointf3_Z()                             { return Pointf3(0, 0, 1); }
//inline Pointf3        Pointf3_Null()                          { return Pointf3(Null); }

inline bool           IsNull(Pointf3 p)                         { return IsNull(p.x); }
inline Pointf3        Nvl(Pointf3 p, Pointf3 q)                 { return !IsNull(p) ? p : q; }
extern String         AsString(const Pointf3& p);

inline bool           operator == (Pointf3 p, Pointf3 q)        { return q.x == p.x && q.y == p.y && q.z == p.z; }
inline bool           operator != (Pointf3 p, Pointf3 q)        { return !(q == p); }

inline Pointf3        operator - (Pointf3 p)                    { return Pointf3(-p.x, -p.y, -p.z); }
inline Pointf3        operator + (Pointf3 p, Pointf3 q)         { return Pointf3(p.x + q.x, p.y + q.y, p.z + q.z); }
inline Pointf3        operator - (Pointf3 p, Pointf3 q)         { return Pointf3(p.x - q.x, p.y - q.y, p.z - q.z); }
inline Pointf3        operator * (Pointf3 p, double val)        { return Pointf3(p.x * val, p.y * val, p.z * val); }
inline Pointf3        operator * (Pointf3 p, Pointf3 q)         { return Pointf3(p.x * q.x, p.y * q.y, p.z * q.z); }
inline Pointf3        operator * (double val, Pointf3 p)        { return Pointf3(p.x * val, p.y * val, p.z * val); }
inline Pointf3        operator / (Pointf3 p, double val)        { return Pointf3(p.x / val, p.y / val, p.z / val); }
inline Pointf3        operator / (Pointf3 p, Pointf3 q)         { return Pointf3(p.x / q.x, p.y / q.y, p.z / q.z); }

inline double         ScalarProduct(Pointf3 p, Pointf3 q)       { return p.x * q.x + p.y * q.y + p.z * q.z; }
inline Pointf3        VectorProduct(Pointf3 p, Pointf3 q)       { return Pointf3(p.y * q.z - p.z * q.y, p.z * q.x - p.x * q.z, p.x * q.y - p.y * q.x); }

inline double         operator ^ (Pointf3 p, Pointf3 q)         { return p.x * q.x + p.y * q.y + p.z * q.z; }
inline Pointf3        operator % (Pointf3 p, Pointf3 q)         { return Pointf3(p.y * q.z - p.z * q.y, p.z * q.x - p.x * q.z, p.x * q.y - p.y * q.x); }

inline Pointf3        Mid(Pointf3 p, Pointf3 q)                 { return Pointf3((p.x + q.x) / 2, (p.y + q.y) / 2, (p.z + q.z) / 2); }
inline Pointf3        Mid(Pointf3 p, Pointf3 q, double w)       { return p + (q - p) * w; }
inline double         Squared(Pointf3 p)                        { return p.x * p.x + p.y * p.y + p.z * p.z; }
inline double         Length(Pointf3 p)                         { return sqrt(Squared(p)); }
extern Pointf3        Length(Pointf3 p, double l);
inline double         Distance(Pointf3 p, Pointf3 q)            { return Length(p - q); }
inline Pointf3        UnitV(Pointf3 p)                          { return Length(p, 1.0); }
inline Pointf3        Scale(Pointf3 p, double d, Pointf3 c = Pointf(0, 0)) { return (p - c) * d + c; }
inline double         Determinant(Pointf3 a, Pointf3 b, Pointf3 c) { return (a % b) ^ c; }
extern Pointf3        Bezier2(Pointf3 a, Pointf3 b, Pointf3 c, double t);
extern Pointf3        Orthogonal(Pointf3 v, Pointf3 against);
extern Pointf3        Orthonormal(Pointf3 v, Pointf3 against);
extern Pointf3        FarthestAxis(Pointf3 v);
extern Pointf3        AtLine(Pointf3 a, double ta, Pointf3 b, double tb, double tx, double *ratio = NULL);

inline double         GetXYBearing(Pointf3 a)                   { return atan2(a.y, a.x); }
inline double         GetYZBearing(Pointf3 a)                   { return atan2(a.z, a.y); }
inline double         GetZXBearing(Pointf3 a)                   { return atan2(a.x, a.z); }
extern double         Angle(Pointf3 a, Pointf3 b);

inline Pointf3        fpmin(Pointf3 p, Pointf3 q)               { return Pointf3(min(p.x, q.x), min(p.y, q.y), min(p.z, q.z)); }
inline Pointf3        fpmax(Pointf3 p, Pointf3 q)               { return Pointf3(max(p.x, q.x), max(p.y, q.y), max(p.z, q.z)); }

inline double         fpmin(Pointf3 p)                          { return min(min(p.x, p.y), p.z); }
inline double         fpmax(Pointf3 p)                          { return max(max(p.x, p.y), p.z); }
inline double         fpabsmin(Pointf3 p)                       { return min(min(fabs(p.x), fabs(p.y)), fabs(p.z)); }
inline double         fpabsmax(Pointf3 p)                       { return max(max(fabs(p.x), fabs(p.y)), fabs(p.z)); }

inline Pointf3        PolarPointf3XY(double angle)              { return Pointf3(cos(angle), sin(angle), 0.0); }
inline Pointf3        PolarPointf3YZ(double angle)              { return Pointf3(0.0, cos(angle), sin(angle)); }
inline Pointf3        PolarPointf3ZX(double angle)              { return Pointf3(sin(angle), 0.0, cos(angle)); }

extern Pointf3        RotateX(Pointf3 v, double angle, Pointf3 centre = Pointf3(0, 0, 0));
extern Pointf3        RotateY(Pointf3 v, double angle, Pointf3 centre = Pointf3(0, 0, 0));
extern Pointf3        RotateZ(Pointf3 v, double angle, Pointf3 centre = Pointf3(0, 0, 0));
extern Pointf3        Rotate(Pointf3 v, Pointf3 axis, double angle, Pointf3 centre = Pointf3(0, 0, 0));

double LineSegmentsDistance(const Pointf3& P0, const Pointf3& P1, const Pointf3& Q0, const Pointf3& Q1);

class Plane3
{
public:
	Plane3() {}
	Plane3(double delta, Pointf3 normal) : delta(delta), normal(normal) {}
	Plane3(Pointf3 A, Pointf3 normal) : delta(-(normal ^ A)), normal(normal) {}
	Plane3(Pointf3 A, Pointf3 B, Pointf3 C) { normal = (B - A) % (C - A); delta = -(normal ^ A); }

public:
	double      delta;
	Pointf3     normal; // (normal ^ point) + delta = 0
};

String          AsString(const Plane3& plane);

inline double   Distance(Plane3 plane, Pointf3 point)       { return (point ^ plane.normal) + plane.delta; }
extern double   Intersect(Plane3 p, Pointf3 la, Pointf3 lb);

inline Plane3   operator - (Plane3 p)                              { return Plane3(-p.delta, -p.normal); }

inline Plane3&  operator += (Plane3& p, Pointf3 v)                 { p.delta += v ^ p.normal; return p; }
inline Plane3&  operator -= (Plane3& p, Pointf3 v)                 { p.delta -= v ^ p.normal; return p; }

inline Plane3   operator +  (Plane3 p, Pointf3 v)           { Plane3 r = p; r += v; return r; }
inline Plane3   operator -  (Plane3 p, Pointf3 v)           { Plane3 r = p; r -= v; return r; }

extern Plane3   Unit3(Plane3 p);

struct Matrixf3;

const Matrixf3& Matrixf3_0();
const Matrixf3& Matrixf3_1();
const Matrixf3& Matrixf3_Null();
const Matrixf3& Matrixf3_MirrorX(); // mirror X axis
const Matrixf3& Matrixf3_MirrorY(); // mirror Y axis
const Matrixf3& Matrixf3_MirrorZ(); // mirror Z axis

struct Matrixf3 : Moveable<Matrixf3>
{
	Pointf3 x, y, z, a;

	Matrixf3()                                         { *this = Matrixf3_1(); }
	Matrixf3(const Nuller&) : a(Null)                  {}
	Matrixf3(Pointf3 x, Pointf3 y, Pointf3 z, Pointf3 a = Pointf3(0, 0, 0))
		: x(x), y(y), z(z), a(a) {}
	Matrixf3(const Matrixf3& (*fn)())                  { *this = fn(); }
	Matrixf3(double xx, double xy, double xz,
		double yx, double yy, double yz,
		double zx, double zy, double zz,
		double ax = 0, double ay = 0, double az = 0)
		: x(xx, xy, xz), y(yx, yy, yz), z(zx, zy, zz), a(ax, ay, az) {}

	void             Serialize(Stream& stream);

	bool             IsIdentity() const                    { return x.x == 1 && x.y == 0 && x.z == 0 && y.x == 0 && y.y == 1 && y.z == 0 && z.x == 0 && z.y == 0 && z.z == 1 && a.x == 0 && a.y == 0 && a.z == 0; }
	bool             IsZero() const                        { return x.x == 0 && x.y == 0 && x.z == 0 && y.x == 0 && y.y == 0 && y.z == 0 && z.x == 0 && z.y == 0 && z.z == 0 && a.x == 0 && a.y == 0 && a.z == 0; }
	bool             IsNullInstance() const                { return IsNull(a); }

	Matrixf3&        Fix(const Pointf3& v)                 { a = v - v.x * x - v.y * y - v.z * z; return *this; }
	Matrixf3&        operator *= (const Matrixf3& another);

	Pointf3          CX() const                            { return Pointf3(x.x, y.x, z.x); }
	Pointf3          CY() const                            { return Pointf3(x.y, y.y, z.y); }
	Pointf3          CZ() const                            { return Pointf3(x.z, y.z, z.z); }

	String           ToString() const;
};

inline Matrixf3& operator *= (Matrixf3& m, double d) { m.x *= d; m.y *= d; m.z *= d; m.a *= d; return m; }
inline Matrixf3& operator /= (Matrixf3& m, double d) { m.x /= d; m.y /= d; m.z /= d; m.a /= d; return m; }

//template DumpType<Matrixf3>;

extern Matrixf3 Matrixf3Move   (Pointf3 vector);
extern Matrixf3 Matrixf3RotateX(double angle, Pointf3 fix = Pointf3(0, 0, 0));
extern Matrixf3 Matrixf3RotateY(double angle, Pointf3 fix = Pointf3(0, 0, 0));
extern Matrixf3 Matrixf3RotateZ(double angle, Pointf3 fix = Pointf3(0, 0, 0));
extern Matrixf3 Matrixf3Rotate (Pointf3 axis, double angle);
extern Matrixf3 Matrixf3Scale  (double scale, Pointf3 fix = Pointf3(0, 0, 0));
extern Matrixf3 Matrixf3Scale  (Pointf3 scale, Pointf3 fix = Pointf3(0, 0, 0));
//extern Matrixf3  Matrixf3Mirror (Pointf A, Pointf B);
extern Matrixf3  Matrixf3Affine(Pointf3 src1, Pointf3 dest1, Pointf3 src2, Pointf3 dest2);
extern Matrixf3  Matrixf3Affine(Pointf3 src1, Pointf3 dest1, Pointf3 src2, Pointf3 dest2,
	Pointf3 src3, Pointf3 dest3);
extern Matrixf3  Matrixf3Affine(Pointf3 src1, Pointf3 dest1, Pointf3 src2, Pointf3 dest2,
	Pointf3 src3, Pointf3 dest3, Pointf3 src4, Pointf3 dest4);

extern Matrixf3 Matrixf3Inverse(const Matrixf3& mx);
extern double   Determinant    (const Matrixf3& mx);
inline bool     IsReversing    (const Matrixf3& mx)                        { return Determinant(mx) < 0; }
extern double   Matrixf3Measure(const Matrixf3& mx);

inline bool     operator ==   (const Matrixf3& m1, const Matrixf3& m2)     { return m1.x == m2.x && m1.y == m2.y && m1.z == m2.z && m1.a == m2.a; }
inline bool     operator !=   (const Matrixf3& m1, const Matrixf3& m2)     { return !(m1 == m2); }

inline Pointf3  operator %    (Pointf3 vector, const Matrixf3& matrix)     { return vector.x * matrix.x + vector.y * matrix.y + vector.z * matrix.z; }
inline Pointf3  operator *    (Pointf3 point,  const Matrixf3& matrix)     { return point % matrix + matrix.a; }

inline Pointf3& operator %=   (Pointf3& point, const Matrixf3& matrix)     { return point = point % matrix; }
inline Pointf3& operator *=   (Pointf3& point, const Matrixf3& matrix)     { return point = point * matrix; }

inline Matrixf3  operator *   (const Matrixf3& m1, const Matrixf3& m2)     { return Matrixf3(m1) *= m2; }
inline Matrixf3  operator /   (const Matrixf3& m1, const Matrixf3& m2)     { return m1 * Matrixf3Inverse(m2); }

extern Plane3 operator * (Plane3 p, const Matrixf3& m);

const double BIGDOUBLE = 1e300;

class Box3
{
public:
	Box3(const Nuller& = Null) { low[0] = Null; }
	Box3(const Pointf3 *n, const double *l, const double *h)
	{
		normal[0] = n[0]; normal[1] = n[1]; normal[2] = n[2];
		low[0] = l[0]; low[1] = l[1]; low[2] = l[2];
		high[0] = h[0]; high[1] = h[1]; high[2] = h[2];
	}

	Plane3  GetMinPlane(int coord) const { return Plane3(low[coord], normal[coord]); }
	Plane3  GetMaxPlane(int coord) const { return Plane3(-high[coord], -normal[coord]); }

	bool    IsEmpty() const              { return low[0] > high[0] || low[1] > high[1] || low[2] > high[2]; }
	void    Clear()                      { low[0] = low[1] = low[2] = BIGDOUBLE; high[0] = high[1] = high[2] = -BIGDOUBLE; }
	void    Union(Pointf3 pt);

	Pointf3 normal[3];
	double  low[3];
	double  high[3];
};

inline bool IsNull(Box3 b) { return IsNull(b.low[0]); }

class Camera
{
public:
	Camera();

	void     Serialize(Stream& stream);

	Camera&  Location(Pointf3 new_location)             { location = new_location; return *this; }
	Camera&  Target(Pointf3 new_target)                 { target = new_target; return *this; }
	Camera&  Upwards(Pointf3 new_upwards)               { upwards = new_upwards; return *this; }
	Camera&  ViewingAngle(double new_va)                { viewing_angle = new_va; return *this; }
	Camera&  Viewport(double wmm, double hmm)           { width_mm = wmm; height_mm = hmm; return *this; }
	Camera&  Viewport(Sizef mm)                         { width_mm = mm.cx; height_mm = mm.cy; return *this; }
	Camera&  ViewportOffset(double xc, double yc)       { shift_x = xc; shift_y = yc; return *this; }
	Camera&  ViewportSize(Sizef px)                     { view_px = px.cx; view_py = px.cy; return *this; }
	Camera&  ViewportSize(double px, double py)         { view_px = px; view_py = py; return *this; }
	Camera&  Projection(const Matrixf3& pm)             { projection_matrix = pm; return *this; }
	Camera&  FarDistance(double new_fd)                 { far_distance = new_fd; return *this; }
	Camera&  NearDistance(double new_nd)                { near_distance = new_nd; return *this; }

	double   GetDistance(Pointf3 v) const               { return (v ^ direction) + distance_delta; }

	double   GetLengthByDistance(double l, double d)    { return l * near_distance / d; }
	double   GetDistanceByLength(double l, double t)    { return near_distance * l / t; }

	void     Update(); // recalculates camera parameters

	Pointf3  Transform(Pointf3 point) const;
	bool     TransformClip(Pointf3 a, Pointf3 b, Pointf& outa, Pointf& outb) const;
	Pointf3  InverseXY(Pointf point, double z) const;

	void     SetPolar(Pointf3 dz, Pointf3 dx,
		double distance, double azimuth, double bearing, double rotation);
	void     GetPolar(Pointf3 dz, Pointf3 dx,
		double& distance, double& azimuth, double& bearing, double& rotation) const;

public:
	Pointf3  location;
	Pointf3  target;
	Pointf3  upwards;
	double   viewing_angle;
	double   width_mm, height_mm;
	double   view_px, view_py;
	double   shift_x, shift_y;
	double   near_distance;
	double   far_distance;
	Matrixf3 projection_matrix;

	// calculated parameters
	Pointf3  straight_up;
	Pointf3  straight_right;
	Pointf3  direction;
	double   distance_delta;
	Matrixf3 camera_matrix;
	Matrixf3 invcam_matrix;
	Matrixf3 transform_matrix;
	double   z_delta;
	double   viewing_distance;
};
