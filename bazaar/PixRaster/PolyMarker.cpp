#include "PolyMarker.h"

using namespace Upp;

// constructors
PolyMarker::PolyMarker()
{
	kind = Empty;
	borderThickness = 0;
	borderColor = Black;
	borderLineType = 0;
	fillColor.a = 0;
	fillColor.r = 0;
	fillColor.g = 0;
	fillColor.b = 0;
	closed = false;
}

PolyMarker::PolyMarker(const PolyMarker &pm)
{
	kind = pm.GetKind();
	borderThickness = pm.GetBorderThickness();
	borderColor = pm.GetBorderColor();
	borderLineType = pm.GetBorderLineType();
	fillColor = pm.GetFillColor();
	closed = pm.IsClosed();
	
}

// test for point hit
PolyMarker::HitKind PolyMarker::Hit(const Point &p, int minDist)
{
	
	switch(kind)
	{
		case Empty:
			return Miss;
			
		case Rectangle:
			{
				Point p1 = points[0];
				Point p2 = points[1];
				int x1 = min(p1.x, p2.x);
				int y1 = min(p1.y, p2.y);
				int x2 = max(p1.x, p2.x);
				int y2 = max(p1.y, p2.y);
				if(abs(p1.x - p.x) <= minDist && abs(p1.y - p.y) <= minDist)
					return Vertex;
				if(abs(p2.x - p.x) <= minDist && abs(p2.y - p.y) <= minDist)
					return Vertex;
				if(abs(p1.x - p.x) < minDist && p.y >= y1 - minDist && p.y <= y2 + minDist)
					return Side;
				if(abs(p2.x - p.x) < minDist && p.y >= y1 - minDist && p.y <= y2 + minDist)
					return Side;
				if(abs(p1.y - p.y) < minDist && p.x >= x1 - minDist && p.x <= y2 + minDist)
					return Side;
				if(abs(p2.y - p.y) < minDist && p.x >= x1 - minDist && p.x <= y2 + minDist)
					return Side;
				if(p.x > x1 && p.x < x2 && p.y > y1 && p.y < y2)
				   return Inside;
				return Miss;
			}
		
		case Polygon:
			// FIXME -- support polygon
		default:
			NEVER();
	} // switch
}

// gets vertex or side on point
One<Point> const PolyMarker::VertexAt(const Point &p, int minDist)
{
	switch(kind)
	{
		case Empty:
			return One<Point>();
			
		case Rectangle:
			{
				Point p1 = points[0];
				Point p2 = points[1];
				int x1 = min(p1.x, p2.x);
				int y1 = min(p1.y, p2.y);
				int x2 = max(p1.x, p2.x);
				int y2 = max(p1.y, p2.y);
				if(abs(p1.x - p.x) <= minDist && abs(p1.y - p.y) <= minDist)
					return One<Point>(new Point(p1.x, p1.y));
				else if(abs(p2.x - p.x) <= minDist && abs(p1.y - p.y) <= minDist)
					return One<Point>(new Point(p2.x, p1.y));
				else if(abs(p2.x - p.x) <= minDist && abs(p2.y - p.y) <= minDist)
					return One<Point>(new Point(p2.x, p2.y));
				else if(abs(p1.x - p.x) <= minDist && abs(p2.y - p.y) <= minDist)
					return One<Point>(new Point(p1.x, p2.y));
				return One<Point>();
			}
			
		case Polygon:
			{
				for(int i = 0; i < points.GetCount(); i++)
				{
					Point pi = points[i];
					if(abs(pi.x - p.x) <= minDist && abs(pi.y - p.y) <= minDist)
						return One<Point>(new Point(pi));
				}
				return One<Point>();
			}
		
		default:
			return One<Point>();
	} // switch kind
}

One<Rect> const PolyMarker::SideAt(const Point &p, int minDist)
{
	switch(kind)
	{
		case Empty:
			return One<Rect>();
			
		case Rectangle:
			{
				Point p1 = points[0];
				Point p2 = points[1];
				int x1 = min(p1.x, p2.x);
				int y1 = min(p1.y, p2.y);
				int x2 = max(p1.x, p2.x);
				int y2 = max(p1.y, p2.y);
				if(abs(p1.x - p.x) < minDist && p.y >= y1 - minDist && p.y <= y2 + minDist)
					return One<Rect>(new Rect(Point(p1.x, y1), Point(p1.x, y2)));
				else if(abs(p2.x - p.x) < minDist && p.y >= y1 - minDist && p.y <= y2 + minDist)
					return One<Rect>(new Rect(Point(p2.x, y1), Point(p2.x, y2)));
				else if(abs(p1.y - p.y) < minDist && p.x >= x1 - minDist && p.x <= x2 + minDist)
					return One<Rect>(new Rect(Point(x1, p1.y), Point(x2, p1.y)));
				else if(abs(p2.y - p.y) < minDist && p.x >= x1 - minDist && p.x <= x2 + minDist)
					return One<Rect>(new Rect(Point(x1, p2.y), Point(x2, p2.y)));
				return One<Rect>();
			}
			
		case Polygon:
			// fixme : add support
			
		default:
			return One<Rect>();

	} // switch kind
}


// open/close the polygon (invalid for rectangle markers)
bool PolyMarker::Open(void)
{
	closed = false;
	
}

bool PolyMarker::Close(void)

{
	closed = true;
	
}

// adds a vertex to the polygon (invalid for rectangle markers)
bool PolyMarker::AddVertex(const Point &p)
{
	if(closed || kind == Rectangle)
		return false;
	points.Add(p);
	return true;
	
}

bool PolyMarker::AddVertex(const Array<Point> &ap)
{
	if(closed || kind == Rectangle || !ap.GetCount())
		return false;
	for(int i = 0; i < ap.GetCount(); i++)
		points.Add(ap[i]);
	return true;
}

void PolyMarker::MakeRect(const Point &p1, const Point &p2)
{
	points.Clear();
	points.Add(p1);
	points.Add(p1);
	kind = Rectangle;
	closed = true;
}

// clears the marker
void PolyMarker::Clear(void)
{
	points.Clear();
	kind = Empty;
	closed = false;
}

// gets a dragged PolyMarker giving start and end point
PolyMarker PolyMarker::Drag(const Point &startP, const Point &endP)
{
	
}

// changes the PolyMarker at end of drag ops
void PolyMarker::DoDrag(const Point &startP, const Point &endP)
{
	
}

// paint the PolyMarker on a given viewport
void PolyMarker::Paint(const Rect &vport)
{
	
}
