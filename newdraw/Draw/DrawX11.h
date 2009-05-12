#ifdef PLATFORM_X11

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
	Size    pageSize;

	SystemDraw();

	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Font;
	friend class  ViewDraw;

	friend void StaticExitDraw_();
	friend Font StdFont();

	FontInfo  lastFont;

	Point        actual_offset;
	Point        actual_offset_bak;
	struct Cloff : Moveable<Cloff> {
		Point org;
		int clipi;
		int offseti;
		Rect  drawingclip;
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

	Point    GetOffset() const                          { return actual_offset; }

	static XftFont *CreateXftFont(Font f, int angle);
	XftDraw *GetXftDraw() const               { return xftdraw; }

	Rect  GetClip() const;

	void  SetForeground(Color color);
	void  SetLineStyle(int width);
	void  SetFont(Font font, int angle);
	void  SetClip();

	Drawable GetDrawable() const              { return dw; }
	GC       GetGC() const                    { return gc; }
	const Vector<Rect>& GetClipList() const   { return clip.Top(); }

	SystemDraw(Drawable dw, GC gc, XftDraw *xftdraw, const Vector<Rect>& clip);
};

#endif
