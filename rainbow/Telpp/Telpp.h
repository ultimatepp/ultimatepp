#define GUI_TELPP

#define _TODO_ _DBG_

#include <Draw/Draw.h>

#ifdef PLATFORM_POSIX
#include <CtrlCore/stdids.h>
#endif

NAMESPACE_UPP

#define IMAGECLASS FBImg
#define IMAGEFILE <Telpp/FB.iml>
#include <Draw/iml_header.h>

class SystemDraw : public SDraw {
public:
	virtual void  PutImage(Point p, const Image& img, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
public:	
	enum Code {
		RECT = 0,
		IMAGE = 1,
		SETIMAGE = 2,
	};
	
	static Index<int64>           img_index;
	
	static int GetImageI(SystemDraw& w, const Image& img);
	static void ResetI()          { img_index.Clear(); }

	StringBuffer result;

	void Put8(int x)              { result.Cat(x); }
	void Put16(int x);
	void Put(Point p);
	void Put(Size sz);
	void Put(const Rect& r);

	bool    CanSetSurface()                         { return false; }
	static void Flush()                             {}
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

	BackDraw();
	~BackDraw();
};

class ImageDraw : public SImageDraw {
public:
	ImageDraw(Size sz) : SImageDraw(sz) {}
	ImageDraw(int cx, int cy) : SImageDraw(cx, cy) {}
};

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern);

class TopWindowFrame;

#define GUIPLATFORM_CTRL_TOP_DECLS   Ctrl *owner_window;

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <Telpp/Ctrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS \
	bool dnd; \
	friend struct DnDLoop; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <Telpp/Top.h>

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

void USDLSetup(dword flags);


void InitTelpp();

END_UPP_NAMESPACE

#define GUIPLATFORM_INCLUDE_AFTER <Telpp/After.h>
