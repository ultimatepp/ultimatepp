#define GUI_FB

#include <Painter/Painter.h>

NAMESPACE_UPP

class SystemDraw : public BufferPainter {
public:
	Point              GetOffset() const                       { return Point(0, 0); }
	bool               CanSetSurface()                         { return false; }
	bool               Clip(const Rect& r)                     { return Draw::Clip(r); }
	bool               Clip(int x, int y, int cx, int cy)      { return Draw::Clip(x, y, cx, cy); }
	static void Flush()                                        {}

	SystemDraw(ImageBuffer& ib, int mode = MODE_ANTIALIASED) : BufferPainter(ib, mode) {}
};

struct BackDraw__ : public SystemDraw {
	ImageBuffer h;
	
	BackDraw__() : SystemDraw(h) {}
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

	BackDraw();
	~BackDraw();
};

struct ImageDraw__ {
	ImageBuffer    image;
	ImageBuffer    alpha;
	
	ImageDraw__(int cx, int cy) : image(cx, cy), alpha(cx, cy) {}
};

class ImageDraw : private ImageDraw__, public BufferPainter {
	BufferPainter  alpha_painter;
	bool           has_alpha;

	Image Get(bool pm) const;

public:
	Draw& Alpha();

	operator Image() const               { return Get(true); }
	
	Image GetStraight() const            { return Get(false); }
	
	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
};

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern);

#define GUIPLATFORM_KEYCODES_INCLUDE <Framebuffer/Keys.h>

#define GUIPLATFORM_CTRL_TOP_DECLS

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <Framebuffer/Ctrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <Framebuffer/Top.h>


extern ImageBuffer framebuffer;

// to be implemented by final FB {

bool FBIsWaitingEvent();
bool FBProcessEvent(bool *quit);
void FBSleep(int ms);
bool FBEndSession();

// }

class PrinterJob { // Dummy only...
	NilDraw             nil;
	Vector<int>         pages;

public:
	Draw&               GetDraw()                       { return nil; }
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

#define GUIPLATFORM_INCLUDE_AFTER <Framebuffer/After.h>
