#ifndef _GLCanvas_surface_h_
#define _GLCanvas_surface_h_

#include <plugin/Eigen/Eigen.h>

namespace Upp {

const double EPS_XYZ = 0.00001;

template<class T>
inline T avg(T a, T b) 			{return T(a+b)/2;}
template<class T>
inline T avg(T a, T b, T c)		{return T(a+b+c)/3;}
 
template<class T>
void Sort(T& a, T& b, T& c, T& d) {
	if (a > b) 
		Swap(a, b);
	if (c > d) 
		Swap(c, d);
	if (a > c) 
		Swap(a, c);
	if (b > d) 
		Swap(b, d);
	if (b > c) 
		Swap(b, c);
}

template<class T>
void Sort(T& a, T& b, T& c) {
	if (a > b) 
		Swap(a, b);
	if (a > c) 
		Swap(a, c);
	if (b > c) 
		Swap(b, c);
}

void GetTransform(Eigen::Affine3d &aff, double a_x, double a_y, double a_z, double c_x, double c_y, double c_z);
	
class Point3D : public Moveable<Point3D> {
public:
	double x, y, z;

	Point3D() {}
	Point3D(const Nuller&) {SetNull();}
	Point3D(const Point3D &p) : x(p.x), y(p.y), z(p.z) {}
	Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	
	void SetNull() 				{x = Null; y = 0;}
	bool IsNullInstance() const	{return IsNull(x);}
	
	Point3D(bool positive)	{x = Null; y = positive ? 1 : -1;}
	bool IsPosInf()			{return IsNull(x) && y == 1;}
	bool IsNegInf()			{return IsNull(x) && y == -1;}
	
	void Set(const Point3D &p) 					{x = p.x; y = p.y; z = p.z;}
	void Set(double _x, double _y, double _z) 	{x = _x;  y = _y;  z = _z;}
	
	String ToString() const { return FormatDouble(x) + "," + FormatDouble(y) + "," + FormatDouble(z); }
	
	inline bool IsSimilar(const Point3D &p, double similThres) const {
		if (abs(p.x - x) < similThres && abs(p.y - y) < similThres && abs(p.z - z) < similThres)
			return true;
		return false;
	}
	#pragma GCC diagnostic ignored "-Wattributes"
	friend bool operator==(const Point3D& a, const Point3D& b) {return a.IsSimilar(b, EPS_XYZ);}
	#pragma GCC diagnostic warning "-Wattributes"
	
	void Translate(double dx, double dy, double dz);
	void Rotate(const Eigen::Affine3d &quat);
	void Rotate(double da_x, double da_y, double da_z, double c_x, double c_y, double c_z);
		
	// Dot product or scalar product
	double dot(const Point3D& a) const {return x*a.x + y*a.y + z*a.z;}
	
	// Cross product or vector product
	friend Point3D operator%(const Point3D& a, const Point3D& b) {return Point3D(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);}
	
	friend Point3D operator+(const Point3D& a, const Point3D& b) {return Point3D(a.x+b.x, a.y+b.y, a.z+b.z);}
	friend Point3D operator-(const Point3D& a, const Point3D& b) {return Point3D(a.x-b.x, a.y-b.y, a.z-b.z);}
	friend Point3D operator*(const Point3D& a, double b) 		 {return Point3D(a.x*b, a.y*b, a.z*b);}

	double GetLength()	{return sqrt(x*x + y*y + z*z);}
	Point3D &Normalize() {
		double length = GetLength();
		
		if (length < 1e-10) 
			x = y = z = 0;
		else {
		    x = x/length;
		    y = y/length;
		    z = z/length;
		}
		return *this;
	}
	double Distance(const Point3D &p)  const {return sqrt(sqr(x-p.x) + sqr(y-p.y) + sqr(z-p.z));}
	double Manhattan(const Point3D &p) const {return abs(x-p.x) + abs(y-p.y) + abs(z-p.z);}
	double Manhattan() 				   const {return abs(x) + abs(y) + abs(z);}
	
	void SimX() {x = -x;}
	void SimY() {y = -y;}
	void SimZ() {z = -z;}
	
	void Mirror(const Point3D &p0) {
		x = 2*p0.x - x;
		y = 2*p0.y - y;
		z = 2*p0.z - z;
	}
	void Mirror() {
		x = -x;
		y = -y;
		z = -z;
	}
};

typedef Point3D Vector3D;

class Segment3D : public Moveable<Segment3D> {
public:
	Point3D from, to;
	
	Segment3D() {}
	Segment3D(const Nuller&) {SetNull();}
	Segment3D(const Point3D &_from, const Point3D &_to) : from(_from), to(_to) {}
	Segment3D(const Point3D &_from, const Vector3D &normal, double length) : from(_from) {
		to = Point3D(from.x + length*normal.x, from.y + length*normal.y, from.z + length*normal.z);
	}
	void SetNull() 				{from = Null;}
	bool IsNullInstance() const	{return IsNull(from);}
		
	void Set(const Point3D &_from, const Point3D &_to) {
		from.Set(_from);
		to.Set(_to);
	}
	void Set(const Point3D &_from, const Vector3D &normal, double length) {
		from.Set(_from);
		to.Set(from.x + length*normal.x, from.y + length*normal.y, from.z + length*normal.z);
	}
	void SimX() {
		from.SimX();
		to.SimX();
	}
	void SimY() {
		from.SimY();
		to.SimY();
	}
	void SimZ() {
		from.SimZ();
		to.SimZ();
	}
	double Length() const {return from.Distance(to);}
	double Dx()	const 	  {return to.x - from.x;}
	double Dy()	const 	  {return to.y - from.y;}	
	double Dz()	const 	  {return to.z - from.z;}
	
	void Mirror(const Point3D &p0) {
		from.Mirror(p0);
		to.Mirror(p0);
	}
	
	Vector3D Vector() {return Vector3D(to - from);}
	
	Point3D IntersectionPlaneX(double x);
	Point3D IntersectionPlaneY(double y);
	Point3D IntersectionPlaneZ(double z);
	
	Point3D Intersection(const Point3D &planePoint, const Vector3D &planeNormal);
	
	bool PointIn(const Point3D &p) const;
	bool SegmentIn(const Segment3D &in, double in_len) const;
	bool SegmentIn(const Segment3D &in) const;
	
	void Translate(double dx, double dy, double dz) {
		from.Translate(dx, dy, dz);
		to.Translate(dx, dy, dz);
	}
	
	void Rotate(const Eigen::Affine3d &quat) {
		from.Rotate(quat);
		to.Rotate(quat);
	}
};

Point3D GetCentroid(const Point3D &a, const Point3D &b);
Point3D GetCentroid(const Point3D &a, const Point3D &b, const Point3D &c);
Vector3D GetNormal(const Point3D &a, const Point3D &b, const Point3D &c);

Point3D Intersection(const Vector3D &lineVector, const Point3D &linePoint, const Vector3D &planeNormal, const Point3D &planePoint);

bool PointInSegment(const Point3D &p, const Segment3D &seg);
bool SegmentInSegment(const Segment3D &in, double in_len, const Segment3D &seg);
bool SegmentInSegment(const Segment3D &in, const Segment3D &seg);


template <typename T>
inline T const& maxNotNull(T const& a, T const& b) {
	if (IsNull(a))
		return b;
	else if (IsNull(b))
		return a;
	else
    	return a > b ? a : b;
}

template <typename T>
inline T const& minNotNull(T const& a, T const& b) {
	if (IsNull(a))
		return b;
	else if (IsNull(b))
		return a;
	else
    	return a < b ? a : b;
}

class Panel : public MoveableAndDeepCopyOption<Panel> {
public:
	int id[4];
	Point3D centroid0, centroid1, centroidPaint;
	Point3D normal0, normal1, normalPaint;
	double surface0, surface1;

	Panel() {}
	Panel(const Panel &orig, int) {
		memcpy(id, orig.id, sizeof(orig.id));
		centroid0 = orig.centroid0;
		centroid1 = orig.centroid1;
		centroidPaint = orig.centroidPaint;
		normal0 = orig.normal0;
		normal1 = orig.normal1;
		normalPaint = orig.normalPaint;
		surface0 = orig.surface0;
		surface1 = orig.surface1;
	}
	bool operator==(const Panel &p) const {
		int id0 = id[0], id1 = id[1], id2 = id[2], id3 = id[3];
		int pid0 = p.id[0], pid1 = p.id[1], pid2 = p.id[2], pid3 = p.id[3];
		if (id0 + id1 + id2 + id3 != pid0 + pid1 + pid2 + pid3)
			return false;
		Sort(id0, id1, id2, id3);
		Sort(pid0, pid1, pid2, pid3);
		if (id0 == pid0 && id1 == pid1 && id2 == pid2 && id3 == pid3)
			return true;
		return false;
	}
	inline void Swap() {
		if (IsTriangle())
			Upp::Swap(id[1], id[2]);
		else
			Upp::Swap(id[1], id[3]);
	}
	inline bool IsTriangle() const	{return id[0] == id[3];}
	void RedirectTriangles();
	void ShiftNodes(int shift);
	inline int GetNumNodes() const	{return IsTriangle() ? 3 : 4;}
	bool FirstNodeIs0(int in0, int in1) const;
	static double GetSurface(const Point3D &p0, const Point3D &p1, const Point3D &p2);
	
	String ToString() const { return FormatInt(id[0]) + "," + FormatInt(id[1]) + "," + FormatInt(id[2]) + "," + FormatInt(id[3]); }
};

class Segment : public MoveableAndDeepCopyOption<Segment> {
public:
	Segment() {}
	Segment(const Segment &orig, int) {
		inode0 = orig.inode0;
		inode1 = orig.inode1;
		panels = clone(orig.panels);
	}
	int inode0, inode1;
	Upp::Index<int> panels;
};

class VolumeEnvelope : MoveableAndDeepCopyOption<VolumeEnvelope> {
public:
	VolumeEnvelope() {Reset();}
	void Reset() 	 {maxX = minX = maxY = minY = maxZ = minZ = Null;}
	VolumeEnvelope(const VolumeEnvelope &orig, int) {
		maxX = orig.maxX;
		minX = orig.minX;
		maxY = orig.maxY;
		minY = orig.minY;
		maxZ = orig.maxZ;
		minZ = orig.minZ;
	}	
	
	void MixEnvelope(VolumeEnvelope &env);
	double Max()	{return max(max(max(abs(maxX), abs(minX)), max(abs(maxY), abs(minY))), max(abs(maxZ), abs(minZ)));}
	double LenRef()	{return max(max(maxX - minX, maxY - minY), maxZ - minZ);}
	
	double maxX, minX, maxY, minY, maxZ, minZ;
};

class Surface : DeepCopyOption<Surface> {
public:
	Surface() {}
	Surface(const Surface &surf, int);
		
	void Clear();
	bool IsEmpty();
	
	Vector<Point3D> nodes;
	Vector<Panel> panels;
	
	int GetNumNodes() 	{return nodes.GetCount();}
	int GetNumPanels() 	{return panels.GetCount();}
	
	Vector<Segment3D> skewed;
	Vector<Segment3D> segWaterlevel, segTo1panel, segTo3panel;
	
	VolumeEnvelope env;
	
	String Heal(bool basic, Function <void(String, int pos)> Status = Null);
	void Orient();
	void Image(int axis);
	void GetLimits(); 
	void GetPanelParams();
	String CheckErrors() const;
	double GetWaterPlaneArea();
	void GetSurface();
	void GetSegments();
	void GetVolume();
	Point3D GetCenterOfBuoyancy();
	void GetHydrostaticStiffness(Eigen::MatrixXd &c, const Point3D &cb, double rho, const Point3D &cg, double mass, double g);
	
	void CutX(const Surface &orig, double factor = 1);
	void CutY(const Surface &orig, double factor = 1);
	void CutZ(const Surface &orig, double factor = 1);
	
	void Join(const Surface &orig);
	Vector<Vector<int>> GetPanelSets(Function <void(String, int pos)> Status);
	
	void TriangleToQuad(int ip);
	void TriangleToQuad(Panel &pan);
		
	void Translate(double dx, double dy, double dz);
	void Rotate(double ax, double ay, double az, double _c_x, double _c_y, double _c_z);
	
	bool healing{false};
	int numTriangles, numBiQuads, numMonoQuads;
	Vector<Segment> segments;
	int numDupPan, numDupP, numSkewed, numUnprocessed;
	
	double surface = -1, volume = -1, volumex = -1, volumey = -1, volumez = -1;
	double avgFacetSideLen;
	
	void DeployXSymmetry();
	void DeployYSymmetry();
	
	Surface &SelPanels(Vector<int> &_selPanels) {selPanels = pick(_selPanels);	return *this;}
	Surface &SelNodes(Vector<int> &_selNodes) 	{selNodes = pick(_selNodes);	return *this;}
	const Vector<int> &GetSelPanels() const		{return selPanels;}
	const Vector<int> &GetSelNodes() const		{return selNodes;}
	
	void AddNode(Point3D &p);
	int FindNode(Point3D &p);
	
	void AddFlatPanel(double lenX, double lenY, double panelWidth);
	void AddRevolution(Vector<Pointf> &points, double panelWidth);
	void AddPolygonalPanel(Vector<Pointf> &bound, double panelWidth);
	
	static int RemoveDuplicatedPanels(Vector<Panel> &_panels);
	static int RemoveTinyPanels(Vector<Panel> &_panels);
	static int RemoveDuplicatedPointsAndRenumber(Vector<Panel> &_panels, Vector<Point3D> &_nodes);
	static void DetectTriBiP(Vector<Panel> &panels) {int dum;	DetectTriBiP(panels, dum, dum, dum);}
	
protected:
	struct PanelPoints {
		Point3D data[4];
	};
	void SetPanelPoints(Array<PanelPoints> &pans);
	
private:
	inline bool CheckId(int id) {return id >= 0 && id < nodes.GetCount()-1;}
	bool side = true;
	
	static void DetectTriBiP(Vector<Panel> &panels, int &numTri, int &numBi, int &numP);
	int FixSkewed();
	int SegmentInSegments(int iseg) const;
	void AnalyseSegments(double zTolerance);
	void AddSegment(int ip0, int ip1, int ipanel);
	bool ReorientPanels0(bool side);
	void ReorientPanel(int ip);
	bool GetLowest(int &iLowSeg, int &iLowPanel);
	bool SameOrderPanel(int ip0, int ip1, int in0, int in1);
	static int PanelGetNumNodes(const Vector<Panel> &_panels, int ip) {return _panels[ip].GetNumNodes();}
	bool IsPanelTriangle(int ip) 	{return panels[ip].IsTriangle();}
	void GetPanelParams(Panel &panel) const;
	
	Vector<int> selPanels, selNodes;
};


void LoadStl(String fileName, Surface &surf, bool &isText, String &header);
void SaveStlTxt(String fileName, const Surface &surf);
void SaveStlBin(String fileName, const Surface &surf);


}

#endif
