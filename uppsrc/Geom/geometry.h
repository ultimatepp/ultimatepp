//////////////////////////////////////////////////////////////////////

struct Matrixf;

static const double TWOPI = 2 * M_PI;
static const double DEGRAD = M_PI / 180.0;

//////////////////////////////////////////////////////////////////////

//inline Pointf Pointf_0     ()                                 { return Pointf(0, 0); }
//inline Pointf Pointf_X     ()                                 { return Pointf(1, 0); }
//inline Pointf Pointf_Y     ()                                 { return Pointf(0, 1); }
//inline Pointf Pointf_RX    ()                                 { return Pointf(-1, 1); }
//inline Pointf Pointf_RY    ()                                 { return Pointf(1, -1); }

inline Pointf fpmin        (Pointf p, Pointf q)               { return Pointf(min(p.x, q.x), min(p.y, q.y)); }
inline Pointf fpmax        (Pointf p, Pointf q)               { return Pointf(max(p.x, q.x), max(p.y, q.y)); }
inline Pointf fpminmax     (Pointf p, Pointf mn, Pointf mx)   { return Pointf(minmax(p.x, mn.x, mx.x), minmax(p.y, mn.y, mx.y)); }

inline double fpmin        (Pointf p)                         { return min(p.x, p.y); }
inline double fpmax        (Pointf p)                         { return max(p.x, p.y); }
inline double fpabsmin     (Pointf p)                         { return min(fabs(p.x), fabs(p.y)); }
inline double fpabsmax     (Pointf p)                         { return max(fabs(p.x), fabs(p.y)); }

// temporary fix by Mirek Fidler:
//inline double ScalarProduct(Sizef a, Sizef b) { return a.cx * b.cx + a.cy * b.cy; }
//inline double VectorProduct(Sizef a, Sizef b) { return a.cx * b.cy - a.cy * b.cx; }
//inline double Squared(Sizef a)             { return a.cx * a.cx + a.cy * a.cy; }
//inline double Length(Sizef a)              { return hypot(a.cx, a.cy); }

//inline int    ScalarProduct(Size a, Size b) { return a.cx * b.cx + a.cy * b.cy; }
//inline int    VectorProduct(Size a, Size b) { return a.cx * b.cy - a.cy * b.cx; }
//inline int    Squared(Size a)             { return a.cx * a.cx + a.cy * a.cy; }
//inline double Length(Size a)              { return hypot(a.cx, a.cy); }


inline Pointf Move         (Pointf p, double dx, double dy)   { return Pointf(p.x + dx, p.y + dy); }
inline Pointf Mid          (Pointf p, Pointf q)               { return Pointf((p.x + q.x) / 2, (p.y + q.y) / 2); }
inline Pointf Mid          (Pointf p, Pointf q, double wt)    { return p + (q - p) * wt; }
inline double Squared      (Pointf p)                         { return p.x * p.x + p.y * p.y; }
inline double Squared      (Pointf p, Pointf q)               { return Squared(q - p); }
inline double Length       (Pointf p)                         { return sqrt(Squared(p)); }
Pointf        Length       (Pointf p, double l);
inline Pointf Unit         (Pointf p)                         { return Length(p, 1); }
inline Pointf Rotated      (Pointf p, double a)               { double s = sin(a), c = cos(a); return Pointf(p.x * c - p.y * s, p.x * s + p.y * c); }
inline Pointf Rotated      (Pointf p, double angle, Pointf c) { return c + Rotated(p - c, angle); }
Pointf        Project      (Pointf p, Pointf a, Pointf b);
Pointf        Bezier2      (Pointf a, Pointf b, Pointf c, double t);
double        Bezier2Length(Pointf a, Pointf b, Pointf c, double t);
Sizef         Bezier2Tangent(Pointf a, Pointf b, Pointf c, double t);
Sizef         Orthogonal   (Sizef  p, Sizef against);
Sizef         Orthonormal  (Sizef  p, Sizef against);
Sizef         FarthestAxis (Sizef  p);
inline Pointf Reversed     (Pointf p)                         { return Pointf(-p.x, -p.y); }
inline Pointf ReversedX    (Pointf p)                         { return Pointf(-p.x, p.y); }
inline Pointf ReversedY    (Pointf p)                         { return Pointf(p.x, -p.y); }
//inline Pointf Abs          (Pointf p)                         { return Pointf(fabs(p.x), fabs(p.y)); }
inline Pointf Left         (Pointf p)                         { return Pointf(-p.y, p.x); }
inline Pointf Right        (Pointf p)                         { return Pointf(p.y, -p.x); }

inline double Distance     (Pointf p, Pointf q)               { return Length(p - q); }
double        Bearing      (Pointf p); // 0 to 2 * M_PI
inline double Bearing      (Pointf p, Pointf c)               { return Bearing(p - c); }
inline bool   Select       (Pointf p, Pointf A, Pointf B)     { return Squared(p - A) < Squared(p - B); }

inline double operator ^   (Pointf p, Pointf q)               { return p.x * q.x + p.y * q.y; }
inline double operator %   (Pointf p, Pointf q)               { return p.x * q.y - p.y * q.x; }
inline double operator |   (Pointf p, Pointf q)               { return Distance(p, q); }

inline Pointf PolarPointf  (double a)                         { return Pointf(cos(a), sin(a)); }
inline Pointf PolarPointf  (double r, double a)               { return Pointf(r * cos(a), r * sin(a)); }
inline Pointf PolarPointf  (Pointf c, double r, double a)     { return Pointf(c.x + r * cos(a), c.y + r * sin(a)); }

inline Point  PointfToPoint(Pointf p)                         { return Point(fround(p.x), fround(p.y)); }
inline Size   PointfToSize (Pointf p)                         { return Size(fround(p.x), fround(p.y)); }

//////////////////////////////////////////////////////////////////////

//inline Sizef  Sizef_0      ()                                 { return Sizef(0, 0); }
//inline Sizef  Sizef_X      ()                                 { return Sizef(1, 0); }
//inline Sizef  Sizef_Y      ()                                 { return Sizef(0, 1); }
//inline Sizef  Sizef_RX     ()                                 { return Sizef(-1, 1); }
//inline Sizef  Sizef_RY     ()                                 { return Sizef(1, -1); }

inline Sizef  fpmin        (Sizef p, Sizef q)                 { return Sizef(min(p.cx, q.cx), min(p.cy, q.cy)); }
inline Sizef  fpmax        (Sizef p, Sizef q)                 { return Sizef(max(p.cx, q.cx), max(p.cy, q.cy)); }
inline Sizef  fpminmax     (Sizef p, Sizef mn, Sizef mx)      { return Sizef(minmax(p.cx, mn.cx, mx.cx), minmax(p.cy, mn.cy, mx.cy)); }

inline double fpmin        (Sizef p)                          { return min(p.cx, p.cy); }
inline double fpmax        (Sizef p)                          { return max(p.cx, p.cy); }
inline double AbsMin       (Sizef p)                          { return min(fabs(p.cx), fabs(p.cy)); }
inline double AbsMax       (Sizef p)                          { return max(fabs(p.cx), fabs(p.cy)); }

inline Sizef  Mid          (Sizef p, Sizef q)                 { return Sizef((p.cx + q.cx) / 2, (p.cy + q.cy) / 2); }
//inline double Squared      (Sizef p)                          { return p.cx * p.cx + p.cy * p.cy; }
inline double Squared      (Sizef p, Sizef q)                 { return Squared(q - p); }
//inline double Length       (Sizef p)                          { return sqrt(Squared(p)); }
Sizef         Length       (Sizef p, double l);
inline Sizef  Unit         (Sizef p)                          { return Length(p, 1); }
inline Sizef  Reversed     (Sizef s)                          { return Sizef(-s.cx, -s.cy); }
inline Sizef  ReversedX    (Sizef s)                          { return Sizef(-s.cx, s.cy); }
inline Sizef  ReversedY    (Sizef s)                          { return Sizef(s.cx, -s.cy); }
inline Sizef  Abs          (Sizef s)                          { return Sizef(fabs(s.cx), fabs(s.cy)); }
inline Sizef  Left         (Sizef s)                          { return Sizef(-s.cy, s.cx); }
inline Sizef  Right        (Sizef s)                          { return Sizef(s.cy, -s.cx); }

inline Point  SizefToPoint (Sizef s)                          { return Point(fround(s.cx), fround(s.cy)); }
inline Size   SizefToSize  (Sizef s)                          { return Size(fround(s.cx), fround(s.cy)); }

//////////////////////////////////////////////////////////////////////
// Rectf::

//inline Rectf  Rectf_0()                                       { return Rectf(0, 0, 0, 0); }
//inline Rectf  Rectf_1()                                       { return Rectf(0, 0, 1, 1); }

//////////////////////////////////////////////////////////////////////

inline Rectf  operator *   (Rectf r, double f)                { return Rectf(r.left * f, r.top * f, r.right * f, r.bottom * f); }
inline Rectf  operator *   (Rectf r, Sizef s)                 { return Rectf(r.left * s.cx, r.top * s.cy, r.right * s.cx, r.bottom * s.cy); }
inline Rectf  operator *   (Rectf r, Pointf p)                { return Rectf(r.left * p.x, r.top * p.y, r.right * p.x, r.bottom * p.y); }
inline Rectf  operator *   (Rectf r, Rectf s)                 { return Rectf(r.left * s.left, r.top * s.top, r.right * s.right, r.bottom * s.bottom); }

inline Rectf  operator /   (Rectf r, double f)                { return Rectf(r.left / f, r.top / f, r.right / f, r.bottom / f); }
inline Rectf  operator /   (Rectf r, Sizef s)                 { return Rectf(r.left / s.cx, r.top / s.cy, r.right / s.cx, r.bottom / s.cy); }
inline Rectf  operator /   (Rectf r, Pointf p)                { return Rectf(r.left / p.x, r.top / p.y, r.right / p.x, r.bottom / p.y); }
inline Rectf  operator /   (Rectf r, Rectf s)                 { return Rectf(r.left / s.left, r.top / s.top, r.right / s.right, r.bottom / s.bottom); }

extern Rectf& SetUnion     (Rectf& rc, Pointf pt);
inline Rectf  GetUnion     (Rectf rc, Pointf pt)              { return SetUnion(rc, pt); }
extern Rectf  GetUnion     (const Array<Pointf>& pt);
extern Rectf& SetMinMaxMove(Rectf& rc, Rectf outer_rc);
inline Rectf  GetMinMaxMove(Rectf rc, Rectf outer_rc)         { return SetMinMaxMove(rc, outer_rc); }
extern double Distance     (Rectf rc, Pointf pt);
inline double Diagonal     (Rectf rc)                         { return Length(rc.Size()); }
inline double Area         (Rectf rc)                         { return rc.Width() * rc.Height(); }
inline Rectf  PointfRectf  (Pointf pt)                        { return Rectf(pt.x, pt.y, pt.x, pt.y); }
inline Rectf  SortRectf    (Pointf p, Pointf q)               { return Rectf(fpmin(p, q), fpmax(p, q)); }
inline Rect   RectfToRect  (Rectf rc)                         { return Rect(fround(rc.left), fround(rc.top), fround(rc.right), fround(rc.bottom)); }

inline Pointf fpminmax     (Pointf p, Rectf rc)               { return Pointf(minmax(p.x, rc.left, rc.right), minmax(p.y, rc.top, rc.bottom)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const Matrixf& Matrixf_0();
const Matrixf& Matrixf_1();
const Matrixf& Matrixf_Null();
const Matrixf& Matrixf_MirrorX(); // mirror around X axis
const Matrixf& Matrixf_MirrorY(); // mirror around Y axis

//////////////////////////////////////////////////////////////////////

struct Matrixf : Moveable<Matrixf>
{
	Pointf x, y, a;

	Matrixf()                                         { *this = Matrixf_1(); }
	Matrixf(const Nuller&) : a(Null)                  {}
	Matrixf(Pointf x, Pointf y, Pointf a = Pointf(0, 0)) : x(x), y(y), a(a) {}
	Matrixf(const Matrixf& (*fn)())                   { *this = fn(); }
	Matrixf(double xx, double xy, double yx, double yy, double ax, double ay)
		: x(xx, xy), y(yx, yy), a(ax, ay) {}

	bool             IsIdentity() const                    { return x.x == 1 && x.y == 0 && y.x == 0 && y.y == 1 && a.x == 0 && a.y == 0; }
	bool             IsZero() const                        { return x.x == 0 && x.y == 0 && y.x == 0 && y.y == 0 && a.x == 0 && a.y == 0; }

	void             Fix(Pointf vector)                    { a = vector - vector.x * x - vector.y * y; }
	Matrixf&         operator *= (const Matrixf& another);

	String           ToString() const;
};

//template DumpType<Matrixf>;

extern Matrixf  MatrixfMove   (Pointf vector);
extern Matrixf  MatrixfRotate (double angle, Pointf fix = Pointf(0, 0));
extern Matrixf  MatrixfScale  (double scale, Pointf fix = Pointf(0, 0));
extern Matrixf  MatrixfScale  (Pointf scale, Pointf fix = Pointf(0, 0));
extern Matrixf  MatrixfScale  (Rectf src, Rectf dest);
extern Matrixf  MatrixfMirror (Pointf A, Pointf B);
extern Matrixf  MatrixfAffine (Pointf src1, Pointf dest1, Pointf src2, Pointf dest2);
extern Matrixf  MatrixfAffine (Pointf src1, Pointf dest1, Pointf src2, Pointf dest2, Pointf src3, Pointf dest3);

extern Matrixf  MatrixfInverse(const Matrixf& mx);
inline double   Determinant   (const Matrixf& mx)                         { return mx.x % mx.y; }
inline bool     IsReversing   (const Matrixf& mx)                         { return Determinant(mx) < 0; }
extern double   MatrixfMeasure(const Matrixf& mx);

template<>
inline bool     IsNull        (const Matrixf& mx)                         { return IsNull(mx.a); }
inline bool     operator ==   (const Matrixf& m1, const Matrixf& m2)      { return m1.x == m2.x && m1.y == m2.y && m1.a == m2.a; }
inline bool     operator !=   (const Matrixf& m1, const Matrixf& m2)      { return !(m1 == m2); }

inline Pointf   operator %    (Pointf vector, const Matrixf& matrix)      { return vector.x * matrix.x + vector.y * matrix.y; }
extern Pointf   operator *    (Pointf point,  const Matrixf& matrix);
extern Pointf   operator /    (Pointf point,  const Matrixf& matrix);
extern Rectf    operator *    (const Rectf& rect, const Matrixf& matrix);
extern Rectf    operator /    (const Rectf& rect, const Matrixf& matrix);

inline Pointf&  operator %=   (Pointf& point, const Matrixf& matrix)      { return point = point % matrix; }
inline Pointf&  operator *=   (Pointf& point, const Matrixf& matrix)      { return point = point * matrix; }

inline Matrixf  operator *    (const Matrixf& m1, const Matrixf& m2)      { return Matrixf(m1) *= m2; }
inline Matrixf  operator /    (const Matrixf& m1, const Matrixf& m2)      { return m1 * MatrixfInverse(m2); }

//////////////////////////////////////////////////////////////////////
// set distance of point from an object

double          Distance      (Pointf X, Pointf A, Pointf B, double *arg = NULL);
double          Distance      (Pointf X, Pointf A, Pointf B, double bulge, double *arg = NULL);
double          Distance      (Pointf X, Pointf C, double radius, double *arg = NULL);

//////////////////////////////////////////////////////////////////////
// set intersection with rectangle

bool            Crosses       (Rectf R, Pointf A, Pointf B);
bool            Crosses       (Rectf R, Pointf A, Pointf B, double bulge);
bool            Crosses       (Rectf R, Pointf C, double radius);

//////////////////////////////////////////////////////////////////////
// set intersection with polygon

enum { CMP_OUT = -1, CMP_SECT = 0, CMP_IN = +1 };

int             ContainsPoints(const Array<Pointf>& polygon, const Array<Pointf>& points);
int             ContainsPoints(const Array<Pointf>& polygon, const Vector<int>& polyend, const Array<Pointf>& points);
int             ContainsPoint (const Array<Pointf>& polygon, Pointf pt);
int             ContainsPoint (const Array<Pointf>& polygon, const Vector<int>& polyend, Pointf pt);
int             ContainsPoly  (const Array<Pointf>& chkpoly, const Array<Pointf>& polygon, const Vector<int>& polyend, bool closed);
int             ContainsPoly  (const Array<Pointf>& chkpoly, const Array<Pointf>& polygon, bool closed);

//////////////////////////////////////////////////////////////////////

bool            ClipLine      (Pointf& A, Pointf& B, Rectf box);
bool            ClipLine      (Pointf& A, Pointf& B, Rect box);
bool            ClipLine      (Point& A, Point& B, Rect box);

//////////////////////////////////////////////////////////////////////

Rectf           GetBoundingBox(const Array<Pointf>& vertices);
Rect            GetBoundingBox(const Point *vertices, int vertex_count);
Rect            GetBoundingBox(const Vector<Point>& vertices);

void            SplitPolygon  (const Point *vertices, int vertex_count, const int *counts, int count_count,
	Vector<Point>& out_vertices, Vector<int>& out_counts, Rect clip = Null, int max_trace_points = 8000);
void            SplitPolygon  (const Vector<Point>& vertices, const Vector<int>& counts,
	Vector<Point>& out_vertices, Vector<int>& out_counts, Rect clip = Null, int max_trace_points = 8000);
void            SplitPolygon  (Array<Pointf>::ConstIterator vertices, int vertex_count, const int *counts, int count_count,
	Array<Pointf>& out_vertices, Vector<int>& out_counts, Rectf clip = Null, int max_trace_points = 8000);
void            SplitPolygon  (const Array<Pointf>& vertices, const Vector<int>& counts,
	Array<Pointf>& out_vertices, Vector<int>& out_counts, Rectf clip = Null, int max_trace_points = 8000);

//////////////////////////////////////////////////////////////////////

extern double   Vec_outer_tolerance; // maximum reasonable coordinate value
extern double   Vec_tolerance;       // linear tolerance
extern double   Vec_ang_tolerance;   // angular tolerance

extern bool     VecTolEq      (double x, double y);
extern bool     VecTolEq      (Pointf a, Pointf b);

//////////////////////////////////////////////////////////////////////

class VecLine
{
public:
	VecLine();
	VecLine(Pointf A, Pointf B) : A(A), B(B) {}
	VecLine(double x1, double y1, double x2, double y2) : A(x1, y1), B(x2, y2) {}

	String        ToString() const;

	VecLine&      SetNull();

	double        Length() const { return A | B; }
	Pointf        Mid()    const;
	Pointf        Right()  const;
	Pointf        Left()   const;
	Pointf        Vector() const;

	double        GetArg(Pointf pt) const;
	Pointf        GetPointAt(double arg) const;

	VecLine       Reversed() const;
	VecLine&      SetReversed();

	Pointf        Intersect(VecLine another) const;
	VecLine&      SetClip(Rectf rect);
	VecLine       Clip(Rectf rect) const { return VecLine(*this).Clip(rect); }

	double        Distance(Pointf point, double *arg = NULL) const;

	bool          IsNullInstance() const                   { return IsNull(A); }

	// algorithms
	static Pointf GetPointAt(Pointf A, Pointf B, double arg);

public:
	Pointf        A, B;
};

//template BitWiseType<VecLine>;
//template DumpType<VecLine>;

inline Pointf  operator & (VecLine l1, VecLine l2)        { return l1.Intersect(l2); }
inline VecLine operator & (VecLine line, Rectf rect)      { return line.Clip(rect); }
inline double  operator | (VecLine ln, Pointf pt)         { return ln.Distance(pt); }
inline double  operator | (Pointf pt, VecLine ln)         { return ln.Distance(pt); }

//////////////////////////////////////////////////////////////////////

class VecArc : public VecLine
{
public:
	VecArc();
	VecArc(VecLine line, double bulge = 0) : VecLine(line), bulge(bulge) {}
	VecArc(Pointf A, Pointf B, double bulge = 0) : VecLine(A, B), bulge(bulge) {}
	VecArc(double x1, double y1, double x2, double y2, double bulge = 0)
		: VecLine(x1, y1, x2, y2), bulge(bulge) {}

	String        ToString() const;

	Pointf        ArcMid() const;
	double        ArcLength() const;

	VecArc        Reversed() const;
	VecArc&       SetReversed();

	Pointf        GetPointAt(double t) const;
	Rectf         GetBoundingBox() const;

	// arc algorithms
	static Pointf ArcMid(Pointf P, Pointf Q, double l, double h);
	static double ArcLength(Pointf P, Pointf Q, double l, double h);
	static void   Bisect(Pointf P, Pointf Q, double l, double h,
		Pointf& centre, double& ll, double& hh);
	static Pointf GetPointAt(Pointf P, Pointf Q, double l, double h, double t);

public:
	double        bulge;
};

//template BitWiseType<VecArc>;
//template DumpType<VecArc>;

//////////////////////////////////////////////////////////////////////

/*
struct VecVertex : public Pointf
{
	VecVertex();
	VecVertex(Pointf point, double bulge = 0) : Pointf(point), bulge(bulge) {}
	VecVertex(double x, double y, double bulge = 0) : Pointf(x, y), bulge(bulge) {}

	String Dump() const;

	double bulge; // bulge of segment __ENDING AT THIS VERTEX__
};

template BitWiseType<VecVertex>;
template DumpType<VecVertex>;
*/

//////////////////////////////////////////////////////////////////////

/*
class VecCurve : public Vector<VecVertex>
{
public:
	typedef Vector<VecVertex> Base;

	VecCurve();
	VecCurve(const VecCurve& another, int);

	void       Add(const VecVertex& vertex);
	void       Add(Pointf point); // can be used as lineto callback for Arc::Iterator
	void       Add(Pointf point, double bulge);
	void       Add(double x, double y, double bulge = 0);

	VecArc     Segment(int i) const;

	Vector<Pointf> Check() const; // find self-intersections; empty return value = OK

	String     Dump() const;
};

template class DeepCopyOption<VecCurve>;
template DumpType<VecCurve>;
*/

//////////////////////////////////////////////////////////////////////

#ifdef p
#undef p
#endif

class VecArcInfo : public VecArc
{
public:
	VecArcInfo();
	VecArcInfo(Pointf p, Pointf q)                                  { Set(p, q); }
	VecArcInfo(Pointf p, Pointf q, Pointf x)                        { Set(p, q, x); }
	VecArcInfo(Pointf p, Pointf q, double bulge)                    { Set(p, q, bulge); }
	VecArcInfo(Pointf c, double r, double a, double b)              { Set(c, r, a, b); }
	VecArcInfo(Pointf c, double r)                                  { Set(c, r); }
	VecArcInfo(Pointf c, Pointf a, Pointf b, bool anticlockwise)    { Set(c, a, b, anticlockwise); }
	VecArcInfo(const VecArc& arc)                                   { Set(arc); }
	VecArcInfo(const VecArcInfo& arc)                               { *this = arc; }

	String      ToString() const;

	void        Set(Pointf P, Pointf Q);
	void        Set(Pointf P, Pointf Q, Pointf X);
	void        Set(Pointf P, Pointf Q, double bulge);
	void        Set(Pointf C, double r, double a, double b);
	void        Set(Pointf C, double r);
	void        Set(Pointf C, Pointf A, Pointf B, bool anticlockwise);

	void        Set(const VecArc& arc)                              { Set(arc.A, arc.B, arc.bulge); }

	void        Move(Pointf offset);

	bool        IsCircle() const   { return circle; }
	bool        IsCurved() const   { return curved; }
	bool        IsReversed() const { return reversed; }

	double      Length() const;
	Pointf      CentreOfMass() const;

	bool        ContainsBearing(double bearing) const;
	double      Distance(Pointf point, double *arg = NULL) const;
	bool        Crosses(Rectf rect) const;
	Pointf      GetPointAt(double t) const;
	double      GetMaxDistance(Pointf point, Pointf *farthest = NULL) const;

	double      GetArg(double bearing) const;
	double      GetArg(Pointf point) const;
	VecArcInfo  Subset(double start, double end) const;

	Rectf       GetBoundingBox() const;

	double      GetAngle() const; // oriented included angle
	double      GetStartTangent() const;
	double      GetEndTangent() const;
	Pointf      GetStartDir() const;
	Pointf      GetEndDir() const;

	VecArcInfo  Offset(double dist) const;

	VecArcInfo  Reversed() const                                    { return VecArcInfo(*this).SetReversed(); }
	VecArcInfo& SetReversed();

	Pointf      C;        // centre of circle (Null when not bulged)
	double      bow;      // bow length
	double      alpha;    // start angle (rad); most clockwise point on arc
	double      beta;     // end angle (rad); most anticlockwise point on arc
	double      radius;   // circle radius (Null when not bulged)
	bool        curved;   // true = it's an arc, false = it's a circle
	bool        reversed; // true = arc has been entered as clockwise
	bool        circle;   // true = it's a full circle
};

//////////////////////////////////////////////////////////////////////

class VecIntersection
{
public:
	VecIntersection() { count = 0; }

	void   Mirror();
	bool   Nothing() { count = 0; return false; }
	void   Remove(int index);

	bool   LL(Pointf P1, Pointf Q1, Pointf P2, Pointf Q2);
	bool   LC(Pointf P1, Pointf Q1, Pointf C2, double r2);
	bool   LA(Pointf P1, Pointf Q1, const VecArcInfo& a2);
	bool   CC(Pointf C1, double r1, Pointf C2, double r2);
	bool   CA(Pointf C1, double r1, const VecArcInfo& a2);
	bool   AA(const VecArcInfo& a1, const VecArcInfo& a2);

	bool   IsEmpty() const  { return count == 0; }
	int    GetCount() const { return count; }

public:
	int    count; // number of intersections
	double t[2], u[2];

protected:
	void   CheckBearing(double *hints, const VecArcInfo& a);
};

//////////////////////////////////////////////////////////////////////

class VecArcIterator
{
public:
	typedef Callback1<Pointf>     DrawProc;
	typedef Gate2<Pointf, double> ArcProc;

	VecArcIterator(const VecArc& arc, DrawProc _lineto);
	VecArcIterator(Pointf start, Pointf end, double bulge, DrawProc _lineto);

	VecArcIterator& Level(int _level)              { level = _level; return *this; }
	VecArcIterator& Precision(double prec  )       { precision = prec; return *this; }
	VecArcIterator& Clip(Rectf rect, DrawProc _mv) { clip = rect; moveto = _mv; return *this; }
	VecArcIterator& ArcTo(ArcProc _arc)            { arcto = _arc; return *this; }

	void            Go(); // recurse the arc

public:
	double          arclen;    // degenerate to simple arc under such length
	int             level;     // maximum # segments = 2 ^ level
	double          precision; // degenerate to line whenever |bulge| <= precision
	VecArc          arc;       // arc to interpolate
	Rectf           clip;      // clipping rectangle
	DrawProc        moveto;    // moveto callback (needed only when clipping is on)
	DrawProc        lineto;    // lineto callback
	ArcProc         arcto;     // simple arcto callback

protected:
	enum
	{
		CF_XL = 001,
		CF_XG = 002,
		CF_YL = 010,
		CF_YG = 020,

		DEFAULT_LEVEL = 10,
	};

	Pointf          last;      // last iteration point
	int             last_clip; // clipping status of last vertex

	int             GetClip(Pointf point) const;
	void            Recurse(Pointf to, double l, double h, int depth, int next_flclip);
};

//////////////////////////////////////////////////////////////////////
