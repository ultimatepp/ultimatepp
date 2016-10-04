#ifndef _GtkApp_GtkApp_h_
#define _GtkApp_GtkApp_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <gtk/gtk.h>

class CairoDraw : public Draw {
	virtual dword GetInfo() const;

	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual bool IsPaintingOp(const Rect& r) const;
	virtual Rect GetPaintRect() const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void SysDrawImageOp(int x, int y, const Image& img, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);

	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);

	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);


private:
	void  SetColor(Color c);
	Point GetOffset() const;
	void  RectPath(const Rect& r);
	void  Push();
	void  Pop();
	Vector<Point> offset;

	cairo_t *cr;
	CairoDraw() {}
	
	friend class CairoImageDraw;

public:
	operator cairo_t*()              { return cr; }

	CairoDraw(cairo_t *cr) : cr(cr) {}
};

class CairoImageDraw : public CairoDraw {
	cairo_surface_t *surface;
	Size             isz;
	
	CairoDraw        alpha;
	cairo_surface_t *alpha_surface;

	void Init(Size sz);
	void FetchStraight(ImageBuffer& b) const;

public:
	Draw& Alpha();                       

	operator Image() const;

	Image GetStraight() const;

	CairoImageDraw(Size sz);
	CairoImageDraw(int cx, int cy);
	~CairoImageDraw();
};

#endif
