#ifdef PLATFORM_X11

class SystemDraw {
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

	virtual dword GetInfo() const;
	virtual Size  GetPagePixels() const;

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



class SystemDraw : public Draw {
	dword style;
	Size  pagePixels;
	Size  nativeDpi;

	SystemDraw();

	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Font;

	friend void StaticExitDraw_();

	FontInfo  lastFont;

	Point     actual_offset;
	Point     actual_offset_bak;

	struct Cloff : Moveable<Cloff> {
		Point org;
		HRGN  hrgn;
		Rect  drawingclip;
	};

	Array<Cloff> cloff;
	Rect         drawingclip;

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

public:
	static bool AutoPalette();

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

	virtual dword GetInfo() const;
	virtual Size  GetPagePixels() const;

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

	SystemDraw(HDC hdc);
	virtual ~SystemDraw();
#endif

#ifdef PLATFORM_X11

	static XftFont *CreateXftFont(Font f, int angle);
	XftDraw *GetXftDraw() const               { return xftdraw; }

	void  SetForeground(Color color);
	void  SetLineStyle(int width);
	void  SetFont(Font font, int angle);
	void  SetClip();

	Drawable GetDrawable() const              { return dw; }
	GC       GetGC() const                    { return gc; }
	const Vector<Rect>& GetClipList() const   { return clip.Top(); }

	Draw(Drawable dw, GC gc, XftDraw *xftdraw, const Vector<Rect>& clip);
	virtual ~Draw();
#endif
};


#endif
