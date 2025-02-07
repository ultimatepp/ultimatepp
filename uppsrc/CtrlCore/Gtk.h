#define GUI_GTK

#define PLATFORM_X11 // To keep legacy code happy

#define GDKEY(x) GDK_KEY_##x

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace Upp {

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
	void  RectPath(const Rect& r);
	void  Push();
	void  Pop();
	Vector<Point> offset;
	Vector<Rect>  clip;
	Vector<Rect>  invalid; // for IsPainting checks, if empty, only clip extents is checked

	cairo_t      *cr;
	
	struct TextGlyph : Moveable<TextGlyph> {
		int x;
		int y;
		int index;
	};

	Font              textfont;
	Color             textink;
	int               textangle;
	Vector<TextGlyph> textcache;
	
	SystemDraw() {}
	
	friend class ImageDraw;
	friend class BackDraw;
	friend class TopFrameDraw;

	Rect     GetClip() const;
	void     FlushText();
	
	friend void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size srcsz, Point poff);

public:
	void  SetColor(Color c);
	operator cairo_t*()               { return cr; }

	void     PickInvalid(Vector<Rect>&& inv)   { invalid = pick(inv); }
	Point    GetOffset() const;
	bool     CanSetSurface()          { return true; }
	static void Flush()               {} // TODO?

	SystemDraw(cairo_t *cr);
	~SystemDraw();
};

class ImageDraw : public SystemDraw {
	cairo_surface_t *surface;
	Size             isz;
	
	SystemDraw       alpha;
	cairo_surface_t *alpha_surface;
	bool             del;

	void Init(Size sz);
	void FetchStraight(ImageBuffer& b) const;

public:
	Draw& Alpha();

	operator Image() const;

	Image GetStraight() const;

	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();

	ImageDraw(cairo_t *cr, Size sz); // Special variant for chameleon skinning
};

class BackDraw : public SystemDraw {
	Size        size;
	Draw       *painting;
	Point       painting_offset;
	
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

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

struct GdkRect : GdkRectangle {
	operator GdkRectangle *() { return this; }
	GdkRect(const Rect& r);
};

class ImageGdk {
	GdkPixbuf *pixbuf;
	Image      img;

	void Free();

public:
	operator GdkPixbuf *()    { return pixbuf; }

	bool Set(const Image& m);
	          
	ImageGdk();
	ImageGdk(const Image& m);
	~ImageGdk();
};

class GtkCSD final : Rect { // wayland client side decoration handling
	bool enabled = false;

public:
	static bool IsSSDSupported();
	
	void Create(GdkWindowTypeHint hint);
	
	bool IsEnabled() const   { return enabled; }
	
	int ExtraWidth() const   { return left + right; }
	int ExtraHeight() const  { return top + bottom; }
	
	int LeftMargin() const   { return left; }
	int RightMargin() const  { return right; }
	int TopMargin() const    { return top; }
	int BottomMargin() const { return bottom; }
};

String FilesClipFromUrisFree(gchar **uris);
String ImageClipFromPixbufUnref(GdkPixbuf *pixbuf);

GdkAtom GAtom(const String& id);

#ifdef GDK_WINDOWING_X11
Vector<int> GetPropertyInts(GdkWindow *w, const char *property);
#endif

#ifndef PLATFORM_WIN32
#include <CtrlCore/stdids.h>
#endif

#define GUIPLATFORM_KEYCODES_INCLUDE <CtrlCore/GtkKeys.h>

#define GUIPLATFORM_CTRL_TOP_DECLS \
	GtkWidget            *window; \
	GtkWidget            *header = nullptr; \
	GtkWidget            *drawing_area = nullptr; \
	GtkIMContext         *im_context = nullptr; \
	GtkIMContext         *im_context_simple; \
	GtkIMContext         *im_context_multi; \
	GtkCSD                csd; \
	int64                 cursor_id; \
	int                   id; \

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <CtrlCore/GtkCtrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS \
int type; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <CtrlCore/GtkTop.h>

#define GUIPLATFORM_NOSCROLL

int rmsecs();

}

#define GUIPLATFORM_INCLUDE_AFTER <CtrlCore/GtkAfter.h>

#define HAS_TopFrameDraw
