class BackDraw : public SystemDraw {
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

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
	void  Put(SystemDraw& w, int x, int y);
	void  Put(SystemDraw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(SystemDraw& w, int cx, int cy);
	void Create(SystemDraw& w, Size sz)                { Create(w, sz.cx, sz.cy); }
//	void Create(int cx, int cy);
//	void Create(Size sz)                               { Create(sz.cx, sz.cy); }
	void Destroy();

	void SetPaintingDraw(Draw& w, Point off)           { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
	
// Deprecated:
	void  Put(Draw& w, int x, int y)             { Put(*(SystemDraw*)&w, x, y); }
	void  Put(Draw& w, Point p)                  { Put(*(SystemDraw*)&w, p.x, p.y); }

	void Create(Draw& w, int cx, int cy)         { Create(*(SystemDraw*)&w, cx, cy); }
	void Create(Draw& w, Size sz)                { Create(*(SystemDraw*)&w, sz.cx, sz.cy); }
};
