#ifndef DRAW_H
#define DRAW_H

#include <Core/Core.h>

#ifdef PLATFORM_X11

#define PLATFORM_XFT //TODO - remove

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#ifdef PLATFORM_XFT
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>
#endif

#undef Picture
#undef Time
#undef Font
#undef Display

#undef True
#undef False

#define XFalse 0
#define XTrue  1
#endif


NAMESPACE_UPP

#ifdef PLATFORM_X11

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

#endif

class Drawing;
class Draw;
class Painting;

#ifdef PLATFORM_WIN32
HDC ScreenHDC();
#endif

bool ScreenInPaletteMode();
Size GetScreenSize();

#include "Image.h"

void EnterGuiMutex();
void EnterGuiMutex(int n);
void LeaveGuiMutex();
int  LeaveGuiMutexAll();

#ifdef _MULTITHREADED
struct GuiLock {
	GuiLock()  { EnterGuiMutex(); }
	~GuiLock() { LeaveGuiMutex(); }
};
#else
struct GuiLock {
	GuiLock()  {}
	~GuiLock() {}
};
#endif

typedef GuiLock DrawLock;

const int FONT_V = 40;

class FontInfo;

class Font : AssignValueTypeNo<Font, FONT_V, Moveable<Font> >{
	word  face;
	word  flags;
	int16 height;
	int16 width;

public:
	enum {
		FIXEDPITCH  = 0x0001,
		SCALEABLE   = 0x0002,
		SYMBOLTYPE  = 0x0004,
		COMPOSED    = 0x0008,
		LOCAL       = 0x0010,
	};

	static int    GetFaceCount();
	static String GetFaceName(int index);
	static int    FindFaceNameIndex(const char *name);
	static dword  GetFaceInfo(int index);

	enum {
		STDFONT,
		SCREEN_SERIF,
		SCREEN_SANS,
		SCREEN_FIXED,
		ROMAN,
		ARIAL,
		COURIER,
		SYMBOL,
	#ifdef PLATFORM_WIN32
		WINGDINGS,
		TAHOMA,
	#endif
		OTHER,
	};

	int    GetFace() const          { return face; }
	int    GetHeight() const        { return height; }
	int    GetWidth() const         { return width; }
	bool   IsBold() const           { return flags & 0x8000; }
	bool   IsItalic() const         { return flags & 0x4000; }
	bool   IsUnderline() const      { return flags & 0x2000; }
	bool   IsStrikeout() const      { return flags & 0x1000; }
	bool   IsNonAntiAliased() const { return flags & 0x800; }
	bool   IsTrueTypeOnly() const   { return flags & 0x400; }
	String GetFaceName() const;
	dword  GetFaceInfo() const;

	FontInfo Info() const;

	Font& Face(int n)            { face = n; return *this; }
	Font& Height(int n)          { height = n; return *this; }
	Font& Width(int n)           { width = n; return *this; }
	Font& Bold()                 { flags |= 0x8000; return *this; }
	Font& NoBold()               { flags &= ~0x8000; return *this; }
	Font& Bold(bool b)           { return b ? Bold() : NoBold(); }
	Font& Italic()               { flags |= 0x4000; return *this; }
	Font& NoItalic()             { flags &= ~0x4000; return *this; }
	Font& Italic(bool b)         { return b ? Italic() : NoItalic(); }
	Font& Underline()            { flags |= 0x2000; return *this; }
	Font& NoUnderline()          { flags &= ~0x2000; return *this; }
	Font& Underline(bool b)      { return b ? Underline() : NoUnderline(); }
	Font& Strikeout()            { flags |= 0x1000; return *this; }
	Font& NoStrikeout()          { flags &= ~0x1000; return *this; }
	Font& Strikeout(bool b)      { return b ? Strikeout() : NoStrikeout(); }
	Font& NonAntiAliased()       { flags |= 0x800; return *this; }
	Font& NoNonAntiAliased()     { flags &= ~0x800; return *this; }
	Font& NonAntiAliased(bool b) { return b ? NonAntiAliased() : NoNonAntiAliased(); }
	Font& TrueTypeOnly()         { flags |= 0x400; return *this; }
	Font& NoTrueTypeOnly()       { flags &= ~0x400; return *this; }
	Font& TrueTypeOnly(bool b)   { return b ? TrueTypeOnly() : NoTrueTypeOnly(); }
	Font& FaceName(const String& name);

	Font  operator()() const       { return *this; }
	Font  operator()(int n) const  { return Font(*this).Height(n); }

	void  Serialize(Stream& s);

	bool  operator==(Font f) const { return ((face ^ f.face) | (flags ^ f.flags) |
	                                         (width ^ f.width) | (height ^ f.height)) == 0; }
	bool  operator!=(Font f) const { return !operator==(f); }

	dword GetHashValue() const     { return MAKELONG(flags, width) ^ MAKELONG(face, height); }
	bool  IsNull() const           { return face == 0xffff; }

	Font()                         { height = width = 0; face = flags = 0; }
	Font(int _face, int _height)   { face = _face; height = _height; flags = 0; width = 0; }
	Font(const Nuller&)            { face = 0xffff; height = width = 0; flags = 0; }

	operator Value() const         { return RichValue<Font>(*this); }
	Font(const Value& q)           { *this = RichValue<Font>::Extract(q); }
};

template<>
inline bool IsNull(const Font& f)            { return f.IsNull(); }

template<>
inline unsigned GetHashValue(const Font& f)  { return f.GetHashValue(); }

template<>
String AsString(const Font& f);

class FontInfo : Moveable<FontInfo> {
	struct CharMetrics : Moveable<CharMetrics> {
		int  width;
		int  lspc;
		int  rspc;

		bool operator==(const CharMetrics& b) const
		     { return width == b.width && lspc == b.lspc && rspc == b.rspc; }
	};

	struct Kinfo : Moveable<Kinfo> {
		CharMetrics std;
		byte       *flags;

		Kinfo() {
			flags = NULL;
		}
		~Kinfo() {
			if(flags)
				delete[] flags;
		}
	};

	struct Data : public Link<Data, 2> {
		bool         HasChar(int ch) const;
		void         GetMetrics(CharMetrics *t, int from, int count);
	#ifdef PLATFORM_X11
		void         CreateFont(int i, int cs);
	#endif

		int          refcount;
		Font         font;
		int          angle;
		int          device;
	#ifdef PLATFORM_WIN32
		HFONT        hfont;
	#endif
	#ifdef PLATFORM_XFT
		XftFont     *xftfont;
		XftFont     *xftfont0;
	#endif
		int          ascent;
		int          descent;
		int          external;
		int          internal;
		int          height;
		int          lineheight;
		int          overhang;
		Size         offset;
		int          avewidth;
		int          maxwidth;
		int          firstchar;
		int          charcount;
		int          default_char;

		CharMetrics *base[64];

		Mutex         xmutex;
		Vector<Kinfo> kinfo;
		VectorMap<dword, CharMetrics> xx;

		bool         fixedpitch;
		bool         scaleable;
		int          spacebefore;
		int          spaceafter;
	#ifdef PLATFORM_X11
		int          underline_position;
		int          underline_thickness;
		double       sina;
		double       cosa;
		bool         twobyte;
		String       filename;
	#endif

		VectorMap<dword, int> kerning;

		Data();
		~Data();
	};

	Data *ptr;
	int   charset;

	friend class Draw;

	CharMetrics       *CreateMetricsPage(int page) const;
	CharMetrics       *GetPage(int page) const;
	void               ComposeMetrics(Font fnt, CharMetrics *m, int from) const;
	CharMetrics        GetCM(int c) const;

	void       Release();
	void       Retain(const FontInfo& f);
	FontInfo(Data *ptr) : ptr(ptr)       { charset = CHARSET_UNICODE; }

	bool        IsEqual(byte charset, Font f, int angle, int device) const;
	CharMetrics GetComposedMetrics(int c);

public:
	int        GetAscent() const                  { return ptr->ascent; }
	int        GetDescent() const                 { return ptr->descent; }
	int        GetExternal() const                { return ptr->external; }
	int        GetInternal() const                { return ptr->internal; }
	int        GetHeight() const                  { return ptr->height; }
	int        GetLineHeight() const              { return ptr->lineheight; }
	int        GetOverhang() const                { return ptr->overhang; }
	int        GetAveWidth() const                { return ptr->avewidth; }
	int        GetMaxWidth() const                { return ptr->maxwidth; }
	int        HasChar(int ch) const              { return ptr->HasChar(ch); }
	int        GetWidth(int c) const;
	int        operator[](int c) const            { return GetWidth(c); }
	int        GetLeftSpace(int c) const;
	int        GetRightSpace(int c) const;
	int        GetKerning(int c1, int c2) const   { return ptr->kerning.Get(MAKELONG(c1, c2), 0); }
	bool       IsFixedPitch() const               { return ptr->fixedpitch; }
	bool       IsScaleable() const                { return ptr->scaleable; }

	Font       GetFont() const                    { return ptr->font; }
	int        GetFontHeight() const              { return ptr->font.GetHeight(); }

#ifdef PLATFORM_XFT
	String     GetFileName() const;
	XftFont   *GetXftFont() const                 { return ptr->xftfont0; }
#endif
#ifdef PLATFORM_WIN32
	HFONT      GetHFONT() const                   { return ptr->hfont; }
#endif

	void       Clear()                            { Release(); ptr = NULL; }
	bool       IsEmpty() const                    { return !ptr; }
	operator   bool() const                       { return ptr; }

	FontInfo(const FontInfo& f);
	FontInfo& operator=(const FontInfo& f);

	FontInfo()                                    { ptr = NULL; charset = CHARSET_UNICODE; }
	~FontInfo()                                   { Release(); }
};

Font StdFont();

inline Font StdFont(int h)                        { return StdFont().Height(h); }

struct ScreenSans : public Font  { ScreenSans(int n = 0) : Font(SCREEN_SANS, n) {} };
struct ScreenSerif : public Font { ScreenSerif(int n = 0) : Font(SCREEN_SERIF, n) {} };
struct ScreenFixed : public Font { ScreenFixed(int n = 0) : Font(SCREEN_FIXED, n) {} };

struct Roman     : public Font { Roman(int n) : Font(ROMAN, n) {} };
struct Arial     : public Font { Arial(int n) : Font(ARIAL, n) {} };
struct Courier   : public Font { Courier(int n) : Font(COURIER, n) {} };
struct Symbol    : public Font { Symbol(int n) : Font(SYMBOL, n) {} };

#ifdef PLATFORM_WIN32
struct WingDings : public Font { WingDings(int n) : Font(WINGDINGS, n) {} };
struct Tahoma    : public Font { Tahoma(int n) : Font(TAHOMA, n) {} };
#endif

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE
HPALETTE GetQlibPalette();
#endif
#endif

Size GetTextSize(const wchar *text, Font font, int n = -1);
Size GetTextSize(const WString& text, Font font);
Size GetTextSize(const char *text, byte charset, Font font, int n = -1);
Size GetTextSize(const char *text, Font font, int n = -1);
Size GetTextSize(const String& text, Font font);

enum {
	PEN_NULL = -1,
	PEN_SOLID = -2,
	PEN_DASH = -3,
#ifndef PLATFORM_WINCE
	PEN_DOT = -4,
	PEN_DASHDOT = -5,
	PEN_DASHDOTDOT = -6,
#endif
};

class Image;

//DEPRECATED: TODO
Color SBlack();
Color SGray();
Color SLtGray();
Color SWhiteGray();
Color SWhite();
Color SRed();
Color SGreen();
Color SBrown();
Color SBlue();
Color SMagenta();
Color SCyan();
Color SYellow();
Color SLtRed();
Color SLtGreen();
Color SLtYellow();
Color SLtBlue();
Color SLtMagenta();
Color SLtCyan();
//END OF DEPRECATED

Color SColorPaper();
Color SColorText();
Color SColorHighlight();
Color SColorHighlightText();//
Color SColorMenu();
Color SColorMenuText();
Color SColorInfo();
Color SColorInfoText();//
Color SColorMark();
Color SColorDisabled();
Color SColorLight();
Color SColorFace();
Color SColorLabel();
Color SColorShadow();

Color SColorLtFace();
Color SColorDkShadow();


void SBlack_Write(Color c);
void SGray_Write(Color c);
void SLtGray_Write(Color c);
void SWhiteGray_Write(Color c);
void SWhite_Write(Color c);
void SRed_Write(Color c);
void SGreen_Write(Color c);
void SBrown_Write(Color c);
void SBlue_Write(Color c);
void SMagenta_Write(Color c);
void SCyan_Write(Color c);
void SYellow_Write(Color c);
void SLtRed_Write(Color c);
void SLtGreen_Write(Color c);
void SLtYellow_Write(Color c);
void SLtBlue_Write(Color c);
void SLtMagenta_Write(Color c);
void SLtCyan_Write(Color c);

void SColorPaper_Write(Color c);
void SColorText_Write(Color c);
void SColorHighlight_Write(Color c);
void SColorHighlightText_Write(Color c);//
void SColorMenu_Write(Color c);
void SColorMenuText_Write(Color c);
void SColorInfo_Write(Color c);
void SColorInfoText_Write(Color c);//
void SColorMark_Write(Color c);
void SColorDisabled_Write(Color c);
void SColorLight_Write(Color c);
void SColorFace_Write(Color c);
void SColorLabel_Write(Color c);
void SColorShadow_Write(Color c);

void SColorLtFace_Write(Color c);
void SColorDkShadow_Write(Color c);


inline Color InvertColor() { return Color(255, 0); }
inline Color DefaultInk() { return Black(); } //TODO!

class Painting : AssignValueTypeNo<Painting, 48, Moveable<Painting> > {
	String     cmd;
	ValueArray data;
	Sizef      size;
	
	friend class PaintingPainter;
	friend class Painter;

public:
	Sizef   GetSize() const                     { return size; }

	void    Clear()                             { size = Null; data.Clear(); cmd.Clear(); }
	void    Serialize(Stream& s)                { s % cmd % data % size; }
	bool    IsNullInstance() const              { return cmd.IsEmpty(); }
	bool    operator==(const Painting& b) const { return cmd == b.cmd && data == b.data && size == b.size; }
	unsigned GetHashValue() const               { return CombineHash(cmd, data); }
	String  ToString() const                    { return "painting " + AsString(size); }

	operator Value() const                      { return RichValue<Painting>(*this); }
	Painting(const Value& q)                    { *this = RichValue<Painting>::Extract(q); }

	Painting()                                  { size = Null; }
	Painting(const Nuller&)                     { size = Null; }
};

enum {
	MODE_ANTIALIASED = 0,
	MODE_NOAA        = 1,
	MODE_SUBPIXEL    = 2,
};

bool HasPainter();
void PaintImageBuffer(ImageBuffer& ib, const Painting& p, Size sz, Point pos, int mode = MODE_ANTIALIASED);
void PaintImageBuffer(ImageBuffer& ib, const Painting& p, int mode = MODE_ANTIALIASED);
void PaintImageBuffer(ImageBuffer& ib, const Drawing& p, int mode = MODE_ANTIALIASED);

class Draw {
protected:
	bool      palette:1;
	bool      color16:1;
	bool      printer:1;
	bool      pixels:1;
	bool      aborted:1;
	bool      backdraw:1;
	bool      is_mono:1;

	static bool      sFini;

	enum { LRU, HASH };
	static FontInfo::Data *GetFontHash(int hash);
	static FontInfo::Data *GetFontLru();
	static int       FontCached;

	static bool      StdFontSizeSet;
	static Size      StdFontSize;
	static Font      AStdFont;

	typedef Link<FontInfo::Data, 2> FontLink;

	Size   pageDots;
	Size   pagePixels;
	Size   pageMMs;
	Size   inchPixels;
	Size   nativeDpi;
	Size   sheetPixels;
	Point  pageOffset;
	int    native;

	Draw();

	friend class  BackRectDraw;
	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Font;

	friend void StaticExitDraw_();
	friend Font StdFont();

	static void      InitColors();
	static void      InitFonts();
	static void      FreeFonts();
	static void      Release(FontInfo::Data *font);

	int       device;
	FontInfo  lastFont;

	Point        actual_offset;
	Point        actual_offset_bak;
	struct Cloff : Moveable<Cloff> {
		Point org;
	#ifdef PLATFORM_WIN32
		HRGN  hrgn;
	#endif
	#ifdef PLATFORM_X11
		int clipi;
		int offseti;
	#endif
		Rect  drawingclip;
	};
	Array<Cloff> cloff;
	Rect         drawingclip;

	static void      InitPlatformFonts();

	enum FontHashConst { FONTHASH = 97 };

	void ComposeText(int x, int y, int angle, const wchar *text, Font font, Color ink,
                     int n, const int *dx);


#ifdef PLATFORM_WIN32
	static int CALLBACK AddFace(const LOGFONT *logfont, const TEXTMETRIC *, dword type,
	                            LPARAM param);
	static int       EnumFace(HDC hdc, const char *face);
	static void      ForceFace(HDC hdc, const char *face, const char *aface);
	static void      Win32__InitColors();
	static FontInfo  Acquire(Font font, HDC hdc, int angle, int device);

	HDC       handle;
	COLORREF  lastTextColor;
	Color     lastColor;
	HBRUSH    orgBrush;
	HBRUSH    actBrush;
	HFONT     orgFont;
	HPEN      orgPen;
	HPEN      actPen;
	int       lastPen;
	Color     lastPenColor;

	void   Unselect0();
	void   Cinit();
	void   Init();

	void   LoadCaps();
	void   SetDevice(const char *devicename);
	void   SetPrinterMode();
	void   Reset();
	void   SetOrg();
	friend HPALETTE GetQlibPalette();
	void   DotsMode();


	static const char *FontScreenSans;
	static const char *FontScreenSerif;
	static const char *FontScreenFixed;
	static const char *FontRoman;
	static const char *FontArial;
	static const char *FontCourier;
	static const char *FontSymbol;
	static const char *FontWingdings;
	static const char *FontTahoma;

public:
	static bool AutoPalette();
	static void SetAutoPalette(bool ap);

#endif

#ifdef PLATFORM_X11
	static int       AddFonts(const char *xlfd_pattern, int count);
	static FontInfo  Acquire(Font font, int angle, int device);

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
#endif

	enum {
		BEGIN               = 1,
		OFFSET              = 2,
		CLIP                = 3,
		CLIPOFF             = 4,
		EXCLUDECLIP         = 5,
		INTERSECTCLIP       = 6,
		END                 = 7,
		DRAWRECT            = 8,
		DRAWIMAGE           = 9,
		DRAWMONOIMAGE       = 10,
		DRAWDRAWING         = 11,
		DRAWLINE            = 12,
		DRAWELLIPSE         = 13,
		DRAWTEXT            = 14,
		DRAWARC             = 15,
		DRAWPOLYPOLYLINE    = 16,
		DRAWPOLYPOLYPOLYGON = 17,
		DRAWDATA            = 18,
		DRAWPAINTING        = 19,
	};
	
	struct DrawingPos;

public:
	static int  FontCacheMax;

	static void SinCos(int angle, double& sina, double& cosa);

	static void SetStdFont(Font font);
	static Font GetStdFont();
	static Size GetStdFontSize();
	static int  GetStdFontCy()                          { return GetStdFontSize().cy; }

#ifdef PLATFORM_WIN32
#ifdef PLATFORM_WINCE
	static void Flush()                                 {}
#else
	static void Flush()                                 { GdiFlush(); }
#endif
#endif
#ifdef PLATFORM_X11
	static void Flush()                                 { XSync(Xdisplay, false); }
#endif

	bool  PaletteMode() const                           { return palette; }
	bool  IsMono() const                                { return is_mono; }

	Size  GetPagePixels() const                         { return native ? pagePixels : pageDots; }
	Size  GetPixelsPerInch() const                      { return native ? nativeDpi : inchPixels; }
	Size  GetPageMMs() const                            { return pageMMs; }

	bool  Pixels() const                                { return pixels; }
	bool  Dots() const                                  { return !pixels; }
	bool  IsNative() const                              { return native; }

	void BeginNative();
	void EndNative();
	Size GetNativeDpi() const;
	int  GetNativeX(int x) const;
	int  GetNativeY(int x) const;
	void Native(int& x, int& y) const;
	void Native(Point& p) const;
	void Native(Size& sz) const;
	void Native(Rect& r) const;

	bool  IsPrinter() const                             { return printer; }
	bool  IsAborted() const                             { return aborted; }
	bool  IsBack() const                                { return backdraw; }
#ifdef PLATFORM_WIN32
	bool  IsSystem() const                              { return handle; }
#endif
#ifdef PLATFORM_X11
	bool  IsSystem() const                              { return gc != None; }
#endif
	bool  IsDrawing() const;

	virtual void StartPage();
	virtual void EndPage();

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
	virtual void DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);

#ifndef PLATFORM_WINCE
	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);
#endif
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	virtual void DrawDrawingOp(const Rect& target, const Drawing& w);
	virtual void DrawPaintingOp(const Rect& target, const Painting& w);

	void  Begin()                                       { BeginOp(); }
	void  End()                                         { EndOp(); }
	void  Offset(Point p)                               { OffsetOp(p); }
	void  Offset(int x, int y);
	bool  Clip(const Rect& r)                           { return ClipOp(r); }
	bool  Clip(int x, int y, int cx, int cy);
	bool  Clipoff(const Rect& r)                        { return ClipoffOp(r); }
	bool  Clipoff(int x, int y, int cx, int cy);
	bool  ExcludeClip(const Rect& r)                    { return ExcludeClipOp(r); }
	bool  ExcludeClip(int x, int y, int cx, int cy);
	bool  IntersectClip(const Rect& r)                  { return IntersectClipOp(r); }
	bool  IntersectClip(int x, int y, int cx, int cy);
	Rect  GetClip() const                               { return GetClipOp(); }
	bool  IsPainting(const Rect& r) const               { return IsPaintingOp(r); }
	bool  IsPainting(int x, int y, int cx, int cy) const;

	Point GetOffset() const                             { return actual_offset; }

	int   GetCloffLevel() const                         { return cloff.GetCount(); }


	void DrawRect(int x, int y, int cx, int cy, Color color)
	{ DrawRectOp(x, y, cx, cy, color); }
	void DrawRect(const Rect& rect, Color color);

	void DrawImage(int x, int y, int cx, int cy, const Image& img, const Rect& src);
	void DrawImage(int x, int y, int cx, int cy, const Image& img);
	void DrawImage(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	void DrawImage(int x, int y, int cx, int cy, const Image& img, Color color);

	void DrawImage(const Rect& r, const Image& img, const Rect& src);
	void DrawImage(const Rect& r, const Image& img);
	void DrawImage(const Rect& r, const Image& img, const Rect& src, Color color);
	void DrawImage(const Rect& r, const Image& img, Color color);

	void DrawImage(int x, int y, const Image& img, const Rect& src);
	void DrawImage(int x, int y, const Image& img);
	void DrawImage(int x, int y, const Image& img, const Rect& src, Color color);
	void DrawImage(int x, int y, const Image& img, Color color);

	void DrawData(int x, int y, int cx, int cy, const String& data, const char *type);
	void DrawData(const Rect& r, const String& data, const char *type);

	void DrawLine(int x1, int y1, int x2, int y2, int width = 0, Color color = DefaultInk)
	{ DrawLineOp(x1, y1, x2, y2, width, color); }
	void DrawLine(Point p1, Point p2, int width = 0, Color color = DefaultInk);

	void DrawEllipse(const Rect& r, Color color = DefaultInk,
	                 int pen = Null, Color pencolor = DefaultInk)
	{ DrawEllipseOp(r, color, pen, pencolor); }
	void DrawEllipse(int x, int y, int cx, int cy, Color color = DefaultInk,
		             int pen = Null, Color pencolor = DefaultInk);

#ifndef PLATFORM_WINCE
	void DrawArc(const Rect& rc, Point start, Point end, int width = 0, Color color = DefaultInk)
	{ DrawArcOp(rc, start, end, width, color); }

	void DrawPolyPolyline(const Point *vertices, int vertex_count,
	                      const int *counts, int count_count,
	                      int width = 0, Color color = DefaultInk, Color doxor = Null)
	{ DrawPolyPolylineOp(vertices, vertex_count, counts, count_count, width, color, doxor); }
	void DrawPolyPolyline(const Vector<Point>& vertices, const Vector<int>& counts,
		                  int width = 0, Color color = DefaultInk, Color doxor = Null);
	void DrawPolyline(const Point *vertices, int count,
		              int width = 0, Color color = DefaultInk, Color doxor = Null);
	void DrawPolyline(const Vector<Point>& vertices,
		              int width = 0, Color color = DefaultInk, Color doxor = Null);

	void   DrawPolyPolyPolygon(const Point *vertices, int vertex_count,
		                       const int *subpolygon_counts, int subpolygon_count_count,
		                       const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
		                       Color color = DefaultInk, int width = 0, Color outline = Null,
		                       uint64 pattern = 0, Color doxor = Null)
	{ DrawPolyPolyPolygonOp(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
		                  disjunct_polygon_counts, disjunct_polygon_count_count,
		                  color, width, outline, pattern, doxor); }

	void   DrawPolyPolyPolygon(const Vector<Point>& vertices,
	                           const Vector<int>& subpolygon_counts,
	                           const Vector<int>& disjunct_polygon_counts,
	                           Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
	void   DrawPolyPolygon(const Point *vertices, int vertex_count,
	                       const int *subpolygon_counts, int subpolygon_count_count,
	                       Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
	void   DrawPolyPolygon(const Vector<Point>& vertices, const Vector<int>& subpolygon_counts,
	                       Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
	void   DrawPolygons(const Point *vertices, int vertex_count,
	                    const int *polygon_counts, int polygon_count_count,
	                    Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
	void   DrawPolygons(const Vector<Point>& vertices, const Vector<int>& polygon_counts,
	                    Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
	void   DrawPolygon(const Point *vertices, int vertex_count,
	                   Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
	void   DrawPolygon(const Vector<Point>& vertices,
	                   Color color = DefaultInk, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
#endif

	void DrawDrawing(const Rect& r, const Drawing& iw) { DrawDrawingOp(r, iw); }
	void DrawDrawing(int x, int y, int cx, int cy, const Drawing& iw);

	void DrawPainting(const Rect& r, const Painting& iw) { DrawPaintingOp(r, iw); }
	void DrawPainting(int x, int y, int cx, int cy, const Painting& iw);

	void DrawText(int x, int y, int angle, const wchar *text, Font font = StdFont(),
		          Color ink = DefaultInk, int n = -1, const int *dx = NULL);
	void DrawText(int x, int y, const wchar *text, Font font = StdFont(),
		          Color ink = DefaultInk, int n = -1, const int *dx = NULL);

	void DrawText(int x, int y, const WString& text, Font font = StdFont(),
		          Color ink = DefaultInk, const int *dx = NULL);
	void DrawText(int x, int y, int angle, const WString& text, Font font = StdFont(),
		          Color ink = DefaultInk, const int *dx = NULL);

	void DrawText(int x, int y, int angle, const char *text, byte charset,
	              Font font = StdFont(), Color ink = DefaultInk, int n = -1, const int *dx = NULL);
	void DrawText(int x, int y, const char *text, byte charset, Font font = StdFont(),
		          Color ink = DefaultInk, int n = -1, const int *dx = NULL);

	void DrawText(int x, int y, int angle, const char *text,
	              Font font = StdFont(), Color ink = DefaultInk, int n = -1, const int *dx = NULL);
	void DrawText(int x, int y, const char *text, Font font = StdFont(),
		          Color ink = DefaultInk, int n = -1, const int *dx = NULL);

	void DrawText(int x, int y, const String& text, Font font = StdFont(),
		          Color ink = DefaultInk, const int *dx = NULL);
	void DrawText(int x, int y, int angle, const String& text, Font font = StdFont(),
		          Color ink = DefaultInk, const int *dx = NULL);

#ifdef PLATFORM_WIN32
	bool         IsMetaFile() const                     { return device == -1; }

	COLORREF GetColor(Color color) const;
#ifndef PLATFORM_WINCE
	Point LPtoDP(Point p) const;
	Point DPtoLP(Point p) const;
	Rect  LPtoDP(const Rect& r) const;
	Rect  DPtoLP(const Rect& r) const;
#endif
	void SetColor(Color color);
	void SetFont(Font font, int angle = 0);
	void SetDrawPen(int width, Color color);

	Size  GetSizeCaps(int i, int j) const;
	HDC   BeginGdi();
	void  EndGdi();
	HDC   GetHandle()                    { return handle; }
	operator HDC() const                 { return handle; }
	void  Unselect();
	void  Attach(HDC ahandle)            { handle = ahandle; Init(); }
	HDC   Detach()                       { Unselect(); HDC h = handle; handle = NULL; return h; }

	Draw(HDC hdc);
	virtual ~Draw();
#endif

#ifdef PLATFORM_X11

#ifdef PLATFORM_XFT
	static XftFont *CreateXftFont(Font f, int angle);
	XftDraw *GetXftDraw() const               { return xftdraw; }
#endif
	void  SetForeground(Color color);
	void  SetLineStyle(int width);
	void  SetFont(Font font, int angle);
	void  SetClip();

	Drawable GetDrawable() const              { return dw; }
	GC       GetGC() const                    { return gc; }
	const Vector<Rect>& GetClipList() const   { return clip.Top(); }
#ifdef PLATFORM_XFT
	Draw(Drawable dw, GC gc, XftDraw *xftdraw, const Vector<Rect>& clip);
#else
	Draw(Drawable dw, GC gc, const Vector<Rect>& clip);
#endif
	virtual ~Draw();
#endif

private: //Deprecated
	FontInfo GetFontInfoW(Font font = StdFont());

private:
	Draw(const Draw&);
	void operator=(const Draw&);
};

typedef Draw SystemDraw;

void DrawImageBandRLE(Draw& w, int x, int y, const Image& m, int minp);

class DataDrawer {
	typedef DataDrawer *(*Factory)();
	template <class T> static DataDrawer *FactoryFn() { return new T; }
	static void AddFormat(const char *id, Factory f);
	static VectorMap<String, void *>& Map();

public:
	virtual void  Open(const String& data, int cx, int cy) = 0;
	virtual void  Render(ImageBuffer& ib) = 0;
	virtual ~DataDrawer();

	static  One<DataDrawer> Create(const String& id);

	template <class T>	static void Register(const char *id)  { AddFormat(id, &DataDrawer::FactoryFn<T>); }
};

#ifdef PLATFORM_WIN32
class WinMetaFile;
#endif

class Drawing : AssignValueTypeNo<Drawing, 49, Moveable<Drawing> > {
	Size       size;
	String     data;
	ValueArray val;

	friend class DrawingDraw;
	friend class Draw;

public:
	operator bool() const          { return !data.IsEmpty(); }
	Size GetSize() const           { return size; }
	void SetSize(Size sz)          { size = sz; }
	void SetSize(int cx, int cy)   { size = Size(cx, cy); }

	Size RatioSize(int cx, int cy) const;
	Size RatioSize(Size sz) const  { return RatioSize(sz.cx, sz.cy); }

	void Clear()                   { data.Clear(); size = Null; }

	void Append(Drawing& dw);

	void Serialize(Stream& s);

	bool    IsNullInstance() const             { return data.IsEmpty(); }
	bool    operator==(const Drawing& b) const { return val == b.val && data == b.data && size == b.size; }
	String  ToString() const                   { return "drawing " + AsString(size); }
	unsigned GetHashValue() const              { return CombineHash(data, val); }

	operator Value() const         { return RichValue<Drawing>(*this); }
	Drawing(const Value& src);

	Drawing()                      { size = Null; }
	Drawing(const Nuller&)         { size = Null; }

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE
	// deprecated
	static Drawing FromWMF(const WinMetaFile& wmf);
	static Drawing LoadWMF(const char *file);
	static Drawing ReadClipboardWMF();
	WinMetaFile AsWMF() const;
	void        WriteClipboardWMF() const;
#endif
#endif
};

class DrawingDraw : public Draw {
public:
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
	virtual void DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id);
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
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);

	virtual void DrawDrawingOp(const Rect& target, const Drawing& w);
	virtual void DrawPaintingOp(const Rect& target, const Painting& w);

private:
	Size         size;
	StringStream drawing;
	ValueArray   val;

	void         DInit();
	void         DrawingBegin();
	Stream&      DrawingOp(int code);

public:
	void     SetPixels(bool b = true)         { pixels = b; }

	void     Create(int cx, int cy);
	void     Create(Size sz);

	Size     GetSize() const                  { return size; }

	Drawing  GetResult();
	operator Drawing()                        { return GetResult(); }

	DrawingDraw();
	DrawingDraw(int cx, int cy);
	DrawingDraw(Size sz);
	~DrawingDraw();
};

class NilDraw : public Draw {
public:
	virtual void BeginOp();
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id);
	virtual void DrawDrawingOp(const Rect& target, const Drawing& w);
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor);
	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor);
	virtual void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx);
	virtual void EndOp();
	virtual void EndPage();
	virtual bool ExcludeClipOp(const Rect& r);
	virtual Rect GetClipOp() const;
	virtual bool IntersectClipOp(const Rect& r);
	virtual bool IsPaintingOp(const Rect& r) const;
	virtual void OffsetOp(Point p);
	virtual void StartPage();

	NilDraw();
	~NilDraw();
};

class BackDraw : public Draw {
public:
	virtual bool IsPaintingOp(const Rect& r) const;

protected:
#ifdef PLATFORM_WIN32
	HBITMAP hbmpold;
	HBITMAP hbmp;
#endif
#ifdef PLATFORM_X11
	Pixmap  pixmap;
#endif
	Size    size;
	Draw   *painting;
	Point   painting_offset;


public:
	void  Put(Draw& w, int x, int y);
	void  Put(Draw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(Draw& draw, int cx, int cy);
	void Create(Draw& draw, Size sz)             { Create(draw, sz.cx, sz.cy); }
	void Create(int cx, int cy);
	void Create(Size sz)                         { Create(sz.cx, sz.cy); }
	void Destroy();

	void SetPaintingDraw(Draw& w, Point off)     { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
};

void         AddNotEmpty(Vector<Rect>& result, int left, int right, int top, int bottom);
bool         Subtract(const Rect& r, const Rect& sub, Vector<Rect>& result);
bool         Subtract(const Vector<Rect>& rr, const Rect& sub, Vector<Rect>& result);
Vector<Rect> Subtract(const Vector<Rect>& rr, const Rect& sub, bool& changed);
Vector<Rect> Intersect(const Vector<Rect>& b, const Rect& a, bool& changed);

void Subtract(Vector<Rect>& rr, const Rect& sub);
void Union(Vector<Rect>& rr, const Rect& add);

#ifdef PLATFORM_X11
void SetClip(GC gc, XftDraw *xftdraw, const Vector<Rect>& cl);
#endif

void DrawRect(Draw& w, const Rect& rect, const Image& img, bool ralgn = false); //??? TODO
void DrawRect(Draw& w, int x, int y, int cx, int cy, const Image& img, bool ra = false);

void DrawTiles(Draw& w, int x, int y, int cx, int cy, const Image& img);
void DrawTiles(Draw& w, const Rect& rect, const Image& img);

void DrawFatFrame(Draw& w, int x, int y, int cx, int cy, Color color, int n);
void DrawFatFrame(Draw& w, const Rect& r, Color color, int n);

void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Color leftcolor, Color topcolor, Color rightcolor, Color bottomcolor);
void DrawFrame(Draw& w, const Rect& r,
			   Color leftcolor, Color topcolor, Color rightcolor, Color bottomcolor);
void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Color topleftcolor, Color bottomrightcolor);
void DrawFrame(Draw& w, const Rect& r,
			   Color topleftcolor, Color bottomrightcolor);
void DrawFrame(Draw& w, int x, int y, int cx, int cy, Color color);
void DrawFrame(Draw& w, const Rect& r, Color color);

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *colors_ltrd); //TODO
void DrawBorder(Draw& w, const Rect& r, const ColorF *colors_ltrd);

const ColorF *BlackBorder();
const ColorF *ButtonPushBorder();
const ColorF *EdgeButtonBorder();
const ColorF *DefButtonBorder();
const ColorF *ButtonBorder();
const ColorF *InsetBorder();
const ColorF *OutsetBorder();
const ColorF *ThinOutsetBorder();
const ColorF *ThinInsetBorder();

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *(*colors_ltrd)());
void DrawBorder(Draw& w, const Rect& r, const ColorF *(*colors_ltrd)());

void DrawRectMinusRect(Draw& w, const Rect& rect, const Rect& inner, Color color);

void DrawHighlightImage(Draw& w, int x, int y, const Image& img, bool highlight = true,
                        bool enabled = true, Color maskcolor = SColorPaper);

Color GradientColor(Color fc, Color tc, int i, int n);

void DrawDragRect(Draw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n, Color color, uint64 pattern);

enum {
	BUTTON_NORMAL, BUTTON_OK, BUTTON_HIGHLIGHT, BUTTON_PUSH, BUTTON_DISABLED, BUTTON_CHECKED,
	BUTTON_VERTICAL = 0x100,
	BUTTON_EDGE = 0x200,
	BUTTON_TOOL = 0x400,
	BUTTON_SCROLL = 0x800,
};

void DrawXPButton(Draw& w, Rect r, int type);

void DrawTextEllipsis(Draw& w, int x, int y, int cx, const char *text, const char *ellipsis,
				      Font font = StdFont(), Color ink = SColorText(), int n = -1);
void DrawTextEllipsis(Draw& w, int x, int y, int cx, const wchar *text, const char *ellipsis,
				      Font font = StdFont(), Color ink = SColorText(), int n = -1);
Size GetTLTextSize(const wchar *text, Font font = StdFont());
int  GetTLTextHeight(const wchar *s, Font font = StdFont());
void DrawTLText(Draw& draw, int x, int y, int cx, const wchar *text, Font font = StdFont(),
                Color ink = SColorText(), int accesskey = 0);


typedef String (*DrawingToPdfFnType)(const Array<Drawing>& report, Size pagesize, int margin);

void SetDrawingToPdfFn(DrawingToPdfFnType Pdf);
DrawingToPdfFnType GetDrawingToPdfFn();

#ifdef PLATFORM_WIN32

#include "DrawWin32.h"

#endif

#include "Display.h"
#include "ImageDraw.h"
#include "Debug.h"
#include "Cham.h"

END_UPP_NAMESPACE

#endif
