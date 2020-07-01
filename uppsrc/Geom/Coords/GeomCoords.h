#ifndef _Geom_Coords_Coords_h_
#define _Geom_Coords_Coords_h_

#include <Geom/Geom.h>
//#include <TCore/TCore.h>

namespace Upp {

#define MINRAD (DEGRAD / 60.0)
#define SECRAD (DEGRAD / 3600.0)
#define ANGDEG(d, m, s) ((d) + (m) / 60.0 + (s) / 3600.0)
#define ANGRAD(d, m, s) (DEGRAD * ANGDEG((d), (m), (s)))

double DegreeStep(double min_degrees);
int    GisLengthDecimals(double pixel_len);
int    DegreeDecimals(double pixel_angle);
String FormatDegree(double degrees, int decimals, bool spaces = true);
Value  ScanDegree(const char *p);
int    DegreeMask(double start_angle, double end_angle);
enum { AM_E0 = 1, AM_E1 = 2, AM_E2 = 4, AM_E3 = 8, AM_Q0 = 16, AM_Q1 = 32, AM_Q2 = 64, AM_Q3 = 128 };
Rectf  DegreeToExtent(const Rectf& eps_rho);
inline Rectf DegreeToExtent(double minphi, double minrho, double maxphi, double maxrho) { return DegreeToExtent(Rectf(minphi, minrho, maxphi, maxrho)); }
Rectf  ExtentToDegree(const Rectf& xy);

inline double SafeArcCos(double d) { return d <= -1 ? M_PI : d >= 1 ? 0 : acos(d); }
inline double SafeArcCos(double d, bool turn) { return turn ? -SafeArcCos(d) : SafeArcCos(d); }
inline double SafeArcSin(double d) { return d <= -1 ? -M_PI / 2 : d >= 1 ? +M_PI / 2 : asin(d); }

inline double Determinant(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3)
{
	return a1 * (b2 * c3 - b3 * c2) + a2 * (b3 * c1 - b1 * c3) + a3 * (b1 * c2 - b2 * c1);
}

class GeomRefBase
{
public:
	GeomRefBase()
	{
		refcount = 0;
#ifdef REF_DEBUG
		allocindex = ++nextindex;
#endif//REF_DEBUG
	}

	GeomRefBase(const GeomRefBase& rb)
	{
		refcount = 0;
#ifdef REF_DEBUG
		allocindex = ++nextindex;
#endif//REF_DEBUG
	}

	virtual ~GeomRefBase()
	{
		ASSERT(refcount == 0);
	}

	void           AddRef() const      { AtomicInc(refcount); }
	int            GetRefCount() const { return refcount; }
	void           Release() const     { if(!AtomicDec(refcount)) delete this; }
#ifdef REF_DEBUG
	int            GetAllocIndex() const { return allocindex; }
#endif//REF_DEBUG

private:
	mutable Atomic refcount;
#ifdef REF_DEBUG
	int            allocindex;
	static int     nextindex;
#endif//REF_DEBUG

private:
	GeomRefBase&    operator = (const GeomRefBase& rb) { NEVER(); return *this; }
};

template <class T>
class GeomRefCon : Moveable< GeomRefCon<T> >
{
public:
	GeomRefCon(const Nuller& = Null) : t(0) {}
	GeomRefCon(const T *t);
	GeomRefCon(const GeomRefCon<T>& rp);
	~GeomRefCon();

	void            Clear()                                { if(t) { t->Release(); t = NULL; } }
	bool            IsNullInstance() const                 { return !t; }
	dword           GetHashValue() const                   { return UPP::GetHashValue((unsigned)(uintptr_t)t); }

	GeomRefCon<T>&      operator = (const GeomRefCon<T>& rp);

	bool            operator ! () const                    { return !t; }
	const T        *Get() const                            { return t; }
	const T        *operator ~ () const                    { return t; }
	const T        *operator -> () const                   { ASSERT(t); return t; }
	const T&        operator * () const                    { ASSERT(t); return *t; }

	String          ToString() const                       { return t ? AsString(*t) : String("NULL"); }

	friend bool     operator == (GeomRefCon<T> a, GeomRefCon<T> b) { return a.t == b.t; }
	friend bool     operator != (GeomRefCon<T> a, GeomRefCon<T> b) { return a.t != b.t; }

protected:
	const T        *t;
};

template <class T>
GeomRefCon<T>::GeomRefCon(const T *t)
: t(t)
{
	t->AddRef();
#ifdef REF_DEBUG
	if(t && t->GetRefCount() == 1)
		RefMemStat::App().Add(typeid(*t).name(), t->GetAllocIndex());
#endif//REF_DEBUG
}

template <class T>
GeomRefCon<T>::GeomRefCon(const GeomRefCon<T>& rp)
: t(rp.t)
{ t->AddRef(); }

template <class T>
GeomRefCon<T>::~GeomRefCon()
{
#ifdef REF_DEBUG
	if(t && t->GetRefCount() == 1)
		RefMemStat::App().Remove(typeid(*t).name(), t->GetAllocIndex());
#endif//REF_DEBUG
	if(t)
		t->Release();
}

template <class T>
GeomRefCon<T>& GeomRefCon<T>::operator = (const GeomRefCon<T>& rp)
{
	const T *old = t;
	t = rp.t;
	t->AddRef();
#ifdef REF_DEBUG
	if(old && old->GetRefCount() == 1)
		RefMemStat::App().Remove(typeid(*old).name(), old->GetAllocIndex());
#endif//REF_DEBUG
	if(old)
		old->Release();
	return *this;
}

class GisBSPTree
{
public:
	struct Split;

	struct Node
	{
		Node() {}
		Node(int branch) : branch(branch) {}
		Node(One<Split> split) : branch(-1), split(pick(split)) {}
		Node(Node&&) = default;

		int        branch;
		One<Split> split;
	};

	struct Split
	{
		Split(Pointf n, double c, Node&& minus, Node&& plus)
			: n(n), c(c), minus(pick(minus)), plus(pick(plus)) {}

		Pointf     n;
		double     c; // np = c
		Node       minus; // np - c < 0
		Node       plus; // np - c > 0
	};

	struct Tree : GeomRefBase
	{
		Tree(Node pick_ root) : root(pick(root)) {}

		Node root;
	};

public:
	GisBSPTree() {}
	GisBSPTree(int branch) : tree(new Tree(Node(branch))) {}
	GisBSPTree(Node pick_ root) : tree(new Tree(pick(root))) {}

	bool         IsEmpty() const   { return !tree->root.split; }
	int          GetBranch() const { return tree->root.branch; }
	const Node&  GetRoot() const   { return tree->root; }

private:
	GeomRefCon<Tree> tree;
};

template <class T>
class GeomRefPtr : public GeomRefCon<T>, public Moveable< GeomRefPtr<T> >
{
public:
	GeomRefPtr(const Nuller& = Null)                 {}
	GeomRefPtr(T *t) : GeomRefCon<T>(t)                  {}
	GeomRefPtr(const GeomRefPtr<T>& rp) : GeomRefCon<T>(rp)  {}

	GeomRefPtr<T>&  operator = (const GeomRefPtr<T>& rp) { GeomRefCon<T>::operator = (rp); return *this; }

	T          *Get() const                      { return const_cast<T *>(this->t); }
	T          *operator ~ () const              { return Get(); }
	T          *operator -> () const             { ASSERT(this->t); return Get(); }
	T&          operator * () const              { ASSERT(this->t); return *Get(); }
};

class ConvertDegree : public Convert
{
public:
	ConvertDegree(int decimals = 3, bool not_null = true, double min = -360, double max = +360);
	virtual ~ConvertDegree();

	virtual Value    Format(const Value& v) const;
	virtual Value    Scan(const Value& v) const;
	virtual int      Filter(int c) const;

	ConvertDegree&   Decimals(int d)                 { decimals = d; return *this; }
	int              GetDecimals() const             { return decimals; }

	ConvertDegree&   NotNull(bool nn = true)         { not_null = nn; return *this; }
	bool             IsNotNull() const               { return not_null; }

	ConvertDegree&   MinMax(double l, double h)      { min = l; max = h; return *this; }
	double           GetMin() const                  { return min; }
	double           GetMax() const                  { return max; }

private:
	int              decimals;
	bool             not_null;
	double           min, max;
};

class GisFunction
{
public:
	virtual ~GisFunction() {}
	virtual double Get(double x) const = 0;
	double         operator () (double x) const { return Get(x); }
	void           Dump(double xmin, double xmax, int steps) const;
};

class GisInverse : public GisFunction
{
public:
	GisInverse(double xmin, double xmax, const GisFunction& fn, int sections, double epsilon);

	double              GetXMin() const { return xmin; }
	double              GetXMax() const { return xmin; }
	double              GetYMin() const { return ymin; }
	double              GetYMax() const { return ymax; }

	virtual double      Get(double y) const;

private:
	double              xmin, xmax;
	double              ymin, ymax;
	double              rawxmin, rawxmax;
	double              rawymin, rawymax;
	double              xstep, ystep;
	double              epsilon;
	const GisFunction&  fn;
	Vector<double>      ymap;
	VectorMap<int, int> accel;
};

String GisInverseDelta(double xmin, double xmax, const GisFunction& fn, int sections, double epsilon, int samples);
String GisInverseTiming(double xmin, double xmax, const GisFunction& fn, int sections, double epsilon);

class GisInterpolator
{
public:
	GisInterpolator() {}
//	Interpolator(Callback2<double, double&> calc, double xmin, double xmax, int min_depth, int max_depth, double epsilon)
//	{ Create(calc, xmin, xmax, min_depth, max_depth, epsilon); }
	GisInterpolator(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples) { Create(xmin, xmax, fn, buckets, sections, samples); }

	void           Clear()                      { bucket_index.Clear(); abc.Clear(); }
	bool           IsEmpty() const              { return bucket_index.IsEmpty(); }

//	void           Create(Callback2<double, double&> calc, double xmin, double xmax, int min_depth, int max_depth, double epsilon);
	void           Create(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples);
	void           CreateInverse(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples);
	String         CreateDump(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples, int check);
	String         CreateInverseDump(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples, int check);
	String         Dump(const GisFunction& fn, int check);

	double         GetXMin() const              { return xmin; }
	double         GetXMax() const              { return xmax; }

	double         Get(double x) const;
	double         operator () (double x) const { return Get(x); }
//	double         Linear(double x) const;

private:
/*
	struct Tree
	{
		double    mid_y;
		One<Tree> left;
		One<Tree> right;
	};

	One<Tree> CreateTree(Callback2<double, double&> calc,
		const Rectf& extent, const Pointf& mid,
		int depth, int min_depth, int max_depth, double epsilon);
*/

private:
	struct Bucket : Moveable<Bucket>
	{
		int    begin, count;
		double a0, a1, a2;
	};

	double         xmin, xmax;
	double         step;
	int            limit;
/*
	Vector<short>  index;
	Vector<short>  begin;
	Vector<short>  len;
	Vector<double> abc;
*/
//	Vector<Bucket> bucket_list;
	Vector<unsigned> bucket_index;
	Vector<double> abc;
//	Rectf     extent;
//	double         divisor;
//	Vector<double> y;
//	int       scale;
//	One<Tree> tree;
};

class GisCoords;
class GisTransform;

class Gis2DPolynome {
public:
	Gis2DPolynome() {}

	void   Calculate(const GisTransform& transform, const Rectf& src_extent);

	Pointf Transform(double x, double y) const;
	Pointf Transform(const Pointf& pt) const { return Transform(pt.x, pt.y); }

	enum COEF {
		COEF_1,
		COEF_X,
		COEF_Y,
		COEF_X2,
		COEF_XY,
		COEF_Y2,
		COEF_X3,
		COEF_X2Y,
		COEF_XY2,
		COEF_Y3,
		COEF_COUNT
	};
	Sizef coef[COEF_COUNT];
};

inline Pointf operator * (const Pointf& xy, const Gis2DPolynome& poly) { return poly.Transform(xy); }

class Gis2DGrid : public Pte<Gis2DGrid> {
public:
	Gis2DGrid(const Sizef& block_size = Sizef(1, 1), const Rect& block_limit = Rect(-180, -90, +180, +90));

	void   Grow(const GisTransform& transform, const Rectf& extent);

	Point  GetBlockIndex(const Pointf& point) const;
	Rect   GetBlockSpan(const Rectf& rc) const;
	Pointf Transform(const Pointf& pt) const;

	const Gis2DPolynome *GetBlock(int x, int y) const;
	const Gis2DPolynome *GetBlock(Point pt) const { return GetBlock(pt.x, pt.y); }

	int    SizeOf() const;

public:
	BiVector< BiArray<Gis2DPolynome> > block_rows;
	Sizef block_size;
	Rect  block_span;
	Rect  block_limit;
};

inline Pointf operator * (const Pointf& pt, const Gis2DGrid& poly) { return poly.Transform(pt); }

String GisInterpolatorDelta(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples, int check);
String GisInterpolatorTiming(double xmin, double xmax, const GisFunction& fn, int buckets, int sections, int samples, int check);

class GisOrientation
{
public:
	GisOrientation(Pointf lonlat_pole = Pointf(0, 90));

	Pointf       Local(double lon, double lat) const                       { return (this->*localproc)(lon, lat); }
	Pointf       Local(Pointf lonlat) const                                { return (this->*localproc)(lonlat.x, lonlat.y); }

	Rectf        LocalExtent(const Rectf& lonlat) const                    { return (this->*localextent)(lonlat); }
	Rectf        LocalExtent(double l, double t, double r, double b) const { return (this->*localextent)(Rectf(l, t, r, b)); }

	Pointf       Global(double lon, double lat) const                      { return (this->*globalproc)(lon, lat); }
	Pointf       Global(Pointf lonlat) const                               { return (this->*globalproc)(lonlat.x, lonlat.y); }

	Rectf        GlobalExtent(const Rectf& lonlat) const                   { return (this->*globalextent)(lonlat); }
	Rectf        GlobalExtent(double l, double t, double r, double b) const { return (this->*globalextent)(Rectf(l, t, r, b)); }

	bool         IsIdentity() const        { return identity; }

private:
	Pointf       LocalAny(double lon, double lat) const;
	Pointf       GlobalAny(double lon, double lat) const;
	Pointf       LocalDelta(double lon, double lat) const;
	Pointf       GlobalDelta(double lon, double lat) const;
	Pointf       Identity(double lon, double lat) const;

	Rectf        LocalAnyExtent(const Rectf& lonlat) const;
	Rectf        GlobalAnyExtent(const Rectf& lonlat) const;
	Rectf        LocalDeltaExtent(const Rectf& lonlat) const;
	Rectf        GlobalDeltaExtent(const Rectf& lonlat) const;
	Rectf        IdentityExtent(const Rectf& lonlat) const;

private:
	Pointf       (GisOrientation::*localproc)(double lon, double lat) const;
	Pointf       (GisOrientation::*globalproc)(double lon, double lat) const;
	Rectf        (GisOrientation::*localextent)(const Rectf& rc) const;
	Rectf        (GisOrientation::*globalextent)(const Rectf& rc) const;
	bool         identity;
	Pointf       pole;
	double       delta_phi;
	double       suk, cuk;
	bool         sukneg, cukneg;
};

class SphericalLatitudeFunction : public GisFunction
{
public:
	SphericalLatitudeFunction(double alpha, double k, double R, double e, double U0)
	: alpha(alpha), k(k), R(R), e(e), U0(U0) {}

	virtual double Get(double x) const;

private:
	double alpha, k, R, e, U0;
};

class GisCoordsGaussLatitude {
public:
	GisCoordsGaussLatitude();
	
	void Create(double a, double e2, double base_parallel);
	
	double Spherical(double latitude) const { return gauss_projected(latitude); }
	double Elliptical(double latitude) const { return gauss_latitude(latitude); }
	
public:
	double radius;

private:
	mutable GisInterpolator gauss_projected;
	mutable GisInterpolator gauss_latitude;
};

class GisEllipsoid
{
public:
	GisEllipsoid() : a(0), from_wgs84(Matrixf3_1()) {}

	static const Index<int>&  EnumEPSG();
	static GisEllipsoid       GetEPSG(int code);

	static GisEllipsoid       AB(double a, double b);
	static GisEllipsoid       AI(double a, double i);
	static GisEllipsoid       AE2(double a, double e2);
	static GisEllipsoid       R(double r);

	enum
	{
		BESSEL_1841     = 7004,
		HAYFORD_1909    = 7022,
		KRASSOWSKY_1940 = 7024,
		WGS_1984        = 7030,
		GRS_1980        = 7019,
	};

	bool                      IsNullInstance() const { return !a; }
	bool                      IsSphere() const       { return b == a; }

	double                    M(double phi) const    { double t = 1 - e2 * sqr(sin(phi)); return a * a / (b * t * sqrt(t)); }
	double                    N(double phi) const    { double t = 1 - e2 * sqr(sin(phi)); return a / sqrt(t); }

	Pointf3                   To3D(double lon, double lat, double elev = 0) const;
	Pointf3                   To3D(const Pointf& lonlat, double elev = 0) const { return To3D(lonlat.x, lonlat.y, elev); }
	Pointf3                   To3D(const Pointf3& lonlatelev) const             { return To3D(lonlatelev.x, lonlatelev.y, lonlatelev.z); }
	Pointf                    From3D(double x, double y, double z) const;
	Pointf                    From3D(const Pointf3& xyz) const { return From3D(xyz.x, xyz.y, xyz.z); }

public:
	double                    a;
	double                    b;
	double                    e2;
	double                    i;

	Matrixf3                  from_wgs84;

	int                       epsg_code;
	String                    name;
	String                    description;
	String                    source;
};

bool operator == (const GisEllipsoid& ea, const GisEllipsoid& eb);
inline bool operator != (const GisEllipsoid& ea, const GisEllipsoid& eb) { return !(ea == eb); }

class GisCoords : Moveable<GisCoords>
{
public:
	enum STYLE
	{
		STYLE_EDIT,
		STYLE_ANGLE,
		STYLE_DROPLIST,
		STYLE_OPTION,
		STYLE_OPTION3,
	};

	struct Arg
	{
		Arg() : vtype(ERROR_V), min(Null), max(Null), not_null(true) {}

		static Arg Edit(double& v, String ident, String name, String help_topic = Null, double min = Null, double max = Null, bool not_null = true);
		static Arg Angle(double& v, String ident, String name, String help_topic = Null, double min = -2 * M_PI, double max = +2 * M_PI, bool not_null = true);
		static Arg Edit(String& v, String ident, String name, String help_topic = Null, bool not_null = true);
		static Arg Edit(Date& d, String ident, String name, String help_topic = Null, bool not_null = true);
		static Arg Edit(Time& t, String ident, String name, String help_topic = Null, bool not_null = true);
		static Arg DropList(String& v, String ident, String name, String help_topic, String variants, bool not_null = true);
		static Arg Option(bool& b, String ident, String name, String help_topic = Null);
		static Arg Option3(int& b, String ident, String name, String help_topic = Null);

		Arg&       Cond(String ident, Value value) { cond_ident = ident; cond_value = value; return *this; }

		String     ident;
		String     name;
		String     help_topic;
		int        vtype;
		STYLE      style;
		String     variants;
		String     cond_ident;
		Value      cond_value;
		double     min;
		double     max;
		bool       not_null;
		Ref        ref;
	};

	struct ConstArg
	{
		ConstArg(String ident, String name, Value value, String help_topic = Null)
		: ident(ident), name(name), value(value), help_topic(help_topic) {}

		String     ident;
		String     name;
		Value      value;
		String     help_topic;
	};

	class Data : public GeomRefBase
	{
	public:
		Data();

		virtual GisCoords             DeepCopy() const = 0;

		virtual int                   GetBranchCount() const;
		virtual Array<Pointf>         LonLatBoundary(const Rectf& lonlat_extent, int branch) const;
		virtual GisBSPTree            GetBranchTree(const Rectf& lonlat_extent) const;

		virtual Pointf                LonLat(Pointf xy) const = 0;
		virtual Rectf                 LonLatExtent(const Rectf& xy_extent) const;
		Pointf3                       LonLat3D(Pointf xy, double elevation = 0) const { return ellipsoid.To3D(LonLat(xy), elevation); }
		virtual Pointf                Project(Pointf lonlat, int branch) const = 0;
		virtual Rectf                 ProjectExtent(const Rectf& lonlat_extent) const;
		Pointf                        Project3D(Pointf3 xyz, int branch) const        { return Project(ellipsoid.From3D(xyz), branch); }
		virtual double                ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const;
		virtual double                ProjectRatio(Pointf lonlat, int branch) const;
		virtual double                ExtentDeviation(const Rectf& lonlat_extent) const;
		virtual Sizef                 MinMaxRatio(const Rectf& lonlat_extent) const;

//		virtual String                ToString() const = 0;
		virtual Array<Arg>            EnumArgs() = 0;
		virtual Array<ConstArg>       EnumConstArgs() const = 0;
		virtual void                  SyncArgs() = 0;

		virtual bool                  IsProjected() const;
		bool                          IsIdentity(const Data *data) const;

	public:
		String                        ident;
		GisEllipsoid                  ellipsoid;
		Rectf                         lonlat_limits;
		int                           epsg_code;
		String                        name;
		String                        description;
		String                        coordsys;
	};

	GisCoords(const Nuller& = Null) {}
	GisCoords(GeomRefPtr<Data> data) : data(data) {}
	GisCoords(Data *data) : data(data) {}
	GisCoords(const Value& v)                                                                  { if(IsTypeRaw<GisCoords>(v)) *this = ValueTo<GisCoords>(v); }

	operator Value() const                                                                     { return RawToValue(*this); }

	bool                  IsNullInstance() const                                               { return !data; }
	bool                  Equals(const GisCoords& co) const;

	GisCoords             DeepCopy() const                                                     { return data->DeepCopy(); }

	int                   GetBranchCount() const                                               { return data->GetBranchCount(); }
	Array<Pointf>         LonLatBoundary(const Rectf& lonlat_extent, int branch) const         { return data->LonLatBoundary(lonlat_extent, branch); }
	GisBSPTree            GetBranchTree(const Rectf& lonlat_extent) const                      { return data->GetBranchTree(lonlat_extent); }

	Pointf                LonLat(Pointf xy) const                                              { return data->LonLat(xy); }
	Rectf                 LonLatExtent(const Rectf& xy_extent) const                           { return data->LonLatExtent(xy_extent); }
	Pointf3               LonLat3D(Pointf xy, double elevation = 0) const                      { return data->LonLat3D(xy, elevation); }
	Pointf                Project(Pointf lonlat, int branch) const                             { return data->Project(lonlat, branch); }
	Rectf                 ProjectExtent(const Rectf& lonlat_extent) const                      { return data->ProjectExtent(lonlat_extent); }
	Pointf                Project3D(Pointf3 xyz, int branch) const                             { return data->Project3D(xyz, branch); }
	double                ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const   { return data->ProjectDeviation(lonlat1, lonlat2, branch); }
	double                ProjectRatio(Pointf lonlat, int branch) const                        { return data->ProjectRatio(lonlat, branch); }
	double                ExtentDeviation(const Rectf& lonlat_extent) const                    { return data->ExtentDeviation(lonlat_extent); }
	Sizef                 MinMaxRatio(const Rectf& lonlat_extent) const                        { return data->MinMaxRatio(lonlat_extent); }
	Rectf                 LonLatLimits() const                                                 { return data->lonlat_limits; }

//	String                ToString() const                                                     { return data->ToString(); }
	String                GetIdent() const                                                     { return !!data ? data->ident : String::GetVoid(); }
	int                   GetEPSGCode() const                                                  { return !!data ? data->epsg_code : (int)Null; }
	String                GetName() const                                                      { return !!data ? data->name : String::GetVoid(); }
	String                GetDescription() const                                               { return !!data ? data->description : String::GetVoid(); }
	String                GetCoordsys() const                                                  { return !!data ? data->coordsys : String::GetVoid(); }
	Array<Arg>            EnumArgs()                                                           { return data->EnumArgs(); }
	Array<ConstArg>       EnumConstArgs() const                                                { return data->EnumConstArgs(); }
	void                  SyncArgs()                                                           { data->SyncArgs(); }

	bool                  IsProjected() const                                                  { return data->IsProjected(); }
	bool                  IsIdentity(GisCoords gp) const                                       { return data->IsIdentity(~gp.data); }

	const GisEllipsoid&   GetEllipsoid() const                                                 { return data->ellipsoid; }
	void                  SetEllipsoid(const GisEllipsoid& e)                                  { data->ellipsoid = e; }
	Pointf3               To3D(Pointf lonlat, double elevation = 0) const                      { return data->ellipsoid.To3D(lonlat, elevation); }
	Pointf                From3D(Pointf3 xyz) const                                            { return data->ellipsoid.From3D(xyz); }
	const Matrixf3&       FromWGS84() const                                                    { return data->ellipsoid.from_wgs84; }

	Data                 *operator ~ () const                                                  { return ~data; }

	static const Vector<int>& EnumEPSG();
	static GisCoords      GetEPSG(int code);

public:
	GeomRefPtr<Data>          data;
};

inline bool operator == (const GisCoords& a, const GisCoords& b) { return a.Equals(b); }
inline bool operator != (const GisCoords& a, const GisCoords& b) { return !a.Equals(b); }

class GisTransform : Moveable<GisTransform>
{
public:
	class Data : public GeomRefBase
	{
	public:
		virtual ~Data() {}

		virtual bool         IsNullInstance() const { return IsNull(source) && IsNull(target); }
		virtual GisTransform DeepCopy() const = 0;

		virtual Pointf       Target(Pointf src) const = 0;
		virtual Pointf       Source(Pointf trg) const = 0;
		virtual Rectf        TargetExtent(const Rectf& src) const = 0;
		virtual Rectf        SourceExtent(const Rectf& src) const = 0;
		virtual double       SourceDeviation(Pointf src1, Pointf src2) const = 0;
		virtual double       TargetDeviation(Pointf trg1, Pointf trg2) const = 0;
		virtual double       SourceExtentDeviation(const Rectf& src) const = 0;
		virtual double       TargetExtentDeviation(const Rectf& trg) const = 0;
		virtual Sizef        SourceMinMaxRatio(const Rectf& src) const = 0;
		virtual Sizef        TargetMinMaxRatio(const Rectf& trg) const = 0;
		virtual GisBSPTree   TargetBranchTree(const Rectf& trg) const = 0;
		virtual GisBSPTree   SourceBranchTree(const Rectf& src) const = 0;
		virtual Rectf        TargetLimits() const = 0;
		virtual Rectf        SourceLimits() const = 0;
		virtual bool         IsProjected() const = 0;
		virtual bool         IsIdentity() const = 0;

	public:
		GisCoords            source;
		GisCoords            target;
		Ptr<Gis2DGrid>       grid;
	};

public:
	GisTransform(const Nuller& = Null);
	GisTransform(GeomRefPtr<Data> data) : data(data) {}
	GisTransform(Data *data) : data(data) {}
	GisTransform(GisCoords source, GisCoords target);

	bool         IsNullInstance() const                          { return data->IsNullInstance(); }

	GisTransform DeepCopy() const                                { return data->DeepCopy(); }

	Pointf       Target(Pointf src) const                        { return data->Target(src); }
	Pointf       Source(Pointf trg) const                        { return data->Source(trg); }
	Rectf        TargetExtent(const Rectf& src) const            { return data->TargetExtent(src); }
	Rectf        SourceExtent(const Rectf& trg) const            { return data->SourceExtent(trg); }
	double       SourceDeviation(Pointf src1, Pointf src2) const { return data->SourceDeviation(src1, src2); }
	double       TargetDeviation(Pointf trg1, Pointf trg2) const { return data->TargetDeviation(trg1, trg2); }
	double       SourceExtentDeviation(const Rectf& src) const   { return data->SourceExtentDeviation(src); }
	double       TargetExtentDeviation(const Rectf& trg) const   { return data->TargetExtentDeviation(trg); }
	Sizef        SourceMinMaxRatio(const Rectf& src) const       { return data->SourceMinMaxRatio(src); }
	Sizef        TargetMinMaxRatio(const Rectf& trg) const       { return data->TargetMinMaxRatio(trg); }
	GisBSPTree   SourceBranchTree(const Rectf& src) const        { return data->SourceBranchTree(src); }
	GisBSPTree   TargetBranchTree(const Rectf& trg) const        { return data->TargetBranchTree(trg); }
	Rectf        TargetLimits() const                            { return data->TargetLimits(); }
	Rectf        SourceLimits() const                            { return data->SourceLimits(); }
	bool         IsProjected() const                             { return data->IsProjected(); }
	bool         IsIdentity() const                              { return data->IsIdentity(); }

	GisCoords    Source() const                                  { return data->source; }
	GisCoords    Target() const                                  { return data->target; }
	GisTransform Reverse() const                                 { return GisTransform(Target(), Source()); }
	Gis2DGrid   *Grid() const                                    { return data->grid; }

	Data        *operator ~ () const                             { return ~data; }

	bool         Equals(const GisTransform& t) const;

public:
	GeomRefPtr<Data> data;
};

inline bool   operator == (const GisTransform& a, const GisTransform& b) { return a.Equals(b); }
inline bool   operator != (const GisTransform& a, const GisTransform& b) { return !a.Equals(b); }

inline Pointf operator * (Pointf pt, const GisTransform& t)       { return t.Target(pt); }
inline Rectf  operator * (const Rectf& rc, const GisTransform& t) { return t.TargetExtent(rc); }
inline Pointf operator / (Pointf pt, const GisTransform& t)       { return t.Source(pt); }
inline Rectf  operator / (const Rectf& rc, const GisTransform& t) { return t.SourceExtent(rc); }

struct SegmentTreeInfo
{
	Matrixf      img_src;
	GisTransform src_trg;
	Matrixf      trg_pix;
	int          branch;
	int          max_depth;
	double       max_deviation;
	bool         antialias;
};

inline Point operator * (Point pt, const SegmentTreeInfo& sti) { return PointfToPoint(Pointf(pt) * sti.img_src * sti.src_trg * sti.trg_pix); }

class LinearSegmentTree
{
public:
	struct Node
	{
		Point     source;
		Point     target;
		One<Node> below;
		One<Node> above;
	};

public:
	Point     source1;
	Point     source2;

	Point     target1;
	Point     target2;
	One<Node> split;
	
	rval_default(LinearSegmentTree);
	LinearSegmentTree() {}
};

LinearSegmentTree CreateLinearTree(Point s1, Point s2, const SegmentTreeInfo& info);

class PlanarSegmentTree
{
public:
	struct Split;
	struct Node
	{
		Rect       source;
		Point      trg_topleft, trg_topright, trg_bottomleft, trg_bottomright;
		One<Split> split;
		
		rval_default(Node);
		Node() {}
	};

	struct Split
	{
		Node topleft;
		Node topright;
		Node bottomleft;
		Node bottomright;
	};

public:
	Node root;
	
	rval_default(PlanarSegmentTree);
	PlanarSegmentTree() {}
};

PlanarSegmentTree CreatePlanarTree(const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom, const SegmentTreeInfo& info);

}

#endif
