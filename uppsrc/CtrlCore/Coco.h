#define GUI_COCO

NAMESPACE_UPP

struct RectCG;

class SystemDraw : public Draw {
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
	Vector<Point> offset;
	Vector<Rect>  clip;

	int   top;
	Color fill = Null;
	
	void   Push();
	void   Pop();
	
	void  *handle;
	
	void   ClipCG(const Rect& r);
	void   FlipY(int& y)           { y = top - y; }
	Rect   GetClip() const         { return clip.GetCount() ? clip.Top() : Rect(-999999, -999999, 999999, 999999); }
	Point  GetOffset() const       { return offset.GetCount() ? offset.Top() : Point(0, 0); }
	RectCG Convert(int x, int y, int cx, int cy);
	RectCG Convert(const Rect& r);

	void  Set(Color c);

	void Init(void *cgContext, int cy);

	SystemDraw() {}
	
	friend class ImageDraw;
	friend class BackDraw;
	friend class ViewDraw;
	friend class BackDraw__;

public:
	bool     CanSetSurface()          { return false; }
	static void Flush()               {} // TODO?

	SystemDraw(void *cgContext, int cy);
	~SystemDraw();
};

 
inline void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size srcsz, Point poff)
{ // TODO: Unless we can do this...
	NEVER();
}

class ImageDraw : public SystemDraw {
	ImageBuffer ib;
	
	One<ImageDraw> alpha;

	void Init(int cx, int cy);

public:
	Draw& Alpha();

	operator Image();

	Image GetStraight();

	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};

struct BackDraw__ : public SystemDraw {
	BackDraw__() : SystemDraw() {}
};

class BackDraw : public BackDraw__ { // Dummy only, as we are running in GlobalBackBuffer mode
	Size        size;
	Draw       *painting;
	Point       painting_offset;
	ImageBuffer ib;
	
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

public:
	void  Put(SystemDraw& w, int x, int y)             {}
	void  Put(SystemDraw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(SystemDraw& w, int cx, int cy)         {}
	void Create(SystemDraw& w, Size sz)                { Create(w, sz.cx, sz.cy); }
	void Destroy()                                     {}

	void SetPaintingDraw(Draw& w, Point off)           { painting = &w; painting_offset = off; }

	Point GetOffset() const                            { return Point(0, 0); }

	BackDraw();
	~BackDraw();
};

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern);

#ifndef PLATFORM_WIN32
#include <CtrlCore/stdids.h>
#endif

#define GUIPLATFORM_KEYCODES_INCLUDE <CtrlCore/CocoKeys.h>

struct CocoTop;
struct MMCtrl;
struct MMImp;

#define GUIPLATFORM_CTRL_TOP_DECLS CocoTop *coco = NULL;

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <CtrlCore/CocoCtrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <CtrlCore/CocoTop.h>

class PrinterJob {
	NilDraw             nild;
	Vector<int>         pages;

public:
	Draw&               GetDraw()                       { return nild; }
	operator            Draw&()                         { return GetDraw(); }
	const Vector<int>&  GetPages() const                { return pages; }
	int                 operator[](int i) const         { return 0; }
	int                 GetPageCount() const            { return 0; }

	bool                Execute()                       { return false; }

	PrinterJob& Landscape(bool b = true)                { return *this; }
	PrinterJob& MinMaxPage(int minpage, int maxpage)    { return *this; }
	PrinterJob& PageCount(int n)                        { return *this; }
	PrinterJob& CurrentPage(int currentpage)            { return *this; }
	PrinterJob& Name(const char *_name)                 { return *this; }

	PrinterJob(const char *name = NULL)                 {}
	~PrinterJob()                                       {}
};

END_UPP_NAMESPACE

#define GUIPLATFORM_INCLUDE_AFTER <CtrlCore/CocoAfter.h>
