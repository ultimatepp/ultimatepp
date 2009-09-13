#ifndef _PolyMarker_h_
#define _PolyMarker_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class PolyMarker
{
	public:
		typedef PolyMarker CLASSNAME;
		
		enum HitKind { Miss, Vertex, Side, Inside };
		enum PolyKind { Empty, Rectangle, Polygon };
		
	protected:
	
		PolyKind kind;
		Array<Point> points;
		int borderThickness;
		Color borderColor;
		int borderLineType;
		RGBA fillColor;
		bool closed;

	public:
	
		// constructors
		PolyMarker();
		PolyMarker(const PolyMarker &pm);
		
		// test for point hit
		HitKind Hit(const Point &p, int minDist);
		
		// gets nearest vertex or side to point
		One<Point> const VertexAt(const Point &p, int minDist);
		One<Rect> const SideAt(const Point &p, int minDist);
		
		// open/close the polygon (invalid for rectangle markers)
		bool Open(void);
		bool Close(void);

		// adds a vertex to the polygon (invalid for rectangle markers)
		bool AddVertex(const Point &p);
		bool AddVertex(const Array<Point> &ap);
		void MakeRect(const Point &p1, const Point &p2);
		
		// clears the marker
		void Clear(void);
		
		// member read access
		PolyKind GetKind(void) const { return kind; }
		bool IsClosed(void) const { return closed; }
		Color GetBorderColor(void) const { return borderColor; }
		int GetBorderLineType(void) const { return borderLineType; }
		int GetBorderThickness(void) const { return borderThickness; }
		RGBA GetFillColor(void) const { return fillColor; }
		Rect const &GetBoundingBox(void);
		Array<Point> const &GetPoints(void);
		
		// gets a dragged PolyMarker giving start and end point
		PolyMarker Drag(const Point &startP, const Point &endP);
		
		// changes the PolyMarker at end of drag ops
		void DoDrag(const Point &startP, const Point &endP);
		
		// paint the PolyMarker on a given viewport
		void Paint(const Rect &vport);
	
} ; // END Class PolyMarker

typedef Array<PolyMarker> PolyMarkers;

END_UPP_NAMESPACE

#endif
