class DDARasterizer {
	struct Segment : Moveable<Segment> {
		int  l;
		int  h;
		bool flag;
		
		bool operator<(const Segment& b) const { return h < b.h; }
	};
	
	struct Segments;
	
	int           cy;
	int           diry;
	int           dirx;
	One<Segments> pseg;
	Point         p0, p1;
	Point         j1, j2;
	Point         b1, b2;
	int           width;
	bool          close;

	void AHorz(int x, int y, int cx);
	void AVert(int x, int y, int cy);
	void DoLine(Point p1, Point p2, bool last);
	void FatLine(Point p2);

public:
	virtual void PutHorz(int x, int y, int cx) = 0;
	virtual void PutVert(int x, int y, int cy) = 0;
	
	int  GetDirx() const                      { return dirx; }
	int  GetDiry() const                      { return diry; }
	
	DDARasterizer& Move(Point p);
	DDARasterizer& Line(Point p);
	DDARasterizer& Close();
	
	DDARasterizer& Polygon();
	DDARasterizer& Fill();
	
	DDARasterizer& Width(int width_)          { width = width_; return *this; }
	
	DDARasterizer& Ellipse(const Rect& rect, int width);

	void Cy(int cy_)                          { cy = cy_; }
	
	DDARasterizer();
	virtual ~DDARasterizer();
};
