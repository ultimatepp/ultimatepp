#define GUI_X11
#define PLATFORM_X11 // To keep legacy code happy

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xinerama.h>

#undef Picture
#undef Time
#undef Font
#undef Display

#undef True
#undef False

#define XFalse 0
#define XTrue  1

namespace Upp {

extern XDisplay   *Xdisplay;
extern Visual     *Xvisual;
extern int         Xscreenno;
extern Window      Xroot;
extern Screen     *Xscreen;
extern Colormap    Xcolormap;
extern int         Xheight;
extern int         Xwidth;
extern int         XheightMM;
extern int         XwidthMM;
extern int         Xdepth;
extern dword       Xblack;
extern dword       Xwhite;
extern int         Xconnection;

extern dword   (*Xgetpixel)(int r, int g, int b);

void          InitX11Draw(const char *dispname = NULL);
void          InitX11Draw(XDisplay *display);

void   XError();
void   XError(const char *s);

inline dword GetXPixel(int r, int g, int b) { return (*Xgetpixel)(r, g, b); }
inline dword GetXPixel(Color color)         { return (*Xgetpixel)(color.GetR(), color.GetG(), color.GetB()); }

enum {
	X11_ROP2_ZERO,
	X11_ROP2_AND,
	X11_ROP2_AND_NOT,
	X11_ROP2_COPY,
	X11_ROP2_NOT_AND,
	X11_ROP2_NOP,
	X11_ROP2_XOR,
	X11_ROP2_OR,
	X11_ROP2_NOT_AND_NOT,
	X11_ROP2_NOT_XOR,
	X11_ROP2_INVERT,
	X11_ROP2_OR_NOT,
	X11_ROP2_NOT_COPY,
	X11_ROP2_NOT_OR,
	X11_ROP2_NOT_OR_NOT,
	X11_ROP2_ONE,
};

void SetClip(GC gc, XftDraw *xftdraw, const Vector<Rect>& cl);

class SystemDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Size  GetPageSize() const;

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
	virtual void SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color);
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
	
	virtual Size GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;

private:
	Size    pageSize;

	SystemDraw();

	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Font;
	friend class  TopFrameDraw;

	friend void StaticExitDraw_();
	friend Font StdFont();

	Point        actual_offset;
	Point        actual_offset_bak;
	struct Cloff : Moveable<Cloff> {
		Point org;
		int clipi;
		int offseti;
	};

	Array<Cloff> cloff;
	Rect         drawingclip;
	Vector< Vector<Rect> > clip;
	Vector<Point> offset;

	Drawable   dw;
	GC         gc;
	XftDraw   *xftdraw;

	int        foreground;
	int        linewidth;

	void       Init();
	void       Init(const Vector<Rect>& clip, Point offset = Point(0, 0));
	void       CloneClip();

	friend class BackDraw;
	friend class ScreenDraw;

public:
	static void Flush()                                 { XSync(Xdisplay, false); }
	static Image X11Cursor(int c);
	static void *CursorX11(const Image& img);

	Point    GetOffset() const                          { return actual_offset; }

	static XftFont *CreateXftFont(Font f, int angle);
	XftDraw *GetXftDraw() const               { return xftdraw; }

	Rect  GetClip() const;

	void  SetForeground(Color color);
	void  SetLineStyle(int width);
	void  SetClip();

	Drawable GetDrawable() const              { return dw; }
	GC       GetGC() const                    { return gc; }
	const Vector<Rect>& GetClipList() const   { return clip.Top(); }

	SystemDraw(Drawable dw, GC gc, XftDraw *xftdraw, const Vector<Rect>& clip);
	
	bool CanSetSurface()                      { return true; }
};

Image X11Cursor(int c);
void *CursorX11(const Image& img);

class BackDraw : public SystemDraw {
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

protected:
	Pixmap  pixmap;
	Size    size;
	Draw   *painting;
	Point   painting_offset;

public:
	void  Put(SystemDraw& w, int x, int y);
	void  Put(SystemDraw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(SystemDraw& w, int cx, int cy);
	void Create(SystemDraw& w, Size sz)                { Create(w, sz.cx, sz.cy); }
	void Destroy();

	void SetPaintingDraw(Draw& w, Point off)           { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
};

class ImageDraw : public SystemDraw {
	Size       size;
	SystemDraw alpha;
	bool       has_alpha;

	void Init();
	Image Get(bool pm) const;

public:
	Draw& Alpha();                       

	operator Image() const;
	
	Image GetStraight() const;
	
	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};

}

#include "X11/keysym.h"


#define GUIPLATFORM_KEYCODES_INCLUDE "X11Keys.h"


#define GUIPLATFORM_CTRL_TOP_DECLS \
	Window         window;


#define GUIPLATFORM_CTRL_DECLS_INCLUDE "X11Ctrl.h"

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE "X11Top.h"

#define GUIPLATFORM_PASTECLIP_DECLS \
	int          type;

namespace Upp {

String XAtomName(Atom atom);
Atom   XAtom(const char *name);

String      GetProperty(Window w, Atom property, Atom rtype = AnyPropertyType);
Vector<int> GetPropertyInts(Window w, Atom property, Atom rtype = AnyPropertyType);
String      ReadPropertyData(Window w, Atom property, Atom rtype = AnyPropertyType);

Index<Atom>& _NET_Supported();

#include "stdids.h"

#define GUIPLATFORM_INCLUDE_AFTER "X11GuiA.h"

#define HAS_TopFrameDraw

}
