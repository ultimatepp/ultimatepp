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
		
// adds an arc with point and bulge; if bulge is null, just add a line segment
DXFLwPolyline &DXFLwPolyline::AddArc(Pointf const &p, double bulge)
{
	if(!vertices.GetCount())
		return AddPoints(p);
	bulges.Top() = bulge;
	vertices.Add(T(p));
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
	width = S(w);
	return *this;
}

// normalizes an angle in range 0..2 * M_PI
static double norm(double a)
{
	a = fmod(a, 2 * M_PI);
	if(a < 0)
		a = 2 * M_PI + a;
	return a;
}

// gets bounding box of element
Rectf DXFLwPolyline::GetBoundingBox(void) const
{
	Rectf r(1e99, -1e99, -1e99, 1e99);
	for(int i = 0; i < vertices.GetCount(); i++)
	{
		Pointf const &p = vertices[i];
		
		// first, check the vertex
		r.left 		= min(r.left, p.x);
		r.top		= max(r.top, p.y);
		r.right		= max(r.right, p.x);
		r.bottom	= min(r.bottom, p.y);
		
		// get the bulge at vertex; if not zero, we need to check
		// the arc for extents
		if(bulges[i] != 0)
		{
			Pointf const &p2 = (i < vertices.GetCount() - 1 ? vertices[i + 1] : vertices[0]);
			
			// skip me if points are the same OR second point is first one and pline
			// is not closed
			if(&p != &p2 && (closed || &p2 != &vertices[0]))
			{
				// first, we need radius and center
				// here quite complicated, but it should be a simpler way...
				double chord = sqrt(sqr(p2.x - p.x) + sqr(p2.y - p.y));
				double sagitta = fabs(chord / 2 * bulges[i]);
				double radius = (sqr(chord / 2) + sqr(sagitta)) / 2 / sagitta;
				double a12 = atan2(p2.y - p.y, p2.x - p.x);
				double xm = (p2.x + p.x) / 2;
				double ym = (p2.y - p.y) / 2;
				double apot = radius - sagitta;
				double ac = bulges[i] > 0 ? a12 + M_PI / 2 : a12 - M_PI / 2;
				double xc = xm + apot * cos(ac);
				double yc = ym + apot * sin(ac);
				
				// now we check how many quadrants does the arc span
				// first, we take the angle of arc endpoints and adjust it
				// depending on arc direction
				double a1 = atan2(p.y - yc, p.x - xc);
				double a2 = atan2(p2.y - yc, p2.x - xc);
				if(bulges[i] < 0)
				{
					double t = a1;
					a1 = a2;
					a2 = t;
				}
			
				// check if crossing quadrants
				bool cross0		= norm(a1) > norm(a2);
				bool cross90	= norm(a1 - M_PI / 2) > norm(a2 - M_PI / 2);
				bool cross180	= norm(a1 - M_PI) > norm(a2 - M_PI);
				bool cross270	= norm(a1 - 3 * M_PI / 2) > norm(a2 - 3 * M_PI / 2);
				
				if(cross0)		r.right		= xc + radius;
				if(cross90)		r.top		= yc + radius;
				if(cross180)	r.left		= xc - radius;
				if(cross270)	r.bottom	= yc - radius;
			}
		}
	}
	return r;
}
