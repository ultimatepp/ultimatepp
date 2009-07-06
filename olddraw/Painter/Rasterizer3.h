class Rasterizer {
	Rectf                   cliprect;
	double                  x1, y1;
	Buffer< Vector<dword> >  cell;
	int                     xmax, ymax;
	int                     min_y;
	int                     max_y;
	Size                    sz;

	inline dword Cell(int x, int w, int c)     { return (x << 18) + (w << 9) + c; }
	inline int   GetX(dword cell)              { return cell >> 18; }
	inline int   GetW(dword cell)              { return (cell >> 9) & 511; }
	inline int   GetC(dword cell)              { return (cell & 256) ? -512 | (cell & 511) : (cell & 511); }

	void  Init();
	dword *AddCells(int y, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);

public:
	struct Target {
		virtual void Start(int x, int len) = 0;
		virtual void Render(int val) = 0;
		virtual void Render(int val, int len) = 0;
	};

	void LineRaw(int x1, int y1, int x2, int y2);
	
	void Move(double x, double y);
	void Line(double x, double y);

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	
	void SetClip(const Rectf& rect);

	void Render(int y, Target& g);

	void Reset();
	
	Rasterizer(int cx, int cy);
};
