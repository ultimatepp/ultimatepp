#ifndef _Marker_h_
#define _Marker_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

// Image Marker class
// supports rectangular and polygonal areas
class Marker
{
	public:
		typedef Marker CLASSNAME;

		enum HitKind { Miss, PolyVertex, PolySide, Left, Right, Top, Bottom, TopLeft, TopRight, BottomLeft, BottomRight, Inside };
		enum MarkerKind { EmptyMarker, RectMarker, PolyMarker };
		
	protected:

		MarkerKind kind;
		bool closed;

		int borderThickness;
		Color borderColor;
		int borderLineType;
		RGBA fillColor;
		
		int selBorderThickness;
		Color selBorderColor;
		int selBorderLineType;
		RGBA selFillColor;
		
		Vector<Point> points;
		
	public:
	
		// construct an empty marker
		Marker();
	
		// construct a rectangular Marker
		Marker(Point const &p1, Point const &p2);
		
		// construct a polygonal Marker
		Marker(Vector<Point> const &pts);
		
		// copy constructor
		Marker(Marker const &m);
		
		// sets the marker to a rectangular area
		void Set(const Point &p1, const Point &p2);
		
		// sets the marker to be a polygonal area
		void Set(const Vector<Point> &pts);
		
		// empties the marker
		void Clear();
		
		// opens/closes the marker
		void Open();
		void Close();
	
		// test for point hit
		// hitIndex is only for polygons, gives vertex's index of selected vertex
		HitKind Hit(const Point &p, int minDist, int &hitIndex);
		
		// member read access
		virtual MarkerKind GetKind() const	{ return kind; }
		bool IsClosed() const				{ return closed; }
		Color GetBorderColor() const		{ return borderColor; }
		int GetBorderLineType() const		{ return borderLineType; }
		int GetBorderThickness() const		{ return borderThickness; }
		RGBA GetFillColor() const			{ return fillColor; }
		Color GetSelBorderColor() const		{ return selBorderColor; }
		int GetSelBorderLineType() const	{ return selBorderLineType; }
		int GetSelBorderThickness() const	{ return selBorderThickness; }
		RGBA GetSelFillColor() const		{ return selFillColor; }
		Rect GetBoundingBox() const;
		Vector<Point> GetPoints() const;
		
		// member write access
		void SetKind(MarkerKind k);
		void SetBorderColor(Color c);
		void SetBorderLineType(int lt);
		void SetBorderThickness(int th);
		RGBA SetFillColor(RGBA const &rgba);
		void SetSelBorderColor(Color c);
		void SetSelBorderLineType(int lt);
		void SetSelBorderThickness(int th);
		RGBA SetSelFillColor(RGBA const &rgba);
		
		// gets dragged outline
		Vector<Point>DragOutline(Point const &startPt, Point const &endPt, int minDist);
		
		// drag the marker to final position
		void Drag(Point const &startPt, Point const &endPt, int minDist);

}; // END Class Marker

typedef Array<Marker> Markers;

END_UPP_NAMESPACE

#endif
