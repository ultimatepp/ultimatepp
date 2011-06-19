#define GUI_FB

#include <Painter/Painter.h>

NAMESPACE_UPP

class SystemDraw : public BufferPainter {
public:
	Point    GetOffset() const                             { return Point(0, 0); }
	bool     CanSetSurface()                               { return false; }
	static void Flush()                                    {}

	SystemDraw(ImageBuffer& ib, int mode = MODE_ANTIALIASED) : BufferPainter(ib, mode) {}
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

class ImageDraw__ {
	ImageBuffer    image;
	ImageBuffer    alpha;
	
	ImageDraw__(int cx, int cy) : image(cx, cy), alpha(cx, cy) {}
};

class ImageDraw : public BufferPainter, private ImageDraw__ {
	Size           size;
	BufferPainter  alpha_painter;
	bool           has_alpha;

public:
	Draw& Alpha();

	operator Image() const;
	
	Image GetStraight() const;
	
	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
}

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

class PrinterJob {
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
