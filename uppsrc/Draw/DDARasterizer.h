class DDARasterizer {
	struct Segment : Moveable<Segment> {
		int  l;
		int  h;
		bool flag;
		
		bool operator<(const Segment& b) const { return l < b.l; }
	};
	
	struct Segments;
	
	int           cy;
	int           diry;
	int           dirx;
	One<Segments> pseg;
	Point         p0, p1;
	int           width;

	void AHorz(int x, int y, int cx);
	void AVert(int x, int y, int cy);
	void DoLine(Point p1, Point p2, bool last);
	void FatLine(Point p1, Point p2, int n);

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
	
	DDARasterizer& Ellipse(const Rect& rect);
	
	DDARasterizer& Width(int width_)           { width = width_; return *this; }

	void Cy(int cy_)                          { cy = cy_; }
	
	DDARasterizer();
	~DDARasterizer();
};
