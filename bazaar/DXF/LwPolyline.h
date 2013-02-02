#ifndef _DXF_LwPolyline_h_
#define _DXF_LwPolyline_h_

#include "Entity.h"

class DXFLwPolyline : public DXFEntity
{
	friend class DXFEntities;
	private:

		// the line vertices
		Vector<Pointf> vertices;
		
		// the bulges
		Vector<double> bulges;
		
		// starting and ending widths
		Vector<double> startWidths, endWidths;
		
		// closed flag
		bool closed;
		
		// constant width
		double width;
		
		// calculates last tangent from previous 2 vertices and bulge
		double GetLastTangent(void);
		
	protected:
	
		virtual bool Write(Stream &s);
		
		// private constructor -- may be created only by DXFEntities
		DXFLwPolyline(DXFEntities *e);
	
	public:
	
		// get/set points
		Vector<Pointf> const &GetPoints(void) const { return vertices; }
		DXFLwPolyline &SetPoints(Vector<Pointf> const &pts);
		
		// add one or more points
		DXFLwPolyline &AddPoints(Vector<Pointf> const &pts);
		DXFLwPolyline &AddPoints(Pointf const &p1);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7);
		DXFLwPolyline &AddPoints(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7, Pointf const &p8);
		
		// add one arc to polyline
		
		// gets tangent from previous vertex, if any
		// if first vertex, simply add a point
		DXFLwPolyline &AddArc(Pointf const &p);
		
		// adds an arc passing from previous point, if any, and given 2 points
		// if no previous point, add a counterclockwise full semicircle passing from 2 points
		DXFLwPolyline &AddArc(Pointf const &p1, Pointf const &p2);
		
		// adds an arc passing from previous point, if any, and given center point
		// if no previous point, simply add a point
		DXFLwPolyline &AddArcCenter(Pointf const &center, Pointf const &p, bool big = false);
		
		// adds an arc with point and bulge; if bulge is null, just add a line segment
		DXFLwPolyline &AddArc(Pointf const &p, double bulge);
		
		// close/open polyline
		DXFLwPolyline &Close(void);
		DXFLwPolyline &Open(void);
		
		// get/set global width
		double GetWidth(void) { return width; }
		DXFLwPolyline &SetWidth(double w);

		// gets bounding box of element
		virtual Rectf GetBoundingBox(void) const;
};

#endif
