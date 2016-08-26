#include "GeomCoords.h"

namespace Upp {

GisCoords::Arg GisCoords::Arg::Edit(double& v, String ident, String name, String help_topic, double min, double max, bool not_null)
{
	Arg out;
	out.vtype = DOUBLE_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.min = min;
	out.max = max;
	out.not_null = not_null;
	out.ref = Ref(v);
	return out;
}

GisCoords::Arg GisCoords::Arg::Angle(double& v, String ident, String name, String help_topic, double min, double max, bool not_null)
{
	Arg out;
	out.vtype = DOUBLE_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_ANGLE;
	out.min = min;
	out.max = max;
	out.not_null = not_null;
	out.ref = Ref(v);
	return out;
}

GisCoords::Arg GisCoords::Arg::Edit(String& v, String ident, String name, String help_topic, bool not_null)
{
	Arg out;
	out.vtype = STRING_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.not_null = not_null;
	out.ref = Ref(v);
	return out;
}

GisCoords::Arg GisCoords::Arg::Edit(Date& d, String ident, String name, String help_topic, bool not_null)
{
	Arg out;
	out.vtype = DATE_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.not_null = not_null;
	out.ref = Ref(d);
	return out;
}

GisCoords::Arg GisCoords::Arg::Edit(Time& t, String ident, String name, String help_topic, bool not_null)
{
	Arg out;
	out.vtype = TIME_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.not_null = not_null;
	out.ref = Ref(t);
	return out;
}

GisCoords::Arg GisCoords::Arg::DropList(String& v, String ident, String name, String help_topic, String variants, bool not_null)
{
	Arg out;
	out.vtype = STRING_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_DROPLIST;
	out.variants = variants;
	out.not_null = not_null;
	out.ref = Ref(v);
	return out;
}

struct GeomBoolRef : public RefManager
{
	virtual int        GetType()                               { return UNKNOWN_V; }
	virtual Value      GetValue(const void *x)                 { return *(const bool *)x ? 1 : 0; }
	virtual void       SetValue(void *x, const Value& v)       { *(bool *)x = !UPP::IsNull(v) && (double)v; }
	virtual void       SetNull(void *x)                        { *(bool *)x = false; }

	static RefManager *Manager()                               { static GeomBoolRef m; return &m; }
};

inline Ref GeomBoolAsRef(bool& b) { return Ref(&b, GeomBoolRef::Manager()); }

GisCoords::Arg GisCoords::Arg::Option(bool& b, String ident, String name, String help_topic)
{
	Arg out;
	out.vtype = INT_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_OPTION;
	out.not_null = true;
	out.ref = GeomBoolAsRef(b);
	return out;
}

GisCoords::Arg GisCoords::Arg::Option3(int& b, String ident, String name, String help_topic)
{
	Arg out;
	out.vtype = INT_V;
	out.ident = ident;
	out.name = name;
	out.help_topic = help_topic;
	out.style = STYLE_OPTION3;
	out.not_null = false;
	out.ref = Ref(b);
	return out;
}

GisCoords::Data::Data()
{
	epsg_code = 0;
	lonlat_limits = Rectf(-180, -90, 180, 90);
}

bool GisCoords::Data::IsProjected() const
{
	return true;
}

int GisCoords::Data::GetBranchCount() const
{
	return 1;
}

Array<Pointf> GisCoords::Data::LonLatBoundary(const Rectf& lonlat_extent, int b) const
{
	Array<Pointf> out;
	out.SetCount(5);
	out[0] = lonlat_extent.TopLeft();
	out[1] = lonlat_extent.TopRight();
	out[2] = lonlat_extent.BottomRight();
	out[3] = lonlat_extent.BottomLeft();
	out[4] = lonlat_extent.TopLeft();
	return out;
}

GisBSPTree GisCoords::Data::GetBranchTree(const Rectf& lonlat_extent) const
{
	return GisBSPTree::Node(0);
}

Rectf GisCoords::Data::LonLatExtent(const Rectf& xy_extent) const
{
	Rectf lonlat_extent = Null;
	lonlat_extent |= LonLat(xy_extent.TopLeft());
	lonlat_extent |= LonLat(xy_extent.TopRight());
	lonlat_extent |= LonLat(xy_extent.BottomLeft());
	lonlat_extent |= LonLat(xy_extent.BottomRight());
	return lonlat_extent;
}

Rectf GisCoords::Data::ProjectExtent(const Rectf& lonlat_extent) const
{
//	ASSERT(GetBranchCount() == 1);
	Rectf xy_extent = Null;
	xy_extent |= Project(lonlat_extent.TopLeft(), 0);
	xy_extent |= Project(lonlat_extent.TopRight(), 0);
	xy_extent |= Project(lonlat_extent.BottomLeft(), 0);
	xy_extent |= Project(lonlat_extent.BottomRight(), 0);
	return xy_extent;
}

double GisCoords::Data::ProjectDeviation(Pointf lonlat1, Pointf lonlat2, int branch) const
{
	Pointf mid = (lonlat1 + lonlat2) / 2.0;
	Pointf p1 = Project(lonlat1, branch), p2 = Project(mid, branch), p3 = Project(lonlat2, branch);
	Pointf pmid = (p1 + p3) / 2.0;
	double length = Distance(p1, p3);
	if(length <= 1e-10)
		return Distance(p2, pmid);
	return ((p1 - pmid) ^ (p3 - p1)) / length;
}

double GisCoords::Data::ProjectRatio(Pointf lonlat, int branch) const
{
	/*Pointf proj = */Project(lonlat, branch);
	static const double DELTA = 1e-3;
	Sizef east = Project(lonlat + Sizef(DELTA, 0), branch);
	Sizef north = Project(lonlat + Sizef(0, DELTA), branch);
	return (Length(east) + Length(north)) * (DELTA / 2);
}

double GisCoords::Data::ExtentDeviation(const Rectf& lonlat_extent) const
{
	return 0;
}

Sizef GisCoords::Data::MinMaxRatio(const Rectf& lonlat_extent) const
{
	return Sizef(1, 1);
}

bool GisCoords::Data::IsIdentity(const Data *data) const
{
	if(strcmp(typeid(*this).name(), typeid(*data).name()))
		return false;
	Array<Arg> args = const_cast<GisCoords::Data *>(this)->EnumArgs();
	Array<Arg> args2 = const_cast<GisCoords::Data *>(data)->EnumArgs();
	if(args.GetCount() != args2.GetCount())
		return false;
	for(int i = 0; i < args.GetCount(); i++)
		if(args[i].name != args2[i].name
		|| args[i].vtype != args2[i].vtype
		|| Value(args[i].ref) != Value(args2[i].ref))
			return false;
	return true;
}

bool GisCoords::Equals(const GisCoords& co) const
{
	bool na = IsNullInstance(), nb = co.IsNullInstance();
	if(na || nb)
		return na && nb;
	return GetIdent() == co.GetIdent();
}

class GisTransformIdentity : public GisTransform::Data
{
public:
	GisTransformIdentity()                                               {}
	virtual GisTransform DeepCopy() const                                { return new GisTransformIdentity(*this); }

	virtual Pointf       Target(Pointf s) const                          { return s; }
	virtual Pointf       Source(Pointf t) const                          { return t; }
	virtual Rectf        TargetExtent(const Rectf& s) const              { return s; }
	virtual Rectf        SourceExtent(const Rectf& t) const              { return t; }
	virtual double       SourceDeviation(Pointf src1, Pointf src2) const { return 0; }
	virtual double       TargetDeviation(Pointf trg1, Pointf trg2) const { return 0; }
	virtual double       SourceExtentDeviation(const Rectf& src) const   { return 0; }
	virtual double       TargetExtentDeviation(const Rectf& trg) const   { return 0; }
	virtual Sizef        SourceMinMaxRatio(const Rectf& s) const         { return Sizef(1, 1); }
	virtual Sizef        TargetMinMaxRatio(const Rectf& t) const         { return Sizef(1, 1); }
	virtual GisBSPTree   TargetBranchTree(const Rectf& t) const          { return GisBSPTree(0); }
	virtual GisBSPTree   SourceBranchTree(const Rectf& t) const          { return GisBSPTree(0); }
	virtual Rectf        TargetLimits() const                            { return Null; }
	virtual Rectf        SourceLimits() const                            { return Null; }
	virtual bool         IsProjected() const                             { return true; }
	virtual bool         IsIdentity() const                              { return true; }
};

class GisTransformProjectIdentity : public GisTransform::Data
{
public:
	GisTransformProjectIdentity(GisCoords st)                            { source = target = st; }
	virtual GisTransform DeepCopy() const                                { return new GisTransformProjectIdentity(*this); }

	virtual Pointf       Target(Pointf s) const                          { return s; }
	virtual Pointf       Source(Pointf t) const                          { return t; }
	virtual Rectf        TargetExtent(const Rectf& s) const              { return s; }
	virtual Rectf        SourceExtent(const Rectf& t) const              { return t; }
	virtual double       SourceDeviation(Pointf src1, Pointf src2) const { return 0; }
	virtual double       TargetDeviation(Pointf trg1, Pointf trg2) const { return 0; }
	virtual double       SourceExtentDeviation(const Rectf& src) const   { return 0; }
	virtual double       TargetExtentDeviation(const Rectf& trg) const   { return 0; }
	virtual Sizef        SourceMinMaxRatio(const Rectf& s) const         { return Sizef(1, 1); }
	virtual Sizef        TargetMinMaxRatio(const Rectf& t) const         { return Sizef(1, 1); }
	virtual GisBSPTree   TargetBranchTree(const Rectf& t) const          { return GisBSPTree(0); }
	virtual GisBSPTree   SourceBranchTree(const Rectf& s) const          { return GisBSPTree(0); }
	virtual Rectf        TargetLimits() const                            { return target.ProjectExtent(source.LonLatLimits()); }
	virtual Rectf        SourceLimits() const                            { return source.ProjectExtent(source.LonLatLimits()); }
	virtual bool         IsProjected() const                             { return target.IsProjected(); }
	virtual bool         IsIdentity() const                              { return true; }
};

class GisTransformSameEllip : public GisTransform::Data
{
public:
	GisTransformSameEllip(GisCoords src, GisCoords trg)                  { source = src; target = trg; }

	virtual GisTransform DeepCopy() const                                { return new GisTransformSameEllip(*this); }

	virtual Pointf       Target(Pointf s) const                          { return target.Project(source.LonLat(s), 0); }
	virtual Pointf       Source(Pointf t) const                          { return source.Project(target.LonLat(t), 0); }
	virtual Rectf        TargetExtent(const Rectf& s) const              { return target.ProjectExtent(source.LonLatExtent(s)); }
	virtual Rectf        SourceExtent(const Rectf& t) const              { return source.ProjectExtent(target.LonLatExtent(t)); }
	virtual double       SourceDeviation(Pointf src1, Pointf src2) const;
	virtual double       TargetDeviation(Pointf trg1, Pointf trg2) const;
	virtual double       SourceExtentDeviation(const Rectf& src) const;
	virtual double       TargetExtentDeviation(const Rectf& trg) const;
	virtual Sizef        SourceMinMaxRatio(const Rectf& s) const;
	virtual Sizef        TargetMinMaxRatio(const Rectf& t) const;
	virtual GisBSPTree   TargetBranchTree(const Rectf& t) const          { return target.GetBranchTree(target.LonLatExtent(t)); }
	virtual GisBSPTree   SourceBranchTree(const Rectf& s) const          { return target.GetBranchTree(source.LonLatExtent(s)); }
	virtual Rectf        TargetLimits() const                            { return target.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual Rectf        SourceLimits() const                            { return source.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual bool         IsProjected() const                             { return target.IsProjected(); }
	virtual bool         IsIdentity() const                              { return false; }

	Matrixf3             source_ellip;
	Matrixf3             target_ellip;
};

double GisTransformSameEllip::SourceDeviation(Pointf src1, Pointf src2) const
{
	Pointf ll1 = source.LonLat(src1), ll2 = source.LonLat(src2);
	Rectf ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
			* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformSameEllip::TargetDeviation(Pointf trg1, Pointf trg2) const
{
	Pointf ll1 = target.LonLat(trg1), ll2 = target.LonLat(trg2);
	Rectf ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformSameEllip::SourceExtentDeviation(const Rectf& src) const
{
	Rectf ll = source.LonLatExtent(src);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformSameEllip::TargetExtentDeviation(const Rectf& trg) const
{
	Rectf ll = target.LonLatExtent(trg);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

Sizef GisTransformSameEllip::SourceMinMaxRatio(const Rectf& s) const
{
	Rectf ll = source.LonLatExtent(s);
	Sizef sr = source.MinMaxRatio(ll);
	Sizef dr = target.MinMaxRatio(ll);
	return Sizef(dr.cx / sr.cy, dr.cy / sr.cx);
}

Sizef GisTransformSameEllip::TargetMinMaxRatio(const Rectf& t) const
{
	Rectf ll = target.LonLatExtent(t);
	Sizef sr = source.MinMaxRatio(ll);
	Sizef dr = target.MinMaxRatio(ll);
	return Sizef(dr.cx / sr.cy, dr.cy / sr.cx);
}

class GisTransformGeneric : public GisTransform::Data
{
public:
	GisTransformGeneric(GisCoords src, GisCoords trg);
	virtual GisTransform DeepCopy() const                                { return new GisTransformGeneric(*this); }

	virtual Pointf       Target(Pointf s) const;
	virtual Pointf       Source(Pointf t) const;
	virtual Rectf        TargetExtent(const Rectf& s) const;
	virtual Rectf        SourceExtent(const Rectf& t) const;
	virtual double       SourceDeviation(Pointf src1, Pointf src2) const;
	virtual double       TargetDeviation(Pointf trg1, Pointf trg2) const;
	virtual double       SourceExtentDeviation(const Rectf& src) const;
	virtual double       TargetExtentDeviation(const Rectf& trg) const;
	virtual Sizef        SourceMinMaxRatio(const Rectf& s) const;
	virtual Sizef        TargetMinMaxRatio(const Rectf& t) const;
	virtual GisBSPTree   TargetBranchTree(const Rectf& t) const          { return target.GetBranchTree(target.LonLatExtent(t)); }
	virtual GisBSPTree   SourceBranchTree(const Rectf& s) const          { return target.GetBranchTree(source.LonLatExtent(s)); }
	virtual Rectf        TargetLimits() const                            { return target.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual Rectf        SourceLimits() const                            { return source.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual bool         IsProjected() const                             { return target.IsProjected(); }
	virtual bool         IsIdentity() const                              { return false; }

	Matrixf3             source_ellip;
	Matrixf3             target_ellip;
};

GisTransformGeneric::GisTransformGeneric(GisCoords src, GisCoords trg)
{
	source = src;
	target = trg;
	target_ellip = Matrixf3Inverse(source.FromWGS84()) * target.FromWGS84();
	source_ellip = Matrixf3Inverse(target_ellip);
}

Pointf GisTransformGeneric::Target(Pointf s) const
{
	return target.Project3D(source.LonLat3D(s) * target_ellip, 0);
}

Pointf GisTransformGeneric::Source(Pointf t) const
{
	return source.Project3D(target.LonLat3D(t) * source_ellip, 0);
}

Rectf GisTransformGeneric::TargetExtent(const Rectf& s) const
{
	Rectf lonlat = source.LonLatExtent(s);
	Rectf lontar = Null;
	lontar |= target.From3D(source.To3D(lonlat.TopLeft()) * target_ellip);
	lontar |= target.From3D(source.To3D(lonlat.TopRight()) * target_ellip);
	lontar |= target.From3D(source.To3D(lonlat.BottomLeft()) * target_ellip);
	lontar |= target.From3D(source.To3D(lonlat.BottomRight()) * target_ellip);
	return target.ProjectExtent(lontar);
}

Rectf GisTransformGeneric::SourceExtent(const Rectf& t) const
{
	Rectf lonlat = target.LonLatExtent(t);
	Rectf lonsrc = Null;
	lonsrc |= source.From3D(target.To3D(lonlat.TopLeft()) * source_ellip);
	lonsrc |= source.From3D(target.To3D(lonlat.TopRight()) * source_ellip);
	lonsrc |= source.From3D(target.To3D(lonlat.BottomLeft()) * source_ellip);
	lonsrc |= source.From3D(target.To3D(lonlat.BottomRight()) * source_ellip);
	return source.ProjectExtent(lonsrc);
}

double GisTransformGeneric::SourceDeviation(Pointf src1, Pointf src2) const
{
	Pointf ll1 = source.LonLat(src1), ll2 = source.LonLat(src2);
	Rectf ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
			* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformGeneric::TargetDeviation(Pointf trg1, Pointf trg2) const
{
	Pointf ll1 = target.LonLat(trg1), ll2 = target.LonLat(trg2);
	Rectf ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformGeneric::SourceExtentDeviation(const Rectf& src) const
{
	Rectf ll = source.LonLatExtent(src);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformGeneric::TargetExtentDeviation(const Rectf& trg) const
{
	Rectf ll = target.LonLatExtent(trg);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

Sizef GisTransformGeneric::SourceMinMaxRatio(const Rectf& s) const
{
	Rectf ll = source.LonLatExtent(s);
	Sizef sr = source.MinMaxRatio(ll);
	Sizef dr = target.MinMaxRatio(ll);
	return Sizef(dr.cx / sr.cy, dr.cy / sr.cx);
}

Sizef GisTransformGeneric::TargetMinMaxRatio(const Rectf& t) const
{
	Rectf ll = target.LonLatExtent(t);
	Sizef sr = source.MinMaxRatio(ll);
	Sizef dr = target.MinMaxRatio(ll);
	return Sizef(dr.cx / sr.cy, dr.cy / sr.cx);
}

GisTransform::GisTransform(const Nuller&) : data(new GisTransformIdentity) {}

GisTransform::GisTransform(GisCoords src, GisCoords dest)
{
	if(IsNull(src) && IsNull(dest))
		data = new GisTransformIdentity();
	else if(IsNull(src))
		data = new GisTransformProjectIdentity(dest);
	else if(IsNull(dest))
		data = new GisTransformProjectIdentity(src);
	else if(src.IsIdentity(dest))
		data = new GisTransformProjectIdentity(dest);
	else if(src.GetEllipsoid() == dest.GetEllipsoid())
		data = new GisTransformSameEllip(src, dest);
	else
		data = new GisTransformGeneric(src, dest);
}

bool GisTransform::Equals(const GisTransform& t) const
{
	return Source() == t.Source() && Target() == t.Target();
}

}
