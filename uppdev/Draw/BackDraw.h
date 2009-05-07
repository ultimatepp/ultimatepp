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
