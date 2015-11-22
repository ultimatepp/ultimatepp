#define GUI_SDL20

NAMESPACE_UPP

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
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);

	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	
	virtual Size GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;

private:
	Size  pageSize;
	Size  nativeSize;
	Size  nativeDpi;
	bool  palette:1;
	bool  color16:1;
	bool  is_mono:1;
	int   native;

	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Font;

	friend void StaticExitDraw_();

	Point     actual_offset_bak;

	struct Cloff : Moveable<Cloff> {
		Point org;
		HRGN  hrgn;
		Rect  drawingclip;
	};

	Array<Cloff> cloff;
	Rect         drawingclip;

	COLORREF  lastTextColor;
	Color     lastColor;
	HBRUSH    orgBrush;
	HBRUSH    actBrush;
	HPEN      orgPen;
	HPEN      actPen;
	int       lastPen;
	Color     lastPenColor;

	void   Unselect0();
	void   Cinit();

	void   LoadCaps();
	void   SetPrinterMode();
	void   Reset();
	void   SetOrg();
	friend HPALETTE GetQlibPalette();
	void   DotsMode();

	static void      InitColors();
	
	friend class BackDraw;
	friend class ScreenDraw;
	friend class PrintDraw;

protected:
	dword style;
	HDC   handle;
	Point actual_offset;

	SystemDraw();
	void   Init();
	void   InitClip(const Rect& clip);

public:
	static Rect GetVirtualScreenArea();

	static void SetAutoPalette(bool ap);
	static bool AutoPalette();
	bool PaletteMode()                                  { return palette; }

	static void Flush()                                 { GdiFlush(); }

	COLORREF GetColor(Color color) const;
	
	Point    GetOffset() const                          { return actual_offset; }

#ifndef PLATFORM_WINCE
	Point LPtoDP(Point p) const;
	Point DPtoLP(Point p) const;
	Rect  LPtoDP(const Rect& r) const;
	Rect  DPtoLP(const Rect& r) const;
#endif

	void SetColor(Color color);
	void SetDrawPen(int width, Color color);

	Size  GetSizeCaps(int i, int j) const;
	HDC   BeginGdi();
	void  EndGdi();
	HDC   GetHandle()                    { return handle; }
	operator HDC() const                 { return handle; }
	void  Unselect();
	void  Attach(HDC ahandle)            { handle = ahandle; Init(); }
	HDC   Detach()                       { Unselect(); HDC h = handle; handle = NULL; return h; }

	SystemDraw(HDC hdc);
	virtual ~SystemDraw();
	
	bool CanSetSurface()                 { return IsGui() && IsWinNT(); }
};

#ifndef PLATFORM_WINCE
class WinMetaFile {
	Size size;
	HENHMETAFILE hemf;

	void     Init();

public:
	void         Attach(HENHMETAFILE emf);
	HENHMETAFILE Detach();
	
	void     Set(const void *data, dword len);
	void     Set(const String& data)        { Set(~data, data.GetCount()); }
	
	String   Get() const;

	operator bool() const                   { return hemf; }
	void     SetSize(const Size& sz)        { size = sz; }
	Size     GetSize() const                { return hemf ? size : Size(0, 0); }

	void     Clear();

	void     Paint(Draw& w, const Rect& r) const;
	void     Paint(Draw& w, int x, int y, int cx, int cy) const;

	void     Serialize(Stream& s);

	void     ReadClipboard();
	void     WriteClipboard() const;
	void     Load(const char *file)         { Set(LoadFile(file)); }

	WinMetaFile()                           { Init(); }
	WinMetaFile(HENHMETAFILE hemf);
	WinMetaFile(HENHMETAFILE hemf, Size sz);
	WinMetaFile(const char *file);
	WinMetaFile(void *data, int len);
	WinMetaFile(const String& data);

	~WinMetaFile()                                { Clear(); }

	HENHMETAFILE GetHEMF() const                  { return hemf; }
};

class WinMetaFileDraw : public SystemDraw {
	Size size;

public:
	bool        Create(HDC hdc, int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	bool        Create(int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	WinMetaFile Close();

	WinMetaFileDraw() {}
	WinMetaFileDraw(HDC hdc, int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	WinMetaFileDraw(int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	~WinMetaFileDraw();
};

void DrawWMF(Draw& w, int x, int y, int cx, int cy, const String& wmf);
void DrawWMF(Draw& w, int x, int y, const String& wmf);
Drawing LoadWMF(const char *path, int cx, int cy);
Drawing LoadWMF(const char *path);

String  AsWMF(const Drawing& iw);

#endif

class ScreenDraw : public SystemDraw {
public:
	ScreenDraw(bool ic = false);
	~ScreenDraw();
};

#ifndef PLATFORM_WINCE
class PrintDraw : public SystemDraw {
public:
	virtual void StartPage();
	virtual void EndPage();

private:
	bool aborted;
	
	void  InitPrinter();
public:
	PrintDraw(HDC hdc, const char *jobname);
	~PrintDraw();
};
#endif

inline bool     BitBlt(HDC ddc, Point d, HDC sdc, const Rect& s, dword rop = SRCCOPY)
{ return BitBlt(ddc, d.x, d.y, s.Width(), s.Height(), sdc, s.left, s.top, rop); }

inline bool     StretchBlt(HDC ddc, const Rect& r, HDC sdc, const Rect& s, dword rop = SRCCOPY)
{ return StretchBlt(ddc, r.left, r.top, r.Width(), r.Height(), sdc, s.left, s.top, s.Width(), s.Height(), rop); }

inline bool     PatBlt(HDC dc, const Rect& r, dword rop = PATCOPY)
{ return PatBlt(dc, r.left, r.top, r.Width(), r.Height(), rop); }

inline void     MoveTo(HDC hdc, Point pt)                         { MoveToEx(hdc, pt.x, pt.y, 0); }
inline void     LineTo(HDC hdc, Point pt)                         { LineTo(hdc, pt.x, pt.y); }

inline void     DrawLine(HDC hdc, Point p, Point q)               { MoveTo(hdc, p); LineTo(hdc, q); }
inline void     DrawLine(HDC hdc, int px, int py, int qx, int qy) { MoveToEx(hdc, px, py, 0); LineTo(hdc, qx, qy); }

#ifndef PLATFORM_WINCE
inline void     DrawArc(HDC hdc, const Rect& rc, Point p, Point q){ Arc(hdc, rc.left, rc.top, rc.right, rc.bottom, p.x, p.y, q.x, q.y); }
#endif
inline void     DrawCircle(HDC hdc, int x, int y, int radius)     { Ellipse(hdc, x - radius, y - radius, x + radius + 1, y + radius + 1); }
inline void     DrawCircle(HDC hdc, Point centre, int radius)     { DrawCircle(hdc, centre.x, centre.y, radius); }
inline void     DrawEllipse(HDC hdc, const Rect& rc)              { Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom); }

inline void     DrawRect(HDC hdc, const Rect& rc)                 { Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); }

HDC      ScreenHDC();
HPALETTE GetQlibPalette();

Image Win32Icon(LPCSTR id, int iconsize = 0);
Image Win32Icon(int id, int iconsize = 0);
Image Win32Cursor(LPCSTR id);
Image Win32Cursor(int id);
HICON IconWin32(const Image& img, bool cursor = false);
Image Win32DllIcon(const char *dll, int ii, bool large);

class BackDraw : public SystemDraw {
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

protected:
	HBITMAP hbmpold;
	HBITMAP hbmp;

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
	Size    size;

	struct  Section {
		HDC     dc;
		HBITMAP hbmp, hbmpOld;
		RGBA   *pixels;

		void Init(int cx, int cy);
		~Section();
	};

	Section     rgb;
	Section     a;
	SystemDraw  alpha;


	bool    has_alpha;

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

END_UPP_NAMESPACE

#define GUIPLATFORM_KEYCODES_INCLUDE "Win32Keys.h"


#define GUIPLATFORM_CTRL_TOP_DECLS \
	HWND           hwnd; \
	UDropTarget   *dndtgt; \


#define GUIPLATFORM_CTRL_DECLS_INCLUDE "Win32Ctrl.h"


#define GUIPLATFORM_PASTECLIP_DECLS \
	UDropTarget *dt; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE "Win32Top.h"

NAMESPACE_UPP

inline unsigned GetHashValue(const HWND& hwnd)
{
	return (unsigned)(intptr_t)hwnd;
}
END_UPP_NAMESPACE

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

#include <ShellAPI.h>

#endif
#endif

#define GUIPLATFORM_INCLUDE_AFTER "Win32GuiA.h"
