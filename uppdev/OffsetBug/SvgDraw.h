#ifndef _SvgDraw_SvgDraw_h_
#define _SvgDraw_SvgDraw_h_

#include <Draw/Draw.h>

NAMESPACE_UPP

class SvgDraw : public Draw 
{
private:
	XmlNode _svgNode;
	XmlNode *_currentParentNode;
	XmlNode *_defsNode;
	int _nextId;
	Array<XmlNode *> _parentNodes;
	Array<Point> _offsetStack;
	
public:
	SvgDraw(int xPixels, int yPixels)              { Init(xPixels, yPixels); }
	SvgDraw(Size size = Size(800, 600))            { Init(size.cx, size.cy); }
	
	void Init(int xPixels, int yPixels);
	String Finish();
	
	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual Rect GetClipOp() const;
	virtual bool IsPaintingOp(const Rect& r) const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	
private:
	String GetNextId();
};

END_UPP_NAMESPACE

#endif
