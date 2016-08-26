#ifndef _Gis3_Coords_method_h_
#define _Gis3_Coords_method_h_

namespace Upp {

class GisCoordsLonLat : public GisCoords::Data
{
public:
	GisCoordsLonLat(double prime_meridian = 0.0);

	virtual GisCoords     DeepCopy() const { return new GisCoordsLonLat(*this); }

	virtual Pointf        LonLat(Pointf xy) const;
	virtual Rectf         LonLatExtent(const Rectf& xy_extent) const;
	virtual Pointf        Project(Pointf lonlat, int branch) const;
	virtual Rectf         ProjectExtent(const Rectf& lonlat_extent) const;
	virtual double        ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const;
	virtual double        ProjectRatio(Pointf lonlat, int branch) const;

//	virtual String        ToString() const;
	virtual Array<GisCoords::Arg> EnumArgs();
	virtual Array<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();
	virtual bool          IsProjected() const;

public:
	double                prime_meridian;
};

class GisCoordsSphericalData
{
public:
	double                base_parallel;
};

class GisCoordsSpherical : public GisCoords::Data
	, public GisCoordsSphericalData
{
public:
	typedef GisCoordsSpherical CLASSNAME;
	GisCoordsSpherical(double base_parallel = 50.0);
	GisCoordsSpherical(const GisCoordsSpherical& cgs)
		: GisCoords::Data(cgs), GisCoordsSphericalData(cgs) { SyncArgs(); }

	virtual GisCoords DeepCopy() const { return new GisCoordsSpherical(*this); }

	virtual Pointf        LonLat(Pointf xy) const;
	virtual Rectf         LonLatExtent(const Rectf& xy_extent) const;
	virtual Pointf        Project(Pointf lonlat, int branch) const;
	virtual Rectf         ProjectExtent(const Rectf& lonlat_extent) const;
	virtual double        ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const;
	virtual double        ProjectRatio(Pointf lonlat, int branch) const;

//	virtual String        ToString() const;
	virtual Array<GisCoords::Arg> EnumArgs();
	virtual Array<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

private:
	void                  CheckInterpolator() const { if(gauss_projected.IsEmpty()) SyncInterpolator(); }
	void                  SyncInterpolator() const;

private:
	mutable GisInterpolator gauss_projected;
	mutable GisInterpolator gauss_latitude;
	double                alpha, k, R, e, U0;
};

class ConicalRadiusFunction : public GisFunction
{
public:
	ConicalRadiusFunction(double n, double rho_coef);

	virtual double Get(double x) const;

private:
	double n;
	double k0, k1;
	double rho_coef;
};

class GisCoordsConicalData
{
public:
//	bool                  gauss_sphere;
	double                gauss_base_parallel;
	double                pole_meridian;
	double                pole_parallel;
	double                central_parallel;
//	double                prime_meridian;
	double                north_parallel;
	double                south_parallel;
	double                multiplier;
	double                xmeteroffset;
	double                ymeteroffset;
};

class GisCoordsConical : public GisCoords::Data
	, public GisCoordsConicalData
{
public:
	typedef GisCoordsConical CLASSNAME;
	GisCoordsConical(
		double gauss_base_parallel = 49.5,
		double pole_meridian = ANGDEG(42, 31, 31.41725),
		double pole_parallel = ANGDEG(59, 42, 42.6969),
		double central_parallel = 78.5,
		double north_parallel = 78.5,
		double south_parallel = 78.5,
		double multiplier = 0.9999,
		double xmeteroffset = 0, double ymeteroffset = 0);
	GisCoordsConical(const GisCoordsConical& cgs)
		: GisCoords::Data(cgs), GisCoordsConicalData(cgs) { SyncArgs(); }

	virtual GisCoords DeepCopy() const { return new GisCoordsConical(*this); }

	virtual int           GetBranchCount() const;
	virtual Array<Pointf> LonLatBoundary(const Rectf& lonlat_extent, int branch) const;
	virtual GisBSPTree    GetBranchTree(const Rectf& lonlat_extent) const;

	virtual Pointf        LonLat(Pointf xy) const;
	virtual Rectf         LonLatExtent(const Rectf& xy_extent) const;
	virtual Pointf        Project(Pointf lonlat, int branch) const;
	virtual Rectf         ProjectExtent(const Rectf& lonlat_extent) const;
	virtual double        ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const;
	virtual double        ExtentDeviation(const Rectf& lonlat_extent) const;
	virtual double        ProjectRatio(Pointf lonlat, int branch) const;
	virtual Sizef         MinMaxRatio(const Rectf& lonlat_extent) const;

//	virtual String        ToString() const;
	virtual Array<GisCoords::Arg> EnumArgs();
	virtual Array<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

	String                DumpProjectedDelta() const;

private:
	void                  CheckInterpolator() const { if(tgsin_projected.IsEmpty()) SyncInterpolator(); }
	void                  SyncInterpolator() const;

private:
	mutable GisInterpolator gauss_projected;
	mutable GisInterpolator gauss_latitude;
	mutable GisInterpolator tgsin_projected;
	mutable GisInterpolator tgsin_latitude;
	GisOrientation        orientation;
	double                gauss_alpha, gauss_k, gauss_R, gauss_e, gauss_U0;
	double                rho0;
	double                n;
//	double                e;
	double                rho_coef;
};

class GisCoordsUTMData
{
public:
	double                central_meridian;
	double                multiplier;
	double                xmeteroffset;
	double                ymeteroffset;
};

class GisCoordsUTM : public GisCoords::Data, public GisCoordsUTMData
{
public:
	typedef GisCoordsUTM CLASSNAME;
	GisCoordsUTM(
		double central_meridian = 0.0,
		double multiplier = 1.0,
		double xmeteroffset = 0.0,
		double ymeteroffset = 0.0);
	GisCoordsUTM(const GisCoordsUTM& cgs)
		: GisCoords::Data(cgs), GisCoordsUTMData(cgs) { SyncArgs(); }

	virtual GisCoords     DeepCopy() const { return new GisCoordsUTM(*this); }

	virtual Pointf        LonLat(Pointf xy) const;
	virtual Pointf        Project(Pointf lonlat, int branch) const;

	virtual Array<GisCoords::Arg> EnumArgs();
	virtual Array<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

	String                DumpProjectedDelta() const;

private:
	double                E21, E12;
};

class GisCoordsAzimuthal : public GisCoords::Data
{
public:
	typedef GisCoordsAzimuthal CLASSNAME;
	GisCoordsAzimuthal(const Pointf& pole = Pointf(0, 90),
		const Sizef& scale = Sizef(1, 1), const Sizef& offset = Sizef(0, 0));
//	GisCoordsAzimuthal(const GisCoordsAzimuthal& a)

	virtual GisCoords     DeepCopy() const;

	virtual int           GetBranchCount() const;
	virtual Array<Pointf> LonLatBoundary(const Rectf& lonlat_extent, int branch) const;
	virtual GisBSPTree    GetBranchTree(const Rectf& lonlat_extent) const;

	virtual Pointf        LonLat(Pointf xy) const;
	//virtual Rectf         LonLatExtent(const Rectf& xy_extent) const;
	virtual Pointf        Project(Pointf lonlat, int branch) const;
	//virtual Rectf         ProjectExtent(const Rectf& lonlat_extent) const;
	//virtual double        ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const;
	//virtual double        ProjectRatio(Pointf lonlat, int branch) const;

//	virtual String        ToString() const;
	virtual Array<GisCoords::Arg> EnumArgs();
	virtual Array<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

private:
	void                  CheckInterpolator() const {} // if(false) SyncInterpolator(); }
	void                  SyncInterpolator() const;

public:
	Pointf                pole;
	Pointf                gauss_pole;
	Sizef                 scale;
	Sizef                 offset;
	GisOrientation        orientation;
	GisCoordsGaussLatitude gauss;
};

}

#endif
