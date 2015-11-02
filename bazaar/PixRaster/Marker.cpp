#include "Marker.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
Marker::Marker()
{
	kind = EmptyMarker;
	borderThickness = 2;
	borderColor = Blue;
	borderLineType = 0;
	fillColor = WhiteGray();
	selBorderThickness = 2;
	selBorderColor = LtBlue;
	selBorderLineType = 0;
	selFillColor = LtGray();
	closed = false;

} // END Constructor class Marker

// construct a rectangular Marker
Marker::Marker(Point const &p1, Point const &p2)
{
	kind = RectMarker;
	borderThickness = 2;
	borderColor = Blue;
	borderLineType = 0;
	fillColor = WhiteGray();
	selBorderThickness = 2;
	selBorderColor = LtBlue;
	selBorderLineType = 0;
	selFillColor = LtGray();
	closed = true;
	points.Add(Point(min(p1.x, p2.x), min(p1.y, p2.y)));
	points.Add(Point(max(p1.x, p2.x), max(p1.y, p2.y)));
	
} // END Constructor class Marker

// construct a polygonal Marker
Marker::Marker(Vector<Point> const &pts)
{
	kind = PolyMarker;
	borderThickness = 2;
	borderColor = Blue;
	borderLineType = 0;
	fillColor = WhiteGray();
	selBorderThickness = 2;
	selBorderColor = LtBlue;
	selBorderLineType = 0;
	selFillColor = LtGray();
	closed = true;
	points <<= pts;
	
} // END Constructor class Marker
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Marker::Marker(const Marker &m)
{
	kind = m.GetKind();
	borderThickness = m.GetBorderThickness();
	borderColor = m.GetBorderColor();
	borderLineType = m.GetBorderLineType();
	fillColor = m.GetFillColor();
	selBorderThickness = m.GetSelBorderThickness();
	selBorderColor = m.GetSelBorderColor();
	selBorderLineType = m.GetSelBorderLineType();
	selFillColor = m.GetSelFillColor();
	closed = m.IsClosed();
	points <<= m.GetPoints();
	
} // END Constructor class Marker

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sets the marker to a rectangular area
void Marker::Set(const Point &p1, const Point &p2)
{
	kind = RectMarker;
	points.Clear();
	points.Add(Point(min(p1.x, p2.x), min(p1.y, p2.y)));
	points.Add(Point(max(p1.x, p2.x), max(p1.y, p2.y)));
		
} // END Marker::Set()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sets the marker to be a polygonal area
void Marker::Set(const Vector<Point> &pts)
{
	kind = PolyMarker;
	points <<= pts;
	
} // END Marker::Set()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// test for point hit
Marker::HitKind Marker::Hit(const Point &p, int minDist, int &hitIndex)
{
	
	switch(kind)
	{
		case EmptyMarker:
			return Miss;
			
		case RectMarker:
		{
			Point p1 = points[0];
			Point p2 = points[1];
			if(abs(p1.x - p.x) <= minDist && abs(p1.y - p.y) <= minDist)
				return TopLeft;
			if(abs(p2.x - p.x) <= minDist && abs(p1.y - p.y) <= minDist)
				return TopRight;
			if(abs(p1.x - p.x) <= minDist && abs(p2.y - p.y) <= minDist)
				return BottomLeft;
			if(abs(p2.x - p.x) <= minDist && abs(p2.y - p.y) <= minDist)
				return BottomRight;
			if(abs(p1.y - p.y) <= minDist && p.x >= p1.x && p.x <= p2.x)
				return Top;
			if(abs(p2.y - p.y) <= minDist && p.x >= p1.x && p.x <= p2.x)
				return Bottom;
			if(abs(p1.x - p.x) <= minDist && p.y >= p1.y && p.y <= p2.y)
				return Left;
			if(abs(p2.x - p.x) <= minDist && p.y >= p1.y && p.y <= p2.y)
				return Right;
			if(p.x > p1.x && p.x < p2.x && p.y > p1.y && p.y < p2.y)
				return Inside;
			return Miss;
		}
		
		case PolyMarker:
			// FIXME -- support polygon
		default:
			NEVER();
	} // switch
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// open/close the polygon (invalid for rectangle markers)
void Marker::Open()
{
	closed = false;
	
}

void Marker::Close()
{
	closed = true;
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clears the marker
void Marker::Clear(void)
{
	points.Clear();
	kind = EmptyMarker;
	closed = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gets rectangular bounding box of marker
Rect Marker::GetBoundingBox() const
{
	switch(kind)
	{
		case EmptyMarker:
			return Rect(0, 0, 0, 0);
			
		case RectMarker:
			return Rect(points[0], points[1]);
			
		case PolyMarker:
		{
			int minX = INT_MAX;
			int maxX = INT_MIN;
			int minY = INT_MAX;
			int maxY = INT_MIN;
			for(int i = 0; i < points.GetCount(); i++)
			{
				Point p = points[i];
				if(p.x < minX) minX = p.x;
				if(p.x > maxX) maxX = p.x;
				if(p.y < minY) minY = p.y;
				if(p.y > maxY) maxY = p.y;
			}
			return Rect(minX, minY, maxX, maxY);
		}
			
		default:
			NEVER();
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gets vertexs of marker
Vector<Point> Marker::GetPoints() const
{
	Vector<Point> res;
	
	switch(kind)
	{
		case RectMarker:
			res.Add(points[0]);
			res.Add(Point(points[1].x, points[0].y));
			res.Add(points[1]);
			res.Add(Point(points[0].x, points[1].y));
			return res;
			
		case PolyMarker:
			return Vector<Point>(points, 1);
			
		default:
			return Vector<Point>();
	}
}

// gets dragged outline
Vector<Point> Marker::DragOutline(Point const &startPt, Point const &endPt, int minDist)
{
	// if empty marker, just return empty polygon
	if(kind == EmptyMarker)
		return Vector<Point>();

	// find kind of hit on start point
	int index = 0;
	HitKind hitKind = Hit(startPt, minDist, index);
	if(hitKind == Miss)
		return Vector<Point>();

	int dx = endPt.x - startPt.x;
	int dy = endPt.y - startPt.y;
	switch(kind)
	{
		case RectMarker:
		{
			int x1 = points[0].x;
			int y1 = points[0].y;
			int x2 = points[1].x;
			int y2 = points[1].y;
			Vector<Point> res;
			switch(hitKind)
			{
				case TopLeft:
					x1 += dx;
					y1 += dy;
					break;
				case TopRight:
					x2 += dx;
					y1 += dy;
					break;
				case BottomLeft:
					x1 += dx;
					y2 += dy;
					break;
				case BottomRight:
					x2 += dx;
					y2 += dy;
					break;
				case Top:
					y1 += dy;
					break;
				case Bottom:
					y2 += dy;
					break;
				case Left:
					x1 += dx;
					break;
				case Right:
					x2 += dx;
					break;
				default: // inside
					x1 += dx; y1 += dy;
					x2 += dx; y2 += dy;
					break;
			}
			res.Add(Point(x1, y1));
			res.Add(Point(x2, y1));
			res.Add(Point(x2, y2));
			res.Add(Point(x1, y2));
			return res;
		}
			
		case PolyMarker:
			// FIXME -- support polygon
			
		default:
			NEVER();

	} // switch(kind)
	
	
} // END Marker::DragOutline()

// drag the marker to final position
void Marker::Drag(Point const &startPt, Point const &endPt, int minDist)
{
	if(kind == EmptyMarker)
		return;
	
	Vector<Point> pts = DragOutline(startPt, endPt, minDist);
	switch(kind)
	{
		case RectMarker:
		{
			points[0] = pts[0];
			points[1] = pts[2];
			break;
		}
		
		case PolyMarker:
			points = pick(pts);
			break;
			
		default:
			NEVER();
	}
	
} // END Marker::Drag()

END_UPP_NAMESPACE
