#include <Core/Core.h>

#include <plugin/Eigen/Eigen.h>
#include <Surface/Surface.h>
#include <Geom/Geom.h>
#include <Functions4U/Functions4U.h>

namespace Upp {
using namespace Eigen;

Point3D GetCentroid(const Point3D &a, const Point3D &b) {
	return Point3D(avg(a.x, b.x), avg(a.y, b.y), avg(a.z, b.z));	
}

Point3D GetCentroid(const Point3D &a, const Point3D &b, const Point3D &c) {
	return Point3D(avg(a.x, b.x,c.x), avg(a.y, b.y, c.y), avg(a.z, b.z, c.z));	
}

Vector3D GetNormal(const Point3D &a, const Point3D &b, const Point3D &c) {
	return Vector3D((a - b) % (b - c)).Normalize();
}

Point3D Intersection(const Vector3D &lineVector, const Point3D &linePoint, const Point3D &planePoint, const Vector3D &planeNormal) {
	Vector3D diff = planePoint - linePoint;
	double prod1 = diff.dot(planeNormal);
	double prod2 = lineVector.dot(planeNormal);
	if (abs(prod2) < EPS_XYZ)
		return Null;
	double factor = prod1/prod2;
	return linePoint + lineVector*factor;	
}

void Point3D::Translate(double dx, double dy, double dz) {
	x += dx;
	y += dy;
	z += dz;
}

void Point3D::Rotate(double da_x, double da_y, double da_z, double c_x, double c_y, double c_z) {
	Affine3d aff;
	GetTransform(aff, da_x, da_y, da_z, c_x, c_y, c_z);
	Rotate(aff);
}

void Point3D::Rotate(const Affine3d &quat) {
	Vector3d pnt0(x, y, z);	
	Vector3d pnt = quat * pnt0;

	x = pnt[0];
	y = pnt[1];
	z = pnt[2];
}

void GetTransform(Affine3d &aff, double a_x, double a_y, double a_z, double c_x, double c_y, double c_z) {
	Vector3d c(c_x, c_y, c_z);	
	aff =	Translation3d(c) *
			AngleAxisd(a_x*M_PI/180, Vector3d::UnitX()) *
		 	AngleAxisd(a_y*M_PI/180, Vector3d::UnitY()) *
		 	AngleAxisd(a_z*M_PI/180, Vector3d::UnitZ()) *
		 	Translation3d(-c);
}

Point3D Segment3D::IntersectionPlaneX(double x) {
	if (from.x >= x && to.x >= x)
		return Point3D(true);
	if (from.x <= x && to.x <= x)
		return Point3D(false);
	
	double factor = (x - from.x)/(to.x - from.x);
	return Point3D(x, from.y + (to.y - from.y)*factor, from.z + (to.z - from.z)*factor);
}

Point3D Segment3D::IntersectionPlaneY(double y) {
	if (from.y >= y && to.y >= y)
		return Point3D(true);
	if (from.y <= y && to.y <= y)
		return Point3D(false);
	
	double factor = (y - from.y)/(to.y - from.y);
	return Point3D(from.x + (to.x - from.x)*factor, y, from.z + (to.z - from.z)*factor);
}

Point3D Segment3D::IntersectionPlaneZ(double z) {
	if (from.z >= z && to.z >= z)
		return Point3D(true);
	if (from.z <= z && to.z <= z)
		return Point3D(false);
	
	double factor = (z - from.z)/(to.z - from.z);
	return Point3D(from.x + (to.x - from.x)*factor, from.y + (to.y - from.y)*factor, z);
}

Point3D Segment3D::Intersection(const Point3D &planePoint, const Vector3D &planeNormal) {
	Vector3D vector = Vector();
	Vector3D diff = planePoint - from;
	double prod1 = diff.dot(planeNormal);
	double prod2 = vector.dot(planeNormal);
	if (abs(prod2) < EPS_XYZ)
		return Null;
	double factor = prod1/prod2;
	if (factor >= 1)
		return Point3D(true);
	if (factor <= 0)
		return Point3D(false);
	return from + vector*factor;	
}

bool Segment3D::PointIn(const Point3D &p) const {
	return PointInSegment(p, *this);
}

bool Segment3D::SegmentIn(const Segment3D &in) const {
	return SegmentInSegment(in, *this);
}

bool Segment3D::SegmentIn(const Segment3D &in, double in_len) const {
	return SegmentInSegment(in, in_len, *this);
}


bool PointInSegment(const Point3D &p, const Segment3D &seg) {
	double dpa = p.Distance(seg.from);
	double dpb = p.Distance(seg.to);
	double dab = seg.Length();
	
	return abs(dpa + dpb - dab) < EPS_XYZ;
}

bool SegmentInSegment(const Segment3D &in, double in_len, const Segment3D &seg) {
	double seg_len = seg.Length();
	
	double seg_from_in_from = seg.from.Distance(in.from);
	double in_to_seg_to = in.to.Distance(seg.to);
	if (abs(seg_from_in_from + in_len + in_to_seg_to - seg_len) < EPS_XYZ)
		return true;

	double seg_from_in_to = seg.from.Distance(in.to);
	double in_from_seg_to = in.from.Distance(seg.to);
	if (abs(seg_from_in_to + in_len + in_from_seg_to - seg_len) < EPS_XYZ)
		return true;
	
	return false;
}

bool SegmentInSegment(const Segment3D &in, const Segment3D &seg) {
	return SegmentInSegment(in, in.Length(), seg);
}


void Surface::Clear() {
	nodes.Clear();
	panels.Clear();
	skewed.Clear();
	segWaterlevel.Clear();
	segTo1panel.Clear();
	segTo3panel.Clear();
	segments.Clear();
	//x = y = z = a_x = a_y = a_z = 0;
	selPanels.Clear();
	selNodes.Clear();
}

Surface::Surface(const Surface &orig, int) {
	healing = orig.healing;
	numTriangles = orig.numTriangles;
	numBiQuads = orig.numBiQuads;
	numMonoQuads = orig.numMonoQuads;
	
	panels = clone(orig.panels);
	nodes = clone(orig.nodes);
	skewed = clone(orig.skewed);
	segWaterlevel = clone(orig.segWaterlevel);
	segTo1panel = clone(orig.segTo1panel);
	segTo3panel = clone(orig.segTo3panel);
	segments = clone(orig.segments);
	
	env = clone(orig.env);
	
	surface = orig.surface;
	volume = orig.volume;
	volumex = orig.volumex;
	volumey = orig.volumey;
	volumez = orig.volumez;
}

bool Surface::IsEmpty() {
	return nodes.IsEmpty();
}

int Surface::FixSkewed() {	
	int num = 0;
	for (int i = 0; i < panels.GetCount(); ++i) {
		int &id0 = panels[i].id[0];
		int &id1 = panels[i].id[1];
		int &id2 = panels[i].id[2];
		int &id3 = panels[i].id[3];
		Point3D &p0 = nodes[id0];
		Point3D &p1 = nodes[id1];
		Point3D &p2 = nodes[id2];
		Point3D &p3 = nodes[id3];
		if (id0 != id3) {		// Is not triangular 
			Vector3D normal301 = GetNormal(p3, p0, p1);
			Vector3D normal012 = GetNormal(p0, p1, p2);
			Vector3D normal123 = GetNormal(p1, p2, p3);
			Vector3D normal230 = GetNormal(p2, p3, p0);
			double d0  = normal301.Manhattan();
			double d01 = normal301.Manhattan(normal012);
			double d02 = normal301.Manhattan(normal123);
			double d03 = normal301.Manhattan(normal230);
			
			int numg = 0;
			if (d0 < d01)
				numg++;
			if (d0 < d02)
				numg++;
			if (d0 < d03)
				numg++;	 
			if (numg > 1) {
				skewed << Segment3D(p0, p1) << Segment3D(p1, p2) << Segment3D(p2, p3) << Segment3D(p3, p0);
				
				if (d0 < d01)
					Swap(panels[i].id[1], panels[i].id[2]);
				else if (d0 < d02)
					Swap(panels[i].id[2], panels[i].id[3]);
				num++;
			}
		}
	}	
	return num;
}

void Surface::DetectTriBiP(Vector<Panel> &panels, int &numTri, int &numBi, int &numP) {
	numTri = numBi = numP = 0;
	for (int i = panels.GetCount()-1; i >= 0; --i) {
		Panel &panel = panels[i];
		Upp::Index<int> ids;
		ids.FindAdd(panel.id[0]);
		ids.FindAdd(panel.id[1]);
		ids.FindAdd(panel.id[2]);
		ids.FindAdd(panel.id[3]);
		if (ids.GetCount() == 4)
			;
		else if (ids.GetCount() == 3) {
			numTri++;
			panel.id[0] = ids[0];
			panel.id[1] = ids[1];
			panel.id[2] = ids[2];	
			//TriangleToQuad(i);
		} else if (ids.GetCount() == 2) {
			numBi++;
			panels.Remove(i, 1);
		} else {
			numP++;
			panels.Remove(i, 1);
		}
	}
}

void Surface::TriangleToQuad(Panel &pan) {
	panels << pan;
	TriangleToQuad(panels.GetCount() - 1);
}

void Surface::TriangleToQuad(int ipanel) {
	Panel &pan00 = panels[ipanel];
	int id0 = pan00.id[0];
	int id1 = pan00.id[1];
	int id2 = pan00.id[2];
	Point3D &p0 = nodes[id0];
	Point3D &p1 = nodes[id1];
	Point3D &p2 = nodes[id2];
		
	Point3D p012= GetCentroid(p0, p1, p2);	nodes.Add(p012);	int id012= nodes.GetCount()-1;
	Point3D p01 = GetCentroid(p0, p1);		nodes.Add(p01);		int id01 = nodes.GetCount()-1;
	Point3D p12 = GetCentroid(p1, p2);		nodes.Add(p12);		int id12 = nodes.GetCount()-1;
	Point3D p20 = GetCentroid(p2, p0);		nodes.Add(p20);		int id20 = nodes.GetCount()-1;
	
	panels.Remove(ipanel, 1);
	Panel &pan0 = panels.Add();	pan0.id[0] = id0;	pan0.id[1] = id01;	pan0.id[2] = id012;	pan0.id[3] = id20;
	Panel &pan1 = panels.Add();	pan1.id[0] = id01;	pan1.id[1] = id1;	pan1.id[2] = id12;	pan1.id[3] = id012;
	Panel &pan2 = panels.Add();	pan2.id[0] = id012;	pan2.id[1] = id12;	pan2.id[2] = id2;	pan2.id[3] = id20;
}

int Surface::RemoveDuplicatedPanels(Vector<Panel> &_panels) {		
	int num = 0;
	for (int i = 0; i < _panels.GetCount()-1; ++i) {
		Panel &panel = _panels[i];
		for (int j = _panels.GetCount()-1; j >= i+1; --j) {
			if (panel == _panels[j]) {
				num++;
				_panels.Remove(j, 1);
			}
		}
	}
	return num;
}

int Surface::RemoveTinyPanels(Vector<Panel> &_panels) {		
	int num = 0;
	double avgsurface = 0;
	for (int i = 0; i < _panels.GetCount(); ++i) 
		avgsurface += _panels[i].surface0 + _panels[i].surface1;
	avgsurface /= _panels.GetCount();
	double tiny = avgsurface/1000000;
			
	for (int i = _panels.GetCount()-1; i >= 0; --i) {
		double surface = _panels[i].surface0 + _panels[i].surface1;
		if (surface < tiny) {
			num++;
			_panels.Remove(i, 1);
		}
	}
	return num;
}

int Surface::RemoveDuplicatedPointsAndRenumber(Vector<Panel> &_panels, Vector<Point3D> &_nodes) {
	int num = 0;
	
	// Detect duplicated points in nodes
	double similThres = EPS_XYZ;
	Upp::Index<int> duplic, goods;
	for (int i = 0; i < _nodes.GetCount()-1; ++i) {
		if (duplic.Find(i) >= 0)
			continue;
		for (int j = i+1; j < _nodes.GetCount(); ++j) {
			if (_nodes[i].IsSimilar(_nodes[j], similThres)) {
				duplic << j;
				goods << i;
				num++;
			}
		}
	}
	
	// Replace duplicated points with good ones in panels
	for (int i = 0; i < _panels.GetCount(); ++i) {
		for (int j = 0; j < 4; ++j) {
			int &id = _panels[i].id[j];
			int pos = duplic.Find(id);
			if (pos >= 0)
				id = goods[pos];
		}
	}
	
	// Find unused nodes
	Vector<int> newId;
	newId.SetCount(_nodes.GetCount());
	int avId = 0;
	for (int i = 0; i < _nodes.GetCount(); ++i) {
		bool found = false;
		for (int ip = 0; ip < _panels.GetCount() && !found; ++ip) {
			int numP = PanelGetNumNodes(_panels, ip);
			for (int j = 0; j < numP; ++j) {
				if (_panels[ip].id[j] == i) {
					found = true;
					break;
				}
			}
		}
		if (!found)
			newId[i] = Null;	// Remove unused nodes
		else if (duplic.Find(i) >= 0)
			newId[i] = Null;	
 		else {	
			newId[i] = avId;
			avId++;
		} 
	}
	
	// Remove duplicated nodes
	for (int i = _nodes.GetCount()-1; i >= 0; --i) {
		if (IsNull(newId[i]))
			_nodes.Remove(i, 1);
	}
	
	// Renumber panels
	for (int i = 0; i < _panels.GetCount(); ++i) {
		for (int j = 0; j < 4; ++j) {
			int& id = _panels[i].id[j];
			id = newId[id];
		}
	}
	return num;
}
	
void Surface::AddSegment(int inode0, int inode1, int ipanel) {
	ASSERT(!IsNull(inode0));
	ASSERT(!IsNull(inode1));
	for (int i = 0; i < segments.GetCount(); ++i) {
		if ((segments[i].inode0 == inode0 && segments[i].inode1 == inode1) ||
			(segments[i].inode1 == inode0 && segments[i].inode0 == inode1)) {
			segments[i].panels << ipanel;
			return;
		}
	}
	Segment &sg = segments.Add();
	sg.inode0 = inode0;
	sg.inode1 = inode1;
	sg.panels << ipanel;
}

int Surface::SegmentInSegments(int iseg) const {
	Segment3D seg(nodes[segments[iseg].inode0], nodes[segments[iseg].inode1]);
	double lenSeg = seg.Length();
			
	for (int i = 0; i < segments.GetCount(); ++i) {
		if (i != iseg) {
			const Segment &segment = segments[i];
			Segment3D is(nodes[segment.inode0], nodes[segment.inode1]);
			if (is.SegmentIn(seg, lenSeg))		
				return i;
			if (seg.SegmentIn(is))
				return i;
		}
	}
	return -1;
}

void Surface::GetSegments() {
	segments.Clear();
	
	for (int i = 0; i < panels.GetCount(); ++i) {
		int id0 = panels[i].id[0];
		int id1 = panels[i].id[1];
		int id2 = panels[i].id[2];
		int id3 = panels[i].id[3];
		AddSegment(id0, id1, i);
		AddSegment(id1, id2, i);
		if (IsPanelTriangle(i)) 
			AddSegment(id2, id0, i);
		else {
			AddSegment(id2, id3, i);
			AddSegment(id3, id0, i);
		}
	}
}
	
void Surface::AnalyseSegments(double zTolerance) {
	GetSegments();
	
	for (int i = 0; i < segments.GetCount(); ++i) {
		int inode0 = segments[i].inode0;
		int inode1 = segments[i].inode1;
		
		if (inode0 >= nodes.GetCount())
			throw Exc(Format(t_("Node %d is pointing out of scope"), inode0+1));	
		if (inode1 >= nodes.GetCount())
			throw Exc(Format(t_("Node %d is pointing out of scope"), inode1+1));
		
		int num = segments[i].panels.GetCount();
				
		if (num == 1) {
			if (nodes[inode0].z >= zTolerance && nodes[inode1].z >= zTolerance)
				segWaterlevel << Segment3D(nodes[inode0], nodes[inode1]);
			else {
				if (SegmentInSegments(i) < 0)	
					segTo1panel << Segment3D(nodes[inode0], nodes[inode1]);
			}
		} else if (num > 2)
			segTo3panel << Segment3D(nodes[inode0], nodes[inode1]);
	}
}

bool Surface::GetLowest(int &iLowSeg, int &iLowPanel) {	// Get the lowest panel with normal non horizontal
	iLowSeg = iLowPanel = Null;
	double zLowSeg = DBL_MAX;
	for (int i = 0; i < segments.GetCount(); ++i) {
		const Segment &seg = segments[i];
		if (seg.panels.GetCount() == 2) {
			for (int ip = 0; ip < seg.panels.GetCount(); ++ip) {
				if (panels[seg.panels[ip]].normal0.z != 0) {
					double zz = max(nodes[seg.inode0].z, nodes[seg.inode1].z);
					if (zz < zLowSeg) {
						zLowSeg = zz;
						iLowSeg = i;
						iLowPanel = ip;
					}
				}
			}
		}
	}
	if (!IsNull(iLowSeg))
		return true;
	for (int i = 0; i < segments.GetCount(); ++i) {
		const Segment &seg = segments[i];
		if (seg.panels.GetCount() == 2) {
			for (int ip = 0; ip < seg.panels.GetCount(); ++ip) {
				double zz = min(nodes[seg.inode0].z, nodes[seg.inode1].z);
				if (zz < zLowSeg) {
					zLowSeg = zz;
					iLowSeg = i;
					iLowPanel = ip;
				}
			}
		}
	}
	return !IsNull(iLowSeg);
}
	
bool Surface::ReorientPanels0(bool _side) {
	numUnprocessed = -1;
	
	int iLowSeg, iLowPanel;
	if (!GetLowest(iLowSeg, iLowPanel))
		return false;
	
	// Reorient lowest panel downwards to be the seed
	int ip = segments[iLowSeg].panels[iLowPanel];
	if (panels[ip].normal0.z != 0) {
		if (_side && panels[ip].normal0.z > 0 || !_side && panels[ip].normal0.z < 0)
			ReorientPanel(ip);
	} else if (panels[ip].normal0.x != 0) {
		if (_side && panels[ip].normal0.x > 0 || !_side && panels[ip].normal0.x < 0)
			ReorientPanel(ip);
	} else {
		if (_side && panels[ip].normal0.y > 0 || !_side && panels[ip].normal0.y < 0)
			ReorientPanel(ip);
	}
	
	Vector<int> panelStack;
	Upp::Index<int> panelProcessed;
	
	panelStack << ip;
	while (!panelStack.IsEmpty()) {
		int id = panelStack.GetCount() - 1;
		int ipp = panelStack[id];
		panelStack.Remove(id, 1);
		panelProcessed << ipp;
		
		for (int is = 0; is < segments.GetCount(); ++is) {
			const Upp::Index<int> &segPanels = segments[is].panels;
			if (segPanels.Find(ipp) >= 0) {
				for (int i = 0; i < segPanels.GetCount(); ++i) {
					int ipadyac = segPanels[i];
					if (ipadyac != ipp && panelProcessed.Find(ipadyac) < 0) {
						panelStack << ipadyac;
						if (!SameOrderPanel(ipp, ipadyac, segments[is].inode0, segments[is].inode1))
							ReorientPanel(ipadyac);
					}
				}
			}
		}
	}
	
	numUnprocessed = panels.GetCount() - panelProcessed.GetCount();

	return true;
}

Vector<Vector<int>> Surface::GetPanelSets(Function <void(String, int pos)> Status) {
	Vector<Vector<int>> ret;
	
	double zTolerance = -0.1;
	AnalyseSegments(zTolerance);	
	
	Index<int> allPanels;
	for (int i = 0; i < panels.GetCount(); ++i)
		allPanels << i;
	
	while (allPanels.GetCount() > 0) {
		Vector<int> panelStack;
		Upp::Index<int> panelProcessed;
	
		panelStack << allPanels[0];
		while (!panelStack.IsEmpty()) {
			int id = panelStack.GetCount() - 1;
			int ipp = panelStack[id];
			panelStack.Remove(id, 1);
			int iall = allPanels.Find(ipp);
			if (iall < 0)
				continue;
			allPanels.Remove(iall);
			panelProcessed << ipp;
			
			for (int is = 0; is < segments.GetCount(); ++is) {
				const Upp::Index<int> &segPanels = segments[is].panels;
				if (segPanels.Find(ipp) >= 0) {
					for (int i = 0; i < segPanels.GetCount(); ++i) {
						int ipadyac = segPanels[i];
						if (ipadyac != ipp && panelProcessed.Find(ipadyac) < 0) 
							panelStack << ipadyac;
					}
				}
			}
		}
		ret << panelProcessed.PickKeys();
		panelProcessed.Clear();
		Status(Format(t_("Split mesh #%d"), ret.GetCount()), 0);
	}
	return ret;
}

void Surface::ReorientPanel(int ip) {
	panels[ip].Swap();
	panels[ip].normal0.Mirror();
	if (panels[ip].IsTriangle()) 
		panels[ip].normal1.Mirror();
}

bool Panel::FirstNodeIs0(int in0, int in1) const {
	if (IsTriangle()) {
		if ((id[0] == in0 && id[1] == in1) ||
			(id[1] == in0 && id[2] == in1) ||
			(id[2] == in0 && id[0] == in1))
			return true;
		else
			return false;
	} else {
		if ((id[0] == in0 && id[1] == in1) ||
			(id[1] == in0 && id[2] == in1) ||
			(id[2] == in0 && id[3] == in1) ||
			(id[3] == in0 && id[0] == in1))
			return true;
		else
			return false;
	}
}

void Panel::RedirectTriangles() {
	int shift = 0;
	if (id[0] == id[1])
		shift = -1;
	else if (id[1] == id[2])
		shift = -2;
	else if (id[2] == id[3])
		shift = 1;
	else
		return;
	ShiftNodes(shift);
}

void Panel::ShiftNodes(int shift) {
	int id_0 = id[0];
	int id_1 = id[1];
	int id_2 = id[2];
	int id_3 = id[3];
	if (shift == 1) {
		id[1] = id_0;
		id[2] = id_1;
		id[3] = id_2;
		id[0] = id_3;
	} else if (shift == -1) { 
		id[0] = id_1;
		id[1] = id_2;
		id[2] = id_3;
		id[3] = id_0;
	} else if (shift == -2) { 
		id[0] = id_2;
		id[1] = id_3;
		id[2] = id_0;
		id[3] = id_1;
	} else
		throw t_("ShiftNodes value not implemented");
}

double Panel::GetSurface(const Point3D &p0, const Point3D &p1, const Point3D &p2) {
	double l01 = p0.Distance(p1);
	double l12 = p1.Distance(p2);
	double l02 = p0.Distance(p2);

	double s = (l01 + l12 + l02)/2;
	return sqrt(max(s*(s - l01)*(s - l12)*(s - l02), 0.)); 
}

bool Surface::SameOrderPanel(int ip0, int ip1, int in0, int in1) {
	bool first0in0 = panels[ip0].FirstNodeIs0(in0, in1);
	bool first1in0 = panels[ip1].FirstNodeIs0(in0, in1);
	
	return first0in0 != first1in0;
}

String Surface::Heal(bool basic, Function <void(String, int pos)> Status) {
	String ret;
	
	if (basic) {
		Status(t_("Removing duplicated panels (pass 1)"), 25);
		numDupPan = RemoveDuplicatedPanels(panels);
		
		Status(t_("Removing duplicated points"), 50);
		numDupP = RemoveDuplicatedPointsAndRenumber(panels, nodes);
		if (numDupP > 0) 
			ret << "\n" << Format(t_("Removed %d duplicated points"), numDupP);	
	
		Status(t_("Removing duplicated panels (pass 2)"), 75);
		numDupPan += RemoveDuplicatedPanels(panels);	// Second time after duplicated points
		if (numDupPan > 0) 
			ret << "\n" << Format(t_("Removed %d duplicated panels"), numDupPan);
	} else {	
		Status(t_("Detecting triangles and wrong panels"), 40);
		DetectTriBiP(panels, numTriangles, numBiQuads, numMonoQuads);
		if (numTriangles > 0)
			ret << "\n" << Format(t_("Fixed %d triangles"), numTriangles);
		if (numBiQuads > 0)
			ret << "\n" << Format(t_("Removed %d 2 points quads"), numBiQuads);
		if (numMonoQuads > 0)
			ret << "\n" << Format(t_("Removed %d 1 points quads"), numMonoQuads);
		
		Status(t_("Removing tiny panels"), 45);
		RemoveTinyPanels(panels);
		
		Status(t_("Removing duplicated panels (pass 1)"), 55);
		numDupPan = RemoveDuplicatedPanels(panels);
		
		Status(t_("Fixing skewed panels"), 60);
		numSkewed = FixSkewed();
		if (numSkewed > 0) 
			ret << "\n" << Format(t_("Fixed %d skewed panels"), numSkewed);
	
		Status(t_("Removing duplicated points"), 65);
		numDupP = RemoveDuplicatedPointsAndRenumber(panels, nodes);
		if (numDupP > 0) 
			ret << "\n" << Format(t_("Removed %d duplicated points"), numDupP);	
	
		Status(t_("Removing duplicated panels (pass 2)"), 70);
		numDupPan += RemoveDuplicatedPanels(panels);	// Second time after duplicated points
		if (numDupPan > 0) 
			ret << "\n" << Format(t_("Removed %d duplicated panels"), numDupPan);
	
		Status(t_("Analysing water tightness"), 75);
		double zTolerance = -0.1;
		AnalyseSegments(zTolerance);
		ret << "\n" << Format(t_("%d segments, %d water level, %d water leak and %d multipanel"), 
									segments.GetCount(), segWaterlevel.GetCount(), 
									segTo1panel.GetCount(), segTo3panel.GetCount());
		
		Status(t_("Reorienting panels water side"), 80);
		if (!ReorientPanels0(true))
			ret << "\n" << t_("Failed to reorient panels to water side");
		else if (numUnprocessed > 0)
			ret << "\n" << Format(t_("%d panels not reoriented. Body contains separated surfaces"), numUnprocessed);
		
		healing = true;
	}
	return ret;
}

void Surface::Orient() {
	GetSegments();
	ReorientPanels0(side);
	side = !side;
}		
		
void Surface::Image(int axis) {
	for (int i = 0; i < nodes.GetCount(); ++i) {
		Point3D &node = nodes[i];
		if (axis == 0)
			node.x = -node.x;
		else if (axis == 1)
			node.y = -node.y;
		else
			node.z = -node.z;
	}
	for (int i = 0; i < panels.GetCount(); ++i) 
		ReorientPanel(i);
}
	
void Surface::GetLimits() {
	env.maxX = env.maxY = env.maxZ = -DBL_MAX; 
	env.minX = env.minY = env.minZ = DBL_MAX;
	for (int i = 0; i < nodes.GetCount(); ++i) {
		env.maxX = max(env.maxX, nodes[i].x);
		env.minX = min(env.minX, nodes[i].x);
		env.maxY = max(env.maxY, nodes[i].y);
		env.minY = min(env.minY, nodes[i].y);
		env.maxZ = max(env.maxZ, nodes[i].z);
		env.minZ = min(env.minZ, nodes[i].z);
	}
}

void Surface::GetPanelParams(Panel &panel) const {
	panel.RedirectTriangles();
	
	const Point3D &p0 = nodes[panel.id[0]];
	const Point3D &p1 = nodes[panel.id[1]];
	const Point3D &p2 = nodes[panel.id[2]];
	const Point3D &p3 = nodes[panel.id[3]];
	
	panel.surface0 = panel.GetSurface(p0, p1, p2);
	panel.centroid0 = GetCentroid(p0, p1, p2);
	panel.normal0 = GetNormal(p0, p1, p2);
	if (!panel.IsTriangle()) {
		panel.surface1 = panel.GetSurface(p2, p3, p0);
		panel.centroid1 = GetCentroid(p2, p3, p0);
		panel.normal1 = GetNormal(p2, p3, p0);
		double surf = panel.surface0 + panel.surface1;
		panel.centroidPaint.x = (panel.centroid0.x*panel.surface0 + panel.centroid1.x*panel.surface1)/surf;
		panel.centroidPaint.y = (panel.centroid0.y*panel.surface0 + panel.centroid1.y*panel.surface1)/surf;
		panel.centroidPaint.z = (panel.centroid0.z*panel.surface0 + panel.centroid1.z*panel.surface1)/surf;
		panel.normalPaint.x = (panel.normal0.x*panel.surface0 + panel.normal1.x*panel.surface1)/surf;
		panel.normalPaint.y = (panel.normal0.y*panel.surface0 + panel.normal1.y*panel.surface1)/surf;
		panel.normalPaint.z = (panel.normal0.z*panel.surface0 + panel.normal1.z*panel.surface1)/surf;
		panel.normalPaint.Normalize();
	} else {
		panel.surface1 = 0;
		panel.centroidPaint = panel.centroid1 = panel.centroid0;
		panel.normalPaint = panel.normal1 = panel.normal0;
	}
}

String Surface::CheckErrors() const {
	for (int ip = 0; ip < panels.GetCount(); ++ip) {
		const Panel &panel = panels[ip];
		for (int i = 0; i < 4; ++i) {
			if (panel.id[i] >= nodes.GetCount())
				return Format(t_("Node %d in panel %d [%d] does not exist"), panel.id[i]+1, ip+1, i+1);
		}
	}
	return Null;
}
		
void Surface::GetPanelParams() {
	for (int ip = 0; ip < panels.GetCount(); ++ip) {
		Panel &panel = panels[ip];
		GetPanelParams(panel);
	}	
}

void Surface::GetSurface() {
	surface = 0;
	for (int ip = 0; ip < panels.GetCount(); ++ip) 
		surface += panels[ip].surface0 + panels[ip].surface1;
	avgFacetSideLen  = sqrt(surface/panels.GetCount());
}

double Surface::GetWaterPlaneArea() {
	double area = 0;
	
	for (int ip = 0; ip < panels.GetCount(); ++ip) {
		Panel &panel = panels[ip];
		
		area += -(panel.surface0*panel.normal0.z + panel.surface1*panel.normal1.z);
	}
	return area;
}

void Surface::GetVolume() {
	volumex = volumey = volumez = 0;
	
	for (int ip = 0; ip < panels.GetCount(); ++ip) {
		const Panel &panel = panels[ip];
		
		volumex += panel.surface0*panel.normal0.x*panel.centroid0.x;
		volumey += panel.surface0*panel.normal0.y*panel.centroid0.y;
		volumez += panel.surface0*panel.normal0.z*panel.centroid0.z;
		
		if (!panel.IsTriangle()) {
			volumex += panel.surface1*panel.normal1.x*panel.centroid1.x;
			volumey += panel.surface1*panel.normal1.y*panel.centroid1.y;
			volumez += panel.surface1*panel.normal1.z*panel.centroid1.z;
		}
	}
	volume = avg(volumex, volumey, volumez);
}
	
Point3D Surface::GetCenterOfBuoyancy() {
	double xb = 0, yb = 0, zb = 0;
	
	for (int ip = 0; ip < panels.GetCount(); ++ip) {
		const Panel &panel = panels[ip];
		
		xb += panel.surface0*panel.normal0.x*sqr(panel.centroid0.x);
		yb += panel.surface0*panel.normal0.y*sqr(panel.centroid0.y);
		zb += panel.surface0*panel.normal0.z*sqr(panel.centroid0.z);
		
		if (!panel.IsTriangle()) {
			xb += panel.surface1*panel.normal1.x*sqr(panel.centroid1.x);
			yb += panel.surface1*panel.normal1.y*sqr(panel.centroid1.y);
			zb += panel.surface1*panel.normal1.z*sqr(panel.centroid1.z);
		}
	}
	
	xb /= 2*volumex;
	yb /= 2*volumey;
	zb /= 2*volumez;
	
	return Point3D(xb, yb, zb);
}

// Hydrostatic restoring is only expected from heave, roll and pitch
void Surface::GetHydrostaticStiffness(MatrixXd &c, const Point3D &cb, double rho, 
					const Point3D &cg, double mass, double g) {
	c.setConstant(6, 6, 0);
	
	if (volume < EPS_XYZ)
		return;
	
	if (IsNull(mass))
		mass = rho*volume;
		
	for (int ip = 0; ip < panels.GetCount(); ++ip) {	
		const Panel &panel = panels[ip];

		double momentz0 = panel.normal0.z*panel.surface0;
		double momentz1 = panel.normal1.z*panel.surface1;
		double x0 = panel.centroid0.x;
		double y0 = panel.centroid0.y;
		double x1 = panel.centroid1.x;
		double y1 = panel.centroid1.y;
		c(2, 2) -= (momentz0 + momentz1);
        c(2, 3) -= (y0*momentz0 + y1*momentz1);
        c(2, 4) += (x0*momentz0 + x1*momentz1);
        c(3, 3) -= (y0*y0*momentz0 + y1*y1*momentz1);
        c(3, 4) += (x0*y0*momentz0 + x1*y1*momentz1);
        c(4, 4) -= (x0*x0*momentz0 + x1*x1*momentz1);
	}
	double rho_g = rho*g;
	
	c(2, 2) = c(2, 2)*rho_g;									
	c(2, 3) = c(2, 3)*rho_g;
	c(2, 4) = c(2, 4)*rho_g;
	c(3, 4) = c(3, 4)*rho_g;
	
	c(3, 3) = (c(3, 3) + volumez*cb.z)*rho_g - mass*g*cg.z;
	c(4, 4) = (c(4, 4) + volumez*cb.z)*rho_g - mass*g*cg.z;
	
	if (abs(cb.x) > EPS_XYZ)
		c(3, 5) -= rho_g*volume*cb.x;
	if (abs(cg.x) > EPS_XYZ)
		c(3, 5) += mass*g*cg.x;
	
	if (abs(cb.x) > EPS_XYZ)
		c(4, 5) -= rho_g*volume*cb.y;
	if (abs(cg.x) > EPS_XYZ)
		c(4, 5) += mass*g*cg.y;
			
	c(3, 2) = c(2, 3);
	c(4, 2) = c(2, 4);
	c(4, 3) = c(3, 4);
}

inline static void AddSegZero(Vector<Segment3D> &seg, const Point3D &p0, const Point3D &p1, 
			const Point3D &p2, const Point3D &p3) {
	if (abs(p0.z - 0) <= EPS_XYZ && abs(p1.z - 0) <= EPS_XYZ)
		seg << Segment3D(p0, p1);
	if (abs(p1.z - 0) <= EPS_XYZ && abs(p2.z - 0) <= EPS_XYZ)
		seg << Segment3D(p1, p2);
	if (abs(p2.z - 0) <= EPS_XYZ && abs(p3.z - 0) <= EPS_XYZ)
		seg << Segment3D(p2, p3);
	if (abs(p3.z - 0) <= EPS_XYZ && abs(p0.z - 0) <= EPS_XYZ)
		seg << Segment3D(p3, p0);
}

void Surface::CutZ(const Surface &orig, double factor) {
	nodes = clone(orig.nodes);
	panels.Clear();
	
	segWaterlevel.Clear();
	factor *= -1;
	
	for (int ip = 0; ip < orig.panels.GetCount(); ++ip) {
		const int &id0 = orig.panels[ip].id[0];
		const int &id1 = orig.panels[ip].id[1];
		const int &id2 = orig.panels[ip].id[2];
		const int &id3 = orig.panels[ip].id[3];
		const Point3D &p0 = nodes[id0];
		const Point3D &p1 = nodes[id1];
		const Point3D &p2 = nodes[id2];
		const Point3D &p3 = nodes[id3];	
		
		if ((p0.z)*factor >= 0 && (p1.z)*factor >= 0 && (p2.z)*factor >= 0 && (p3.z)*factor >= 0) 
			AddSegZero(segWaterlevel, p0, p1, p2, p3);
		else if ((p0.z)*factor <= 0 && (p1.z)*factor <= 0 && (p2.z)*factor <= 0 && (p3.z)*factor <= 0) {
			AddSegZero(segWaterlevel, p0, p1, p2, p3);
			panels << Panel(orig.panels[ip]);
		} else {
			const int *origPanelid = orig.panels[ip].id;
			Vector<int> nodeFrom, nodeTo;
			Segment3D segWL;
			segWL.from = segWL.to = Null;
			const int ids[] = {0, 1, 2, 3, 0};
			for (int i = 0; i < 4; ++i) {
				if (origPanelid[ids[i]] == origPanelid[ids[i+1]])
					;
				else {
					const Point3D &from = nodes[origPanelid[ids[i]]];
					const Point3D &to   = nodes[origPanelid[ids[i+1]]];
					if (abs(from.z) <= EPS_XYZ && abs(to.z) <= EPS_XYZ) {
						//nodeFrom << origPanelid[ids[i]];
						//nodeTo << origPanelid[ids[i+1]];
						segWL.from = from;
						segWL.to = to;	
					} else if ((from.z)*factor <= 0 && (to.z)*factor <= 0) {
						nodeFrom << origPanelid[ids[i]];
						nodeTo << origPanelid[ids[i+1]];
					} else if ((from.z)*factor >= 0 && (to.z)*factor >= 0) 
						;
					else {
						Segment3D seg(from, to);
						Point3D inter = seg.IntersectionPlaneZ(0);
						if (!IsNull(inter)) {
							if ((from.z)*factor < 0) {
								nodeFrom << origPanelid[ids[i]];
								nodes << inter;
								nodeTo << nodes.GetCount() - 1;
							} else {
								nodeTo << origPanelid[ids[i+1]];
								nodes << inter;
								nodeFrom << nodes.GetCount() - 1;
							}
						}
						if (IsNull(segWL.from))
							segWL.from = inter;
						else if (IsNull(segWL.to))
							segWL.to = inter;
					}
				}
			}
			if (!IsNull(segWL))
				segWaterlevel << segWL;
			
			int pos = -1, nFrom, nTo;
			for (int i = 0; i < nodeFrom.GetCount(); ++i) {
				int i_1 = i + 1;
				if (i_1 >= nodeFrom.GetCount())
					i_1 = 0;
				if (nodeTo[i] != nodeFrom[i_1]) {
					pos = i+1;
					nFrom = nodeTo[i];
					nTo = nodeFrom[i_1];
					break;
				}
			}
			if (pos == nodeTo.GetCount()) {
				nodeFrom << nFrom;
				nodeTo << nTo;
			} else if (pos >= 0) {
				nodeFrom.Insert(pos, nFrom);		
				nodeTo.Insert(pos, nTo);
			}
			
			Panel panel;
			if (nodeFrom.GetCount() == 3) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[2];
			} else if (nodeFrom.GetCount() == 4) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[3];
			} else if (nodeFrom.GetCount() == 5) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[3];
				Panel panel2;
				panel2.id[0] = nodeFrom[0];
				panel2.id[1] = nodeFrom[3];
				panel2.id[2] = nodeFrom[4];
				panel2.id[3] = nodeFrom[4];
				//TriangleToQuad(panel2); 
				panels << panel2;
			}
			//TriangleToQuad(panel);
			panels << panel;
		}
	}
}

void Surface::CutX(const Surface &orig, double factor) {
	nodes = clone(orig.nodes);
	panels.Clear();
	
	factor *= -1;
	
	for (int ip = 0; ip < orig.panels.GetCount(); ++ip) {
		const int &id0 = orig.panels[ip].id[0];
		const int &id1 = orig.panels[ip].id[1];
		const int &id2 = orig.panels[ip].id[2];
		const int &id3 = orig.panels[ip].id[3];
		const Point3D &p0 = nodes[id0];
		const Point3D &p1 = nodes[id1];
		const Point3D &p2 = nodes[id2];
		const Point3D &p3 = nodes[id3];	
		
		if ((p0.x)*factor >= 0 && (p1.x)*factor >= 0 && (p2.x)*factor >= 0 && (p3.x)*factor >= 0) 
			;
		else if ((p0.x)*factor <= 0 && (p1.x)*factor <= 0 && (p2.x)*factor <= 0 && (p3.x)*factor <= 0) 
			panels << Panel(orig.panels[ip]);
		else {
			const int *origPanelid = orig.panels[ip].id;
			Vector<int> nodeFrom, nodeTo;

			const int ids[] = {0, 1, 2, 3, 0};
			for (int i = 0; i < 4; ++i) {
				if (origPanelid[ids[i]] == origPanelid[ids[i+1]])
					;
				else {
					const Point3D &from = nodes[origPanelid[ids[i]]];
					const Point3D &to   = nodes[origPanelid[ids[i+1]]];
					Segment3D seg(from, to);
					if (abs(from.x) <= EPS_XYZ && abs(to.x) <= EPS_XYZ) {
						nodeFrom << origPanelid[ids[i]];
						nodeTo << origPanelid[ids[i+1]];
					} else if ((from.x)*factor <= 0 && (to.x)*factor <= 0) {
						nodeFrom << origPanelid[ids[i]];
						nodeTo << origPanelid[ids[i+1]];
					} else if ((from.x)*factor >= 0 && (to.x)*factor >= 0) 
						;
					else {
						Point3D inter = seg.IntersectionPlaneX(0);
						if (!IsNull(inter)) {
							if ((from.x)*factor < 0) {
								nodeFrom << origPanelid[ids[i]];
								nodes << inter;
								nodeTo << nodes.GetCount() - 1;
							} else {
								nodeTo << origPanelid[ids[i+1]];
								nodes << inter;
								nodeFrom << nodes.GetCount() - 1;
							}
						}
					}
				}
			}
			
			int pos = -1, nFrom, nTo;
			for (int i = 0; i < nodeFrom.GetCount(); ++i) {
				int i_1 = i + 1;
				if (i_1 >= nodeFrom.GetCount())
					i_1 = 0;
				if (nodeTo[i] != nodeFrom[i_1]) {
					pos = i+1;
					nFrom = nodeTo[i];
					nTo = nodeFrom[i_1];
					break;
				}
			}
			if (pos == nodeTo.GetCount()) {
				nodeFrom << nFrom;
				nodeTo << nTo;
			} else if (pos >= 0) {
				nodeFrom.Insert(pos, nFrom);		
				nodeTo.Insert(pos, nTo);
			}
			
			Panel panel;
			if (nodeFrom.GetCount() == 3) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[2];
			} else if (nodeFrom.GetCount() == 4) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[3];
			} else if (nodeFrom.GetCount() == 5) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[3];
				Panel panel2;
				panel2.id[0] = nodeFrom[0];
				panel2.id[1] = nodeFrom[3];
				panel2.id[2] = nodeFrom[4];
				panel2.id[3] = nodeFrom[4];
				//TriangleToQuad(panel2); 
				panels << panel2;
			}
			//TriangleToQuad(panel);
			panels << panel;
		}
	}
}

void Surface::CutY(const Surface &orig, double factor) {
	nodes = clone(orig.nodes);
	panels.Clear();
	
	factor *= -1;
	
	for (int ip = 0; ip < orig.panels.GetCount(); ++ip) {
		const int &id0 = orig.panels[ip].id[0];
		const int &id1 = orig.panels[ip].id[1];
		const int &id2 = orig.panels[ip].id[2];
		const int &id3 = orig.panels[ip].id[3];
		const Point3D &p0 = nodes[id0];
		const Point3D &p1 = nodes[id1];
		const Point3D &p2 = nodes[id2];
		const Point3D &p3 = nodes[id3];	
		
		if ((p0.y)*factor >= 0 && (p1.y)*factor >= 0 && (p2.y)*factor >= 0 && (p3.y)*factor >= 0) 
			;
		else if ((p0.y)*factor <= 0 && (p1.y)*factor <= 0 && (p2.y)*factor <= 0 && (p3.y)*factor <= 0) 
			panels << Panel(orig.panels[ip]);
		else {
			const int *origPanelid = orig.panels[ip].id;
			Vector<int> nodeFrom, nodeTo;

			const int ids[] = {0, 1, 2, 3, 0};
			for (int i = 0; i < 4; ++i) {
				if (origPanelid[ids[i]] == origPanelid[ids[i+1]])
					;
				else {
					const Point3D &from = nodes[origPanelid[ids[i]]];
					const Point3D &to   = nodes[origPanelid[ids[i+1]]];
					if ((from.y)*factor <= 0 && (to.y)*factor <= 0) {
						nodeFrom << origPanelid[ids[i]];
						nodeTo << origPanelid[ids[i+1]];
					} else if ((from.y)*factor >= 0 && (to.y)*factor >= 0) 
						;
					else {
						Segment3D seg(from, to);
						Point3D inter = seg.IntersectionPlaneY(0);
						if (!IsNull(inter)) {
							if ((from.y)*factor < 0) {
								nodeFrom << origPanelid[ids[i]];
								nodes << inter;
								nodeTo << nodes.GetCount() - 1;
							} else {
								nodeTo << origPanelid[ids[i+1]];
								nodes << inter;
								nodeFrom << nodes.GetCount() - 1;
							}
						}
					}
				}
			}
			
			int pos = -1, nFrom, nTo;
			for (int i = 0; i < nodeFrom.GetCount(); ++i) {
				int i_1 = i + 1;
				if (i_1 >= nodeFrom.GetCount())
					i_1 = 0;
				if (nodeTo[i] != nodeFrom[i_1]) {
					pos = i+1;
					nFrom = nodeTo[i];
					nTo = nodeFrom[i_1];
					break;
				}
			}
			if (pos == nodeTo.GetCount()) {
				nodeFrom << nFrom;
				nodeTo << nTo;
			} else if (pos >= 0) {
				nodeFrom.Insert(pos, nFrom);		
				nodeTo.Insert(pos, nTo);
			}
			
			Panel panel;
			if (nodeFrom.GetCount() == 3) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[2];
			} else if (nodeFrom.GetCount() == 4) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[3];
			} else if (nodeFrom.GetCount() == 5) {
				panel.id[0] = nodeFrom[0];
				panel.id[1] = nodeFrom[1];
				panel.id[2] = nodeFrom[2];
				panel.id[3] = nodeFrom[3];
				Panel panel2;
				panel2.id[0] = nodeFrom[0];
				panel2.id[1] = nodeFrom[3];
				panel2.id[2] = nodeFrom[4];
				panel2.id[3] = nodeFrom[4];
				//TriangleToQuad(panel2); 
				panels << panel2;
			}
			//TriangleToQuad(panel);
			panels << panel;
		}
	}
}

void Surface::Join(const Surface &orig) {
	int num = nodes.GetCount();
	int numOrig = orig.nodes.GetCount();
	nodes.SetCount(num + numOrig);
	for (int i = 0; i < numOrig; ++i)
		nodes[num+i] = orig.nodes[i];
	
	int numPan = panels.GetCount();
	int numPanOrig = orig.panels.GetCount();
	panels.SetCount(numPan + numPanOrig);
	for (int i = 0; i < numPanOrig; ++i) {
		Panel &pan = panels[numPan+i];
		const Panel &panOrig = orig.panels[i];
		
		for (int ii = 0; ii < 4; ++ii)
			pan.id[ii] = panOrig.id[ii] + num;
		
		GetPanelParams(pan);
	}
	
	Surface::RemoveDuplicatedPanels(panels);
	Surface::RemoveDuplicatedPointsAndRenumber(panels, nodes);
	Surface::RemoveDuplicatedPanels(panels);
}
	
void Surface::Translate(double x, double y, double z) {
	for (int i = 0; i < nodes.GetCount(); ++i) 
		nodes[i].Translate(x, y, z); 
	
	for (int i = 0; i < skewed.GetCount(); ++i) 
		skewed[i].Translate(x, y, z);
	
	for (int i = 0; i < segTo1panel.GetCount(); ++i) 
		segTo1panel[i].Translate(x, y, z);
	for (int i = 0; i < segTo3panel.GetCount(); ++i) 
		segTo3panel[i].Translate(x, y, z);
}

void Surface::Rotate(double a_x, double a_y, double a_z, double c_x, double c_y, double c_z) {
	Affine3d quat;
	GetTransform(quat, a_x, a_y, a_z, c_x, c_y, c_z);
	
	for (int i = 0; i < nodes.GetCount(); ++i) 
		nodes[i].Rotate(quat);

	for (int i = 0; i < skewed.GetCount(); ++i) 
		skewed[i].Rotate(quat);
	
	for (int i = 0; i < segTo1panel.GetCount(); ++i) 
		segTo1panel[i].Rotate(quat);
	for (int i = 0; i < segTo3panel.GetCount(); ++i) 
		segTo3panel[i].Rotate(quat);
}

void Surface::DeployXSymmetry() {
	int nnodes = nodes.GetCount();
	for (int i = 0; i < nnodes; ++i) {
		Point3D 	  &dest = nodes.Add();
		const Point3D &orig = nodes[i];
		dest.x = -orig.x;
		dest.y =  orig.y;
		dest.z =  orig.z;
	}
	int npanels = panels.GetCount();
	for (int i = 0; i < npanels; ++i) {
		Panel 		&dest = panels.Add();
		const Panel &orig = panels[i];
		dest.id[0] = orig.id[3] + nnodes;
		dest.id[1] = orig.id[2] + nnodes;
		dest.id[2] = orig.id[1] + nnodes;
		dest.id[3] = orig.id[0] + nnodes;
	}
}

void Surface::DeployYSymmetry() {
	int nnodes = nodes.GetCount();
	for (int i = 0; i < nnodes; ++i) {
		Point3D 	  &dest = nodes.Add();
		const Point3D &orig = nodes[i];
		dest.x =  orig.x;
		dest.y = -orig.y;
		dest.z =  orig.z;
	}
	int npanels = panels.GetCount();
	for (int i = 0; i < npanels; ++i) {
		Panel 		&dest = panels.Add();
		const Panel &orig = panels[i];
		dest.id[0] = orig.id[3] + nnodes;
		dest.id[1] = orig.id[2] + nnodes;
		dest.id[2] = orig.id[1] + nnodes;
		dest.id[3] = orig.id[0] + nnodes;
	}
}

void VolumeEnvelope::MixEnvelope(VolumeEnvelope &env) {
	maxX = maxNotNull(env.maxX, maxX);
	minX = minNotNull(env.minX, minX);
	maxY = maxNotNull(env.maxY, maxY);
	minY = minNotNull(env.minY, minY);
	maxZ = maxNotNull(env.maxZ, maxZ);
	minZ = minNotNull(env.minZ, minZ);
}

void Surface::AddNode(Point3D &p) {
	double similThres = EPS_XYZ;
	for (int i = 0; i < nodes.GetCount(); ++i) {
		if (nodes[i].IsSimilar(p, similThres))
			return;
	}
	nodes << p;
}

int Surface::FindNode(Point3D &p) {
	for (int i = 0; i < nodes.GetCount(); ++i) {
		if (nodes[i] == p)
			return i;
	}
	return -1;
}
	
void Surface::AddFlatPanel(double lenX, double lenY, double panelWidth) {
	int numX = int(round(lenX/panelWidth));
	ASSERT(numX > 0);
	double widthX = lenX/numX;	
	
	int numY = int(round(lenY/panelWidth));
	ASSERT(numY > 0);
	double widthY = lenY/numY;	
	
	Array<PanelPoints> pans;
	pans.SetCount(numX*numY);
	int n = 0;
	for (int i = 0; i < numX; ++i) {
		for (int j = 0; j < numY; ++j) {
			pans[n].data[0].x = widthX*i;		pans[n].data[0].y = widthY*j;		pans[n].data[0].z = 0; 
			pans[n].data[1].x = widthX*(i+1);	pans[n].data[1].y = widthY*j;		pans[n].data[1].z = 0;
			pans[n].data[2].x = widthX*(i+1);	pans[n].data[2].y = widthY*(j+1);	pans[n].data[2].z = 0;
			pans[n].data[3].x = widthX*i;		pans[n].data[3].y = widthY*(j+1);	pans[n].data[3].z = 0;
			n++;
		}
	}
	SetPanelPoints(pans);
}

void Surface::AddRevolution(Vector<Pointf> &points, double panelWidth) {
	if (points.GetCount() < 2)
		throw Exc(t_("Point nimver has to be higher than 2"));
	
	for (int i = points.GetCount()-2; i >= 0; --i) {
		double len = sqrt(sqr(points[i].x-points[i+1].x) + sqr(points[i].y-points[i+1].y)); 
		int num = int(round(len/panelWidth));
		if (num > 1) {
			double x0 = points[i].x;
			double lenx = points[i+1].x - points[i].x;
			double y0 = points[i].y;
			double leny = points[i+1].y - points[i].y;
			for (int in = num-1; in >= 1; --in) {
				Pointf p(x0 + lenx*in/num, y0 + leny*in/num);
				points.Insert(i+1, p);
			}
		}
	}

	double maxx = 0;
	for (Pointf &p : points)
		maxx = max(maxx, p.x);
	
	int numSlices = int(round(2*M_PI*maxx/panelWidth));
	if (Odd(numSlices))
		numSlices++;
	
	if (numSlices < 3)
		throw Exc(t_("Panel width too large"));
		
	//double angle = 2*M_PI/numSlices;
		
	Array<PanelPoints> pans;
	pans.SetCount(numSlices*(points.GetCount()-1));
	int n = 0;
	for (int i = 0; i < points.GetCount()-1; ++i) {
		if (points[i].x == 0) {
			for (int j = 0; j < numSlices; j += 2) {
				pans[n].data[0].x = 0;	
				pans[n].data[0].y = 0;
				pans[n].data[0].z = points[i].y;

				pans[n].data[1].x = points[i+1].x*cos((2*M_PI*j)/numSlices);	
				pans[n].data[1].y = points[i+1].x*sin((2*M_PI*j)/numSlices);
				pans[n].data[1].z = points[i+1].y;
	
				pans[n].data[2].x = points[i+1].x*cos((2*M_PI*(j+1))/numSlices);	
				pans[n].data[2].y = points[i+1].x*sin((2*M_PI*(j+1))/numSlices);
				pans[n].data[2].z = points[i+1].y;
											
				pans[n].data[3].x = points[i+1].x*cos((2*M_PI*(j+2))/numSlices);	
				pans[n].data[3].y = points[i+1].x*sin((2*M_PI*(j+2))/numSlices);
				pans[n].data[3].z = points[i+1].y;
				
				n++;
			}
		} else if (points[i+1].x == 0) {
			for (int j = 0; j < numSlices; j += 2) {
				pans[n].data[0].x = points[i].x*cos((2*M_PI*j)/numSlices);	
				pans[n].data[0].y = points[i].x*sin((2*M_PI*j)/numSlices);
				pans[n].data[0].z = points[i].y;
	
				pans[n].data[1].x = points[i].x*cos((2*M_PI*(j+1))/numSlices);	
				pans[n].data[1].y = points[i].x*sin((2*M_PI*(j+1))/numSlices);
				pans[n].data[1].z = points[i].y;
											
				pans[n].data[2].x = points[i].x*cos((2*M_PI*(j+2))/numSlices);	
				pans[n].data[2].y = points[i].x*sin((2*M_PI*(j+2))/numSlices);
				pans[n].data[2].z = points[i].y;

				pans[n].data[3].x = 0;	
				pans[n].data[3].y = 0;
				pans[n].data[3].z = points[i+1].y;
								
				n++;
			}
		} else {
			for (int j = 0; j < numSlices; ++j) {
				pans[n].data[0].x = points[i].x*cos((2*M_PI*j)/numSlices);	
				pans[n].data[0].y = points[i].x*sin((2*M_PI*j)/numSlices);
				pans[n].data[0].z = points[i].y;
				
				pans[n].data[1].x = points[i].x*cos((2*M_PI*(j+1))/numSlices);	
				pans[n].data[1].y = points[i].x*sin((2*M_PI*(j+1))/numSlices);
				pans[n].data[1].z = points[i].y;
	
				pans[n].data[2].x = points[i+1].x*cos((2*M_PI*(j+1))/numSlices);	
				pans[n].data[2].y = points[i+1].x*sin((2*M_PI*(j+1))/numSlices);
				pans[n].data[2].z = points[i+1].y;
	
				pans[n].data[3].x = points[i+1].x*cos((2*M_PI*j)/numSlices);	
				pans[n].data[3].y = points[i+1].x*sin((2*M_PI*j)/numSlices);
				pans[n].data[3].z = points[i+1].y;
				
				n++;
			}
		}
	}
	pans.SetCount(n);
	SetPanelPoints(pans);
}

void Surface::SetPanelPoints(Array<PanelPoints> &pans) {
	for (int i = 0; i < pans.GetCount(); ++i) {
		PanelPoints &pan = pans[i];
		for (int j = 0; j < 4; ++j) {
			Point3D p(pan.data[j].x, pan.data[j].y, pan.data[j].z);
			AddNode(p);
		}
	}
	
	for (int i = 0; i < pans.GetCount(); ++i) {
		PanelPoints &pan = pans[i];
		Panel &panel = panels.Add();
		for (int j = 0; j < 4; ++j) {
			Point3D p(pan.data[j].x, pan.data[j].y, pan.data[j].z);
			int id = FindNode(p);
			if (id < 0)
				throw Exc("Node not found in SetPanelPoints()");
			panel.id[j] = id;
		}
	}
}

void Surface::AddPolygonalPanel(Vector<Pointf> &bound, double panelWidth) {
	ASSERT(bound.GetCount() >= 2);
	
	bound << bound[0];
	
	for (int i = bound.GetCount()-2; i >= 0; --i) {
		double len = sqrt(sqr(bound[i].x-bound[i+1].x) + sqr(bound[i].y-bound[i+1].y)); 
		int num = int(round(len/panelWidth));
		if (num > 1) {
			double x0 = bound[i].x;
			double lenx = bound[i+1].x - bound[i].x;
			double y0 = bound[i].y;
			double leny = bound[i+1].y - bound[i].y;
			for (int in = num-1; in >= 1; --in) {
				Pointf p(x0 + lenx*in/num, y0 + leny*in/num);
				bound.Insert(i+1, p);
			}
		}
	}
	bound.Remove(bound.GetCount()-1);
	
	double avgx = 0, avgy = 0;
	for (Pointf &p : bound) {
		avgx += p.x;
		avgy += p.y;
	}
	Pointf avgp(avgx/bound.GetCount(), avgy/bound.GetCount());

	Upp::Array<Pointf> delp;
	delp.SetCount(bound.GetCount());
	for (int i = 0; i < bound.GetCount(); ++i) 
		delp[i] = bound[i]; 
	delp << avgp;
	
	Delaunay del;
	
	while (true) {
		del.Build(delp);
		double avglen = 0, maxlen = 0;
		int maxid, maxid3, num = 0;
		for (int i = 0; i < del.GetCount(); ++i) {
			const Delaunay::Triangle &tri = del[i];
			if (tri[0] < 0 || tri[1] < 0 || tri[2] < 0)  
				continue;

			double len;
			len = sqrt(sqr(delp[tri[0]].x - delp[tri[1]].x) + sqr(delp[tri[0]].y - delp[tri[1]].y));
			avglen += len;	num++;
			if (maxlen < len) {
				maxlen = len;	maxid = i;	maxid3 = 0;
			}
			len = sqrt(sqr(delp[tri[1]].x - delp[tri[2]].x) + sqr(delp[tri[1]].y - delp[tri[2]].y));
			avglen += len;	num++;
			if (maxlen < len) {
				maxlen = len;	maxid = i;	maxid3 = 1;
			}			
			len = sqrt(sqr(delp[tri[2]].x - delp[tri[0]].x) + sqr(delp[tri[2]].y - delp[tri[0]].y));
			avglen += len;	num++;
			if (maxlen < len) {
				maxlen = len;	maxid = i;	maxid3 = 2;
			}
		}
		avglen /= num;
		if (avglen < panelWidth) 
			break;
		
		int maxid33 = maxid3 == 2 ? 0 : maxid3+1;
		
		delp << Pointf(Avg(delp[del[maxid][maxid3]].x, delp[del[maxid][maxid33]].x), Avg(delp[del[maxid][maxid3]].y, delp[del[maxid][maxid33]].y));	
	}
	
	Array<PanelPoints> pans;
	for (int i = 0; i < del.GetCount(); ++i) {
		const Delaunay::Triangle &tri = del[i];
		if (tri[0] < 0 || tri[1] < 0 || tri[2] < 0)  
			continue;

		PanelPoints &pan = pans.Add();
		pan.data[0].x = delp[tri[0]].x;		pan.data[0].y = delp[tri[0]].y;		pan.data[0].z = 0;
		pan.data[1].x = delp[tri[1]].x;		pan.data[1].y = delp[tri[1]].y;		pan.data[1].z = 0;
		pan.data[2].x = delp[tri[2]].x;		pan.data[2].y = delp[tri[2]].y;		pan.data[2].z = 0;
		pan.data[3].x = delp[tri[2]].x;		pan.data[3].y = delp[tri[2]].y;		pan.data[3].z = 0;
	}
	SetPanelPoints(pans);
}

}