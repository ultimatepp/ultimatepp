struct SDraw : Draw, DDARasterizer {
	virtual dword GetInfo() const;
	
	virtual void BeginOp();
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual void OffsetOp(Point p);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual void EndOp();
	virtual bool IsPaintingOp(const Rect& r) const;

	virtual void SysDrawImageOp(int x, int y, const Image& img, Color color);
	virtual void SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color);
	virtual void DrawRectOp(int x, int y, int cx, int cy, Color color);

	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx);

	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor);
	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor);
	
	virtual void PutHorz(int x, int y, int cx);
	virtual void PutVert(int x, int y, int cy);

private:
	struct Cloff {
		Vector<Rect> clip;
		Point        offset;
		
		void operator<<=(const Cloff& b)      { clip <<= b.clip; offset = b.offset; }
	};
	
	bool  paintonly;
	
	Array<Cloff> cloff;
	
	Color docolor;

public:
	virtual void  PutImage(Point p, const Image& img, const Rect& src) = 0;
	virtual void  PutImage(Point p, const Image& img, const Rect& src, Color color);
	virtual void  PutRect(const Rect& r, Color color) = 0;
	virtual Image RenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz);
	
	Point GetOffset() const                                       { return cloff.Top().offset; }

	void Init(const Rect& r);
	void Init(const Vector<Rect>& rs, int height, Point offset = Point(0, 0));
	
	void PaintOnly()                                              { paintonly = true; }

	SDraw()                                                       { paintonly = false; }
};

class SImageDraw1 : public SDraw {
protected:
	ImageBuffer ib;
	friend class SImageDraw;

public:
	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);

	void Create(Size sz);
};

class SImageDraw : public SImageDraw1 {
	SImageDraw1 alpha;
	
	bool has_alpha;

public:
	Draw& Alpha();

	operator Image() const;
	
	SImageDraw(Size sz);
	SImageDraw(int cx, int cy);
};
