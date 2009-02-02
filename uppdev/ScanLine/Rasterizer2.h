class Rasterizer {
	struct Cell : MoveableWithSwap<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };

	Rectf                   cliprect;
	double                  x1, y1;
	Buffer< Vector<Cell> >  cell;
	int                     xmax, ymax;
	int                     min_y;
	int                     max_y;
	Size                    sz;

	void  Init();
	Cell *AddCells(int y, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);
	bool  BeginRender(int y, const Cell *&c, const Cell *&e);

public:
	struct Target {
		virtual void Start(int x, int len) = 0;
		virtual void Render(int val) = 0;
		virtual void Render(int val, int len) = 0;
	};

	void LineRaw(int x1, int y1, int x2, int y2);
	
	void SetClip(const Rectf& rect);
	void Move(double x, double y);
	void Line(double x, double y);

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	void Render(int y, Target& g, bool evenodd);

	void Reset();
	
	Rasterizer(int cx, int cy);
};
