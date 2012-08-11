#include "DXF.h"

// private constructor -- may be created only by DXFEntities
DXFLwPolyline::DXFLwPolyline(DXFEntities *e) : DXFEntity(e, "LWPOLYLINE")
{
	vertices.Clear();
	bulges.Clear();
	startWidths.Clear();
	endWidths.Clear();
	width = 0;
	closed = false;
}

// updates last tangent after adding points
double DXFLwPolyline::GetLastTangent(void)
{
	// if zero or one vertex there, just set the tangent
	// to M_PI / 2
	if(vertices.GetCount() < 2)
		return M_PI / 2;
	
	// we got at least 2 vertices, we can get the tangent from them
	int idx = vertices.GetCount() - 2;
	Pointf const &p1 = vertices[idx];
	Pointf const &p2 = vertices[idx + 1];
	double bulge = bulges[idx];
	double a12 = atan2(p2.y - p1.y, p2.x - p1.x);
	double aInc = 4 * atan(bulge);
	return a12 + aInc / 2;
}
		
bool DXFLwPolyline::Write(Stream &s)
{
	/* TO DO : BULGES AND WIDTHS !!!
	90 Number of vertices
	70 Polyline flag (bit-coded); default is 0:
	   1 = Closed; 128 = Plinegen
	43 Constant width (optional; default = 0). Not used if variable width (codes 40 and/or 41) is set
	38 Elevation (optional; default = 0)
	39 Thickness (optional; default = 0)
	10 Vertex coordinates (in OCS), multiple entries; one entry for each vertex
	   DXF: X value; APP: 2D point
	20 DXF: Y value of vertex coordinates (in OCS), multiple entries; one entry for each vertex
	40 Starting width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set
	41 End width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set
	42 Bulge (multiple entries; one entry for each vertex) (optional; default = 0)
	*/
	// base class before...
	if(!DXFEntity::Write(s))
		return false;
	
	s << "100\nAcDbPolyline\n";
	s << "90\n" << vertices.GetCount() << "\n";
	s << "70\n" << (closed ? "1\n" : "0\n");
	if(width != 0)
		s << "43\n" << width << "\n";
	for(int i = 0; i < vertices.GetCount(); i++)
	{
		Pointf const &p = vertices[i];
	    s << "10\n" << p.x << "\n";
	    s << "20\n" << p.y << "\n";
	    if(bulges[i] != 0)
	        s << "42\n" << bulges[i] << "\n";
	}
	return true;
}

// get/set points
DXFLwPolyline &DXFLwPolyline::SetPoints(Vector<Pointf> const &pts)
{
	vertices.Clear();
	bulges.Clear();
	for(int i = 0; i < pts.GetCount(); i++)
	{
		vertices.Add(T(pts[i]));
		bulges.Add(0);
	}
	return *this;
}

// add one or more points
DXFLwPolyline &DXFLwPolyline::AddPoints(Vector<Pointf> const &pts)
{
	for(int i = 0; i < pts.GetCount(); i++)
	{
		vertices.Add(T(pts[i]));
		bulges.Add(0);
	}
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1)
{
	vertices.Add(T(p1));
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	vertices.Add(T(p3));
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	vertices.Add(T(p3));
	vertices.Add(T(p4));
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	vertices.Add(T(p3));
	vertices.Add(T(p4));
	vertices.Add(T(p5));
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	vertices.Add(T(p3));
	vertices.Add(T(p4));
	vertices.Add(T(p5));
	vertices.Add(T(p6));
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	vertices.Add(T(p3));
	vertices.Add(T(p4));
	vertices.Add(T(p5));
	vertices.Add(T(p6));
	vertices.Add(T(p7));
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

DXFLwPolyline &DXFLwPolyline::AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7, Pointf const &p8)
{
	vertices.Add(T(p1));
	vertices.Add(T(p2));
	vertices.Add(T(p3));
	vertices.Add(T(p4));
	vertices.Add(T(p5));
	vertices.Add(T(p6));
	vertices.Add(T(p7));
	vertices.Add(T(p8));
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	bulges.Add(0);
	return *this;
}

// gets tangent from previous vertex, if any
// if first vertex, simply add a point
DXFLwPolyline &DXFLwPolyline::AddArc(Pointf const &p)
{
	if(vertices.IsEmpty())
		return AddPoints(p);
	double startTangent = GetLastTangent();
	Pointf const &pp = vertices.Top();
	Pointf pi = T(p);
	double aPts = atan2(pi.y - pp.y, pi.x - pp.x);

	double aInc = 2 * (aPts - startTangent);

	bulges.Top() = tan(aInc / 4);
	vertices.Add(pi);
	bulges.Add(0);
	return *this;
}

// adds an arc passing from previous point, if any, and given 2 points
// if no previous point, add a counterclockwise full semicircle passing from 2 points
DXFLwPolyline &DXFLwPolyline::AddArc(Pointf const &p1, Pointf const &p2)
{
	if(!vertices.GetCount())
	{
		// full semicircle with given 2 points
		vertices.Add(T(p1));
		bulges.Add(1);
		vertices.Add(T(p2));
		bulges.Add(0);
	}
	else
	{
		// arc passing from last point and the 2 given points
		double x1 = vertices.Top().x;
		double y1 = vertices.Top().y;
		Pointf tp1 = T(p1);
		Pointf tp2 = T(p2);
		double x2 = tp1.x;
		double y2 = tp1.y;
		double x3 = tp2.x;
		double y3 = tp2.y;
		
		// fraction denominator, to find out center point
		double d = 2 * ((x2 - x1) * y3 + (x1 - x3) * y2 + (x3 - x2) * y1);
		if(fabs(d) < DXF_EPSILON)
			// points are collinear, just add a 2 straight segments
			return AddPoints(p1, p2);
		
		double xc =
			(y2 - y1) * y3*y3 +
			(-y2*y2 + y1*y1 -x2*x2 + x1*x1) * y3 +
			y1 * y2*y2 +
			(-y1*y1 + x3*x2 - x1*x1) * y2 +
			(x2*x2 - x3*x3) * y1;
		xc /= d;
		double yc =
			(x2 - x1) * y3 * y3 +
			(x1 - x3) * y2 * y2 +
			(x3 - x2) * y1 * y1 +
			(x2 - x1) * x3 * x3 +
			(x1 * x1 - x2 * x2) * x3 +
			x1 * x2 * x2 - x1 * x1 * x2;
		yc /= d;
		
		// included angle
		double aInc = atan2(y3 - yc, x3 - xc) - atan2(y1 - yc, x1 - xc);
		vertices.Add(tp2);
		bulges.Top() = tan(aInc / 4);
		bulges.Add(0);
	}
	return *this;
}

// adds an arc passing from previous point, if any, and given center point
// if no previous point, simply add a point
DXFLwPolyline &DXFLwPolyline::AddArcCenter(Pointf const &center, Pointf const &p, bool big)
{
	if(!vertices.GetCount())
		return AddPoints(p);
		
	Pointf &p1 = vertices.Top();
	Pointf tp = T(p);
	Pointf tCenter = T(center);
	double aInc = atan2(tp.y - tCenter.y, tp.x - tCenter.x) - atan2(p1.y - tCenter.y, p1.x - tCenter.x);
	aInc = fmod(aInc, 2 * M_PI);
	if(big && fabs(aInc) <= M_PI || !big && fabs(aInc) > M_PI)
	{
		if(aInc < 0)
			aInc = 2 * M_PI + aInc;
		else
			aInc = 2 * M_PI - aInc;
	}
	bulges.Top() = tan(aInc / 4);
	vertices.Add(tp);
	bulges.Add(0);
	return *this;
}
		
// close/open polyline
DXFLwPolyline &DXFLwPolyline::Close(void)
{
	closed = true;
	return *this;
}

DXFLwPolyline &DXFLwPolyline::Open(void)
{
	closed = false;
	return *this;
}

// get/set global width
DXFLwPolyline &DXFLwPolyline::SetWidth(double w)
{
	width = w;
	return *this;
}
